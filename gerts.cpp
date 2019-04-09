#include "BrickPi3.h"
#include <iostream>
#include <unistd.h>
#include <signal.h>
using namespace std;

BrickPi3 BP;

void exit_signal_handler(int signo);

const float dps_reduction = 4.5;
const int rpm = 3;
const int motor_dps = 360*rpm;

void fwd(void){
    BP.set_motor_dps(PORT_B, motor_dps);
    BP.set_motor_dps(PORT_C, motor_dps);
}

void left(void){
    BP.set_motor_dps(PORT_B, motor_dps);
    BP.set_motor_dps(PORT_C, motor_dps/dps_reduction);
}

void right(void){
    BP.set_motor_dps(PORT_B, motor_dps/dps_reduction);
    BP.set_motor_dps(PORT_C, motor_dps);
}

void follow(int followColor, sensor_color_t Color1, sensor_ultrasonic_t Ultrasonic2, sensor_light_t Light3){
    while(true) {
        BP.get_sensor(PORT_1, Color1);
        BP.get_sensor(PORT_2, Ultrasonic2);
        BP.get_sensor(PORT_3, Light3);

        if(Ultrasonic2.cm <= 20){
            BP.set_motor_dps(PORT_B, 0);
            BP.set_motor_dps(PORT_C, 0);
            sleep(1);
        }
        else {
            int colorLeft = Color1.color;
            usleep(0.01);
            BP.get_sensor(PORT_3, Light3);
            int colorRight = Color1.color;
            usleep(0.5);
            if (colorLeft != 1 || colorLeft != 6 || colorRight != 1 || colorRight != 6) {
                if (colorLeft == followColor) {
                    left();
                    left();
                }
                if (colorRight == followColor) {
                    right();
                    right();
                }
            }

            int line_edge = 1900;
            if(Light3.reflected < line_edge){
               right();
            }
            if(Light3.reflected > line_edge){
                left();
            }
            if(Light3.reflected == line_edge){
                fwd();
            }
        }
    }
}

void armmotor(int angle){
    BP.detect();
    BP.offset_motor_encoder(PORT_A, BP.get_motor_encoder(PORT_A));
    BP.set_motor_limits(PORT_A, 50, 200);
    int32_t nul = 0;
    BP.set_motor_position(PORT_A, nul);
    BP.set_motor_position_relative(PORT_A, angle);
}

int main(){
	BP.detect();
	BP.set_sensor_type(PORT_1, SENSOR_TYPE_NXT_COLOR_FULL);
    BP.set_sensor_type(PORT_2, SENSOR_TYPE_NXT_ULTRASONIC);
    BP.set_sensor_type(PORT_3, SENSOR_TYPE_NXT_LIGHT_ON);

	sensor_color_t      Color1;
    sensor_ultrasonic_t Ultrasonic2;
    sensor_light_t      Light3;

	signal(SIGINT, exit_signal_handler);

	armmotor(-145);
	sleep(1);
    
    bool waiting = true;
    int color;

    while (waiting) {
    	if(BP.get_sensor(PORT_1, Color1) == 0){
    		color = Color1.color;
            switch(color){
                case 1:
                    continue;
                case 2:
                    cout << "Blue\n";
                    waiting = false;
                    break;
                case 3:
                    cout << "Green\n";
                    waiting = false;
                    break;
                case 4:
                    cout << "Yellow\n";
                    waiting = false;
                    break;
                case 5:
                    cout << "Red\n";
                    waiting = false;
                    break;
            }
        }
    }
    armmotor(130);
    follow(color, Color1, Ultrasonic2, Light3);
}

void exit_signal_handler(int signo){
	if(signo == SIGINT){
        
		BP.reset_all();
		exit(-2);
	}
}

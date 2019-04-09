#include "BrickPi3.h"
#include <iostream>
#include <unistd.h>
#include <signal.h>
using namespace std;

BrickPi3 BP;

void exit_signal_handler(int signo);

const float dps_reduction = 4.5;
const int rpm = 3;
const float wait = 0.001;
unsigned int loop = 0;
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

void follow(void){
	BP.detect();
	BP.set_sensor_type(PORT_2, SENSOR_TYPE_NXT_ULTRASONIC);
    BP.set_sensor_type(PORT_3, SENSOR_TYPE_NXT_LIGHT_ON);

    sensor_ultrasonic_t Ultrasonic2;
    sensor_light_t      Light3;

    BP.get_sensor(PORT_2, Ultrasonic2);
    BP.get_sensor(PORT_3, Light3);

    if(Ultrasonic2.cm <= 10){
        BP.set_motor_dps(PORT_B, 0);
        BP.set_motor_dps(PORT_C, 0);
        sleep(1);
        BP.set_motor_dps(PORT_B, (motor_dps/2)*-1);
        BP.set_motor_dps(PORT_C, motor_dps/2);
        sleep(1);
        BP.set_motor_dps(PORT_B, motor_dps/2);
        BP.set_motor_dps(PORT_C, motor_dps/2);
        sleep(1);
        BP.set_motor_dps(PORT_B, motor_dps/2);
        BP.set_motor_dps(PORT_C, (motor_dps/2)*-1);
        sleep(1);
        BP.set_motor_dps(PORT_B, motor_dps-180);
        BP.set_motor_dps(PORT_C, motor_dps-180);
        sleep(2);
        BP.set_motor_dps(PORT_B, motor_dps/2);
        BP.set_motor_dps(PORT_C, (motor_dps/2)*-1);
        sleep(1);
        BP.set_motor_dps(PORT_B, motor_dps/3);
        BP.set_motor_dps(PORT_C, motor_dps/3);
        sleep(1);
        BP.set_motor_dps(PORT_B, (motor_dps/2)*-1);
        BP.set_motor_dps(PORT_C, motor_dps/2);
        sleep(1);
    } else {
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
sleep(wait);
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

	sensor_color_t      Color1;

	signal(SIGINT, exit_signal_handler);

	armmotor(-145);
	sleep(1);
    bool waiting = true;
    while (waiting) {
    	if(BP.get_sensor(PORT_1, Color1) == 0) {
    		int color = Color1.color;
    		// if(color == 1) {
    		// 	armmotor(-1);
    		// 	int color = Color1.color;
    		// }

            switch(color) {
                case 0:
                case 1:

                case 2:
                    cout << "Blue\n";
                case 3:
                    cout << "Green\n";
                case 4:
                    cout << "Yellow\n";
                case 5:
                    cout << "Red\n";
                default:
                    waiting = false;
            }
        }
    }
    armmotor(130);
}
void exit_signal_handler(int signo){
	if(signo == SIGINT){
		BP.reset_all();
		exit(-2);
	}
}

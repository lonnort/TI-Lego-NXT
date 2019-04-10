#include "BrickPi3.h"
#include <iostream>
#include <unistd.h>
#include <signal.h>
#include <ctime>
using namespace std;

BrickPi3 BP;

const float        dps_reduction = 4.5;
const unsigned int rpm = 3;
const unsigned int motor_dps = 360*rpm;

const int   line_edge = 1900;
const float collision_distance = 20;

unsigned int color;

void exit_signal_handler(int signo);

void armMotor(int angle);
void detectCrossing(sensor_color_t Color1);
bool detectObstacle(sensor_ultrasonic_t Ultrasonic2);

void moveFwd(void){
    BP.set_motor_dps(PORT_B, motor_dps);
    BP.set_motor_dps(PORT_C, motor_dps);
}

void moveLeft(void){
    BP.set_motor_dps(PORT_B, motor_dps);
    BP.set_motor_dps(PORT_C, motor_dps/dps_reduction);
}

void moveRight(void){
    BP.set_motor_dps(PORT_B, motor_dps/dps_reduction);
    BP.set_motor_dps(PORT_C, motor_dps);
}

void followLine(sensor_color_t Color1, sensor_ultrasonic_t Ultrasonic2, sensor_light_t Light3){
    while (true) {
        if (detectObstacle(Ultrasonic2)) {
            BP.set_motor_dps(PORT_B, 0);
            BP.set_motor_dps(PORT_C, 0);
        }
        else {
            detectCrossing(Color1);

            BP.get_sensor(PORT_3, Light3);
            
            if(Light3.reflected < line_edge){
                   moveRight();
            }
            if(Light3.reflected > line_edge){
                    moveLeft();
            }
            if(Light3.reflected == line_edge){
                    moveFwd();
            }
        }
    }
}

void armMotor(int angle){
    int32_t nul = 0;

    BP.detect();
    BP.offset_motor_encoder(PORT_A, BP.get_motor_encoder(PORT_A));

    BP.set_motor_limits(PORT_A, 50, 200);
    BP.set_motor_position(PORT_A, nul);
    BP.set_motor_position_relative(PORT_A, angle);
}

bool detectObstacle(sensor_ultrasonic_t Ultrasonic2){
    BP.get_sensor(PORT_2, Ultrasonic2);
	return (Ultrasonic2.cm <= 20);
}

void detectCrossing(sensor_color_t Color1){
    const unsigned int followColor = color;
    
    // BP.get_sensor(PORT_1, Color1);
    cout << "color: " << Color1.color << endl;

    int colorLeft, colorRight;
	if(Color1.color != 1 || Color1.color != 6) {
        colorLeft = Color1.color;
        long double starttime = time(0);

        while (true) {
            BP.get_sensor(PORT_1, Color1);
            if (time(0) > (starttime + 500)) {
                break;
            }
            if (Color1.color != colorLeft || Color1.color != 1 || Color1.color != 6) {
                colorRight = Color1.color;
            }
        }
    }
    if (colorLeft == followColor) {
        moveLeft;
        sleep(2);
    }
    else if (colorRight == followColor) {
        moveRight;
        sleep(2);
    }
}

void detectMed(sensor_color_t Color1){
	armMotor(-145);
	sleep(1);
	
	bool waiting = true;
    while (waiting){
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
    armMotor(130);
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

	detectMed(Color1);
	followLine(Color1, Ultrasonic2, Light3);
}

void exit_signal_handler(int signo){
	if(signo == SIGINT){
		BP.reset_all();
		exit(-2);
	}
}

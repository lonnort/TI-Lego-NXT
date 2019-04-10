#include "BrickPi3.h"
#include <iostream>
#include <unistd.h>
#include <signal.h>
using namespace std;

BrickPi3 BP;

//Calibrate motors:
const float dps_reduction = 4.5;
const int rpm = 3;
const int motor_dps = 360*rpm;

//Calibrate sensors:
const int line_edge = 1900;
const float collision_distance = 20;

void exit_signal_handler(int signo);
void detectObstacle(sensor_ultrasonic_t Ultrasonic2);
void armMotor(int angle);

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

void followLine(sensor_light_t Light3, sensor_ultrasonic_t Ultrasonic2, sensor_color_t Color1){
	detectCrossing(sensor_color_t Color1);
	detectObstacle(sensor_ultrasonic_t Ultrasonic2);
	
	while(true) {
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

void armMotor(int angle){
    BP.detect();
    BP.offset_motor_encoder(PORT_A, BP.get_motor_encoder(PORT_A));
    BP.set_motor_limits(PORT_A, 50, 200);
    int32_t nul = 0;
    BP.set_motor_position(PORT_A, nul);
    BP.set_motor_position_relative(PORT_A, angle);
}

void detectObstacle(sensor_ultrasonic_t Ultrasonic2){
	if(Ultrasonic2.cm <= 20){
            BP.set_motor_dps(PORT_B, 0);
            BP.set_motor_dps(PORT_C, 0);
            sleep(1);
	}
}

// void detectCrossing(sensor_color_t Color1){
	
// }

void detectMed(sensor_color_t Color1){
	armMotor(-145);
	sleep(1);
	
	bool waiting = true;
    int color;

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
	while(true){
		BP.detect();
		BP.set_sensor_type(PORT_1, SENSOR_TYPE_NXT_COLOR_FULL);
		BP.set_sensor_type(PORT_2, SENSOR_TYPE_NXT_ULTRASONIC);
		BP.set_sensor_type(PORT_3, SENSOR_TYPE_NXT_LIGHT_ON);

		sensor_color_t      Color1;
		sensor_ultrasonic_t Ultrasonic2;
		sensor_light_t      Light3;

		signal(SIGINT, exit_signal_handler);
		
		detectMed(sensor_color_t Color1);
		
		followLine(sensor_light_t Light3, sensor_ultrasonic_t Ultrasonic2, sensor_color_t Color1);
		
	}
}

void exit_signal_handler(int signo){
	if(signo == SIGINT){
		BP.reset_all();
		exit(-2);
	}
}

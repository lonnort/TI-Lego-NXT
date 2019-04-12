#include "BrickPi3.h"
#include <iostream>
#include <unistd.h>
#include <signal.h>
#include <ctime>
using namespace std;

BrickPi3 BP;

// Calibration
const float        dps_reduction = 7;
const unsigned int rpm = 3;
const int          motor_dps = 360 * rpm;

const int		   line_edge = 1900;
const float		   collision_distance = 15;
const int		   cross_sensitivity = 600;

string med_color;

void exit_signal_handler(int signo);

void armMotor(int angle);
void detectCrossing(sensor_color_t Color1);
bool detectObstacle(sensor_ultrasonic_t Ultrasonic2);

// Full speed onward movement
void moveForward(void){
    BP.set_motor_dps(PORT_B, motor_dps);
    BP.set_motor_dps(PORT_C, motor_dps);
}

// Full stop
void fullStop(void){
	BP.set_motor_dps(PORT_B, 0);
    BP.set_motor_dps(PORT_C, 0);
}

// Small increments to the left
void moveLeft(void){
    BP.set_motor_dps(PORT_B, motor_dps);
    BP.set_motor_dps(PORT_C, motor_dps/dps_reduction);
}

// Small increments to the right
void moveRight(void){
    BP.set_motor_dps(PORT_B, motor_dps/dps_reduction);
    BP.set_motor_dps(PORT_C, motor_dps);
}

// 90 degree turn to the left
void turnLeft(void){
	BP.set_motor_dps(PORT_C, (motor_dps * -1));
	BP.set_motor_dps(PORT_B, motor_dps);
	usleep(0.1);
}

// 90 degree turn to the right
void turnRight(void){
	BP.set_motor_dps(PORT_C, motor_dps);
	BP.set_motor_dps(PORT_B, (motor_dps * -1));
	usleep(0.1);
}

// Collision detection
bool detectObstacle(sensor_ultrasonic_t Ultrasonic2){
	BP.get_sensor(PORT_2, Ultrasonic2);
	return (Ultrasonic2.cm <= collision_distance);
	
	// if (Ultrasonic2.cm <= collision_distance) {
	// 	return (false);
	// }else{
	// 	return (true);
	// }
}

// Crossing detection
void detectCrossing(sensor_color_t Color1){
    if(BP.get_sensor(PORT_1, Color1) == 0){
		if(Color1.reflected_red >= cross_sensitivity){
			if(med_color == "red"){
				turnRight();
				cout << "Naar rechts" << endl;
				usleep(0.3);
			}else{
				turnLeft();
				cout << "Naar links" << endl;
				usleep(0.3);
			}
		}
		
		else if(Color1.reflected_red < cross_sensitivity){
			cout << "Lijn volgen..." << endl;
		}
	}
}

// Motorarm aansturing
void armMotor(int angle){
    int32_t nul = 0;

    BP.detect();
    BP.offset_motor_encoder(PORT_A, BP.get_motor_encoder(PORT_A));

    BP.set_motor_limits(PORT_A, 50, 200);
    BP.set_motor_position(PORT_A, nul);
    BP.set_motor_position_relative(PORT_A, angle);
}

// Detect the medication color
void detectMed(sensor_color_t Color1){
	armMotor(-147);
	sleep(2);
	
	bool waiting = true;
    while (waiting){
    	if(BP.get_sensor(PORT_1, Color1) == 0){
			
			// No meds
			if((Color1.reflected_red <= 340) && (Color1.reflected_green <= 300) && (Color1.reflected_blue <= 100)){
				waiting = true;
			}
			
			// Color red detected
    		else if((Color1.reflected_red >= 400) && (Color1.reflected_green <= 300) && (Color1.reflected_blue <= 300)){
				med_color = "red";
				waiting = false;
				break;
			}
			
			// Color yellow detected
    		else if((Color1.reflected_red >= 400) && (Color1.reflected_green >= 350) && (Color1.reflected_blue <= 300)){
				med_color = "yellow";
				waiting = false;
				break;
			}else{
				waiting = true;
			}
        }
    }
    cout << med_color << endl;
    armMotor(130);
    sleep(1);
}

// Line following and monitorig
void followLine(sensor_color_t Color1, sensor_ultrasonic_t Ultrasonic2, sensor_light_t Light3){
	while(true){
		if(detectObstacle(Ultrasonic2)){
			detectCrossing(Color1);
			
			BP.get_sensor(PORT_3, Light3);

            if(Light3.reflected < line_edge) moveRight();
            else if(Light3.reflected > line_edge) moveLeft();
            else if(Light3.reflected == line_edge) moveForward();
		}else{
			fullStop();
		}
	}
}


// Main
int main(){
    system("python3 ./display.py");
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


// Exit signal handler
void exit_signal_handler(int signo){
	if(signo == SIGINT){
		BP.reset_all();
		exit(-2);
	}
}

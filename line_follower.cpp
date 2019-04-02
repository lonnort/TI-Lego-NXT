#include "BrickPi3.h"
#include <iostream>
#include <unistd.h>
#include <signal.h>
#include <cmath>
#include <iomanip>
using namespace std;

BrickPi3 BP;

void exit_signal_handler(int signo);

const int dps_reduction = 5;
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

int main () {
	cout << "Welcome to our software. We take no responibility for broken stuff or people dying.\n";

    signal(SIGINT, exit_signal_handler);
    BP.detect();
    int error;

	BP.set_sensor_type(PORT_3, SENSOR_TYPE_NXT_LIGHT_ON);
	BP.set_sensor_type(PORT_4, SENSOR_TYPE_TOUCH);
	BP.set_sensor_type(PORT_2, SENSOR_TYPE_NXT_ULTRASONIC);

	sensor_ultrasonic_t Ultrasonic2;
	sensor_light_t Light3;
	sensor_touch_t Touch4;

	while(true) {
		BP.get_sensor(PORT_2, Ultrasonic2);
		BP.get_sensor(PORT_4, Touch4);
		BP.get_sensor(PORT_3, Light3);

		cout << Touch4.pressed << endl;
		error = 0;
		
		if(Touch4.pressed == 1) {
			BP.set_motor_dps(PORT_B, 0);
			BP.set_motor_dps(PORT_C, 0);
			break;
		}

		if(Ultrasonic2.cm <= 10) {
			BP.set_motor_dps(PORT_B, 0);
			BP.set_motor_dps(PORT_C, 0);
			sleep(5);
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

		// Voor de cout met de gemeten waarden
		loop++;
		if (loop%50 == 0) {
			loop = 0;
		}

		sleep(wait);
	}
}

void exit_signal_handler(int signo){
        if(signo == SIGINT){
		cout << endl << endl;
                BP.reset_all();
                exit(-2);
        }
}

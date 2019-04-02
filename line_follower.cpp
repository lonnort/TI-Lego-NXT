#include "BrickPi3.h"
#include <iostream>
#include <unistd.h>
#include <signal.h>
#include <cmath>
#include <iomanip>
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

	BP.get_sensor(PORT_2, Ultrasonic2);
	sleep(3);

	while(true) {
		BP.get_sensor(PORT_2, Ultrasonic2);
		BP.get_sensor(PORT_3, Light3);
		error = 0;

		cout << "Battery level: " << BP.get_voltage_battery() << "\r";

		if(Ultrasonic2.cm <= 10) {
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

#include "BrickPi3.h"
#include <iostream>
#include <unistd.h>
#include <signal.h>
using namespace std;

BrickPi3 BP;

void exit_signal_handler(int signo);

int motorPower = 100;

void fwd(void) {
	BP.set_motor_power(PORT_B, motorPower);
	BP.set_motor_power(PORT_C, motorPower);
}

int main () {
	signal(SIGINT, exit_signal_handler);
	BP.detect();
	int error;

	BP.set_sensor_type(PORT_2, SENSOR_TYPE_NXT_COLOR_FULL);
	BP.set_sensor_type(PORT_1, SENSOR_TYPE_NXT_LIGHT_ON);
//	BP.set_sensor_type(PORT_1, SENSOR_TYPE_NXT_ULTRASONIC);

	sensor_color_t		Color1;
	// sensor_ultrasonic_t	Ultrasonic2;
	sensor_light_t		Light3;

	while(true) {
		error = 0;

		if (BP.get_sensor(PORT_1, Light3) == 0) {
			cout << "Light: " << Light3.reflected << endl;
			if (Light3.reflected < 2250) {
				cout << "Going right\n";
		 		BP.set_motor_power(PORT_B, motorPower/3);
		 		sleep(0.001);
		 		fwd();
		 	}
		 	else {
			 	fwd();
			}
		}

		else if (BP.get_sensor(PORT_2, Color1) == 0) {
			cout << "Color: " << (int)Color1.ambient << endl;
			if (Color1.ambient < 130) {
				cout << "Going left\n";
				BP.set_motor_power(PORT_C, motorPower/3);
				sleep(0.001);
				fwd();
		 	}
		 	else {
			 	fwd();
			}
		}
	}
}

void exit_signal_handler(int signo){
	if(signo == SIGINT){
		BP.reset_all();
		exit(-2);
	}
}

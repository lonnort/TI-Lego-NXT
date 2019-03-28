#include "BrickPi3.h"
#include <iostream>
#include <unistd.h>
#include <signal.h>
using namespace std;

BrickPi3 BP;

void exit_signal_handler(int signo);

void fwd(void) {
	BP.set_motor_power(PORT_B, 50);
	BP.set_motor_power(PORT_C, 50);
}

void stop(void) {
	BP.set_motor_power(PORT_B, 0);
	BP.set_motor_power(PORT_C, 0);
}

int main () {
	signal(SIGINT, exit_signal_handler);
	BP.detect();
	int error;

	BP.set_sensor_type(PORT_2, SENSOR_TYPE_NXT_COLOR_FULL);
	BP.set_sensor_type(PORT_3, SENSOR_TYPE_NXT_LIGHT_ON);
	BP.set_sensor_type(PORT_4, SENSOR_TYPE_NXT_ULTRASONIC);

	sensor_color_t		Color1;
	sensor_ultrasonic_t	Ultrasonic2;
	sensor_light_t		Light3;

	while(true) {
		error = 0;

		// if (BP.get_sensor(PORT_4, Light3) == 0 && BP.get_sensor(PORT_3, Color1) == 0 && BP.get_sensor(PORT_3, Ultrasonic2) == 0) {
		// 	if (Color1.color != 1) {
		// 		BP.set_motor_power(PORT_B, 100);
		// 		sleep(1);
		// 		fwd();
		// 	}

		// 	if (Light3.reflected < 2200) {
		// 		BP.set_motor_power(PORT_C, 100);
		// 		sleep(1);
		// 		fwd();
		// 	}

		// 	else {
		// 		fwd();
		// 	}
		// }

		// else {
		// 	// neem aan dat er iets fout gaat dus stop
		// 	stop();
		// }
		fwd();
		sleep(5);
		stop();
		break;
	}
}

void exit_signal_handler(int signo){
	if(signo == SIGINT){
		BP.reset_all();    // Reset everything so there are no run-away motors
		exit(-2);
	}
}
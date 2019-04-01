#include "BrickPi3.h"
#include <iostream>
#include <unistd.h>
#include <signal.h>
using namespace std;

BrickPi3 BP;

void exit_signal_handler(int signo);

void fwd(void){
	BP.set_motor_dps(PORT_B, 100);
	BP.set_motor_dps(PORT_C, 100);
}

void left(void){
	BP.set_motor_dps(PORT_B, 100);
	BP.set_motor_dps(PORT_C, 50);
}

void right(void){
	BP.set_motor_dps(PORT_B, 50);
	BP.set_motor_dps(PORT_C, 100);
}

int main () {
        signal(SIGINT, exit_signal_handler);
        BP.detect();
        int error;

        BP.set_sensor_type(PORT_3, SENSOR_TYPE_NXT_LIGHT_ON);

        sensor_color_t          Color1;
        sensor_ultrasonic_t     Ultrasonic2;
        sensor_light_t          Light3;

	while(true){
		error = 0;

		if(Light3.reflected < 1900){
			right();
		}

		if(Light3.reflected > 1900){
			left();
		}

		if(Light3.reflected == 1900){
			fwd();
		}
	}
}

void exit_signal_handler(int signo){
        if(signo == SIGINT){
                BP.reset_all();    // Reset everything so there are no run-away motors
                exit(-2);
        }
}


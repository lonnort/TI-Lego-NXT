#include "BrickPi3.h"
#include <iostream>
#include <unistd.h>
#include <signal.h>
using namespace std;

BrickPi3 BP;

void exit_signal_handler(int signo);

int dps_reduction = 5;
int rpm = 4;
int wait = 0.001;
int motor_dps = 360*rpm;

void fwd(void){
	cout << "fwd\n";
	BP.set_motor_dps(PORT_B, motor_dps);
	BP.set_motor_dps(PORT_C, motor_dps);
}

void left(void){
	cout << "left\n";
	BP.set_motor_dps(PORT_B, motor_dps);
	BP.set_motor_dps(PORT_C, motor_dps/dps_reduction);
}

void right(void){
	cout << "right\n";
	BP.set_motor_dps(PORT_B, motor_dps/dps_reduction);
	BP.set_motor_dps(PORT_C, motor_dps);
}

int main () {
        signal(SIGINT, exit_signal_handler);
        BP.detect();
        int error;

        BP.set_sensor_type(PORT_3, SENSOR_TYPE_NXT_LIGHT_ON);
	BP.set_sensor_type(PORT_4, SENSOR_TYPE_TOUCH);

	sensor_light_t Light3;
	sensor_touch_t Touch4;

	while(true) {
		BP.get_sensor(PORT_4, Touch4);
		BP.get_sensor(PORT_3, Light3);

		cout << Touch4.pressed << endl;

		if(Touch4.pressed == 1) {
			cout << "stop\n";
			BP.set_motor_dps(PORT_B, 0);
			BP.set_motor_dps(PORT_C, 0);
			break;
		}

		cout << Light3.reflected << endl;;
		error = 0;
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
                BP.reset_all();
                exit(-2);
        }
}


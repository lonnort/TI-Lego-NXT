#include "BrickPi3.h"
#include <iostream>
#include <unistd.h>
#include <signal.h>

BrickPi3 BP;

int32_t angle = 360;

void exit_signal_handler(int signo);

int main() {
//	command = "./display" + (char)angle;
//	system(command);
	signal(SIGINT, exit_signal_handler);

	BP.detect();
	BP.offset_motor_encoder(PORT_A, BP.get_motor_encoder(PORT_A));
	int32_t nul = 0;
	BP.set_motor_position(PORT_A, nul);
	BP.set_motor_position_relative(PORT_A, angle);
}

void exit_signal_handler(int signo) {
	if (signo == SIGINT) {
		BP.reset_all();
		exit(-2);
	}
}

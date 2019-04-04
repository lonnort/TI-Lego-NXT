#include "BrickPi3.h"
#include <iostream>
#include <unistd.h>
#include <signal.h>

BrickPi3 BP;

void exit_signal_handler(int signo);

int main() {
	signal(SIGINT, exit_signal_handler);

	BP.detect();

	BP.set_motor_power(PORT_B, 100);
	sleep(100);
}

void exit_signal_handler(int signo) {
	if (signo == SIGINT) {
		BP.reset_all();
		exit(-2);
	}
}

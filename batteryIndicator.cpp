#include <iostream>
#include <csignal>
#include "BrickPi3.h"

bool RUNNING = true;
BrickPi3 BP;

void exit_handler(int signal) {
	std::cout << "CTRL-C: Exiting\n";
	RUNNING = false;
}

#define OUTPUT 1;
#define INPUT  0;

#define HIGH   1;
#define LOW    0;

void pinMode(int pin, int mode) {
	if (mode == 1) {
		system("gpio mode %i OUTPUT", pin);
	}
	if (mode == 0) {
		system("gpio mode %i INPUT", pin);
	}
}

void digitalWrite(int pin, int val) {
	system("gpio %i %i", pin, val);
}

int main() {
	std::signal(SIGINT, exit_handler);

	int r = 0, y = 2, g = 4;

	pinMode(r, OUTPUT);
	pinMode(y, OUTPUT);
	pinMode(g, OUTPUT);

	while(RUNNING) {
		V = BP.get_voltage_battery();

		// V tussen 10 en 12 Volt
		// r   :         V < 10
		// r,y : 10   <= V < 10.5
		// y   : 10.5 <= V < 11
		// y,g : 11   <= V < 11.5
		// g   : 11.5 <= V

		if (V < 10.0) {
			digitalWrite(r, HIGH);
			digitalWrite(y, LOW);
			digitalWrite(g, LOW);
		}
		else if (10.0 <= V || V < 10.5) {
			digitalWrite(r, HIGH);
			digitalWrite(y, HIGH);
			digitalWrite(g, LOW);
		}
		else if (10.5 <= V || V < 11.0) {
			digitalWrite(r, LOW);
			digitalWrite(y, HIGH);
			digitalWrite(g, LOW);
		}
		else if (11.0 <= V || V < 11.5) {
			digitalWrite(r, LOW);
			digitalWrite(y, HIGH);
			digitalWrite(g, HIGH);
		}
		else if (11.5 <= V) {
			digitalWrite(r, LOW);
			digitalWrite(y, LOW);
			digitalWrite(g, HIGH);
		}
	}
}
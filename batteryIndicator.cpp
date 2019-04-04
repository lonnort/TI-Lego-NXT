#include <unistd.h>
#include <iostream>
#include <fstream>
#include "BrickPi3.h"

bool RUNNING = true;
BrickPi3 BP;

int main() {
	BP.detect();

	while(true) {
		std::ofstream file;
		file.open("batteryVoltage");
		std::cout << BP.get_voltage_battery();
		file << BP.get_voltage_battery();
		file.close();
		sleep(5);
	}
}

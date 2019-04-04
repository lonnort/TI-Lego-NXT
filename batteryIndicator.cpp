#include <string>
#include <iostream>
#include <fstream>
#include "BrickPi3.h"

bool RUNNING = true;
BrickPi3 BP;

int main() {
	while(true) {
		std::ofstream file;
		file.open("batteryVoltage");
		file << BP.get_voltage_battery();
		file.close();
		sleep(5);
	}
}

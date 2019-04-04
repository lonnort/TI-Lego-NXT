#include <unistd.h>
#include <iostream>
#include <fstream>
#include "BrickPi3.h"

bool RUNNING = true;
BrickPi3 BP;

int main() {
	std::cout << "Battery Indicator\n";
	BP.detect();
	float voltage = 0.0;
	while(true) {
		std::ofstream file;
		file.open("batteryVoltage");
		voltage = BP.get_voltage_battery();
//		std::cout << voltage << std::endl;
		file << voltage;
		file.close();
		sleep(5);
	}
}

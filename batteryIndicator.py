#!/usr/bin/env python3

from gpiozero import LED
from time import sleep

red    = LED(17)
yellow = LED(27)
green  = LED(23)

while(True):
	with open("batteryVoltage") as file:
		voltage = int(file.read());

	if voltage < 10:
		red.on()
		yellow.off()
		green.off()
	elif 10 <= voltage < 10.5:
		red.on()
		yellow.on()
		green.off()
	elif 10.5 <= voltage < 11:
		red.off()
		yellow.on()
		green.off()
	elif 11 <= voltage < 11.5:
		red.off()
		yellow.on()
		green.on()
	elif 11.5 <= voltage:
		red.off()
		yellow.off()
		green.on()

	sleep(5)

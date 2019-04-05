#!/usr/bin/env python3

import time
import Adafruit_GPIO.SPI as SPI
import Adafruit_SSD1306
from PIL import Image, ImageDraw, ImageFont
import subprocess

RST = 0
display = Adafruit_SSD1306.SSD1306_128_64(rst=RST)
display.begin()
display.clear()
display.display()

width = display.width
height = display.height

image1 = Image.new('1', (width, height))

draw = ImageDraw.Draw(image1)
draw.rectangle((0, 0, width, height), outline = 0, fill = 0)

padding = -2
top = padding
bottom = height-padding

x = 0
font = ImageFont.truetype('Comic_Sans_MS', 56)

display.clear()
display.display()

while True:
	draw.text((x, top), 'UwU', font=font, fill=255)
	
	display.image(image1)
	display.display()
	time.sleep(2)

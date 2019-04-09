#!/usr/bin/env python3

# This requires pins 1, 3, 5, 6
import time
import sys
import Adafruit_GPIO.SPI as SPI
import Adafruit_SSD1306
from PIL import Image, ImageDraw, ImageFont
import subprocess

text = str(sys.argv[1]) # dit is correct want argv[0] == display.py

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
font = ImageFont.truetype('Comic_Sans_MS', size)

display.clear()
display.display()
draw.text((x, top), text, font=font, fill=255)
display.image(image1)
display.display()

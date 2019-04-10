#!/usr/bin/env python3

# This requires pins 1, 3, 5, 6
import time
import sys
import Adafruit_GPIO.SPI as SPI
import Adafruit_SSD1306
from PIL import Image, ImageDraw, ImageFont

#text = str(sys.argv[1]) # dit is correct want argv[0] == display.py

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

x = 1
font = ImageFont.truetype('Comic_Sans_MS', 13)

display.clear()
display.display()
#draw.text((x, top), text, font=font, fill=255)

draw.text((x, 0),  'Dit is de robot', font=font, fill=255)
draw.text((x, 15), 'van groep A6', font=font, fill=255)
draw.text((x, 30), 'onder leiding', font=font, fill=255)
draw.text((x, 45), 'van onze homeboy A3', font=font, fill=255)

display.image(image1)
display.display()

time.sleep(3)

display.clear()
display.display()

image1 = Image.new('1', (width, height))
draw = ImageDraw.Draw(image1)
font = ImageFont.truetype('Comic_Sans_MS', 24)
draw.text((x, 0), 'Ik ben', font=font, fill=255)
draw.text((x, 26), 'Gert', font=font, fill=255)

display.image(image1)
display.display()

time.sleep(1)

display.clear()
display.display()

image1 = Image.new('1', (width, height))
draw = ImageDraw.Draw(image1)
font = ImageFont.truetype('Comic_Sans_MS', 48)
draw.text((x, 0), 'UwU', font=font, fill=255)

display.image(image1)
display.display()

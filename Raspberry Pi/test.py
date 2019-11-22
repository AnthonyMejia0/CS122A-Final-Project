#!/usr/bin/env python

import RPi.GPIO as GPIO
import spidev
from  mfrc522 import SimpleMFRC522
from os import system, name
from time import sleep

def clear():
	_ = system('clear')

def createSPI(device):
	spi = spidev.SpiDev()
	spi.open(0, device)
	spi.max_speed_hz = 1000000
	spi.mode = 0
	return spi


reader = SimpleMFRC522()

try:
	while True:
		clear()
		print("Waiting for tag...")
		id, text = reader.read()
		newText = text.replace(" ", "")
		if not newText:
			print("Tag not recognized")
			sleep(3)
		elif newText == "Master":
			pin = input("Enter 4 digit pin for new card or Enter to remove a card: ")
			pin = pin.replace(" ", "")
			if  (len(pin) == 4 and pin.isdigit()) or (pin == ""):
				print("Scan card to set pin...")
				id, text = reader.read()
				newText = text.replace(" ", "")
				if newText != "Master":
					reader.write(pin)
					print("Pin code written to tag")
					sleep(3)
				else:
					print("ERROR: Master Tag")
					sleep(3)
			else:
				print("ERROR: Invalid pin")
				sleep(3)
		else:
			pin = input("Enter pin code: ")
			if newText == pin:
				print("Access Granted")
				sleep(3)
			else:
				print("Invalid Pin!")
				sleep(3)
except KeyboardInterrupt:
	GPIO.cleanup()

#!/usr/bin/env python

import RPi.GPIO as GPIO
import spidev
from  mfrc522 import SimpleMFRC522
from serial import Serial
from os import system, name
from time import sleep

def clear():
	_ = system('clear')


reader = SimpleMFRC522()
atmega = Serial("/dev/ttyS0", 9600)

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
			print("Tag Recognized")
			atmega.write(int(9))
			sleep(3)
except KeyboardInterrupt:
	GPIO.cleanup()

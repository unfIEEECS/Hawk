import RPi.GPIO as GPIO
import time
#Pin Definitions
goButtonPin = 17 # Broadcom pin 17 (P1 pin 11)
#Pin Setup
GPIO.setup(goButtonPin, GPIO.IN, pull_up_down=GPIO.PUD_UP) # Button pin set as input w/ pull-up
#Variables
userState = 0
print "Here we go! Press CTRL+C to exit"
try:
    while 1:
        if GPIO.input(goButtonPin): # Go button is released
			print "Go button Released"
        else: # Go button is pressed
			#Start
			if userState == 0 :
				print "Started"
				userState += 1
			#Pause
			elif userState == 1:
				print "Paused"
				userState += 1
			#Unpause
			elif userState == 2:
				print "Unpaused"
				userState -= 1
except KeyboardInterrupt: # If CTRL+C is pressed, exit cleanly:
    GPIO.cleanup() 
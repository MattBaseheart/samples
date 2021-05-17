import RPi.GPIO as GPIO
from time import sleep

GPIO.setmode(GPIO.BCM)

GPIO.setup(17, GPIO.IN, pull_up_down=GPIO.PUD_DOWN)

while True:
    if (GPIO.input(17)):
        print("pressed!")
    else:
        print("not pressed!")

    sleep(0.2)
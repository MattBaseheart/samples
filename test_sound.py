import time
import grovepi

mic_pin = 0
led_pin = 5

grovepi.pinMode(mic_pin, "INPUT")
grovepi.pinMode(led_pin, "OUTPUT")

thresh = 400

while True:
	time.sleep(1)
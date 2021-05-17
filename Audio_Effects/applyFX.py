from pysndfx import AudioEffectsChain
#from playsound import playsound
import datetime
import time
#from pydub import AudioSegment
from queue import Queue
from threading import Thread
import pygame
from pygame.locals import *
import os
import serial
import json
import sys
from pynodered import node_red
#
#SERIAL COMMS THREAD

#@node_red(category="pyfuncs")
#def getMess(node, msg):
#    msg['payload'] = str(msg['payload'])
#    print(msg['payload'])
#    return msg

LPF = 16100
REVERB = 0
PHASER = 0.1
TREM_FREQ = 1
TREM_DEPTH = 1
PITCH = 0
HPF = 20
BASSBOOST = 0

def receiving(ser):
	global last_received
	input_str = ser.readLine()
	last_received = input_str.decode('utf-8').strip()
#	while True:
 #       	if (ser.inWaiting() > 0):
  #          		buffer += ser.readline().decode('utf-8').rstrip()
  #          		if '\n' in buffer:
   #             		last_received, buffer = buffer.split('\n')[-2:]

def producer(out_q, check_me):
	#SET INITIAL FX CONDITIONS
	global LPF #100-15k (15k is max distance)
	global REVERB #0-100 (0 is max distance)
	global PHASER #0.1 - 2 (0.1 is max distance?)
	global TREM_FREQ #1-10 (1 is max distance)
	global TREM_DEPTH #0-100, 0 is max distance
	global PITCH #0-1000, 0 is max distance
	global HPF #20-15k, max value is 20
	global BASSBOOST #0-15 for reg boost, 0-50 if we want to DEEP FRY the audio
	global last_received
	#SET INDEX OF FIRST FILE NAME
	x = 0
	time.sleep(4)

	data = None

	while True:
		#TODO GET SENSOR VALS-------------------------------------------------------
		while not last_received:
            		continue
		#last_received = getMess(node,msg)
		# print("Got data in producer thread")
		# print(last_received)
		data = json.loads(last_received)
		# if data:
		LPF = (data['distance1'])* (15000/100) + 1
		REVERB = data['distance2']
		TREM_FREQ =(data['distance3']) * (10/100) + 1
		TREM_DEPTH = data['distance4'] + 1
		PITCH = (data['distance5']) * 10
		HPF = 17001 - (data['distance6']) * (17000/100)
		BASSBOOST = (data['distance7']) * (15/100)
		PHASER = (data['distance8']) * (0.1/100) + 0.1
		#---------------------------------------------------------------------------
		PITCH = PITCH + 100
		if(PITCH == 1100):
			PITCH = 0
		fx = (AudioEffectsChain().reverb(REVERB).lowpass(LPF).phaser(0.9, 0.8, 1, 0.75, PHASER).tremolo(TREM_FREQ, TREM_DEPTH).pitch(PITCH).highpass(20).equalizer(100, 0.1, BASSBOOST))
		fx(infile[x], outfile[x])
		out_q.put(outfile[x])

		#UPDATE INDEX OF NEXT FILE NAME
		x = x + 1
		if(x == 8):
			x = 0

		# if last_received:
			# data = json.loads(last_received)

		#WAIT HERE UNTIL CONSUMER HAS READ THE LAST FILE
		while(check_me.get() != "GO AHEAD"):
			continue

def consumer(in_q, check_me):
	#WAIT FOR FIRST FILE TO BE GENERATED
	while(in_q.qsize() == 0):
			continue
	data = in_q.get()
	pygame.init()
	pygame.mixer.pre_init(44100, 16, 2, 4096)
	pygame.mixer.music.load(data)
	while True:
		while(pygame.mixer.music.get_busy() == True):
			continue
		#IMMEDIATELY PLAY FIRST PIECE OF DATA, NONBLOCKING
		pygame.mixer.music.play()
		pygame.time.wait(1000)

		# print("Playing music in consumer thread")

		#TELL PRODUCER TO START GENERATING THE NEXT FILE
		check_me.put("GO AHEAD")

		#WHILE WAITING ON NEXT FILE TO BE GENERATED
		while(in_q.qsize() == 0):
			continue

		#GET NEXT FILE ONCE ITS READY
		data = in_q.get()
		pygame.mixer.music.load(data)

#MAIN
global infile
infile = ['1.wav', '2.wav', '3.wav', '4.wav', '5.wav', '6.wav', '7.wav', '8.wav']
global outfile
outfile = ['/home/pi/TeenageJeanetteMakesaGarden/Synth/1p.wav', '/home/pi/TeenageJeanetteMakesaGarden/Synth/2p.wav', '/home/pi/TeenageJeanetteMakesaGarden/Synth/3p.wav', '/home/pi/TeenageJeanetteMakesaGarden/Synth/4p.wav', '/home/pi/TeenageJeanetteMakesaGarden/Synth/5p.wav', '/home/pi/TeenageJeanetteMakesaGarden/Synth/6p.wav', '/home/pi/TeenageJeanetteMakesaGarden/Synth/7p.wav', '/home/pi/TeenageJeanetteMakesaGarden/Synth/8p.wav']
global last_received
last_received = ''
os.getcwd()
q = Queue()
check = Queue()
#UNCOMMENT THESE SERIAL LINES to accpept serial comms and start the thread===============
ser = serial.Serial(port='/dev/ttyACM0', baudrate=9600, timeout=5)

t1 = Thread(target = consumer, args = (q,check,  ))
t2 = Thread(target = producer, args = (q,check,  ))
t1.start()
t2.start()

secs_cnt = 0

while True:
	input_str = ser.readline()
	last_received = input_str.decode('utf-8').strip()
	secs_cnt += 1
	if (secs_cnt == 64):
		LPF = 16100
		REVERB = 0
		PHASER = 0.1
		TREM_FREQ = 1
		TREM_DEPTH = 1
		PITCH = 0
		HPF = 20
		BASSBOOST = 0
		
	#if last_received:
		#print(last_received)

# t0 = Thread(target = receiving, args=(ser,)).start()
# #-----------------------------------------------------------------------------------------











#SCRATCH WORK/USEFUL DEBUG/TEST LINES

#Testing Input
#rlevel = (int)(input("Reverb level: "))
#bplo = (int)(input("Bandpass Low Freq: "))
#bphi = (int)(input("Bandpass High Freq: "))
#lp = (int)(input("Lowpass: "))
#eqf = (int)(input("EQ Boost Freq: "))
#dlevel = (int)(input("Delay level: "))

#Timing
#start = datetime.datetime.now()
#print(datetime.datetime.now() - start)

#MIXING 4 PARTS TOGETHER - this works but takes too long, wanted buffer to remain fairly small
#Mix 4 parts with volume
#inst1 = AudioSegment.from_file(pad[x])
#inst2 = AudioSegment.from_file(glock[x])
#inst3 = AudioSegment.from_file(marimba[x])
#inst4 = AudioSegment.from_file(frogs[x])
#comb1 = inst1.overlay(inst2)
#comb2 = comb1.overlay(inst3)
#comb3 = comb2.overlay(inst4)
#comb3.export(infile[x], format = 'wav')

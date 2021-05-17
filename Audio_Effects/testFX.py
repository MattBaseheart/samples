from pysndfx import AudioEffectsChain
from playsound import playsound
import datetime
import time
from pydub import AudioSegment
#from omxplayer.player import OMXPlayer
from queue import Queue
from threading import Thread
#import pygame
#from pygame.locals import 

print("ok")

#REVERB
#SET 0 FOR NO REVERB! ) to 100 MAPPING EASY
fx = (AudioEffectsChain().reverb(80))
fx('1.wav', 'test_reverb.wav')

#LPF SLIDE FROM 20HZ TO 15k - val * ((15k-20)/100)
fx = (AudioEffectsChain().lowpass(700))
fx('1.wav', 'test_lpf.wav')

#PHASER___ HOW TO CONTROL-------------------------------------- last is speed?? between 0.1 and 2-------- WORKS - MAP SPEED 0.1 to 2
fx = (AudioEffectsChain().phaser(0.9, 0.8, 1, 0.75, 0.1)) 
fx('1.wav', 'test_phaser.wav')

#TREMOLO-------------------------- first is frequency second is depth to 100 !! works CAN MAP ONE SENSOR 0-10 for FREQ, 0-100 another for depth
fx = (AudioEffectsChain().tremolo(10, 50))
fx('1.wav', 'test_tremolo.wav')

#PITCH SHIFT??? CAN GO 0 to 1000
fx = (AudioEffectsChain().pitch(0)) 
fx('1.wav', 'test_pitch.wav')

#HPF SLIDE FROM 20HZ TO 15k - val * ((15k-20)/100)
fx = (AudioEffectsChain().highpass(4000))
fx('1.wav', 'test_hpf.wav')

#EQ (Bass boost)- CANT BE 0 FREQ 0-15 db for normal 0-50 for DEEP FRIED
fx = (AudioEffectsChain().equalizer(100, 0.1, 0))
fx('1.wav', 'test_bassBoost.wav')

#DELAY HOW TO CONTROL++++++++++++++++++++++++++++++++++++ EXTENDS THE CLIP!! NEED TO KEEP IT SAME LENGTH
#fx = (AudioEffectsChain().delay(0.8, 0.5))
#fx('1.wav', 'test_delay.wav')

#Chorus
#fx = (AudioEffectsChain().chorus(0.8, 0.6, (1, 2, 4)))
#fx('1.wav', 'test_flange.wav')

#VOL?

#SCRATCH WORK

#Testing Input, comment out once code is recieving values from server
#rlevel = (int)(input("Reverb level: "))
#bplo = (int)(input("Bandpass Low Freq: "))
#bphi = (int)(input("Bandpass High Freq: "))
#lp = (int)(input("Lowpass: "))
#eqf = (int)(input("EQ Boost Freq: "))
#dlevel = (int)(input("Delay level: "))

#Time code if needed
#start = datetime.datetime.now()
#print(datetime.datetime.now() - start)

#OUTFILE AND INFILE are the 4 part mixes, update these as we go along
#Mix 4 parts with volume
#inst1 = AudioSegment.from_file(pad[x])
#inst2 = AudioSegment.from_file(glock[x])
#inst3 = AudioSegment.from_file(marimba[x])
#inst4 = AudioSegment.from_file(frogs[x])

#comb1 = inst1.overlay(inst2)
#comb2 = comb1.overlay(inst3)
#comb3 = comb2.overlay(inst4)

#comb3.export(infile[x], format = 'wav')


#fx = AudioEffectsChain().reverb(50, 100)) #REVERB
#fx = AudioEffectsChain() .sinc(500, 2000)) #SINC is a bandpass or band reject
#fx = AudioEffectsChain().lowpass(4000) #lowpass filter
#fx = AudioEffectsChain().equalizer(300) #boosts one frequency range
#fx = AudioEffectsChain().delay() #unsure of how to change delay parameters
#Above are effects that can be used to manilupate parts

#4 alternate parameters to volume control
#Tremolo, pitch shift, Flanger, master volume

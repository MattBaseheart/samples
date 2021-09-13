from pysndfx import AudioEffectsChain
from playsound import playsound
import datetime
import time
from pydub import AudioSegment
from queue import Queue
from threading import Thread

#REVERB
#SET 0 FOR NO REVERB! ) to 100 MAPPING EASY
fx = (AudioEffectsChain().reverb(80))
fx('1.wav', 'test_reverb.wav')

#LPF SLIDE FROM 20HZ TO 15k - val * ((15k-20)/100)
fx = (AudioEffectsChain().lowpass(700))
fx('1.wav', 'test_lpf.wav')

#PHASER - last is speed between 0.1 and 2
fx = (AudioEffectsChain().phaser(0.9, 0.8, 1, 0.75, 0.1)) 
fx('1.wav', 'test_phaser.wav')

#TREMOLO - first is frequency second is depth to 100 !! works CAN MAP ONE SENSOR 0-10 for FREQ, 0-100 another for depth
fx = (AudioEffectsChain().tremolo(10, 50))
fx('1.wav', 'test_tremolo.wav')

#PITCH SHIFT -  CAN GO 0 to 1000
fx = (AudioEffectsChain().pitch(0)) 
fx('1.wav', 'test_pitch.wav')

#HPF SLIDE FROM 20HZ TO 15k - val * ((15k-20)/100)
fx = (AudioEffectsChain().highpass(4000))
fx('1.wav', 'test_hpf.wav')

#EQ (Bass boost)- CANT BE 0 FREQ 0-15 db 
fx = (AudioEffectsChain().equalizer(100, 0.1, 0))
fx('1.wav', 'test_bassBoost.wav')


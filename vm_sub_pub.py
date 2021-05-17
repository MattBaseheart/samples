"""Security Sound Sensor
   Read the ReadMe File"""

import paho.mqtt.client as mqtt
import time
from pynput import keyboard
import json
import datetime
import matplotlib
import matplotlib.pyplot as plt
import numpy as np
import os

global audio_points
global time_points

filename = "audio_log_file.txt"
audio_points = []
time_points = []

def write_to_file(x,y,filename):
    global THRESH

    data_points = [x, y]
    data_points_t = np.array(data_points).T
    length = len(x)

    #WRITE THE ARRAYS OF DATA POINTS TO THE LOG FILE

    with open(filename, "w") as audio_log_file:
    #   audio_log_file.write(np.array2string(data_points_t, separator="\t"))

 	     #Always starts and ends the plotting on Thresholds
 	    addX = (data_points_t[0][0]) - .0000001
 	    audio_log_file.writelines("{x} {y}\n".format(x=addX,y=THRESH))

 	    for i in range(length):
 	    	if(i != 0):
 	    		addX = data_points_t[i][0]
 	    		addY = data_points_t[i][1]
 	    		audio_log_file.writelines("{x} {y}\n".format(x=addX,y=addY))

 	    addX2 = (data_points_t[length-1][0]) + .0000001
 	    audio_log_file.writelines("{x} {y}\n".format(x=addX2,y=THRESH))
       

def audio_callback(client, userdata, message):
    global audio_points
    global time_points
    
    #APPEND NEW DATA POINTS TO THE ARRAYS
    val = str(message.payload, "utf-8")
    data = json.loads(val)
    print("ALARM : SOUND LEVEL OF " + str(data["sound_val"]) + " AT TIME " + str(data["timestamp"]) +".")
    audio_points.append(data['sound_val'])
    time_points.append(data['time_in_secs'])

def on_connect(client, userdata, flags, rc):
    print("Connected to server (i.e., broker) with result code "+str(rc))

    #SUBSCRIBE
    client.subscribe("192.168.4.53/security_audio")
    client.message_callback_add("192.168.4.53/security_audio", audio_callback)

def on_disconnect(client, userdata, rc):
    print("Disconnected from server (i.e., broker) with result code "+str(rc))

def on_message(client, userdata, msg):
    print("on_message: " + msg.topic + " " + str(msg.payload, "utf-8"))

def on_press(key):
    try:
        k = key.char # single-char keys
    except: 
        k = key.name # other keys
    
    #A = ACTIVE
    if k == 'a':
        client.publish("192.168.4.53/security_status", "STATUS_ON")
        print("          [    ACTIVE    ]          \n-----------------------------------")
    
    #I = INACTIVE
    elif k == 'i':
        client.publish("192.168.4.53/security_status", "STATUS_OFF")
        print("          [   INACTIVE   ]          \n-----------------------------------")
    
    #P = PLOT DATA
    elif k == "p":
        client.publish("192.168.4.53/security_status", "STATUS_OFF")
        print("          [  PLOT  DATA  ]          \n-----------------------------------")
        global time_points
        global audio_points
        
        if(len(audio_points)>0):
	        write_to_file(time_points, audio_points, filename)
	        plot_data_from_file()

    #E = EXIT PROGRAM
    elif k == "e":
        print("          [     EXIT     ]          \n-----------------------------------")
        plt.close()
        global end_Condition
        end_Condition = 1
        

def plot_data_from_file():
    #PLOT THE AUDIO AND TIME STAMP DATA FROM THE UPDATED LOG FILE
    X, xsecs, yval = [], [], []

    #Opens the file
    for line in open("audio_log_file.txt", "r"):
        values = [entry for entry in line.split()]
        xsecs.append(float(values[0]))
        yval.append(float(values[1]))
        addX = time.ctime(float(values[0]))
        X.append(addX)

    xsize = len(xsecs)
    ysize = len(yval)

    #sets the limit of the axis
    if (xsize>0):
        xmax = max(xsecs)
        ymax = max(yval)
        ymin = min(yval)

        if xsize<3600:
            xmin = min(xsecs)
        else:
            delta_t = 3600 # 1hour
            xmin = xmax - delta_t

        stepsize = (xsize/100)
        
        global first_plot
        
        #closes the figure every time we plot
        if(first_plot != 0):
            plt.close()
        
        #creates new figure
        plt.figure(figsize=(10,50))
        first_plot = 1

        #plot the data
        fig = plt.plot(xsecs, yval)
        ax = fig.axes
        plt.title("Audio Activity")
        plt.xlabel("Timestamp")
        plt.ylabel("Noise Level(0-1023) THRES = 450")
        
        #Rename Xticks labels
        locs, label = plt.xticks()
        new_xticks = []
        
        for i in range(xsize):
        	new_xticks[i] = X[i]
        ax.set_xticklabels(new_xticks)
        #plt.xticks(locs, new_xticks, rotation=45)

        #sets y and x axis limitations
        plt.ylim(ymin,ymax)
        plt.xlim(xmin, xmax)

        #saves the figure to a file
        plt.savefig("log_file.png")

        plt.draw()
        plt.pause(0.05)

    else:
        print("No Data Available")

if __name__ == '__main__':
     #setup the keyboard event listener
    lis = keyboard.Listener(on_press=on_press)
    lis.start() # start to listen on a separate thread
    
    #CONNECT TO MQTT Broker
    client = mqtt.Client()
    client.on_message = on_message
    client.on_connect = on_connect
    client.on_disconnect = on_disconnect
    client.connect(host="test.mosquitto.org", port=1883, keepalive=60)
    client.loop_start()

    print("          [    CONNECTED   ]          \n-----------------------------------")

    #initialize global variables
    global THRESH				#Analog input THRESHOLD value (set by RPi)		
    global first_plot			#Flag for replotting the data
    global end_Condition		#flag for terminating the program

    THRESH = 400
    first_plot = 0
    end_Condition = 0

    while (end_Condition == 0):
        time.sleep(0.1)

    client.disconnect()
    print("          [  DISCONNECTED  ]          \n-----------------------------------")

    #Terminate the program
    os._exit(0)

   
"""EE 250L Lab 04 Starter Code
{Name: Vince Espiritu, USCID: 1472908880, Email:vespirit@usc.edu}
{Name: Matt Baseheart, USCID: 5992473415, Email:mattbasehear@usc.edu}
Run rpi_pub_and_sub.py on your Raspberry Pi."""

import paho.mqtt.client as mqtt
import time
import grovepi
import grove_rgb_lcd as lcd
import json

MIC_PIN = 0
LED_PIN = 5
LED_STATUS = 6
THRESH = 400
global FLAG_SWITCH 

def status_callback(client, userdata, message):
    #the third argument is 'message' here unlike 'msg' in on_message 
    global FLAG_SWITCH
    
    if (str(message.payload, "utf-8") == "STATUS_ON"):
        grovepi.digitalWrite(LED_STATUS, 1)
        #turn on continuous recording and data sending
        FLAG_SWITCH = 1
        print(str(message.payload, "utf-8"))
        

    elif (str(message.payload, "utf-8") == "STATUS_OFF"):
        grovepi.digitalWrite(LED_STATUS, 0)

        #turn off continuous recording and data sending
        FLAG_SWITCH = 0
        print("STATUS = " + str(FLAG_SWITCH))
  

def on_connect(client, userdata, flags, rc):
    print("Connected to server (i.e., broker) with result code "+str(rc))

    #subscribe to topics of interest here
    client.subscribe("192.168.4.53/security_status")
    client.message_callback_add("192.168.4.53/security_status", status_callback)

#Default message callback. Please use custom callbacks.
def on_message(client, userdata, msg):
    print("on_message: " + msg.topic + " " + str(msg.payload, "utf-8"))
    

if __name__ == '__main__':
    #this section is covered in publisher_and_subscriber_example.py
    client = mqtt.Client()
    client.on_message = on_message
    client.on_connect = on_connect
    client.connect(host="test.mosquitto.org", port=1883, keepalive=60)
    client.loop_start()

    grovepi.pinMode(MIC_PIN, "INPUT")
    grovepi.pinMode(LED_PIN, "OUTPUT")
    grovepi.pinMode(LED_STATUS, "OUTPUT")

    global FLAG_SWITCH
    FLAG_SWITCH = 0

    data = {"sound_val": 0 , "timestamp": 0, "time_in_secs": 0}


    time.sleep(1)


    while True:
        
        if(FLAG_SWITCH):
            data["sound_val"] = grovepi.analogRead(MIC_PIN)
            sectime = time.time()
            data["time_in_secs"] = sectime
            data["timestamp"] = time.ctime(sectime)

            #if(data.get("sound_val") >THRESH): #made threshold zero for easier testing
            if(data.get("sound_val") >0):
                grovepi.digitalWrite(LED_PIN, 1)
                data_send = json.dumps(data)
                client.publish("192.168.4.53/security_audio", data_send)
            else:
                grovepi.digitalWrite(LED_PIN, 0)

            print(data)
            time.sleep(0.01)
        else:
           # print("STATUS OFF")
            time.sleep(1)
    
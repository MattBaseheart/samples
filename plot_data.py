import time
import datetime
import matplotlib.pyplot as plt
import numpy as np

time_interval = 0.5 #500ms


if __name__ == '__main__':

	

	audio_points = []
	time_points = []
	i=0
	addY = 10

	with open("audio_log_file.txt", "w") as audio_log_file:

		#audio_log_file = open("audio_log_file.txt", "w")
		while i<50:
			
			addY= addY + 10
			addX = time.time()
			print(addX)
			
			time_points.append(addX)
			audio_points.append(addY)

			audio_log_file.writelines("{x} {y}\n".format(x=addX,y=addY))

			i = i + 1

	print("Audio points: {} ".format(audio_points))
	print("time points: {}".format(time_points))

	data_points = [time_points, audio_points]
	data_points_t = np.array(data_points).T
	#data_points_t = np.transpose(data_points)

	print( time.ctime(data_points_t[0,0]))

	

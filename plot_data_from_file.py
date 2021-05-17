import matplotlib.pyplot as plt
import numpy as np
import time
import os
import sys

X, xsecs, yval = [], [], []


for line in open("audio_log_file.txt", "r"):
	values = [entry for entry in line.split()]

	xsecs.append(float(values[0]))
	yval.append(float(values[1]))

	addX = time.ctime(float(values[0]))
	X.append(addX)

xsize = len(xsecs)
ysize = len(yval)

print(yval)
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
	
	plt.figure(figsize=(10,50))
	plt.plot(xsecs, yval)
	plt.title("Audio Activity")
	plt.xlabel("Timestamp")
	plt.ylabel("Noise Level(0-1023) THRES = 450")
	
	locs, label = plt.xticks()

	new_xticks = X
	plt.xticks(locs, new_xticks, rotation=45)

	plt.ylim(ymin,ymax)
	plt.xlim(xmin, xmax)
	plt.savefig("log_file.png")
	plt.show()
	
	

else:
	print("No Data Available")



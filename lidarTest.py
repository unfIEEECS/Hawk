from breezylidar import URG04LX
from pylab import *
import math
import numpy as np
import sys
from time import time

lidarMin = 114
lidarMax = 568

nearMin = 158
nearMax = 524

farMin = 318
farMax = 364

stepDeg = 682/240
degStep = 240/682

lidarPort = '/dev/ttyACM0'
laser = URG04LX(lidarPort)

failCount = 0
print('Started Lidar\n===================')
data = []
for i in range(0,682):
    data.append(0)

for i in range(0,5):
    scan = laser.getScan()
    for j in range(0,len(scan)):
        data[j] += scan[j]
for j in range(0,len(scan)):
    data[j] /= 5

farSum = 0
nearSum = 0
sumT = 0
for i in range(lidarMin, lidarMax):
    if(data[i] > 0):
        print(str(i)+" : "+str(data[i])+" mm")
    else:
        failCount += 1
    #far
    if(i < farMax and i > farMin):
        farSum += data[i]
    if(i < nearMax and i > nearMin):
        nearSum += data[i]
    sumT += data[i]

conversion = 25.4

farAvg = farSum/(farMax-farMin)/conversion
nearAvg = nearSum/(nearMax-nearMin)/conversion
avg = sumT/(lidarMax-lidarMin)/conversion

print('Got Lidar Scan Points : '+ str(len(data)))
print('Failed Scan Points : '+ str(failCount))
print('Far Average : '+ str(farAvg) + ' in')
print('Near Average : '+ str(nearAvg) + ' in')
print('Total Average : '+ str(avg) + ' in')

def plotPoints(scanPoints):
    ax = plt.subplot(111, projection='polar')
    steps = 682
    fov = math.radians(240)
    deg = []
    for i in range(0,steps):
        stp = fov/steps*i-math.radians(30)
        deg.append(stp)
    c = scatter(deg, scanPoints, c=deg, s=30, cmap=cm.hsv)
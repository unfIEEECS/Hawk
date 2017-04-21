import cv2
import numpy as np
from matplotlib import pyplot as plt
def nothing(x):
    pass
# Create a black image, a window
cap = cv2.VideoCapture(0)
_, img = cap.read()
cv2.namedWindow('image')
median = cv2.medianBlur(img,5)
clean = cv2.bilateralFilter(img,5,50,50)

hsv = cv2.cvtColor(clean, cv2.COLOR_BGR2HSV)
hist = cv2.calcHist( [hsv], [0, 1], None, [180, 256], [0, 180, 0, 256] )
lowerBlue = np.array([110,0,0])
upperBlue = np.array([150,255,255])
mask = cv2.inRange(hsv, lowerBlue, upperBlue)
res = cv2.bitwise_and(clean,clean, mask= mask)

cv2.imshow('src',img)
plt.imshow(hist,interpolation = 'nearest')
cv2.imshow('mask',mask)
cv2.imshow('result', res)
plt.show()

cv2.waitKey(0)
cv2.destroyAllWindows()
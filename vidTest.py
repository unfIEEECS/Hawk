import cv2
import numpy as np

cap = cv2.VideoCapture(0)
while(True):
	_, frame = cap.read()
	hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
	k = cv2.waitKey(5) and 0xFF
	#Windows
	cv2.imshow('Original',frame)
	cv2.imshow('HSV',hsv)
	
	if k == 27:
		break
cv2.destroyAllWindows() 

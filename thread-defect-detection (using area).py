# -*- coding: utf-8 -*-
"""
Created on Tue Dec 10 09:36:42 2019

@author: Anshit Vishwakarma
"""
import cv2 
import numpy as np 
from matplotlib import pyplot as plt
  
img = cv2.imread('12-11-2019_11_33_30img_11.jpg')
gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
ret, thresh = cv2.threshold(gray, 15, 255, cv2.THRESH_BINARY_INV)

image, contours, hierarchy = cv2.findContours(thresh, cv2.RETR_TREE,cv2.CHAIN_APPROX_SIMPLE)

areas=[]
for c in contours:
    a=cv2.contourArea(c)
    areas.append(a)
    
maxpos = areas.index(max(areas))
cnt=contours[maxpos]

mult = 1.2   # I wanted to show an area slightly larger than my min rectangle set this to one if you don't

rect = cv2.minAreaRect(cnt)
box = cv2.boxPoints(rect)
box = np.int0(box)
#cv2.drawContours(img, [box], 0, (0,255,0), 2) # this was mostly for debugging you may omit

W = rect[1][0]
H = rect[1][1]

Xs = [i[0] for i in box]
Ys = [i[1] for i in box]
x1 = min(Xs)
x2 = max(Xs)
y1 = min(Ys)
y2 = max(Ys)

rotated = False
angle = rect[2]

if angle < -45:
    angle+=90
    rotated = True

center = (int((x1+x2)/2), int((y1+y2)/2))
size = (int(mult*(x2-x1)),int(mult*(y2-y1)))

M = cv2.getRotationMatrix2D((size[0]/2, size[1]/2), angle, 1.0)

cropped = cv2.getRectSubPix(img, size, center)    
cropped = cv2.warpAffine(cropped, M, size)

croppedW = W if not rotated else H 
croppedH = H if not rotated else W

croppedRotated = cv2.getRectSubPix(cropped, (int(croppedW*mult), int(croppedH*mult)), (size[0]/2, size[1]/2))

#cv2.line(croppedRotated,(65,493),(65,0),(255,255,255),90)

left_image = croppedRotated[:, 0:int(croppedRotated.shape[1]/2)]
right_image = croppedRotated[:, int(croppedRotated.shape[1]/2):croppedRotated.shape[1]]

R_edged = cv2.Canny(right_image, 30, 200) 
_,contours,_ = cv2.findContours(R_edged, cv2.RETR_TREE,cv2.CHAIN_APPROX_SIMPLE)

e=0.0001
for cont in contours:
    a=cv2.contourArea(cont)
    if(a<5):
        M = cv2.moments(cont)
        cX= int(M["m10"] / (M["m00"]+e))
        cY= int(M["m01"] / (M["m00"]+e))
        cv2.circle(right_image, (cX,cY), 10, (0,255,0), -1)

plt.figure(figsize=(20,10))
plt.imshow(croppedRotated)
plt.show()
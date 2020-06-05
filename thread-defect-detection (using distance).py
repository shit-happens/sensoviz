# -*- coding: utf-8 -*-
"""
Created on Tue Dec 10 12:04:50 2019

@author: Anshit Vishwakarma
"""
import cv2 
import numpy as np 
from matplotlib import pyplot as plt
  
img = cv2.imread('flat_thread_defect_part1_img1.png')
gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
gray = 255-gray
ret, thresh = cv2.threshold(gray, 225, 255, cv2.THRESH_BINARY_INV)
image, contours, hierarchy = cv2.findContours(thresh, cv2.RETR_TREE,cv2.CHAIN_APPROX_SIMPLE)
#img = cv2.drawContours(img, contours, -1,(255,0,0),2)
contours.remove(contours[0])

areas=[cv2.contourArea(c) for c in contours]
    
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
#cv2.circle(img, center, 10, (0,255,0), -1) #again this was mostly for debugging purposes

M = cv2.getRotationMatrix2D((size[0]/2, size[1]/2), angle, 1.0)

cropped = cv2.getRectSubPix(img, size, center)    
cropped = cv2.warpAffine(cropped, M, size)

croppedW = W if not rotated else H 
croppedH = H if not rotated else W


croppedRotated = cv2.getRectSubPix(cropped, (int(croppedW*mult), int(croppedH*mult)), (size[0]/2, size[1]/2))
#croppedRotated  = croppedRotated[110:450, 0:130]
upper_image = croppedRotated[0:82, :]
lower_image = croppedRotated[82:164, :]

upper_image = upper_image[:, 95:450]


edged = cv2.Canny(upper_image, 30, 200)
_, contours, _ = cv2.findContours(edged, cv2.RETR_TREE,cv2.CHAIN_APPROX_SIMPLE)
cv2.drawContours(upper_image, contours, -1,(255,0,0),1)

cv2.line(upper_image,(355,18),(0,18),(0,255,255),1)
'''
lower_limit=52
dist=[]
for c in contours:
    for p in c:
        d=65-p[0][0]
        center=(p[0][0],p[0][1])
        print(d)
        if(d>lower_limit):
            cv2.circle(left_image, center, 2, (0,0,255), -1)
        
'''

plt.figure(figsize=(20,10))
plt.imshow(upper_image)
plt.show()
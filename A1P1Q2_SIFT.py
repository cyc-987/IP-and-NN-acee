import numpy as np
import cv2 as cv
import matplotlib.pyplot as plt

img1 = cv.imread('dst.jpg')
gray1 = cv.cvtColor(img1,cv.COLOR_BGR2GRAY)

sift1 = cv.xfeatures2d.SIFT_create()
kp1,des1 = sift1.detectAndCompute(gray1,None)
cv.drawKeypoints(img1,kp1,img1,flags=cv.DRAW_MATCHES_FLAGS_DRAW_RICH_KEYPOINTS)

cv.imwrite('dst_sift.jpg',img1)
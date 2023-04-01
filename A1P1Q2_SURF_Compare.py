import numpy as np
import cv2 as cv
import matplotlib.pyplot as plt

#读取图片并获得灰度值
img1 = cv.imread('dst.jpg')
img2 = cv.imread('src.jpg')
gray1 = cv.cvtColor(img1,cv.COLOR_BGR2GRAY)
gray2 = cv.cvtColor(img2,cv.COLOR_BGR2GRAY)

#进行SURF操作
surf1 = cv.xfeatures2d.SURF_create(400)
surf2 = cv.xfeatures2d.SURF_create(400)
kp1, des1 = surf1.detectAndCompute(gray1,None)
kp2, des2 = surf2.detectAndCompute(gray2,None)

#特征点匹配
bf = cv.BFMatcher(cv.NORM_L2, crossCheck=True)
matches = bf.match(des1,des2)
matches = sorted(matches, key = lambda x:x.distance)
result = cv.drawMatches(img1,kp1,img2,kp2,matches[:100],None,flags=cv.DrawMatchesFlags_NOT_DRAW_SINGLE_POINTS)
plt.imshow(result[:,:,::-1]),plt.show()
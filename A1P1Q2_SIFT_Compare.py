import time
import numpy as np
import cv2 as cv
import matplotlib.pyplot as plt


#读取图片并获得灰度值
img1 = cv.imread('dst.jpg')
img2 = cv.imread('src.jpg')
gray1 = cv.cvtColor(img1,cv.COLOR_BGR2GRAY)
gray2 = cv.cvtColor(img2,cv.COLOR_BGR2GRAY)

start = time.perf_counter()
#进行SIFT操作
sift1 = cv.xfeatures2d.SIFT_create()
sift2 = cv.xfeatures2d.SIFT_create()
kp1,des1 = sift1.detectAndCompute(gray1,None)
kp2,des2 = sift2.detectAndCompute(gray2,None)

end1 = time.perf_counter()

#特征点匹配
bf = cv.BFMatcher(cv.NORM_L2, crossCheck=True)
matches = bf.match(des1,des2)
print(len(matches))
matches = sorted(matches, key = lambda x:x.distance)

#计算匹配效果
distance = 0
for i in range(0,101):
    distance += matches[i].distance
print(distance/100)

result = cv.drawMatches(img1,kp1,img2,kp2,matches[:100],None,flags=cv.DrawMatchesFlags_NOT_DRAW_SINGLE_POINTS)
plt.imshow(result[:,:,::-1]),plt.show()
cv.imwrite('sift.jpg',result)

end2 = time.perf_counter()
print(end1 - start)
print(end2 - end1)
print(end2 - start)
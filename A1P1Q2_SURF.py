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

#绘制特征点
cv.drawKeypoints(img1,kp1,img1,flags=0)
cv.drawKeypoints(img2,kp2,img2,flags=0)
plt.imshow(img1),plt.show()
plt.imshow(img2),plt.show()

#写文件
cv.imwrite('dst_surf.jpg', img1)
cv.imwrite('src_surf.jpg', img2)
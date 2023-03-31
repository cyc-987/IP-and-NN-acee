import numpy as np
import cv2 as cv
import matplotlib.pyplot as plt

lena = cv.imread("../A2P1/Q1/A2P1Q1_03.png")

cv.line(lena,(0,0),(511,511),(0,0,255),3)
plt.imshow(lena[:, :, ::-1])
plt.show()

cv.imwrite("test.png",lena)
import cv2 as cv
import numpy as np
import matplotlib.pyplot as plt

raw = cv.imread('test.jpg')
rows, cols = raw.shape[:2]

m = n = 3
b,g,r = cv.split(raw)

def process(clr,kernal,height,width,kh,kw):
    A = []#处理完的像素丢里面
    for h in range(kh//2,height-kw//2):
        for w in range(kw//2,width-kw//2):
            #获取相邻像素
            pixels = np.zeros((kh,kw),np.uint8)
            temp1 = temp2 = 0
            for i in range(h-kh//2,h+1+kh//2):
                for j in range(w-kw//2,w+1+kw//2):
                    pixels[temp1,temp2] = clr[i,j]
                    temp2 += 1
                temp1 += 1
                temp2 = 0
            #print(pixels)
            #与kernal进行卷积
            temp = 0
            for i in range(0,kh):
                for j in range(0,kw):
                    temp += pixels[i,j]*kernal[i,j]
            #print(temp)
            A.append(temp)
    #保存像素
    k = 0
    for h in range(kh//2,height-kw//2):
        for w in range(kw//2,width-kw//2):
            clr[h,w] = A[k]
            k += 1
    return clr

kernal = np.asarray([[0.11,0.11,0.11],[0.11,0.11,0.11],[0.11,0.11,0.11]])
b = process(b,kernal,rows,cols,n,m)
g = process(g,kernal,rows,cols,n,m)
r = process(r,kernal,rows,cols,n,m)   

img = cv.merge((b,g,r))
cv.imwrite('box_filtering_2.jpg',img)
#cv.imshow(img)
#cv.waitKey(0)
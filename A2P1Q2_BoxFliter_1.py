import numpy as np
import cv2 as cv
import matplotlib.pyplot as plt

#读取图像
img_raw = cv.imread('test.jpg')
#颜色转换
#img_raw = cv.cvtColor(img_raw, cv.COLOR_BGR2RGB)
#输出图像
#plt.imshow(img_raw)
#获取宽度和高度信息
rows,cols = img_raw.shape[:2]
#设定矩形模版,开辟数组存储中间变量
m = n = 3
buff = []

#进行bf主操作并丢进数组里
A = []#这是存放结果的数组
for i in range(0,rows-n+1):
    #创建或更新buff节点
    for j in range(0,cols):
        if(i == 0):
            sumbuff = [0,0,0]
            for add in range(0,n):
                sumbuff[0] += img_raw[add,j][0]
                sumbuff[1] += img_raw[add,j][1]
                sumbuff[2] += img_raw[add,j][2]
            buff.append(sumbuff)
        else:
            buff[j][0] -= img_raw[i-1,j][0]
            buff[j][1] -= img_raw[i-1,j][1]
            buff[j][2] -= img_raw[i-1,j][2]
            buff[j][0] += img_raw[i+n-1,j][0]
            buff[j][1] += img_raw[i+n-1,j][1]
            buff[j][2] += img_raw[i+n-1,j][2]
    #计算每个部分和
    sum = []
    for j in range(0,cols-m+1):
        if(j == 0):
            tempsum = [0,0,0]
            for k in range(0,m):
                tempsum[0] += buff[k][0]
                tempsum[1] += buff[k][1]
                tempsum[2] += buff[k][2]
            sum.append(tempsum)
        else:
            tempsum = [0,0,0]
            sum.append(tempsum)
            sum[j][0] = sum[j-1][0] - buff[j-1][0] + buff[j+m-1][0]
            sum[j][1] = sum[j-1][1] - buff[j-1][1] + buff[j+m-1][1]
            sum[j][2] = sum[j-1][2] - buff[j-1][2] + buff[j+m-1][2]
        A.append(sum[j])
        #print(A[j])

img_new = img_raw
k = 0
divide = m*n
for i in range(0,rows-n+1):
    for j in range(0,cols-m+1):
        A[k][0] = A[k][0]//divide
        A[k][1] = A[k][1]//divide
        A[k][2] = A[k][2]//divide
        img_new[i+n//2,j+n//2] = A[k]
        k += 1
        
cv.imwrite('box_filtering.jpg',img_new)
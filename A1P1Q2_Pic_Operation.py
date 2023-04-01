import numpy as np
import cv2 as cv
import matplotlib.pyplot as plt

img1 = cv.imread('dst.jpg')
img2 = cv.imread('src.jpg')

def blur(img,output_name_with_format,k_size):
    img = cv.medianBlur(img,k_size)
    cv.imwrite(output_name_with_format,img)

def change_size(img,output_name_with_format,relativesize):
    cv.resize(img, dsize=None, fx=relativesize, fy=relativesize, interpolation=cv.INTER_LINEAR)
    cv.imwrite(output_name_with_format,img)

def rotate(img,output_name_with_format,angle,scale):
    rows,cols = img.shape[:2]
    M = cv.getRotationMatrix2D((cols/2,rows/2),angle,scale)
    img = cv.warpAffine(img,M,(cols*2,rows*2))
    cv.imwrite(output_name_with_format,img)

blur(img1,'dst_blurred.jpg',3)
blur(img2,'src_blurred.jpg',3)
change_size(img1,'dst_2x.jpg',2)
change_size(img2,'src_2x.jpg',2)
change_size(img1,'dst_0.5x.jpg',2)
change_size(img2,'src_0.5x.jpg',2)
rotate(img2,'src_rotate_45_1x.jpg',45,1)
rotate(img2,'src_rotate_45_2x.jpg',45,2)
rotate(img2,'src_rotate_60_0.5x.jpg',60,0.5)

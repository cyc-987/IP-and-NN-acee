#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdint.h>

#pragma pack(push, 1)  // 设定结构体按照1字节对齐
typedef struct {
    uint16_t type;           // 文件类型，应为0x4D42
    uint32_t size;           // 文件大小，以字节为单位
    uint16_t reserved1;      // 保留，必须设置为0
    uint16_t reserved2;      // 保留，必须设置为0
    uint32_t offset;         // 文件头到像素数据的偏移量，以字节为单位
    uint32_t dib_header_size; // DIB头大小
    int32_t width;           // 图像宽度，以像素为单位
    int32_t height;          // 图像高度，以像素为单位
    uint16_t planes;         // 必须设置为1
    uint16_t bits_per_pixel; // 每个像素的位数
    uint32_t compression;    // 压缩类型，0表示不压缩
    uint32_t image_size;     // 图像大小，以字节为单位
    int32_t x_resolution;    // 水平分辨率，每米像素数
    int32_t y_resolution;    // 垂直分辨率，每米像素数
    uint32_t num_colors;     // 颜色索引表中颜色数
    uint32_t important_colors; // 重要颜色数，0表示都重要
} BMPHeader;
#pragma pack(pop) // 恢复原有对齐方式

int main()
{
    FILE *bmp;
    bmp = fopen("../A2P1/Q1/A2P1Q1_01.bmp", "rb");
    if(bmp == NULL){
        printf("open failed");
        exit(0);
    }

    /*FILE *bmp_processed;
    bmp_processed = fopen("processed.bmp","wb");
    if(bmp_processed == NULL){
        printf("creat file failed");
        exit(0);
    }*/

    BMPHeader bmpheader;
    fread(&bmpheader,sizeof(bmpheader),1,bmp);
    printf("%u\n",bmpheader.size);

    fclose(bmp);
    //fclose(bmp_processed);
}
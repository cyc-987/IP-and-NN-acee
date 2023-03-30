#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdint.h>
#include<math.h>

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

typedef struct{
    uint8_t R;
    uint8_t G;
    uint8_t B;
} RGB888;           //RGB888数据

typedef struct{
    uint8_t C;
} RGB256;           //256色数据

typedef struct{
    uint8_t B;
    uint8_t G;
    uint8_t R;
    uint8_t a;
} clrb;             //调色板
#pragma pack(pop) // 恢复原有对齐方式

int main()
{
    //打开文件
    FILE *bmp;
    bmp = fopen("../A2P1/Q1/A2P1Q1_01.bmp", "rb");
    if(bmp == NULL){
        printf("open failed");
        exit(0);
    }

    //创建新文件
    FILE *bmp_processed;
    bmp_processed = fopen("processed2.bmp","wb");
    if(bmp_processed == NULL){
        printf("creat file failed");
        exit(0);
    }

    //初始化
    BMPHeader bmpheader;
    fread(&bmpheader,sizeof(bmpheader),1,bmp);//读取源文件头
    BMPHeader new_header;//创建新文件头
    new_header = bmpheader;
    new_header.size = 54 + 256*4 + (long)bmpheader.width*bmpheader.height;//设定文件大小
    new_header.offset = 54 + 256*4;//设定新offset
    new_header.bits_per_pixel = 8;
    new_header.num_colors = 256;
    new_header.important_colors = 0;//设定颜色数
    new_header.image_size = (long)bmpheader.width*bmpheader.height;
    fwrite(&new_header,sizeof(new_header),1,bmp_processed);

    //生成调色板
    clrb new_clrb[256];
    int clr_num = 0;
    for(int i = 1;i<=6;i++){
        for(int j = 1;j<=6;j++){
            for(int k = 1;k<=7;k++){
                new_clrb[clr_num].a = 0;
                new_clrb[clr_num].B = (i-1)*51;
                new_clrb[clr_num].G = (j-1)*51;
                new_clrb[clr_num].R = (k-1)*42;
                clr_num++;
            }
        }
    }
    fwrite(&new_clrb,sizeof(clrb),256,bmp_processed);

    
    //读取RGB颜色
    double total_pixels = bmpheader.width*bmpheader.height;
    long count=1;
    while(count<=total_pixels){
        //读取RGB块
        RGB888 old_clr;
        fread(&old_clr,sizeof(RGB888),1,bmp);
        uint8_t R,G,B;
        B = old_clr.R;
        G = old_clr.G;
        R = old_clr.B;
        //确定最接近的num
        int close_num=0,close_sum=abs(new_clrb[0].B-B)+abs(new_clrb[0].G-G)+abs(new_clrb[0].R-R),temp_sum;
        for(int i = 0;i<252;i++){
            temp_sum = abs(new_clrb[i].B-B) + abs(new_clrb[i].G-G) + abs(new_clrb[i].R-R);
            if(temp_sum < close_sum){
                close_num = i;
                close_sum = temp_sum;
            }
        }
        //存入数据
        RGB256 new_clr;
        new_clr.C = close_num;
        //计数和保存
        count++;
        fwrite(&new_clr,sizeof(new_clr),1,bmp_processed);
    }
    

    //写文件
    fclose(bmp);
    fclose(bmp_processed);
}
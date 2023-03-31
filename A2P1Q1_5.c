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
    bmp = fopen("../A2P1/Q1/A2P1Q1_03.bmp", "rb");
    if(bmp == NULL){
        printf("open failed");
        exit(0);
    }

    //创建新文件
    FILE *bmp_processed;
    bmp_processed = fopen("processed5.bmp","wb");
    if(bmp_processed == NULL){
        printf("creat file failed");
        exit(0);
    }

    //初始化
    BMPHeader bmpheader;
    fread(&bmpheader,sizeof(bmpheader),1,bmp);//读取源文件头
    BMPHeader new_header;//创建新文件头
    new_header = bmpheader;
    new_header.size = 54 + 8 + (long)bmpheader.width*bmpheader.height;//设定文件大小
    new_header.offset = 54 + 8;//设定新offset
    new_header.bits_per_pixel = 1;
    new_header.num_colors = 2;
    new_header.important_colors = 0;//设定颜色数
    new_header.image_size = (long)bmpheader.width*bmpheader.height;
    fwrite(&new_header,sizeof(new_header),1,bmp_processed);

    //生成调色板
    clrb new_clrb[2];
    for(int i = 0;i<2;i++){
        new_clrb[i].a = 0;
        new_clrb[i].B = i*255;
        new_clrb[i].G = i*255;
        new_clrb[i].R = i*255;
    }
    fwrite(&new_clrb,sizeof(clrb),2,bmp_processed);

    
    //读取RGB颜色
    double total_pixels = bmpheader.width*bmpheader.height;
    long count=1;
    int flag = 0;//判断是否到最后缺省像素
    while(count<=total_pixels)
    {
        RGB888 old_clr[8];
        if(flag == 0){
            fread(old_clr,sizeof(RGB888),8,bmp);
        }else{
            long remain = total_pixels - count;
            for(int i = 0;i<8;i++){
                if(i<remain){
                    fread(&old_clr[i],sizeof(RGB888),1,bmp);
                }else continue;
            }
        }
        //创建存储单元
        uint8_t Byte = 0;
        for(int i = 0;i<8;i++){
            //读取RGB值
            uint8_t R,G,B;
            B = old_clr[i].R;
            G = old_clr[i].G;
            R = old_clr[i].B;
            //转化为255单色
            uint8_t Y;
            Y = R*0.299+G*0.587+B*0.114;
            //转化为2色（简单示例，需要算法支持）
            uint8_t C;
            if(Y<127) C = 0;
            else C = 1;
            //构造存储单元
            Byte = Byte << 1;
            Byte = Byte | C;
        }
        //保存存储单元
        fwrite(&Byte,sizeof(Byte),1,bmp_processed);
        count += 8;
        if(flag == 1)break;
        if(count > total_pixels){
            flag = 1;//下次循环退出
            count -= 8;
        }
    }

    //写文件
    fclose(bmp);
    fclose(bmp_processed);
}
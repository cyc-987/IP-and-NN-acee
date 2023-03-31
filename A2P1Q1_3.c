#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdint.h>
#include<math.h>

#define TRUE 1
#define FALSE 0

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

typedef struct{
    uint8_t R;
    uint8_t G;
    uint8_t B;
}pixel;             //像素
#pragma pack(pop) // 恢复原有对齐方式

typedef uint8_t BYTE;
typedef unsigned int UINT;

//八叉树定义节点
typedef struct        _NODE
{
        int bIsLeaf;                        //没有子节点则为TRUE
        unsigned int nPixelCount;                //这个叶子代表的颜色数
        unsigned int nRedSum;                        //红色部分总和
        unsigned int nGreenSum;                        //绿色部分总和
        unsigned int nBlueSum;                        //蓝色部分总和
        struct _NODE* pChild[8];//子节点指针
        struct _NODE* pNext;        //父节点指针
}NODE;

void AddColor(NODE**ppNode,BYTE bR,BYTE bG,BYTE bB,UINT nColorBits,UINT nLevel,UINT*pLeafCount,NODE**pReducibleNodes);
NODE*CreateNode(UINT nLevel,UINT nColorBits,UINT* pLeafCount,NODE**pReducibleNodes);
void ReduceTree(UINT nColorBits,UINT*pLeafCount,NODE**pReducibleNodes);
void DeleteTree(NODE**ppNode);

//添加颜色到八叉树的程序
void AddColor(NODE**ppNode,BYTE bR,BYTE bG,BYTE bB,UINT nColorBits,UINT nLevel,UINT*pLeafCount,NODE**pReducibleNodes)
{
        int nIndex,nShift;
        static BYTE mask[8]={0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01};

        //如果节点不存在，创建它
        if(!(*ppNode))
                *ppNode=CreateNode(nLevel,nColorBits,pLeafCount,pReducibleNodes);

        //如果是树叶节点，更新它的颜色信息
        if((*ppNode)->bIsLeaf)
        {
                (*ppNode)->nPixelCount++;
                (*ppNode)->nRedSum+=bR;
                (*ppNode)->nGreenSum+=bG;
                (*ppNode)->nBlueSum+=bB;
        }
        else//如果不是树叶节点，增加它的深度
        {
                nShift=7-nLevel;
                nIndex=
                        (((bR & mask[nLevel])>>nShift)<<2)|
                        (((bG & mask[nLevel])>>nShift)<<1)|
                        (((bB & mask[nLevel])>>nShift));//根据级数取出3个二进制位
                AddColor(&((*ppNode)->pChild[nIndex]),bR,bG,bB,nColorBits,nLevel+1,pLeafCount,pReducibleNodes);
        }
}

//创建节点
NODE*CreateNode(UINT nLevel,UINT nColorBits,UINT* pLeafCount,NODE** pReducibleNodes)
{
        NODE*pNode;

        if(!(pNode=(NODE*)malloc(sizeof(NODE))))//分配内存
                return NULL;

        memset(pNode,0,sizeof(NODE));//清零

        pNode->bIsLeaf=(nLevel==nColorBits)?TRUE:FALSE;//判断是否树叶节点
        if(pNode->bIsLeaf)
                (*pLeafCount)++;
        else
        {//把节点添加到这级的可删减节点里面去。
                pNode->pNext=pReducibleNodes[nLevel];
                pReducibleNodes[nLevel]=pNode;
        }
        return pNode;
}

void ReduceTree(UINT nColorBits,UINT* pLeafCount,NODE** pReducibleNodes)
{
        int i;
        NODE* pNode;
        UINT nRedSum, nGreenSum, nBlueSum, nChildren;

        //找到包含了至少一个可删减节点的最深级数
        for (i=nColorBits-1;(i>0)&&(pReducibleNodes[i]==NULL);i--);

        //在第i级减掉最近加入列表的节点
        pNode = pReducibleNodes[i];
        pReducibleNodes[i]=pNode->pNext;

        nRedSum=
        nGreenSum=
        nBlueSum=
        nChildren=0;
        for(i=0;i<8;i++)
        {
                if (pNode->pChild[i]!=NULL)//找到子节点
                {
                        nRedSum+=pNode->pChild[i]->nRedSum;
                        nGreenSum+=pNode->pChild[i]->nGreenSum;
                        nBlueSum+=pNode->pChild[i]->nBlueSum;//统计红绿蓝三种颜色的分量
                        pNode->nPixelCount += pNode->pChild[i]->nPixelCount;
                        free(pNode->pChild[i]);//删掉子节点
                        pNode->pChild[i] = NULL;
                        nChildren++;//合并的子节点数量
                }
        }
        pNode->bIsLeaf=TRUE;
        pNode->nRedSum=nRedSum;
        pNode->nGreenSum=nGreenSum;
        pNode->nBlueSum=nBlueSum;//合并完所有子节点以后，将父节点设置成树叶节点。
        *pLeafCount-=(nChildren - 1);
}

void GetPaletteColors(NODE*pTree,clrb*pPalEntries,UINT*pIndex)//从八叉树中取得调色板
{
        int i;

        if(pTree->bIsLeaf)//如果是树叶，把颜色加进调色板
        {
                pPalEntries[*pIndex].R=(BYTE)((pTree->nRedSum)/(pTree->nPixelCount));
                pPalEntries[*pIndex].G=(BYTE)((pTree->nGreenSum)/(pTree->nPixelCount));
                pPalEntries[*pIndex].B=(BYTE)((pTree->nBlueSum)/(pTree->nPixelCount));
                
                (*pIndex)++;
        }
        else//否则遍历下一级
        {
                for(i=0;i<8;i++)
                {
                        if(pTree->pChild[i]!=NULL)
                                GetPaletteColors(pTree->pChild[i],pPalEntries,pIndex);
                }
        }
}

void DeleteTree(NODE**ppNode)//删除整个八叉树的函数
{
        int i;
        for(i=0; i<8; i++)
        {
                if((*ppNode)->pChild[i]!=NULL)
                        DeleteTree(&((*ppNode)->pChild[i]));
        }
        free(*ppNode);
        *ppNode = NULL;
}

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
    bmp_processed = fopen("processed3.bmp","wb");
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

    //保存指针位置
    UINT x,y;
    UINT nLeafCount=0, nIndex=0;
    NODE*pTree=NULL, *pReducibleNodes[9]={NULL};
    BYTE R,G,B;
    //遍历整个位图
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
        count++;

        AddColor(&pTree,R,G,B,8,0,&nLeafCount,pReducibleNodes);//添加颜色
        while(nLeafCount>=256){//颜色数量超过限额的时候减少颜色
            ReduceTree(8,&nLeafCount,pReducibleNodes);
        }
    }
    //取得调色板颜色
    clrb new_clrb[256];
    GetPaletteColors(pTree,new_clrb,&nIndex);//取得调色板颜色
    fwrite(&new_clrb,sizeof(clrb),256,bmp_processed);
    DeleteTree(&pTree);

    //读取RGB颜色
    count=1;
    fseek(bmp,54l,0);//回到颜色起始处
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
        for(int i = 0;i<256;i++){
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
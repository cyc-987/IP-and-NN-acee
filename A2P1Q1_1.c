#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main()
{
    FILE *bmp;
    bmp = fopen("../A2P1/Q1/A2P1Q1_01.bmp", "rb");
    if(bmp == NULL){
        printf("open failed");
        exit(0);
    }

    FILE *bmp_processed;
    bmp_processed = fopen("processed.bmp","wb");
    if(bmp_processed == NULL){
        printf("creat file failed");
        exit(0);
    }

    while(!feof(bmp)){
        void *c;
        c = malloc(1);
        fread(c,1,1,bmp);
        fwrite(c,1,1,bmp_processed);
        free(c);
    }
    fclose(bmp);
    fclose(bmp_processed);
}
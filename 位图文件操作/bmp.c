#include "bmp.h"
//打开地址对应的bmp文件，返回位图数据体
ClImage* clLoadImage(const char* path)
{
    ClImage* bmpImg;
    FILE* pFile;
    unsigned short fileType;
    ClBitMapFileHeader bmpFileHeader;
    ClBitMapInfoHeader bmpInfoHeader;
    int channels = 1;
    int width = 0;
    int height = 0;
    int step = 0;
    int offset = 0;
    unsigned char pixVal;
    ClRgbQuad* quad;
    int i, j, k;

    bmpImg = (ClImage*)malloc(sizeof(ClImage));
    pFile = fopen(path, "rb");
    if (!pFile)
    {
        free(bmpImg);
        return NULL;
    }

    fread(&fileType, sizeof(unsigned short), 1, pFile);
    if (fileType == 0x4D42)
    {
        //printf("bmp file! \n");

        fread(&bmpFileHeader, sizeof(ClBitMapFileHeader), 1, pFile);
        printf("\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\n");
        printf("bmp文件头信息：\n");
        printf("文件大小：%d \n", bmpFileHeader.bfSize);
        printf("保留字：%d \n", bmpFileHeader.bfReserved1);
        printf("保留字：%d \n", bmpFileHeader.bfReserved2);
        printf("位图数据偏移字节数：%d \n", bmpFileHeader.bfOffBits);

        fread(&bmpInfoHeader, sizeof(ClBitMapInfoHeader), 1, pFile);
        printf("\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\n");
        printf("bmp文件信息头\n");
        printf("结构体长度：%d \n", bmpInfoHeader.biSize);
        printf("位图宽度：%d \n", bmpInfoHeader.biWidth);
        printf("位图高度：%d \n", bmpInfoHeader.biHeight);
        printf("位图平面数：%d \n", bmpInfoHeader.biPlanes);
        printf("颜色位数：%d \n", bmpInfoHeader.biBitCount);
        printf("压缩方式：%d \n", bmpInfoHeader.biCompression);
        printf("实际位图数据占用的字节数：%d \n", bmpInfoHeader.biSizeImage);
        printf("X方向分辨率：%d \n", bmpInfoHeader.biXPelsPerMeter);
        printf("Y方向分辨率：%d \n", bmpInfoHeader.biYPelsPerMeter);
        printf("使用的颜色数：%d \n", bmpInfoHeader.biClrUsed);
        printf("重要颜色数：%d \n", bmpInfoHeader.biClrImportant);
        printf("\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\n");

        if (bmpInfoHeader.biBitCount == 8)
        {
            //printf("该文件有调色板，即该位图为非真彩色\n\n");
            channels = 1;
            width = bmpInfoHeader.biWidth;
            height = bmpInfoHeader.biHeight;
            offset = (channels*width)%4;
            if (offset != 0)
            {
                offset = 4 - offset;
            }
            //bmpImg->mat = kzCreateMat(height, width, 1, 0);
            bmpImg->width = width;
            bmpImg->height = height;
            bmpImg->channels = 1;
            bmpImg->imageData = (unsigned char*)malloc(sizeof(unsigned char)*width*height);
            step = channels*width;

            quad = (ClRgbQuad*)malloc(sizeof(ClRgbQuad)*256);
            fread(quad, sizeof(ClRgbQuad), 256, pFile);
            free(quad);

            for (i=0; i<height; i++)
            {
                for (j=0; j<width; j++)
                {
                    fread(&pixVal, sizeof(unsigned char), 1, pFile);
                    bmpImg->imageData[(height-1-i)*step+j] = pixVal;
                }
                if (offset != 0)
                {
                    for (j=0; j<offset; j++)
                    {
                        fread(&pixVal, sizeof(unsigned char), 1, pFile);
                    }
                }
            }
        }
        else if (bmpInfoHeader.biBitCount == 24)
        {
            //printf("该位图为位真彩色\n\n");
            channels = 3;
            width = bmpInfoHeader.biWidth;
            height = bmpInfoHeader.biHeight;

            bmpImg->width = width;
            bmpImg->height = height;
            bmpImg->channels = 3;
            bmpImg->imageData = (unsigned char*)malloc(sizeof(unsigned char)*width*3*height);//总像素点位w*h，每个像素点3字节
            step = channels*width;//图片上一行像素点对应的数据量（字节）
            //图像上的数据按行储存，每一行的字节数为4的倍数，不足的字节用0补齐
            offset = (channels*width)%4;
            if (offset != 0)//判断每一行补上的0
            {
                offset = 4 - offset;//
            }

            for (i=0; i<height; i++)
            {
                for (j=0; j<width; j++)
                {
                    for (k=0; k<3; k++)
                    {
                        fread(&pixVal, sizeof(unsigned char), 1, pFile);//每次读取一个字节
                        bmpImg->imageData[(height-1-i)*step+j*3+k] = pixVal;//确保将第一行的数据储存在数组的末尾，这样像素点在图片中的位置与描述像素点的数据在数组中的位置一致
                    }
                    //kzSetMat(bmpImg->mat, height-1-i, j, kzScalar(pixVal[0], pixVal[1], pixVal[2]));
                }
                //读完一行后删除填充字节
                if (offset != 0)
                {
                    for (j=0; j<offset; j++)
                    {
                        fread(&pixVal, sizeof(unsigned char), 1, pFile);
                    }
                }
            }
        }
    }

    return bmpImg;
}

bool clSaveImage(const char* path, ClImage* bmpImg)
{
    FILE *pFile;
    unsigned short fileType;//保存格式
    ClBitMapFileHeader bmpFileHeader;//文件头
    ClBitMapInfoHeader bmpInfoHeader;//信息头
    int step;//一行数据量（字节）
    int offset;//一行填充的数据量（字节）
    unsigned char pixVal = '\0';
    int i, j;
    ClRgbQuad* quad;//调试板

    pFile = fopen(path, "wb");//在path路径对应的文档中记录数据
    if (!pFile)
    {
        return false;
    }

    fileType = 0x4D42;//文件格式
    fwrite(&fileType, sizeof(unsigned short), 1, pFile);//写入两个字节的文件格式


    //真彩色图的储存
    if (bmpImg->channels == 3)//24位，通道，彩图
    {//为一行数据量赋值（字节）

        step = bmpImg->channels*bmpImg->width;
        offset = step%4;
        if (offset != 4)
        {
            step += 4-offset;
        }
        //为文件头信息赋值，并写入文件
        bmpFileHeader.bfSize = bmpImg->height*step + 54;
        bmpFileHeader.bfReserved1 = 0;
        bmpFileHeader.bfReserved2 = 0;
        bmpFileHeader.bfOffBits = 54;
        fwrite(&bmpFileHeader, sizeof(ClBitMapFileHeader), 1, pFile);
        //为位图信息头赋值，并写入文件
        bmpInfoHeader.biSize = 40;
        bmpInfoHeader.biWidth = bmpImg->width;
        bmpInfoHeader.biHeight = bmpImg->height;
        bmpInfoHeader.biPlanes = 1;
        bmpInfoHeader.biBitCount = 24;
        bmpInfoHeader.biCompression = 0;
        bmpInfoHeader.biSizeImage = bmpImg->height*step;
        bmpInfoHeader.biXPelsPerMeter = 0;
        bmpInfoHeader.biYPelsPerMeter = 0;
        bmpInfoHeader.biClrUsed = 0;
        bmpInfoHeader.biClrImportant = 0;
        fwrite(&bmpInfoHeader, sizeof(ClBitMapInfoHeader), 1, pFile);
        //将位图数据写入文件
        for (i=bmpImg->height-1; i>-1; i--)//确保将数组末尾的数据写在文件的首部，以符合图像位图数据的存储标准
        {
            for (j=0; j<bmpImg->width; j++)
            {
                pixVal = bmpImg->imageData[i*bmpImg->width*3+j*3];
                fwrite(&pixVal, sizeof(unsigned char), 1, pFile);
                pixVal = bmpImg->imageData[i*bmpImg->width*3+j*3+1];
                fwrite(&pixVal, sizeof(unsigned char), 1, pFile);
                pixVal = bmpImg->imageData[i*bmpImg->width*3+j*3+2];
                fwrite(&pixVal, sizeof(unsigned char), 1, pFile);
            }
            //每写完一行像素点的数据判断是否需要填充
            if (offset!=0)
            {
                for (j=0; j<4-offset; j++)
                {
                    pixVal = 0;
                    fwrite(&pixVal, sizeof(unsigned char), 1, pFile);
                }
            }
        }
    }

    //8色位图的储存
    else if (bmpImg->channels == 1)//8位，单通道，灰度图
    {
        step = bmpImg->width;
        offset = step%4;
        if (offset != 4)
        {
            step += 4-offset;
        }
        bmpFileHeader.bfSize = 54 + 256*4 + bmpImg->width;
        bmpFileHeader.bfReserved1 = 0;
        bmpFileHeader.bfReserved2 = 0;
        bmpFileHeader.bfOffBits = 54 + 256*4;
        fwrite(&bmpFileHeader, sizeof(ClBitMapFileHeader), 1, pFile);
        bmpInfoHeader.biSize = 40;
        bmpInfoHeader.biWidth = bmpImg->width;
        bmpInfoHeader.biHeight = bmpImg->height;
        bmpInfoHeader.biPlanes = 1;
        bmpInfoHeader.biBitCount = 8;
        bmpInfoHeader.biCompression = 0;
        bmpInfoHeader.biSizeImage = bmpImg->height*step;
        bmpInfoHeader.biXPelsPerMeter = 0;
        bmpInfoHeader.biYPelsPerMeter = 0;
        bmpInfoHeader.biClrUsed = 256;
        bmpInfoHeader.biClrImportant = 256;
        fwrite(&bmpInfoHeader, sizeof(ClBitMapInfoHeader), 1, pFile);
        quad = (ClRgbQuad*)malloc(sizeof(ClRgbQuad)*256);
        for (i=0; i<256; i++)
        {
            quad[i].rgbBlue = i;
            quad[i].rgbGreen = i;
            quad[i].rgbRed = i;
            quad[i].rgbReserved = 0;
        }
        fwrite(quad, sizeof(ClRgbQuad), 256, pFile);
        free(quad);
        for (i=bmpImg->height-1; i>-1; i--)
        {
            for (j=0; j<bmpImg->width; j++)
            {
                pixVal = bmpImg->imageData[i*bmpImg->width+j];
                fwrite(&pixVal, sizeof(unsigned char), 1, pFile);
            }
            if (offset!=0)
            {
                for (j=0; j<4-offset; j++)
                {
                    pixVal = 0;
                    fwrite(&pixVal, sizeof(unsigned char), 1, pFile);
                }
            }
        }
    }
    fclose(pFile);

    return true;
}

//将传入的位图数据灰度化处理返回处理后的位图数据文件
ClImage* clGrayscaleImage(ClImage* bmpImg)
{
    int i,j;
    int Gray;
    int width = bmpImg->width;
    int height = bmpImg->height;
    int step = bmpImg->channels*bmpImg->width;
    for (i=0; i<height; i++)
    {
        for (j=0; j<width; j++)
        {
                Gray=(bmpImg->imageData[i*step+j*3+0]*0.3+bmpImg->imageData[i*step+j*3+1]*0.59+bmpImg->imageData[i*step+j*3+2]*0.11);
                bmpImg->imageData[i*step+j*3+0]=Gray;
                bmpImg->imageData[i*step+j*3+1]=Gray;
                bmpImg->imageData[i*step+j*3+2]=Gray;
        }
    }
    return bmpImg;
}

ClImage* clBinImage(ClImage* bmpImg)
{
    int i,j;
    int threshold = 0;
    double threshold1 = 0;
    int width = bmpImg->width;
    int height = bmpImg->height;
    int step = bmpImg->channels*bmpImg->width;
    double all = width*height;
    //计算所有像素点的灰度平均值，将其作为阈值
    for (i=0; i<height; i++)
    {
        for (j=0; j<width; j++)
        {
            threshold1 += (((double)bmpImg->imageData[i*step+j*3])/all);
        }
    }
    threshold = (int)threshold1;
    for (i=0; i<height; i++)
    {
        for (j=0; j<width; j++)
        {
                if(bmpImg->imageData[i*step+j*3]<=threshold)
                {
                    bmpImg->imageData[i*step+j*3+0]=0;
                    bmpImg->imageData[i*step+j*3+1]=0;
                    bmpImg->imageData[i*step+j*3+2]=0;
                }
                else
                {
                    bmpImg->imageData[i*step+j*3+0]=255;
                    bmpImg->imageData[i*step+j*3+1]=255;
                    bmpImg->imageData[i*step+j*3+2]=255;
                }
        }
    }
    return bmpImg;
}

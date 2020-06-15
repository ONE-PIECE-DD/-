#ifndef BMP_H
#define BMP_H

#include <stdio.h>
#include <stdlib.h>
//定义布尔变量
#define true 1
#define false 0
typedef int bool;
typedef struct
{
	//unsigned short    bfType;//文件格式标识：BM
	unsigned long    bfSize;//文件大小：小头字节顺序
	unsigned short    bfReserved1;//保留字
	unsigned short    bfReserved2;//保留字
	unsigned long    bfOffBits;//图像数据相对于文件头的偏移量，24位真彩图一般为54；
} ClBitMapFileHeader;//文件头

typedef struct
{
	unsigned long  biSize;//信息头所占字节数：固定为40
	long   biWidth;//以像素为单位的图像宽度
	long   biHeight;//以像素为单位的图像高度
	unsigned short   biPlanes;//目标设备平面数，必须为1
	unsigned short   biBitCount;//描述每个像素所需的位数（位深）

	unsigned long  biCompression;//位图压缩类型；有三种取值
	unsigned long  biSizeImage;//图像数据字节数
	long   biXPelsPerMeter;//目标设备水平方向分辨率（像素/米）
	long   biYPelsPerMeter;//目标设备垂直方向分辨率（像素/米）
	unsigned long   biClrUsed;//该图像实际用到的颜色数，若为0，则是2^位深
	unsigned long   biClrImportant;//重要颜色数，，若为0，则所有颜色都重要
} ClBitMapInfoHeader;//位图信息头

typedef struct
{
	unsigned char rgbBlue; //该颜色的蓝色分量
	unsigned char rgbGreen; //该颜色的绿色分量
	unsigned char rgbRed; //该颜色的红色分量
	unsigned char rgbReserved; //保留值，固定为0
} ClRgbQuad;//调色板结构
//用4位编码16色位图调色板，用1字节编码256色位图调色板，真彩色图需要3字节
//使用调色板可以节省储存空间
typedef struct
{
	int width;
	int height;
	int channels;
	unsigned char* imageData;//1维数组，存储位图数据
}ClImage;

ClImage* clLoadImage(const char* path);//载入函数
bool clSaveImage(const char* path, ClImage* bmpImg);//保存函数
ClImage* clGrayscaleImage(ClImage*);//灰度化
ClImage* clBinImage(ClImage*);//二值化
ClImage* clfingernumber(ClImage*);//手指操作
ClImage* clspan(ClImage*);//旋转
#endif

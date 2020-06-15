#include "bmp.h"
//需读取的文件路径声明，以及保存路径声明；
//注：程序运行时图片需要与主函数文件在同一目录下，且生成图片也在该目录下
const char* fileName1 = "实验图片.bmp";
const char* fileName2 = "旋转实验.bmp";
const char* fileName3 = "灰度化_均值.bmp";
const char* fileName4 = "二值化.bmp";
const char* fileName5 = "顺时针旋转90度.bmp";
int main(int argc, char* argv[])
{
	ClImage* img1 = clLoadImage(fileName1);//从文件路径导出位图数据到指针指向的地址
    clGrayscaleImage(img1);//将指针指向地址保存的位图数据灰度化
	bool flag2 = clSaveImage(fileName3, img1);//将指针指向的地址保存的位图数据写入文件生成灰度化图片
	clBinImage(img1);//将将指针指向地址保存的位图数据（已灰度化）二值化
	bool flag3 = clSaveImage(fileName4, img1);//将指针指向的地址保存的位图数据写入文件生成二值化图片

	ClImage* img2 = clLoadImage(fileName2);//从文件路径导出位图数据到指针指向的地址
    bool flag4 = clSaveImage(fileName5, clspan(img2));//将旋转后的图片保存
    if(flag2)
        printf(" 灰度化保存成功...\n");
	else
		printf("灰度化保存失败...\n");
    if(flag3)
        printf(" 二值化保存成功...\n");
	else
		printf("二值化保存失败...\n");
    if(flag4)
        printf(" 旋转图片保存成功...\n");
	else
		printf("旋转图片保存失败...\n");
	return 0;
}

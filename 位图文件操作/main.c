#include "bmp.h"

const char* fileName1 = "实验图片.bmp";
const char* fileName2 = "D:\\opencv_pic\\Lena - 副本.bmp";

int main(int argc, char* argv[])
{
	ClImage* img = clLoadImage(fileName1);
	bool flag = clSaveImage(fileName2, img);
	if(flag)
	{
		printf("save ok...\n");
	}
	else
	{
		printf("save failure...\n");
	}
	return 0;
}

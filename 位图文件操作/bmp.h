#ifndef BMP_H
#define BMP_H

#include <stdio.h>
#include <stdlib.h>
//���岼������
#define true 1
#define false 0
typedef int bool;
typedef struct
{
	//unsigned short    bfType;//�ļ���ʽ��ʶ��BM
	unsigned long    bfSize;//�ļ���С��Сͷ�ֽ�˳��
	unsigned short    bfReserved1;//������
	unsigned short    bfReserved2;//������
	unsigned long    bfOffBits;//ͼ������������ļ�ͷ��ƫ������24λ���ͼһ��Ϊ54��
} ClBitMapFileHeader;//�ļ�ͷ

typedef struct
{
	unsigned long  biSize;//��Ϣͷ��ռ�ֽ������̶�Ϊ40
	long   biWidth;//������Ϊ��λ��ͼ����
	long   biHeight;//������Ϊ��λ��ͼ��߶�
	unsigned short   biPlanes;//Ŀ���豸ƽ����������Ϊ1
	unsigned short   biBitCount;//����ÿ�����������λ����λ�

	unsigned long  biCompression;//λͼѹ�����ͣ�������ȡֵ
	unsigned long  biSizeImage;//ͼ�������ֽ���
	long   biXPelsPerMeter;//Ŀ���豸ˮƽ����ֱ��ʣ�����/�ף�
	long   biYPelsPerMeter;//Ŀ���豸��ֱ����ֱ��ʣ�����/�ף�
	unsigned long   biClrUsed;//��ͼ��ʵ���õ�����ɫ������Ϊ0������2^λ��
	unsigned long   biClrImportant;//��Ҫ��ɫ��������Ϊ0����������ɫ����Ҫ
} ClBitMapInfoHeader;//λͼ��Ϣͷ

typedef struct
{
	unsigned char rgbBlue; //����ɫ����ɫ����
	unsigned char rgbGreen; //����ɫ����ɫ����
	unsigned char rgbRed; //����ɫ�ĺ�ɫ����
	unsigned char rgbReserved; //����ֵ���̶�Ϊ0
} ClRgbQuad;//��ɫ��ṹ
//��4λ����16ɫλͼ��ɫ�壬��1�ֽڱ���256ɫλͼ��ɫ�壬���ɫͼ��Ҫ3�ֽ�
//ʹ�õ�ɫ����Խ�ʡ����ռ�
typedef struct
{
	int width;
	int height;
	int channels;
	unsigned char* imageData;//1ά���飬�洢λͼ����
}ClImage;

ClImage* clLoadImage(const char* path);//���뺯��
bool clSaveImage(const char* path, ClImage* bmpImg);//���溯��
ClImage* clGrayscaleImage(ClImage*);//�ҶȻ�
ClImage* clBinImage(ClImage*);//��ֵ��
ClImage* clfingernumber(ClImage*);//��ָ����
ClImage* clspan(ClImage*);//��ת
#endif

#include "bmp.h"
//���ȡ���ļ�·���������Լ�����·��������
//ע����������ʱͼƬ��Ҫ���������ļ���ͬһĿ¼�£�������ͼƬҲ�ڸ�Ŀ¼��
const char* fileName1 = "ʵ��ͼƬ.bmp";
const char* fileName2 = "��תʵ��.bmp";
const char* fileName3 = "�ҶȻ�_��ֵ.bmp";
const char* fileName4 = "��ֵ��.bmp";
const char* fileName5 = "˳ʱ����ת90��.bmp";
int main(int argc, char* argv[])
{
	ClImage* img1 = clLoadImage(fileName1);//���ļ�·������λͼ���ݵ�ָ��ָ��ĵ�ַ
    clGrayscaleImage(img1);//��ָ��ָ���ַ�����λͼ���ݻҶȻ�
	bool flag2 = clSaveImage(fileName3, img1);//��ָ��ָ��ĵ�ַ�����λͼ����д���ļ����ɻҶȻ�ͼƬ
	clBinImage(img1);//����ָ��ָ���ַ�����λͼ���ݣ��ѻҶȻ�����ֵ��
	bool flag3 = clSaveImage(fileName4, img1);//��ָ��ָ��ĵ�ַ�����λͼ����д���ļ����ɶ�ֵ��ͼƬ

	ClImage* img2 = clLoadImage(fileName2);//���ļ�·������λͼ���ݵ�ָ��ָ��ĵ�ַ
    bool flag4 = clSaveImage(fileName5, clspan(img2));//����ת���ͼƬ����
    if(flag2)
        printf(" �ҶȻ�����ɹ�...\n");
	else
		printf("�ҶȻ�����ʧ��...\n");
    if(flag3)
        printf(" ��ֵ������ɹ�...\n");
	else
		printf("��ֵ������ʧ��...\n");
    if(flag4)
        printf(" ��תͼƬ����ɹ�...\n");
	else
		printf("��תͼƬ����ʧ��...\n");
	return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include<string.h>

void readfile(int a);//���ļ�
int hdxg(float X[]);//�������
float gljs();//���㹦��
void jshdxg(int a);//���㻬�����
float* txl=NULL;//���浱ǰ������ź�
int main()
{
    int i,pos;
    float max,ret;
    ret=0;max=0;pos=0;
    for(i=0;i<12;i++)
    {
        readfile(i);//���ζ�ȡ�ļ�
        ret=gljs();
        if(ret>max)
        {//�ҵ�����ź�ǿ��
            max=ret;
            pos=i;
        }
        printf("%d���źŵ��ź�ǿ��Ϊ%.3f\n",i,ret);
    }
    printf("�����ź��е�%d���ź���ǿ�����ź�ǿ��Ϊ%.3f\n",pos,max);
    jshdxg(pos);

}
//��ȡ���a��Ӧ���ź�dataa,�������źŵ�ǿ��
void readfile(int a)
{
    FILE *F;
    char b1,b2;
    char *str;
    int n;
    txl=(float*)malloc(30000*sizeof(float));
    //ͨ�������ַ������Ƶ�ǰӦ�ô򿪵��ļ�����ʵ����һ�����ƣ�ֻ�ܲ��ܴ����Ϊ��λ�����ı��ļ�
    char s[]="C:\\Users\\ONE-PIECE-D\\Desktop\\��һ��ʵ������\\ͨ��\\��һ��ʵ������\\dataX.txt";//���ҽ��������ļ��Ը�·���洢����������
    b1=a/10+48;
    b2=a%10+48;
    if(b1!='0')
    {
        char s1[]="C:\\Users\\ONE-PIECE-D\\Desktop\\��һ��ʵ������\\ͨ��\\��һ��ʵ������\\dataXX.txt";
        s1[68]=b1;
        s1[69]=b2;
        str=s1;
    }
    else
    {
        s[68]=b2;
        str=s;
    }

    if((F=fopen(str,"r"))==NULL)
            exit(9);
    for(n=0; !feof(F)&&fscanf(F,"%f\n",txl+n); n++);//���ζ�ȡ�ļ��е�����
}
//������Ҫ���㻬����ص�����źŶ�Ӧ���ı����
void jshdxg(int a)
{
    int n;
    FILE* F1=NULL;
    FILE* F2=NULL;
    FILE* F3=NULL;

    float* X0=NULL;
    float* X1=NULL;
    float* X2=NULL;
    X0=(float*)malloc(4096*sizeof(float));
    X1=(float*)malloc(4096*sizeof(float));
    X2=(float*)malloc(4096*sizeof(float));
    readfile(a);
    char *str0="C:\\Users\\ONE-PIECE-D\\Desktop\\��һ��ʵ������\\ͨ��\\��һ��ʵ������\\PSS0.txt";
    char *str1="C:\\Users\\ONE-PIECE-D\\Desktop\\��һ��ʵ������\\ͨ��\\��һ��ʵ������\\PSS1.txt";
    char *str2="C:\\Users\\ONE-PIECE-D\\Desktop\\��һ��ʵ������\\ͨ��\\��һ��ʵ������\\PSS2.txt";
    if((F1=fopen(str0,"r"))==NULL)
            exit(9);
    if((F2=fopen(str1,"r"))==NULL)
            exit(9);
    if((F3=fopen(str2,"r"))==NULL)
            exit(9);
    //���ζ�ȡ�ļ��������㻬�����
    for(n=0; !feof(F1)&&fscanf(F1,"%f\n",X0+n); n++);
    for(n=0; !feof(F2)&&fscanf(F2,"%f\n",X1+n); n++);
    for(n=0; !feof(F3)&&fscanf(F3,"%f\n",X2+n); n++);
    printf("PSSO:");
    hdxg(X0);       //���㲢չʾ���
    printf("PSS1:");
    hdxg(X1);
    printf("PSS2:");
    hdxg(X2);
}
//���밸���źŵĵ�ַ��������ǿ�ź������������źŵĻ������
int hdxg(float* X)
{
    int i = 0,j = 0,pos = 0;
    float a,b,ret=0,max=0;
	for (i = 0; i < 30000 ; i++)
	{
		ret = 0;
		for (j = 0; j < sizeof(X)/sizeof(float); j++)
		{
		    a=*(txl+j+i);
		    b=*(X+j);
            ret = ret + a * b;
		}
		if (ret > max)
		{
			pos = i;
			max = ret;
		}
	}
    printf("������Ϊ%.3f����ʱλ��Ϊ %d\n",max,pos);
    free(X);
	return 0;
}
//���㵱ǰ�ź�txl�Ĺ��ʣ����ع��ʴ�С
float gljs()
{

    int i,b;
    float m,n,power=0;
    b=30000;
    for(i=0;i+1<=b;i+=2)
    {
        m=*(txl+i);n=*(txl+i+1);
        power+=sqrt(m*m+n*n);
    }
    free(txl);
    return power;
}

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include<string.h>

void readfile(int a);//读文件
int hdxg(float X[]);//滑动相关
float gljs();//计算功率
void jshdxg(int a);//计算滑动相关
float* txl=NULL;//储存当前处理的信号
int main()
{
    int i,pos;
    float max,ret;
    ret=0;max=0;pos=0;
    for(i=0;i<12;i++)
    {
        readfile(i);//依次读取文件
        ret=gljs();
        if(ret>max)
        {//找到最大信号强度
            max=ret;
            pos=i;
        }
        printf("%d号信号的信号强度为%.3f\n",i,ret);
    }
    printf("所有信号中第%d组信号最强，其信号强度为%.3f\n",pos,max);
    jshdxg(pos);

}
//读取序号a对应的信号dataa,并返回信号的强度
void readfile(int a)
{
    FILE *F;
    char b1,b2;
    char *str;
    int n;
    txl=(float*)malloc(30000*sizeof(float));
    //通过控制字符串控制当前应该打开的文件，该实现有一定限制，只能不能打开序号为三位数的文本文件
    char s[]="C:\\Users\\ONE-PIECE-D\\Desktop\\第一次实验数据\\通软\\第一次实验数据\\dataX.txt";//当且仅当数据文件以该路径存储才能正常打开
    b1=a/10+48;
    b2=a%10+48;
    if(b1!='0')
    {
        char s1[]="C:\\Users\\ONE-PIECE-D\\Desktop\\第一次实验数据\\通软\\第一次实验数据\\dataXX.txt";
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
    for(n=0; !feof(F)&&fscanf(F,"%f\n",txl+n); n++);//依次读取文件中的数据
}
//传入需要计算滑动相关的最大信号对应的文本序号
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
    char *str0="C:\\Users\\ONE-PIECE-D\\Desktop\\第一次实验数据\\通软\\第一次实验数据\\PSS0.txt";
    char *str1="C:\\Users\\ONE-PIECE-D\\Desktop\\第一次实验数据\\通软\\第一次实验数据\\PSS1.txt";
    char *str2="C:\\Users\\ONE-PIECE-D\\Desktop\\第一次实验数据\\通软\\第一次实验数据\\PSS2.txt";
    if((F1=fopen(str0,"r"))==NULL)
            exit(9);
    if((F2=fopen(str1,"r"))==NULL)
            exit(9);
    if((F3=fopen(str2,"r"))==NULL)
            exit(9);
    //依次读取文件，并计算滑动相关
    for(n=0; !feof(F1)&&fscanf(F1,"%f\n",X0+n); n++);
    for(n=0; !feof(F2)&&fscanf(F2,"%f\n",X1+n); n++);
    for(n=0; !feof(F3)&&fscanf(F3,"%f\n",X2+n); n++);
    printf("PSSO:");
    hdxg(X0);       //计算并展示结果
    printf("PSS1:");
    hdxg(X1);
    printf("PSS2:");
    hdxg(X2);
}
//传入案例信号的地址，计算最强信号与三个案例信号的滑动相关
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
    printf("最大相关为%.3f，此时位置为 %d\n",max,pos);
    free(X);
	return 0;
}
//计算当前信号txl的功率，返回功率大小
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

#include <stdio.h>
#include <stdlib.h>
#define BD 50//��Զ����,�����þ�������ڵ㲻�ɵ���
#define unexist -1//���㲻���ڣ����ڳ�ʼ��·��ͼ
#define numV 6//������
#define flooding_file "data.txt"//�鷺��Ϣ�ļ�
#define MaxVertices 100 //�������100������
//ȫ�ֱ�������
char S[]={'1','2','3','4','5','6'};//���㼯
int d[numV];//s����Ӧ�±�ľ���
int p[numV];//·��������±�
int p_bef[numV];//ǰ�ö�����±�
int G[numV][numV];// ����ͼ��BDΪ���ɴ����,Gij��ʾi��j����
int Index,num;//IndexΪ��ǰ������±꣬numΪp�ж�������
int first;//��ѯ�ڵ���±�
//��������
void CreateGraph();//���ݺ鷺��Ϣ�����Ȩͼ
void Dijkstra();//Dijkstra�㷨��ʵ��
void findmin();//�ҵ�ת���������·��
void update(int);//���±�Ϊi�Ķ���Ϊ��׼������ת����
void Display();//���չʾ
int isexist(int);//�ж�һ�������Ƿ���·�����Ѿ����ڣ������ڷ���1
//������
int main()
{
    int i;
    printf("���㼯:\n");
    for(i=0;i<numV;i++)
    {
        printf(" %c ",S[i]);
    }
    printf("\n�����ѯ�ڵ��±꣺\n");
    scanf("%d",&first);
    if(first>=numV)
    {
        printf("�ڵ㲻����");
        exit(-1);
    }
    CreateGraph();
    Dijkstra();
    Display();
    return 0;
}
//ͼ�����ɺ���
void CreateGraph()
{
    FILE *fp;
    int vi,vj,w,i,j;
    char ch;
    if((fp=fopen(flooding_file,"r"))==NULL)
        printf("�ļ���ʧ��");
    for(i=0;i<numV;i++)
    {//ͼ�ĳ�ʼ��
        for(j=0;j<numV;j++)
            {
                G[i][j]=BD;
            }
    }
        for(i=0;i<20;i++)
        {
            fscanf(fp,"%d%c%d%c%d%c",&vi,&ch,&vj,&ch,&w,&ch);
            G[vi-1][vj-1]=w;//��
            //����ͼ���жԳ��ԵĹ��ɣ�ͨ���٢�ʵ��
            //����ͼ���߱������ʣ�����ֻ��Ҫ��
        }
}
//Dijkstra�㷨ʵ��
void Dijkstra()
{
    //��ʼ��
    int i;
    Index=first;
    num=0;

    for(i=0;i<numV;i++)
    {//Ϊ·���;��븳��ֵ
        p[i]=unexist;
        d[i]=BD;
    }
    p[0]=Index;//��ѯ�ڵ�
    d[Index]=BD;//Ĭ���Լ����Լ�Ϊ���ɵ���ľ���
    for(i=0;i<numV;i++)//һ����һ����
    {//���±�ΪIndex�Ķ���Ϊ���Ĺ۲�ͼ���ҵ�����·��
        update(Index);
        findmin();//ÿ�δ�·����ѡȡһ�����ѯ�ڵ�����Ľڵ�ŵ�·��ĩβ
    }
}
//����ת����
void update(int a)
{
    int i,c;
    if(a==first)
    {
        for(i=0;i<numV;i++)
        {
            if(isexist(i))
                continue;
            c=G[a][i];//��ǰ��ѯ�ڵ�ͨ����ǰ��ѯ�ڵ㵽i��ľ���
            if(c<d[i])
            {
                d[i]=c;
                p_bef[i]=a;

            }
        }
    }
    else
    {
        for(i=0;i<numV;i++)
        {
            if(isexist(i))
                continue;
            c=G[a][i]+d[a];
            if(c<d[i])
            {
                d[i]=c;
                p_bef[i]=a;

            }
        }
    }
}
//Ѱ����С��·��
void findmin()
{
    int i,min,index,sign;
    min = BD;
    sign=0;
    for(i=0;i<numV;i++)
    {
        if(isexist(i))//����õ��Ѿ���·�����������
            continue;
        else
        {
            if(min>d[i])
            {
                min = d[i];
                index = i;
                sign=1;
            }
        }
    }
    if(sign==1)//��·���������£��򽫹۲������·��ĩβ
    {
        Index = index;
        p[++num] = index;
    }
}
//�жϲ�ѯ�ĵ��Ƿ��ѱ��㵽·����
int isexist(int a)
{
    int i;
    for(i=0;i<numV;i++)
    {
        if(a==p[i])
            return 1;
    }
    return 0;
}
//���չʾ
void Display()
{
    int i;
    printf("\n·����\n");
    for(i=0;i<numV;i++)
    {
        printf("%c  ",S[p[i]]);
    }
    printf("\n��һ����\n");
    printf(">>>");
    for(i=1;i<numV;i++)
    {
        printf("%c  ",S[p_bef[p[i]]]);
    }
    printf("\n����:\n");
    printf(">>>");
    for(i=1;i<numV;i++)
    {
        printf("%d  ",d[p[i]]);
    }
}

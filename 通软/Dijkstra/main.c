#include <stdio.h>
#include <stdlib.h>
#define BD 50//超远距离,超过该距离任务节点不可到达
#define unexist -1//顶点不存在，用于初始化路径图
#define numV 6//顶点数
#define flooding_file "data.txt"//洪泛信息文件
#define MaxVertices 100 //假设包含100个顶点
//全局变量声明
char S[]={'1','2','3','4','5','6'};//顶点集
int d[numV];//s到对应下标的距离
int p[numV];//路径顶点的下标
int p_bef[numV];//前置顶点的下标
int G[numV][numV];// 方向图，BD为不可达距离,Gij表示i到j方向
int Index,num;//Index为当前顶点的下标，num为p中顶点数量
int first;//查询节点的下标
//函数声明
void CreateGraph();//根据洪泛信息构造加权图
void Dijkstra();//Dijkstra算法的实现
void findmin();//找到转发表中最佳路径
void update(int);//以下标为i的顶点为基准，更新转发表
void Display();//结果展示
int isexist(int);//判断一个顶点是否在路径中已经存在，若存在返回1
//主函数
int main()
{
    int i;
    printf("顶点集:\n");
    for(i=0;i<numV;i++)
    {
        printf(" %c ",S[i]);
    }
    printf("\n输入查询节点下标：\n");
    scanf("%d",&first);
    if(first>=numV)
    {
        printf("节点不存在");
        exit(-1);
    }
    CreateGraph();
    Dijkstra();
    Display();
    return 0;
}
//图的生成函数
void CreateGraph()
{
    FILE *fp;
    int vi,vj,w,i,j;
    char ch;
    if((fp=fopen(flooding_file,"r"))==NULL)
        printf("文件打开失败");
    for(i=0;i<numV;i++)
    {//图的初始化
        for(j=0;j<numV;j++)
            {
                G[i][j]=BD;
            }
    }
        for(i=0;i<20;i++)
        {
            fscanf(fp,"%d%c%d%c%d%c",&vi,&ch,&vj,&ch,&w,&ch);
            G[vi-1][vj-1]=w;//①
            //无向图具有对称性的规律，通过①②实现
            //有向图不具备此性质，所以只需要①
        }
}
//Dijkstra算法实现
void Dijkstra()
{
    //初始化
    int i;
    Index=first;
    num=0;

    for(i=0;i<numV;i++)
    {//为路径和距离赋初值
        p[i]=unexist;
        d[i]=BD;
    }
    p[0]=Index;//查询节点
    d[Index]=BD;//默认自己到自己为不可到达的距离
    for(i=0;i<numV;i++)//一次找一个点
    {//以下标为Index的顶点为中心观察图，找到更优路径
        update(Index);
        findmin();//每次从路径外选取一个离查询节点最近的节点放到路径末尾
    }
}
//更新转发表
void update(int a)
{
    int i,c;
    if(a==first)
    {
        for(i=0;i<numV;i++)
        {
            if(isexist(i))
                continue;
            c=G[a][i];//当前查询节点通过当前查询节点到i点的距离
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
//寻找最小的路径
void findmin()
{
    int i,min,index,sign;
    min = BD;
    sign=0;
    for(i=0;i<numV;i++)
    {
        if(isexist(i))//如果该点已经在路径当中则忽略
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
    if(sign==1)//若路径有所更新，则将观察点跳到路径末尾
    {
        Index = index;
        p[++num] = index;
    }
}
//判断查询的点是否已被算到路径中
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
//结果展示
void Display()
{
    int i;
    printf("\n路径：\n");
    for(i=0;i<numV;i++)
    {
        printf("%c  ",S[p[i]]);
    }
    printf("\n上一跳：\n");
    printf(">>>");
    for(i=1;i<numV;i++)
    {
        printf("%c  ",S[p_bef[p[i]]]);
    }
    printf("\n距离:\n");
    printf(">>>");
    for(i=1;i<numV;i++)
    {
        printf("%d  ",d[p[i]]);
    }
}

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<math.h>
//k均值计算公式
#define formula (double)(data[itemID].d1-data[centerID].d1)*(data[itemID].d1-data[centerID].d1)+(double)(data[itemID].d2-data[centerID].d2)*(data[itemID].d2-data[centerID].d2)+(double)(data[itemID].d3-data[centerID].d3)*(data[itemID].d3-data[centerID].d3)
#define filename "data.txt"     //球队排名信息所在文件
#define MAX_ROUND_TIME 100		//最大的聚类次数
#define no_center -1        //没有中心，即是说当前球队未归类
#define abandon 100         //用于废弃数组中指定位置的数，使其不参与排序，全局无意义，只在排名时用到
void readDataFromFile();//读数据到data中
void stander_fy();//数据归一化
void init_center();//随机生成K_num个中心点
void init_memory();//初始化全局变量的储存空间
void K_means();//k均值算法实现
float d2center(int itemID,int centerID);//计算点到中心的距离
int iscenter(int);//判断一个点是否为中心点
void adjust_center();//根据聚类后的结果调整中心点的值
int needcontinue();//判断是否需要继续聚类
void sort();//对聚类的结果排序，将球队分为369等
void result_display();//结果展示

typedef struct Item{
    char name[20];
	float d1;		//06年
	float d2;		//10年
	float d3;       //07年
	int center_id_bef;      //前一个点对应的中心点在data中的下标
	int center_id;      //更新后的点对应的中心点在data中的下标
}Item;//数据结构体
Item* data;//所有数据，一行代表一个国家的信息
int* center;//中心点在data中的下标
int* count;//中心计数器
int data_size,K_num;//总的国家数，选取的中心点数
float* sum_d1;//用于调整中心值，以及排序
float* sum_d2;//用于调整中心值
float* sum_d3;//用于调整中心值
int* rank;  //用于排序
int main(){
    printf("输入样本个数：");
    scanf("%d",&data_size);
    printf("\n设置聚类中心的个数：");
    scanf("%d",&K_num);
    init_memory();
	readDataFromFile();//将数据读取到data空间中
	stander_fy();//归一化
	init_center();//随机选取中心
	do{
        K_means();
        adjust_center();

	}while(needcontinue());
	sort();
	result_display();
	return 0;
}

//从文件中读入数据
void readDataFromFile()
{
    int row;
	FILE* fread;
	if((fread = fopen(filename, "r"))==NULL)
        printf("文件打开失败");
	for( row = 0; row < data_size; row++ )
	{

		fscanf(fread, "%s %f %f %f\n",&data[row].name[0],&data[row].d1,&data[row].d2,&data[row].d3);
		data[row].center_id=no_center;
		data[row].center_id_bef=no_center;
	}
}
//数据归一化
void stander_fy()
{
    int row;
    float min1,min2,min3,max1,max2,max3,c1,c2,c3;
    min1=data[0].d1;max1=data[0].d1;
    min2=data[0].d2;max2=data[0].d2;
    min3=data[0].d3;max3=data[0].d3;
    for(row=0;row<data_size;row++)//找到每1列的峰峰值
    {
        if( min1>data[row].d1)min1 = data[row].d1;if(max1<data[row].d1)max1 = data[row].d1;
        if( min2>data[row].d2)min2 = data[row].d2;if(max2<data[row].d2)max2 = data[row].d2;
        if( min3>data[row].d3)min3 = data[row].d3;if(max3<data[row].d3)max3 = data[row].d3;
    }
    c1=max1-min1;c2=max2-min2;c3=max3-min3;
    for(row=0;row<data_size;row++)//标准化
    {
        data[row].d1 = (data[row].d1-min1)/c1;data[row].d2 = (data[row].d2-min2)/c2;data[row].d3 = (data[row].d3-min3)/c3;
    }
}
//初始化所有全局上有效的变量储存空间
void init_memory()
{
    data = (Item*)malloc(sizeof(Item) * (data_size));//初始化储存数据的空间的大小
	center = (int*)malloc(sizeof(int) * (K_num));//初始化中心点下标的大小
    count = (int*)malloc(sizeof(int)*K_num);//初始化计数器
    rank = (int*)malloc(sizeof(int)*K_num);//初始化计数器
    sum_d1=(float*)malloc(sizeof(float)*K_num);
    sum_d2=(float*)malloc(sizeof(float)*K_num);
    sum_d3=(float*)malloc(sizeof(float)*K_num);
}
//随机生成聚类中心，并初始化
void init_center()
{
    int i,random;//随机生成的中心点在data中的下标
    srand(time(NULL));//初始化随机数种子
	for( i = 0; i < K_num; i++ )
	{
	    random = (rand()%data_size)+1;
		if(iscenter(random))
        {
            i--;continue;
        }
        else
        {
            center[i]=random;
            data[random].center_id=random;
            data[random].center_id_bef=random;
        }
	}
}

//计算球队到聚类中心的距离
float d2center(int itemID,int centerID)
{
	return (float)sqrt(formula);
}
//k均值实现
void K_means()
{
    int i,row,min_index;
    float* a;//临时储存数据，用于选择最小值
    a = (float*)malloc(sizeof(float) * (K_num));
    for(row=0;row<data_size;row++)
    {
        if(iscenter(row))//判断row位置是不是中心点
            continue;
        else
        {
           for(i=0;i<K_num;i++)//若不是，计算row到各中心的距离
           {
               a[i]=d2center(row,center[i]);
           }
           min_index=0;
            for(i=1;i<K_num;i++)//通过找出点到多个中心点距离中的距离最小值对应的中心点的下标，判断该点属于哪一类
            {
                if(a[i]<a[min_index])
                {
                    min_index=i;
                }
            }
            data[row].center_id_bef=data[row].center_id;
            data[row].center_id=center[min_index];
        }
    }
}
//判断当前球队是否为聚类中心
int iscenter(int a)
{
    int i;
    for(i=0;i<K_num;i++)
    {
        if(a==center[i])
            return 1;
    }
    return 0;
}
//判断是否还需要继续聚类
int needcontinue()
{
    int i;
    int need=0;
    for(i=0;i<data_size;i++)//循环查询除中心点外的点的中心点是否变化，但凡有一个发生变化就需要继续；
    {
        if(iscenter(i))//判断row位置是不是中心点
            continue;
        else
        {
            if(data[i].center_id!=data[i].center_id_bef)
                need=1;
        }
    }
    return need;
}
//调整各个聚类中心的位置
void adjust_center()
{
    int row,i;
    for(i=0;i<K_num;i++)
    {
        sum_d1[i]=0;sum_d2[i]=0;sum_d3[i]=0;//将调整中心值依据的数据初始化
        count[i]=0;
    }
    for(row=0;row<data_size;row++)
    {
        for(i=0;i<K_num;i++)
        {
            if(data[row].center_id==center[i])
            {
                sum_d1[i]=sum_d1[i]+data[row].d1;
                sum_d2[i]=sum_d2[i]+data[row].d2;
                sum_d3[i]=sum_d3[i]+data[row].d3;
                count[i]++;
                break;
            }

        }
    }
    for(i=0;i<K_num;i++)//调整均值
    {
        data[center[i]].d1 = sum_d1[i]/count[i];
        data[center[i]].d2 = sum_d2[i]/count[i];
        data[center[i]].d3 = sum_d3[i]/count[i];
    }
}
//根据聚类中心的相关参数对类排名
void sort()
{
    int i,m,index;
    for(i=0;i<K_num;i++)
    {
        sum_d1[i]=(sum_d1[i]+sum_d2[i]+sum_d3[i])/3;
    }
    for(i=0;i<K_num;i++)
    {
        index=0;
        for(m=0;m<K_num;m++)
        {
            if(sum_d1[index]>sum_d1[m])
                index=m;
        }
        sum_d1[index]=abandon;
        rank[i]=index;
    }
}
//结果展示
void result_display()
{
    int i,m;
    for(i=0;i<K_num;i++)
    {
        printf("\n世界 %d 流：\n",i+1);
        for(m=0;m<data_size;m++)
        {
            if(data[m].center_id==center[rank[i]])
            {
                printf("%s  ",data[m].name);
                if((--count[rank[i]])==0)
                    break;
            }
            else
                continue;
        }
        printf("\n选取的中心点：");
        printf("%s\n",data[center[rank[i]]].name);
    }
}

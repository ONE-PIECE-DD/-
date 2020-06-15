#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<math.h>
//k��ֵ���㹫ʽ
#define formula (double)(data[itemID].d1-data[centerID].d1)*(data[itemID].d1-data[centerID].d1)+(double)(data[itemID].d2-data[centerID].d2)*(data[itemID].d2-data[centerID].d2)+(double)(data[itemID].d3-data[centerID].d3)*(data[itemID].d3-data[centerID].d3)
#define filename "data.txt"     //���������Ϣ�����ļ�
#define MAX_ROUND_TIME 100		//���ľ������
#define no_center -1        //û�����ģ�����˵��ǰ���δ����
#define abandon 100         //���ڷ���������ָ��λ�õ�����ʹ�䲻��������ȫ�������壬ֻ������ʱ�õ�
void readDataFromFile();//�����ݵ�data��
void stander_fy();//���ݹ�һ��
void init_center();//�������K_num�����ĵ�
void init_memory();//��ʼ��ȫ�ֱ����Ĵ���ռ�
void K_means();//k��ֵ�㷨ʵ��
float d2center(int itemID,int centerID);//����㵽���ĵľ���
int iscenter(int);//�ж�һ�����Ƿ�Ϊ���ĵ�
void adjust_center();//���ݾ����Ľ���������ĵ��ֵ
int needcontinue();//�ж��Ƿ���Ҫ��������
void sort();//�Ծ���Ľ�����򣬽���ӷ�Ϊ369��
void result_display();//���չʾ

typedef struct Item{
    char name[20];
	float d1;		//06��
	float d2;		//10��
	float d3;       //07��
	int center_id_bef;      //ǰһ�����Ӧ�����ĵ���data�е��±�
	int center_id;      //���º�ĵ��Ӧ�����ĵ���data�е��±�
}Item;//���ݽṹ��
Item* data;//�������ݣ�һ�д���һ�����ҵ���Ϣ
int* center;//���ĵ���data�е��±�
int* count;//���ļ�����
int data_size,K_num;//�ܵĹ�������ѡȡ�����ĵ���
float* sum_d1;//���ڵ�������ֵ���Լ�����
float* sum_d2;//���ڵ�������ֵ
float* sum_d3;//���ڵ�������ֵ
int* rank;  //��������
int main(){
    printf("��������������");
    scanf("%d",&data_size);
    printf("\n���þ������ĵĸ�����");
    scanf("%d",&K_num);
    init_memory();
	readDataFromFile();//�����ݶ�ȡ��data�ռ���
	stander_fy();//��һ��
	init_center();//���ѡȡ����
	do{
        K_means();
        adjust_center();

	}while(needcontinue());
	sort();
	result_display();
	return 0;
}

//���ļ��ж�������
void readDataFromFile()
{
    int row;
	FILE* fread;
	if((fread = fopen(filename, "r"))==NULL)
        printf("�ļ���ʧ��");
	for( row = 0; row < data_size; row++ )
	{

		fscanf(fread, "%s %f %f %f\n",&data[row].name[0],&data[row].d1,&data[row].d2,&data[row].d3);
		data[row].center_id=no_center;
		data[row].center_id_bef=no_center;
	}
}
//���ݹ�һ��
void stander_fy()
{
    int row;
    float min1,min2,min3,max1,max2,max3,c1,c2,c3;
    min1=data[0].d1;max1=data[0].d1;
    min2=data[0].d2;max2=data[0].d2;
    min3=data[0].d3;max3=data[0].d3;
    for(row=0;row<data_size;row++)//�ҵ�ÿ1�еķ��ֵ
    {
        if( min1>data[row].d1)min1 = data[row].d1;if(max1<data[row].d1)max1 = data[row].d1;
        if( min2>data[row].d2)min2 = data[row].d2;if(max2<data[row].d2)max2 = data[row].d2;
        if( min3>data[row].d3)min3 = data[row].d3;if(max3<data[row].d3)max3 = data[row].d3;
    }
    c1=max1-min1;c2=max2-min2;c3=max3-min3;
    for(row=0;row<data_size;row++)//��׼��
    {
        data[row].d1 = (data[row].d1-min1)/c1;data[row].d2 = (data[row].d2-min2)/c2;data[row].d3 = (data[row].d3-min3)/c3;
    }
}
//��ʼ������ȫ������Ч�ı�������ռ�
void init_memory()
{
    data = (Item*)malloc(sizeof(Item) * (data_size));//��ʼ���������ݵĿռ�Ĵ�С
	center = (int*)malloc(sizeof(int) * (K_num));//��ʼ�����ĵ��±�Ĵ�С
    count = (int*)malloc(sizeof(int)*K_num);//��ʼ��������
    rank = (int*)malloc(sizeof(int)*K_num);//��ʼ��������
    sum_d1=(float*)malloc(sizeof(float)*K_num);
    sum_d2=(float*)malloc(sizeof(float)*K_num);
    sum_d3=(float*)malloc(sizeof(float)*K_num);
}
//������ɾ������ģ�����ʼ��
void init_center()
{
    int i,random;//������ɵ����ĵ���data�е��±�
    srand(time(NULL));//��ʼ�����������
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

//������ӵ��������ĵľ���
float d2center(int itemID,int centerID)
{
	return (float)sqrt(formula);
}
//k��ֵʵ��
void K_means()
{
    int i,row,min_index;
    float* a;//��ʱ�������ݣ�����ѡ����Сֵ
    a = (float*)malloc(sizeof(float) * (K_num));
    for(row=0;row<data_size;row++)
    {
        if(iscenter(row))//�ж�rowλ���ǲ������ĵ�
            continue;
        else
        {
           for(i=0;i<K_num;i++)//�����ǣ�����row�������ĵľ���
           {
               a[i]=d2center(row,center[i]);
           }
           min_index=0;
            for(i=1;i<K_num;i++)//ͨ���ҳ��㵽������ĵ�����еľ�����Сֵ��Ӧ�����ĵ���±꣬�жϸõ�������һ��
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
//�жϵ�ǰ����Ƿ�Ϊ��������
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
//�ж��Ƿ���Ҫ��������
int needcontinue()
{
    int i;
    int need=0;
    for(i=0;i<data_size;i++)//ѭ����ѯ�����ĵ���ĵ�����ĵ��Ƿ�仯��������һ�������仯����Ҫ������
    {
        if(iscenter(i))//�ж�rowλ���ǲ������ĵ�
            continue;
        else
        {
            if(data[i].center_id!=data[i].center_id_bef)
                need=1;
        }
    }
    return need;
}
//���������������ĵ�λ��
void adjust_center()
{
    int row,i;
    for(i=0;i<K_num;i++)
    {
        sum_d1[i]=0;sum_d2[i]=0;sum_d3[i]=0;//����������ֵ���ݵ����ݳ�ʼ��
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
    for(i=0;i<K_num;i++)//������ֵ
    {
        data[center[i]].d1 = sum_d1[i]/count[i];
        data[center[i]].d2 = sum_d2[i]/count[i];
        data[center[i]].d3 = sum_d3[i]/count[i];
    }
}
//���ݾ������ĵ���ز�����������
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
//���չʾ
void result_display()
{
    int i,m;
    for(i=0;i<K_num;i++)
    {
        printf("\n���� %d ����\n",i+1);
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
        printf("\nѡȡ�����ĵ㣺");
        printf("%s\n",data[center[rank[i]]].name);
    }
}

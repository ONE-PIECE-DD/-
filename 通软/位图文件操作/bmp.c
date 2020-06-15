#include "bmp.h"
//�򿪵�ַ��Ӧ��bmp�ļ�������λͼ������
ClImage* clLoadImage(const char* path)
{
    ClImage* bmpImg;
    FILE* pFile;
    unsigned short fileType;
    ClBitMapFileHeader bmpFileHeader;
    ClBitMapInfoHeader bmpInfoHeader;
    int channels = 1;
    int width = 0;
    int height = 0;
    int step = 0;
    int offset = 0;
    unsigned char pixVal;
    int i, j, k;

    bmpImg = (ClImage*)malloc(sizeof(ClImage));
    pFile = fopen(path, "rb");
    if (!pFile)
    {
        free(bmpImg);
        return NULL;
    }

    fread(&fileType, sizeof(unsigned short), 1, pFile);
    if (fileType == 0x4D42)
    {
        //printf("bmp file! \n");

        fread(&bmpFileHeader, sizeof(ClBitMapFileHeader), 1, pFile);
        fread(&bmpInfoHeader, sizeof(ClBitMapInfoHeader), 1, pFile);
        if (bmpInfoHeader.biBitCount == 24)
        {
            //printf("��λͼΪλ���ɫ\n\n");
            channels = 3;
            width = bmpInfoHeader.biWidth;
            height = bmpInfoHeader.biHeight;

            bmpImg->width = width;
            bmpImg->height = height;
            bmpImg->channels = 3;
            bmpImg->imageData = (unsigned char*)malloc(sizeof(unsigned char)*width*3*height);//�����ص�λw*h��ÿ�����ص�3�ֽ�
            step = channels*width;//ͼƬ��һ�����ص��Ӧ�����������ֽڣ�
            //ͼ���ϵ����ݰ��д��棬ÿһ�е��ֽ���Ϊ4�ı�����������ֽ���0����
            offset = (channels*width)%4;
            if (offset != 0)//�ж�ÿһ�в��ϵ�0
            {
                offset = 4 - offset;//
            }

            for (i=0; i<height; i++)
            {
                for (j=0; j<width; j++)
                {
                    for (k=0; k<3; k++)
                    {
                        fread(&pixVal, sizeof(unsigned char), 1, pFile);//ÿ�ζ�ȡһ���ֽ�
                        bmpImg->imageData[(height-1-i)*step+j*3+k] = pixVal;//ȷ������һ�е����ݴ����������ĩβ���������ص���ͼƬ�е�λ�����������ص�������������е�λ��һ��
                    }
                    //kzSetMat(bmpImg->mat, height-1-i, j, kzScalar(pixVal[0], pixVal[1], pixVal[2]));
                }
                //����һ�к�ɾ������ֽ�
                if (offset != 0)
                {
                    for (j=0; j<offset; j++)
                    {
                        fread(&pixVal, sizeof(unsigned char), 1, pFile);
                    }
                }
            }
        }
    }

    return bmpImg;
}

bool clSaveImage(const char* path, ClImage* bmpImg)
{
    FILE *pFile;
    unsigned short fileType;//�����ʽ
    ClBitMapFileHeader bmpFileHeader;//�ļ�ͷ
    ClBitMapInfoHeader bmpInfoHeader;//��Ϣͷ
    int step;//һ�����������ֽڣ�
    int offset;//һ���������������ֽڣ�
    unsigned char pixVal = '\0';
    int i, j;

    pFile = fopen(path, "wb");//��path·����Ӧ���ĵ��м�¼����
    if (!pFile)
    {
        return false;
    }

    fileType = 0x4D42;//�ļ���ʽ
    fwrite(&fileType, sizeof(unsigned short), 1, pFile);//д�������ֽڵ��ļ���ʽ

    //���ɫͼ�Ĵ���
    if (bmpImg->channels == 3)//24λ��ͨ������ͼ
    {//Ϊһ����������ֵ���ֽڣ�

        step = bmpImg->channels*bmpImg->width;
        offset = step%4;
        if (offset != 4)
        {
            step += 4-offset;
        }
        //Ϊ�ļ�ͷ��Ϣ��ֵ����д���ļ�
        bmpFileHeader.bfSize = bmpImg->height*step + 54;
        bmpFileHeader.bfReserved1 = 0;
        bmpFileHeader.bfReserved2 = 0;
        bmpFileHeader.bfOffBits = 54;
        fwrite(&bmpFileHeader, sizeof(ClBitMapFileHeader), 1, pFile);
        //Ϊλͼ��Ϣͷ��ֵ����д���ļ�
        bmpInfoHeader.biSize = 40;
        bmpInfoHeader.biWidth = bmpImg->width;
        bmpInfoHeader.biHeight = bmpImg->height;
        bmpInfoHeader.biPlanes = 1;
        bmpInfoHeader.biBitCount = 24;
        bmpInfoHeader.biCompression = 0;
        bmpInfoHeader.biSizeImage = bmpImg->height*step;
        bmpInfoHeader.biXPelsPerMeter = 0;
        bmpInfoHeader.biYPelsPerMeter = 0;
        bmpInfoHeader.biClrUsed = 0;
        bmpInfoHeader.biClrImportant = 0;
        fwrite(&bmpInfoHeader, sizeof(ClBitMapInfoHeader), 1, pFile);
        //��λͼ����д���ļ�
        for (i=bmpImg->height-1; i>-1; i--)//ȷ��������ĩβ������д���ļ����ײ����Է���ͼ��λͼ���ݵĴ洢��׼
        {
            for (j=0; j<bmpImg->width; j++)
            {
                pixVal = bmpImg->imageData[i*bmpImg->width*3+j*3];
                fwrite(&pixVal, sizeof(unsigned char), 1, pFile);
                pixVal = bmpImg->imageData[i*bmpImg->width*3+j*3+1];
                fwrite(&pixVal, sizeof(unsigned char), 1, pFile);
                pixVal = bmpImg->imageData[i*bmpImg->width*3+j*3+2];
                fwrite(&pixVal, sizeof(unsigned char), 1, pFile);
            }
            //ÿд��һ�����ص�������ж��Ƿ���Ҫ���
            if (offset!=0)
            {
                for (j=0; j<4-offset; j++)
                {
                    pixVal = 0;
                    fwrite(&pixVal, sizeof(unsigned char), 1, pFile);
                }
            }
        }
    }
    fclose(pFile);

    return true;
}

//�������λͼ���ݻҶȻ������ش�����λͼ�����ļ�
ClImage* clGrayscaleImage(ClImage* bmpImg)
{
    int i,j;
    int Gray;
    int width = bmpImg->width;
    int height = bmpImg->height;
    int step = bmpImg->channels*bmpImg->width;
    for (i=0; i<height; i++)
    {
        for (j=0; j<width; j++)
        {

                //Gray=(bmpImg->imageData[i*step+j*3+0]*0.3+bmpImg->imageData[i*step+j*3+1]*0.59+bmpImg->imageData[i*step+j*3+2]*0.11);
                Gray=(bmpImg->imageData[i*step+j*3+0]+bmpImg->imageData[i*step+j*3+1]+bmpImg->imageData[i*step+j*3+2])/3;
                bmpImg->imageData[i*step+j*3+0]=Gray;
                bmpImg->imageData[i*step+j*3+1]=Gray;
                bmpImg->imageData[i*step+j*3+2]=Gray;
        }
    }
    return bmpImg;
}
ClImage* clBinImage(ClImage* bmpImg)
{
    int i,j;
    int threshold = 0;
    double threshold1 = 0;
    int width = bmpImg->width;
    int height = bmpImg->height;
    int step = bmpImg->channels*bmpImg->width;
    double all = width*height;
    //�����������ص�ĻҶ�ƽ��ֵ��������Ϊ��ֵ
    for (i=0; i<height; i++)
    {
        for (j=0; j<width; j++)
        {
            threshold1 += (((double)bmpImg->imageData[i*step+j*3])/all);
        }
    }
    threshold = (int)threshold1;
    for (i=0; i<height; i++)
    {
        for (j=0; j<width; j++)
        {
                if(bmpImg->imageData[i*step+j*3]<=threshold)
                {
                    bmpImg->imageData[i*step+j*3+0]=0;
                    bmpImg->imageData[i*step+j*3+1]=0;
                    bmpImg->imageData[i*step+j*3+2]=0;
                }
                else
                {
                    bmpImg->imageData[i*step+j*3+0]=255;
                    bmpImg->imageData[i*step+j*3+1]=255;
                    bmpImg->imageData[i*step+j*3+2]=255;
                }
        }
    }
    return bmpImg;
}

ClImage* clfinger(ClImage* bmpImg)
{//��ֵ������������Ʋ��������뱳���ֿ������Թ������Ʋ��ֵ����ص�Ϊ���ݽ��ж�ֵ������Ч��������
 //���Ա�����ֹʵ�飬

 //����ָ��ϸ�ķ�����
 //����ѹ��ͼƬ
 //�ж����Ʒ���ķ�����
 //����ѹ��ͼƬʹͼƬ�ߴ��������Ƴߴ磬������ͼƬ���������ȷֲ����ж����ưݷ�
    return bmpImg;
}

ClImage* clspan(ClImage* bmpImg)
{
    ClImage* bmpImg1;
    bmpImg1 = (ClImage*)malloc(sizeof(ClImage));
    int i,j,m,n;
    int width = bmpImg->width;
    int height = bmpImg->height;
    int width1 = height;
    int height1 = width;
    bmpImg1->height=height1;
    bmpImg1->width=width1;
    bmpImg1->channels=bmpImg->channels;
    int step = bmpImg->channels*width;
    int step1 = bmpImg1->channels*width1;
    bmpImg1->imageData = (unsigned char*)malloc(sizeof(unsigned char)*width1*3*height1);
    n=width1-1;//��
    for (i=0; i<height; i++)//��
    {
        m=0;//��
        for (j=0; j<width; j++)//��
        {//ͨ������ļ���ʵ������ת��
            bmpImg1->imageData[m*step1+n*3+0]=bmpImg->imageData[i*step+j*3+0];
            bmpImg1->imageData[m*step1+n*3+1]=bmpImg->imageData[i*step+j*3+1];
            bmpImg1->imageData[m*step1+n*3+2]=bmpImg->imageData[i*step+j*3+2];
            m++;
        }
        n--;
    }
    return bmpImg1;
}

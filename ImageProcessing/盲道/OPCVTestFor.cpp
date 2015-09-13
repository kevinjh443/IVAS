// OPCVTestFor.cpp : 定义控制台应用程序的入口点。
//
//#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" )

#include "stdafx.h"
#include "cv.h"
#include "cxcore.h"
#include "ml.h"
#include "stdio.h"
#include "CGlcm.h"
#include "stdlib.h"
#include "highgui.h"
#include <cxcore.h>
#include <time.h>
#include <math.h>
#include "iostream"
#include <tlhelp32.h>
#include <shlwapi.h>


//#include "opencv.hpp"
using namespace std;
using namespace cv; 

#pragma comment(lib, "shlwapi.lib")

struct ColorM{
	double CM1r,CM1g,CM1b;
	double CM2r,CM2g,CM2b;
	double CM3r,CM3g,CM3b;
};



struct Feature{
	ColorM * cm;
	CvHuMoments * humoments;
};



///////////////////////////////////////数据定义////////////////////////////////
#define Data 1315//2260整体盲道识别 //样本数据量
#define In 46 // 输入
#define Out 1 //输出
#define Neuron 49 //神经元
#define TrainC 90000 //训练次数
#define A 0.2//输出层惯性系数
#define B 0.4//输出层学习率
#define a 0.2//隐藏层惯性系统
#define b 0.3//隐藏层学习率
#define PRECISE 0.001//训练精度
#define TEST_SAMPLE 9//测试样本数据个数

/////////////////////////////数据存储准备/////////////////////////////////////////
double d_in[Data][In],d_out[Data][Out]; //样本输出和输出
double w[Neuron][In],o[Neuron],v[Out][Neuron]; //神经元的权值
double Maxin[In],Minin[In],Maxout[Out],Minout[Out];
double OutputData[Out]; //神经元的输出
double dv[Out][Neuron],dw[Neuron][In]; //神经元的权值的修正量
double e;
/////////////////////////////////////////////////////////////////////////////////
#define MulPow10 1.0e+010
#define MulPow20 1.0e+020
#define Scanle 10;
/////////////////////////////////////////////////////////////////////////////////
struct NeuronNet{
	double w[Neuron][In];
	double v[Out][Neuron];
	double o[Neuron];
	double Maxin[In],Minin[In],Maxout[Out],Minout[Out];
};
/////////////////////////////////////////////////////////////////////////////////
int ColorMoment(IplImage * src,ColorM * cm);
int UColorM();
int TrainTheSample(int oris);
void ReadData();
void InitBPNework();
void Comput(int var);
void BackUpdate(int var);
double Result(double * var);
double Result(double * var,NeuronNet &net);
void WriteNeuron();
void TrainNetwork();
void TestData();

void ReadNeuron();
void UseBPNeuron();

void FeatureExtr(IplImage * img, double  feature[]);
void TrainTheSampleFromFile();
void UseBPNeuronFromFile();

void CutImage();

void SplitImage(const IplImage *imgSrc, const int n, vector<IplImage*> &subImgs);

void CutImageSample();

void ReadNeuron(int key,NeuronNet &net);/*指定选择  1=盲道识别网络；*/
void ReadNeuron(int key,NeuronNet &net,const char *neur_filename,const char *MaxMin_filename);
void CutImageSampleClassify();//切割图像分层成类

void UseCutImage();
void Optimization();
void Multi_obj_identify();
int compare_type(int *comp,int n);
void ONE_obj_identify();
char * this_way(char *str);


int _tmain(int argc, _TCHAR* argv[])
{
#if 0//读取网络调用方法
	NeuronNet * nnet = new NeuronNet;

	ReadNeuron(2,*nnet);

	for (int i=0;i<Neuron;i++)
	{
		for (int j=0;j<In;j++)
		{
			cout<<nnet->w[i][j]<<"\t";
			getchar();
		}
	}

	delete []nnet;
#endif
	

#if 1
	int count=-1,oris=0,key=-1;
to1:	cout<<"请输入：（1=样本收集、特征提取、训练；2=使用识别优化；3=正式多目标使用识别，0=out）:\n"<<endl;
	//cin>>count;
	count=4;


	if (count == 1)
	{
		cout<<"\n如果直接利用样本数据BP训练，请按3；样本收集和训练，按4：\n";
		cin>>count;
		if (3 == count)
		{
			ReadData();           //读取训练样本数据
			InitBPNework();       //初始化神经元网络
			TrainNetwork();        //训练神经元网络
			WriteNeuron();         //存储BP权值参数
			TestData();            //测试数据读取并求取结果
			//system("pause");
		}else if (4 == count)
		{
to2:			cout<<"直接从摄像头按4,从视频文件中选择性找样本按3,5=样本切分30再选样本，55分层切割归类样本：（正样本按1，负样本按0，不能作为样本的按2）："<<endl;
			cin>>key;
			if (55 == key)
			{
				CutImageSampleClassify();
			}
			else if (5 == key)
			{
				
to5:				cout<<"\n是否清理样本特征文件？1=yes;0=no:\n"<<endl;
			cin>>key;
			FILE * fp=NULL, * fpr=NULL;
			if (1 == key)
			{
				fp = fopen("sample.txt","w+");fclose(fp);
				fpr = fopen("sampleres.txt","w+");fclose(fp);
			}

			CutImageSample();

			cout<<"是否继续进行：1=yes,0=no,0=退出到顶层:\n"<<endl;
			cin>>key;
			if (1 == key)
			{
				goto to5;
			}else if (0 == key)
			{
				cout<<"\n请用1.3训练数据\n"<<endl;
				goto to1;
			}
			}
			else if (4 == key)
			{
				FILE * fptr=fopen("sampleres.txt","w+");
				FILE * fpt=fopen("sample.txt","w+");
				fclose(fpt);fclose(fptr);
				cout<<"提取正样本？（1=正样本）;：\n"<<endl;
				cin>>oris;
				if (oris == 1)
				{
					TrainTheSample(oris);
				}
		
				cout<<"\n提取负样本？（0=负样本）;：\n"<<endl;
				cin>>oris;
				if (0 == oris)
				{
					TrainTheSample(oris);
				}

				cout<<"\n一个手势的正负样本提取完毕！！！请训练：\n"<<endl;

				//system("pause");


				ReadData();           //读取训练样本数据
				InitBPNework();       //初始化神经元网络
				TrainNetwork();        //训练神经元网络
				WriteNeuron();         //存储BP权值参数
				TestData();            //测试数据读取并求取结果

				//system("pause");
			}else if (3 == key)
			{
	to3:			cout<<"\n是否清理样本特征文件？1=yes;0=no:\n"<<endl;
				cin>>key;
				FILE * fp=NULL, * fpr=NULL;
				if (1 == key)
				{
					fp = fopen("sample.txt","w+");fclose(fp);
					fpr = fopen("sampleres.txt","w+");fclose(fp);
				}
		
				TrainTheSampleFromFile();

				cout<<"是否继续进行：1=yes,0=no,0=退出到顶层:\n"<<endl;
				cin>>key;
				if (1 == key)
				{
					goto to3;
				}else if (0 == key)
				{
					cout<<"\n请用1.3训练数据\n"<<endl;
					goto to1;
				}

			}else{
				goto to2;
			}
		

		}		
	}else if (count == 2)
	{
		cout<<"\n验证视频文件学习请按1，2=摄像头，3视频文件分层验证，33=分层严格验证,333=优化：\n"<<endl;
		cin>>key;
		if (1 == key)
		{
			ReadNeuron();
			UseBPNeuronFromFile();
			//system("pause");
		}else if (2 == key)
		{
			ReadNeuron();
			UseBPNeuron();

			//system("pause");
		}else if (3 == key)
		{
			ReadNeuron();
			CutImage();
			system("pause");
		}else if (33 == key)
		{
			UseCutImage();
		}else if (333 == key)
		{
			Optimization();
		}
		

		count=3;
	}else if (0 == count)
	{
		exit(0);
	}else if (3 == count)
	{
		printf("正式使用（多目标识别）：确定：");
		//getchar();
		Multi_obj_identify();
	}else if (4 == count)
	{
		printf("正式使用（单目标识别）：确定：");
		//getchar();
		ONE_obj_identify();
	}
	
	if (count!=0 && count != 1 && count != 2)
	{
		goto to1;
	}
		
#endif


	return 0;
}

int flag=0;
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,LPSTR lpCmdLine, int nCmdShow )
{
	_tmain(0,0);

	return 0;
}


int ColorMoment(IplImage * src,ColorM * cm)
{
	if (src->nChannels != 3)
	{
		return 0;
	}

	int i,j;
	int x = src->width;
	int y = src->height;
	double tempR=0,tempG=0,tempB=0;
	double CM1r=0,CM1g=0,CM1b=0;
	double CM2r=0,CM2g=0,CM2b=0;
	long double CM3r=0,CM3g=0,CM3b=0;
	int step = src->widthStep/sizeof(uchar);
	int channels = src->nChannels;
	uchar * data = (uchar*)src->imageData;

	for (i = 0;i<y;i++)
	{
		for (j = 0;j<x;j++)
		{
			tempB += data[i*step + j*channels + 0];
			tempG += data[i*step + j*channels + 1];
			tempR += data[i*step + j*channels + 2];
			//printf("(%lf,%lf,%lf)\t",tempB,tempG,tempR);
		}
	}

	CM1b = tempB / (x*y);//B颜色的均值，一阶矩
	CM1g = tempG / (x*y);//G颜色的均值，一阶矩
	CM1r = tempR / (x*y);//R颜色的均值，一阶矩

	tempB = 0; tempG = 0; tempR = 0;
	for (i = 0;i<y;i++)
	{
		for (j = 0;j<x;j++)
		{
			tempB = data[i*step + j*channels + 0];
			tempG = data[i*step + j*channels + 1];
			tempR = data[i*step + j*channels + 2];

			CM2b += (tempB - CM1b) * (tempB - CM1b);
			CM2g += (tempG - CM1g) * (tempG - CM1g);
			CM2r += (tempR - CM1r) * (tempR - CM1r);

			CM3b += (tempB - CM1b) * (tempB - CM1b) * (tempB - CM1b);
			CM3g += (tempG - CM1g) * (tempG - CM1g) * (tempG - CM1g);
			CM3r += (tempR - CM1r) * (tempR - CM1r) * (tempR - CM1r);
			//printf("【%llf】\t%lf\t",CM3b,(tempB - CM1b) * (tempB - CM1b) * (tempB - CM1b));
		}
		//getchar();
	}

	CM2b = sqrt(CM2b / (x*y));//B标准差，二阶矩
	CM2g = sqrt(CM2g / (x*y));//G标准差，二阶矩
	CM2r = sqrt(CM2r / (x*y));//R标准差，二阶矩

	tempB = CM3b / (x*y);
	tempG = CM3g / (x*y);
	tempR = CM3r / (x*y);
	if (tempB < 0)
	{
		tempB = -tempB;
		CM3b = -pow(tempB , 1.0/3);//B三阶矩
	}else{
		CM3b = pow(tempB , 1.0/3);//B三阶矩
	}
	if (tempG < 0)
	{
		tempG = -tempG;
		CM3g = -pow(tempG , 1.0/3);//G三阶矩
	}else{
		CM3g = pow(tempG , 1.0/3);//G三阶矩
	}
	if (tempR < 0)
	{
		tempR = -tempR;
		CM3r = -pow(tempR , 1.0/3);//R三阶矩
	}else{
		CM3r = pow(tempR , 1.0/3);//R三阶矩
	}
	/*
	CM3b = pow(tempB , 1.0/3);//B三阶矩
	CM3g = pow(tempG , 1.0/3);//G三阶矩
	CM3r = pow(tempR , 1.0/3);//R三阶矩*/


	cm->CM1b = CM1b / 255;
	cm->CM1g = CM1g / 255;
	cm->CM1r = CM1r / 255;
	cm->CM2b = CM2b / 255;
	cm->CM2g = CM2g / 255;
	cm->CM2r = CM2r / 255;
	cm->CM3b = CM3b / 255;
	cm->CM3g = CM3g / 255;
	cm->CM3r = CM3r / 255;

	//printf("颜色矩：%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t",CM1b,CM1g,CM1r,CM2b,CM2g,CM2r,CM3b,CM3g,CM3r);
	//getchar();
	return 1;
}

int UColorM()
{
	char imgpathtem[500];
	int count=0;
	int oris=3;
	FILE * fp=fopen("patht.txt","a+");
	FILE * fptr=fopen("sampleres.txt","a+");
	FILE * fpt=fopen("sample.txt","a+");

	IplImage * img = NULL;
	cvNamedWindow("img");
	
	ColorM * cm = new ColorM;

	while (!feof(fp))
	{
		count++;
		fscanf(fp,"%s\n",&imgpathtem);
		img = cvLoadImage(imgpathtem,1);
		IplImage * img2 = img;
		cvSmooth(img,img2,CV_GAUSSIAN,3,3,0,0);
		cvShowImage("img",img2);
		cvWaitKey(500);
		
		ColorMoment(img2 ,cm);

		printf("请确认%d颜色矩(1=正，0=负)：%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\n",count,
					cm->CM1b,cm->CM1g,cm->CM1r,cm->CM2b,cm->CM2g,cm->CM2r,cm->CM3b,cm->CM3g,cm->CM3r);
		scanf("%d",&oris);
		fprintf(fpt,"%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\n",
			cm->CM1b,cm->CM1g,cm->CM1r,cm->CM2b,cm->CM2g,cm->CM2r,cm->CM3b,cm->CM3g,cm->CM3r);
		fprintf(fptr,"%d\n",oris);
	}
	cvReleaseImage(&img);
	cvDestroyWindow("img");

	fclose(fp);
	fclose(fpt);
	fclose(fptr);
	system("pause");

	return 0;
}



//////////////////////////////////////////样本特征集收集/////////////////////////////////////////////////////
int TrainTheSample(int oris)
{
	int count=0,i = 0;

	//声明IplImage指针   
	IplImage* pFrame = NULL;  
	//获取摄像头   
	CvCapture* pCapture = cvCreateCameraCapture(0);  
	//CvCapture *pCapture = cvCaptureFromCAM( CV_CAP_ANY );//初始化摄像头   
	//创建窗口   
	cvNamedWindow("video",1);  
	//显示视屏   


	FILE * fptr=fopen("sampleres.txt","a+");
	FILE * fpt=fopen("sample.txt","a+");
	if (0 == oris)
	{
		//fprintf(fpt,"\n");
		//fprintf(fptr,"\n");
	}

	IplImage * img = NULL;
	//ColorM * cm = new ColorM;

	//CvMoments * moments = new CvMoments;
	//CvHuMoments *  humoments = new CvHuMoments;
	//Feature  feature = new Feature;
	double * feature = new double[In];//特征值存储准备

	while(1)  
	{  
		pFrame=cvQueryFrame( pCapture );  
		if(!pFrame)break;   
		cvShowImage("video",pFrame);

		////////////////////////////////////对获得的照片·预处理////////////////////////////////////////
				

		img = pFrame;
		cvSmooth(pFrame,img,CV_GAUSSIAN,3,3,0,0);

		
		FeatureExtr(img,feature);//求取特征值

		if (feature[0] != 0.0 && feature[1]!= feature[2]&& feature[3]!= feature[4])
		{
			count ++;
			printf("%d个有效颜色矩,hu矩：",count);
			for (i=0;i<In;i++)
			{
				printf("%lf\t",feature[i]);
			}
			printf("\n");

		/*	printf("%d个有效颜色矩,hu矩：%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t   %lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\n"
				
				,count,
				feature->cm->CM1b,feature->cm->CM1g,feature->cm->CM1r,feature->cm->CM2b,feature->cm->CM2g,
				feature->cm->CM2r,feature->cm->CM3b,feature->cm->CM3g,feature->cm->CM3r,
				
				feature->humoments->hu1,feature->humoments->hu2*MulPow10,feature->humoments->hu3*MulPow10,
				feature->humoments->hu4*MulPow10,feature->humoments->hu5*MulPow20,feature->humoments->hu6*MulPow20,
				feature->humoments->hu7*MulPow20
				);*/
			if (count > 5 && count <250)
			{
				/*fprintf(fpt,"%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\n",
					cm->CM1b,cm->CM1g,cm->CM1r,cm->CM2b,cm->CM2g,cm->CM2r,cm->CM3b,cm->CM3g,cm->CM3r,
					humoments->hu1,humoments->hu2*MulPow10,humoments->hu3*MulPow10,humoments->hu4*MulPow10,humoments->hu5*MulPow20,humoments->hu6*MulPow20,humoments->hu7*MulPow20
					);*/

				for (i=0;i<In;i++)
				{
					fprintf(fpt,"%lf\t",feature[i]);
				}
				fprintf(fpt,"\n");
				fprintf(fptr,"%d\n",oris);
			}else if (count > 250)
			{
				break;
			}
			
		}
		
		
		/////////////////////////////////////////////////////////////////////////////////////////////////
		//cvReleaseImage(&dst);
		char c=cvWaitKey(33);  
		if(c==27)break;  
	}  

	fclose(fpt);
	fclose(fptr);

	cvReleaseCapture(&pCapture); 
	//delete []cm;
	//delete []moments;
	//delete []humoments;
	delete []feature;
	cvDestroyWindow("video"); 
	return 0;
}

/////////////////////////////读取训练数据////////////////////////////////////////
void ReadData(){
	FILE *fp1,*fp2;
	int i,j;
	if((fp1=fopen("sample.txt","r"))==NULL)
	{
		printf("can not open the 2 in file\n");
		exit(0);
	}
	for(i=0;i<Data;i++)
	{
		for(j=0; j<In; j++)
		{
			fscanf(fp1,"%lf\t",&d_in[i][j]);
			printf("%lf\t",d_in[i][j]);
		}
		fscanf(fp1,"\n");
		printf("\n");
		//getchar();
	}		
	fclose(fp1);

	if((fp2=fopen("sampleres.txt","r"))==NULL)
	{
		printf("can not open the out file\n");
		exit(0);
	}
	for(i=0;i<Data;i++)
		for(j=0; j<Out; j++)
		{
			fscanf(fp2,"%lf",&d_out[i][j]);
			printf("%lf\t",d_out[i][j]);
		}
		fclose(fp2);
}

////////////////////////初始化神经网络(数据整理)///////////////////////////////
void InitBPNework()
{
	int i,j;
	//输入数据最大值，最小值
	for(i=0; i<In; i++)
	{
		Minin[i]=Maxin[i]=d_in[0][i];
		for(j=0; j<Data; j++)
		{
			Maxin[i]=Maxin[i]>d_in[j][i]?Maxin[i]:d_in[j][i];
			Minin[i]=Minin[i]<d_in[j][i]?Minin[i]:d_in[j][i];
		}
	}
	//输出数据的最大最小值
	for(i=0; i<Out; i++)
	{
		Minout[i]=Maxout[i]=d_out[0][i];
		for(j=0; j<Data; j++)
		{
			Maxout[i]=Maxout[i]>d_out[j][i]?Maxout[i]:d_out[j][i];
			Minout[i]=Minout[i]<d_out[j][i]?Minout[i]:d_out[j][i];
		}
	}
	
	FILE * fp=fopen("MaxMin.txt","w+");
	for(i=0; i<In; i++)
	{
		fprintf(fp,"%lf %lf ",Maxin[i],Minin[i]);
	}
	fprintf(fp,"\n\n\n\n");
	for(i=0; i<Out; i++)
	{
		fprintf(fp,"%lf %lf ",Maxout[i],Minout[i]);
	}
	fclose(fp);

	//输入数据归一化处理
	for (i = 0; i < In; i++)
		for(j = 0; j < Data; j++)
			d_in[j][i]=(d_in[j][i]-Minin[i]+1)/(Maxin[i]-Minin[i]+1);
	//输出数据归一化处理
	for (i = 0; i < Out; i++)
		for(j = 0; j < Data; j++)
			d_out[j][i]=(d_out[j][i]-Minout[i]+1)/(Maxout[i]-Minout[i]+1);

	//隐层输入权值随机      归一到【-0.5,0.5】
	for (i = 0; i < Neuron; ++i)
		for (j = 0; j < In; ++j)
		{
			w[i][j]=(rand()*2.0/RAND_MAX-1)/2;
			//	printf("%lf\t",w[i][j]);
			//	getchar();
			dw[i][j]=0;
		}
		//隐层输出权值随机
		for (i = 0; i < Neuron; ++i)
			for (j = 0; j < Out; ++j)
			{
				v[j][i]=(rand()*2.0/RAND_MAX-1)/2;
				dv[j][i]=0;
			}
}

////////////////正向计算////////////////////////////////////////////////////////////
void Comput(int var) //var为某一个样本数据序号(样本序号)
{
	int i,j;
	double sum,y;
	//隐藏层运算
	for (i = 0; i < Neuron; ++i) //Neuron为神经元个数，此处得在某一个样本数据下，每个神经元的值
	{
		sum=0;
		for (j = 0; j < In; ++j)
			sum += w[i][j]*d_in[var][j]; //神经节点运算
		o[i] = 1 / (1+exp(-1*sum)); //激励函数  得到神经节点输出
	}

	//输出层运算
	for (i = 0; i < Out; ++i)
	{
		sum=0;
		for (j = 0; j < Neuron; ++j)
			sum += v[i][j]*o[j]; //神经网络输出*其权值
		sum =  1 / (1+exp(-1*sum)); //激励函数  得到神经节点输出****////
		OutputData[i] = sum; //输出值
	}
}

//////////////////////////反向传播（更新权值矩阵）////////////////////////////////////
void BackUpdate(int var)
{
	int i,j;
	double t;
	for (i = 0; i < Neuron; ++i)
	{
		t=0;
		for (j = 0; j < Out; ++j)//输出层权值改写
		{
			t += (OutputData[j] - d_out[var][j]) * v[j][i];
			dv[j][i] = A*dv[j][i] + B*(OutputData[j] - d_out[var][j])*o[i];
			v[j][i] -= dv[j][i];
		}

		for (j = 0; j < In; ++j){//隐藏层权值改写
			dw[i][j] = a*dw[i][j] + b*t*o[i]*(1-o[i])*d_in[var][j];
			w[i][j] -= dw[i][j];
		}
	}
}


/////////////////////求解测试结果//////////////////////////////////////////////////////////
double Result(double * var) //如果输入或者输出变量变化，此处需要修改函数
{
	int i,j;
	double sum;

	for (i = 0;i<In;i++)
	{
		var[i] = (var[i]-Minin[i]+1) / (Maxin[i]-Minin[i]+1); //数据归一化
	}
	//var1=(var1-Minin[0]+1)/(Maxin[0]-Minin[0]+1); //数据归一化
	//var2=(var2-Minin[1]+1)/(Maxin[1]-Minin[1]+1); //数据归一化
	for (i = 0; i < Neuron; ++i)
	{
		sum=0;
		for (j = 0;j<In;j++)
		{
			sum += var[j] * w[i][j];
		}
		//sum = w[i][0]*var1 + w[i][1]*var2; //出入变量变化，增加输入
		o[i] = 1/(1+exp(-1*sum));  //激励规则
	}
	sum=0;
	for (j = 0; j < Neuron; ++j)
		sum += v[0][j]*o[j]; //输出只有一个，如果多个，增加输出
	sum = 1/(1+exp(-1*sum));  //激励规则
	return sum*(Maxout[0]-Minout[0]+1)+Minout[0]-1; //数据反归一化，回来到原来的值
}

///////////////////////////////////存储网络参数///////////////////////////////////////////////
void WriteNeuron()
{
	FILE *fp1;
	int i,j;
	if((fp1=fopen("neuron.txt","w+"))==NULL)
	{
		printf("can not open the neuron file\n");
		exit(0);
	}
	for (i = 0; i < Neuron; ++i)
		for (j = 0; j < In; ++j){
			fprintf(fp1,"%lf ",w[i][j]);
		}
		fprintf(fp1,"\n\n\n\n");
		for (i = 0; i < Neuron; ++i)
			for (j = 0; j < Out; ++j){
				fprintf(fp1,"%lf ",v[j][i]);
			}
			fclose(fp1);
}

//////////////////////////////训练BP神经网络////////////////////////////////////////////////
void TrainNetwork()
{
	int i,c=0;
	do{
		e=0;
		for (i = 0; i < Data; ++i)
		{
			Comput(i); //正向传播获取输出值
			e += fabs((OutputData[0] - d_out[i][0])/d_out[i][0]); //输出第一个变量的微分//
			BackUpdate(i); //反向传播误差计算
		}
		printf("训练次数：%d\t精度：%lf\n",c,e/Data);
		c++;
	}while(c<TrainC && e/Data>PRECISE);
}


//////////////////////////////读取应用数据/////////////////////////////////////////////////
void ReadTestData()
{

}
///////////////////////////////测试数据读取并求取结果////////////////////////////////////////
void TestData()          
{

	double x[TEST_SAMPLE][In];int i=0,j=0;
	FILE * fp=fopen("test.txt","a+");
	for (i = 0;i<TEST_SAMPLE;i++)
	{
		for (j = 0;j<In;j++)
		{
			fscanf(fp,"%lf\t",&x[i][j]);
		}
		fscanf(fp,"\n");
	}
	fclose(fp);

	printf("\n识别测试样本输出结果：（1=完全匹配，越接近0越不属于此类）：\n");
	for (i = 0;i<TEST_SAMPLE;i++)
	{
		printf("第%d个样本测试结果：%lf \n",i+1,Result(x[i]) ); //测试数据
	}
}


///////////////////////////////////读取神经网络参数////////////////////////////////////////////////
void ReadNeuron()
{
	int i=0,j=0;
	FILE * fp =fopen("neuron.txt","a+");

	for (i = 0; i < Neuron; ++i)
		for (j = 0; j < In; ++j){
			fscanf(fp,"%lf ",&w[i][j]);
		}
		fscanf(fp,"\n\n\n\n");
		for (i = 0; i < Neuron; ++i)
			for (j = 0; j < Out; ++j){
				fscanf(fp,"%lf ",&v[j][i]);
			}

	fclose(fp);
	fp=fopen("MaxMin.txt","a+");
	for(i=0; i<In; i++)
	{
		fscanf(fp,"%lf %lf ",&Maxin[i],&Minin[i]);
	}
	fscanf(fp,"\n\n\n\n");
	for(i=0; i<Out; i++)
	{
		fscanf(fp,"%lf %lf ",&Maxout[i],&Minout[i]);
	}
	fclose(fp);
}


///////////////////////////////////////运用神经网络///////////////////////////////////////////////////
void UseBPNeuron()
{

	int count=0;
	double *x = new double[In];

	//声明IplImage指针   
	IplImage* pFrame = NULL;  
	//获取摄像头   
	CvCapture* pCapture = cvCreateCameraCapture(0);  
	//CvCapture *pCapture = cvCaptureFromCAM( CV_CAP_ANY );//初始化摄像头   
	//创建窗口   
	cvNamedWindow("video",1);  
	//显示视屏   


	IplImage * img = NULL;
	//ColorM * cm = new ColorM;

	//CvMoments * moments = new CvMoments;
	//CvHuMoments *  humoments = new CvHuMoments;

	while(1)  
	{  
		pFrame=cvQueryFrame( pCapture );  
		if(!pFrame)break;   
		cvShowImage("video",pFrame);

		////////////////////////////////////引用识别////////////////////////////////////////

		img = pFrame;
		cvSmooth(pFrame,img,CV_GAUSSIAN,3,3,0,0);

		FeatureExtr(img,x);
		

		printf("\n识别测试样本输出结果：（1=完全匹配，越接近0越不属于此类）：\n");

		printf("应用结果匹配度：%lf \n",Result(x) ); //应用数据
	
		/////////////////////////////////////////////////////////////////////////////////////////////////
		char c=cvWaitKey(33);  
		if(c==27)break;  
	}  

	delete []x;
	cvReleaseCapture(&pCapture);  
	cvDestroyWindow("video"); 
}


////////////////////////////////////////////////////特征提取集合函数////////////////////////////////////////
void FeatureExtr(IplImage * img, double  feature[])
{
	/////////////////////1   原图 的9个颜色矩值，一二三阶矩///////////////////////

		ColorM * cm = new ColorM;

		ColorMoment(img ,cm);//颜色特征颜色矩9个特征值

		feature[0] = cm->CM1b;
		feature[1] = cm->CM1g;
		feature[2] = cm->CM1r;
		feature[3] = cm->CM2b;
		feature[4] = cm->CM2g;
		feature[5] = cm->CM2r;
		feature[6] = cm->CM3b;
		feature[7] = cm->CM3g;
		feature[8] = cm->CM3r;

		delete []cm;
	//////////////////////////////////////////////////////////////////////////////

		/////////////////////////////////2  灰度图的Hu 不变矩 7个值////////////////////
		CvMoments * moments = new CvMoments;
		CvHuMoments * humoments = new CvHuMoments;

		IplImage* dst = cvCreateImage(cvGetSize(img),img->depth,1);//灰度图dst

		//色彩空间转换，转换类型为CV_BGR2GRAY
		cvCvtColor(img,dst,CV_BGR2GRAY);
		cvMoments(dst,moments,0);
		cvGetHuMoments(moments,humoments);

		feature[9] = humoments->hu1;
		feature[10] = humoments->hu2*MulPow10;
		feature[11] = humoments->hu3*MulPow10;
		feature[12] = humoments->hu4*MulPow10;
		feature[13] = humoments->hu5*MulPow20;
		feature[14] = humoments->hu6*MulPow20;
		feature[15] = humoments->hu7*MulPow20;
		//由于数值太小，需要放大

		delete []moments;
		delete []humoments;
		//余下dst
		/////////////////////////////////////////////////////////////////////////////

		////////////////////////////////////3  二值化图的 雅克比方法 10个矩阵特征值//////////////////////
		// Convert IplImage to cv::Mat
		cv::Mat image = Mat(dst, false); // Do not copy
		//cv::Mat image = cv::imread("1.jpg", CV_LOAD_IMAGE_GRAYSCALE);
		if (image.empty())
		{//std::cout<<"read image failure"<<std::endl;
			return;
		}
		// 局部二值化算法////////////////
		int blockSize = 25;
		int constValue = 10;
		cv::Mat local;
		cv::adaptiveThreshold(image, local, 255, CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY_INV, blockSize, constValue);
		IplImage local_temp = IplImage(local);
		//cvNamedWindow("L");
		//cvShowImage("L",&local_temp);
		//cvWaitKey(0);
		//cvDestroyWindow("L");

		/*	CV_INTER_NN - 最近邻插值,
		CV_INTER_LINEAR - 双线性插值 (缺省使用)
		CV_INTER_AREA - 使用象素关系重采样。当图像缩小时候，该方法可以避免波纹出现。当图像放大时，类似于 CV_INTER_NN 方法..
		CV_INTER_CUBIC - 立方插值.
		实现代码：*/
		IplImage *dst_re = NULL; //目标图像指针 
		CvSize dst_cvsize; //目标图像尺寸
		dst_cvsize.width = Scanle; //目标图像的宽为固定 100 像素 
		dst_cvsize.height = Scanle;//dst->height * ((float)dst_cvsize.width/dst->width); // 高同比例放大 
		dst_re = cvCreateImage( dst_cvsize, dst->depth, dst->nChannels); //构造目标图象 
		cvResize(&local_temp, dst_re, CV_INTER_AREA); //缩放源图像到目标图像

		CvMat * mat = cvCreateMat( dst_cvsize.height, dst_cvsize.width, CV_64FC1 );
		cvConvert( dst_re, mat );//求取特征样本数据准备

		CvMat * dst_vector = cvCloneMat(mat);
		cvSetZero(dst_vector);//创建特征向量存储矩阵并初始化

		CvMat * ProValue = cvCreateMat(dst_cvsize.height,1, CV_64FC1);//特征值矩阵

		cvEigenVV(mat,
			dst_vector,
			ProValue,1.0e-6F
			);//opencv函数 Sanle 10个值 雅克比方法求特征值，一行一个特征值


		int i = 0;
		for(i=0;i<dst_cvsize.height;i++)   {  
			feature[16+i] = cvmGet(ProValue,i,0);
			//printf("%lf\n",feature[16+i]); 
		} // 16 -- 26


		image.release();
		local.release();
		cvReleaseMat(&mat);
		cvReleaseMat(&dst_vector);
		cvReleaseMat(&ProValue);
		//cvReleaseImage(local_temp);
		cvReleaseImage(&dst_re);
		////////////////////////////////////////////////////////////////////////////////////////////////

		///////////////////////////4 边缘提取并 hu不变矩 表示 7个值  26 -- 33//////////////////////////////////////
		IplImage * laplace = 0;
		IplImage * colorlaplace = 0;
		IplImage * planes[3] = {0,0,0};
		CvMoments * moments_t = new CvMoments;
		CvHuMoments * humoments_t = new CvHuMoments;

		IplImage * frame = img;
		if(!laplace)
		{
			//创建图像
			for(int i=0;i<3;i++)
				planes[i] = cvCreateImage(cvSize(frame->width,frame->height),IPL_DEPTH_8U,1);
			laplace = cvCreateImage(cvSize(frame->width,frame->height),IPL_DEPTH_16S,1);
			colorlaplace=cvCreateImage(cvSize(frame->width,frame->height),IPL_DEPTH_8U,3);
		}
		cvCvtPixToPlane(frame,planes[0],planes[1],planes[2],0);
		for(int i=0;i<3;i++)
		{
			//交换，如通道变换
			cvLaplace(planes[i],laplace,3);
			//使用线性变换转换输入函数元素成为无符号整形
			cvConvertScaleAbs(laplace,planes[i],1,0);
		}
		cvCvtPlaneToPix(planes[0],planes[1],planes[2],0,colorlaplace);
		//结构相同（- 顶—左结构,1 - 底—左结构）
		colorlaplace->origin = frame->origin;
		//高斯滤波，平滑图像
		 cvSmooth(colorlaplace, colorlaplace, CV_GAUSSIAN, 1, 0, 0);
		//形态学滤波,闭运算
		cvDilate(colorlaplace, colorlaplace, 0, 1);//膨胀
		cvErode(colorlaplace, colorlaplace, 0, 1);//腐蚀
		//cvNamedWindow("Laplacian");
		
		cvCvtColor(colorlaplace,dst,CV_BGR2GRAY);
		cvMoments(dst,moments_t,0);
		cvGetHuMoments(moments_t,humoments_t);//hu不变矩7个特征值
		//cvShowImage("Laplacian",colorlaplace);
		//cvWaitKey(1000);
		//cvDestroyWindow("Laplacian");
		//cvReleaseImage(&laplace);
		//cvReleaseImage(&planes[0]);
		//cvReleaseImage(&planes[1]);
		//cvReleaseImage(&planes[2]);
		//cvReleaseImage(&colorlaplace);

		feature[26] = humoments_t->hu1;
		feature[27] = humoments_t->hu2*MulPow10;
		feature[28] = humoments_t->hu3*MulPow10;
		feature[29] = humoments_t->hu4*MulPow10;
		feature[30] = humoments_t->hu5*MulPow20;
		feature[31] = humoments_t->hu6*MulPow20;
		feature[32] = humoments_t->hu7*MulPow20;

		cvReleaseImage(&laplace);
		cvReleaseImage(&colorlaplace);
		cvReleaseImage(&planes[0]);
		cvReleaseImage(&planes[1]);
		cvReleaseImage(&planes[2]);

		delete []moments_t;
		delete []humoments_t;
		//////////////////////////////////////////////////////////////////////////////////////////////

		///////////////////////////5  灰度图的 灰度共生矩 33 -- ///////////////////////////////////////
		cvCvtColor(img,dst,CV_BGR2GRAY);
		CGlcm * hgg = new CGlcm(dst);


		feature[33] = hgg->means;
		feature[34] = hgg->variance;
		feature[35] = hgg->std;
		feature[36] = hgg->entropy;
		feature[37] = hgg->eng;

		////////////SVD///////////////////////////// 8个值//////////////45////////////
		feature[38] = hgg->SVDValue[0];
		feature[39] = hgg->SVDValue[1];
		feature[40] = hgg->SVDValue[2];
		feature[41] = hgg->SVDValue[3];
		feature[42] = hgg->SVDValue[4];
		feature[43] = hgg->SVDValue[5];
		feature[44] = hgg->SVDValue[6];
		feature[45] = hgg->SVDValue[7];

		delete []hgg;
		////////////////////////////////////////////////////////////////////////////////////////////////////

		//////////////////////////////////6  灰度共生矩 奇异值分解/////////////////////////////////////////


		///////////////////////////////////////////////////////////////////////////////////////////////////

	//cvReleaseImage(&img);
	//cvReleaseImage(&frame);
	cvReleaseImage(&dst);
}


//////////////////////////////////////////////从视频文件中训练//////////////////////////////////////////////
void TrainTheSampleFromFile()
{
	char* fileName = "D:\\工程项目程序中心\\智能设计\\训练样本\\VID_20130619_074713.3gp";//F:\\视屏区\\swglxh.mp4  
	char* windowTitle = "Vedio";  
	int ESC_KEY = 27;  
	int key = -1,i=0;
	FILE * fp = fopen("sample.txt","a+");
	FILE * fpr = fopen("sampleres.txt","a+");

	cvNamedWindow(windowTitle, CV_WINDOW_AUTOSIZE);  
	CvCapture *capture = cvCreateFileCapture(fileName);  
	IplImage* frame=NULL,*img=NULL;  
	double * feature = new double[In];//特征值存储准备

	while (1) {  
		frame = cvQueryFrame(capture);  
		if (!frame) {  
			break;  
		}  
		img = frame;
		
		cvSmooth(frame,img,CV_GAUSSIAN,3,3,0,0);
		cvShowImage(windowTitle, img);  
		
		////////////////////////////////////////////////////////////////////
		cout<<"（正样本按1，负样本按0，不能作为样本的按2,8中途退出）：\n"<<endl;
		cin>>key;
		if (1 == key)//正样本处理
		{
			FeatureExtr(img,feature);//求取特征值

			for (i=0;i<In;i++)
			{
				fprintf(fp,"%lf\t",feature[i]);
			}
			fprintf(fp,"\n");
			fprintf(fpr,"%d\n",key);

		}else if (0 == key)//负样本处理
		{
			FeatureExtr(img,feature);//求取特征值

			for (i=0;i<In;i++)
			{
				fprintf(fp,"%lf\t",feature[i]);
			}
			fprintf(fp,"\n");
			fprintf(fpr,"%d\n",key);

		}else if (8 == key)
		{
			break;
		}else{//不可作为样本处理

		}
		////////////////////////////////////////////////////////////////////


		int c = cvWaitKey(33);  
		if (c == ESC_KEY) {  
			break;  
		}  
	}  

	delete []feature;
	fclose(fp);
	fclose(fpr);
	cvReleaseImage(&frame);
	cvReleaseImage(&img);
	cvReleaseCapture(&capture);  
	cvDestroyWindow(windowTitle);  

}

////////////////////////////////////////////运用神经网络验证识别视频文件识别物/////////////////////////////////
/////////////////////////////////////////检测到将会有警报声//////////////////////////////////////////////////
void UseBPNeuronFromFile()
{
	char* fileName = "D:\\工程项目程序中心\\智能设计\\训练样本\\VID_20130619_073757.3gp";//未经训练的视频检测 
	//char* fileName = "训练样本\\temp\\VID_20130619_073757.3gp";//经训练的视频正样本检测 
	//char* fileName = "训练样本\\temp\\VID_20130619_075808.3gp";//经训练的视频负样本检测 

	char* windowTitle = "Vedio";  
	int ESC_KEY = 27;  
	double *x = new double[In];
	double check_is;
	cvNamedWindow(windowTitle, CV_WINDOW_AUTOSIZE);  
	CvCapture *capture = cvCreateFileCapture(fileName);  
	IplImage* frame=NULL,*img=NULL;  


	while (1) {  
		frame = cvQueryFrame(capture);  
		if (!frame) {  
			break;  
		}  
		img = frame;

		cvSmooth(frame,img,CV_GAUSSIAN,3,3,0,0);
		cvShowImage(windowTitle, img);  
		//////////////////////////////////检测并警报////////////////////////////////////////

		FeatureExtr(img,x);


		printf("\n识别测试样本输出结果：（1=完全匹配，越接近0越不属于此类）：\n");
		check_is = Result(x);
		if (check_is > 0.9855)
		{
			printf("\a");
		}
		printf("应用结果匹配度：%lf \n",check_is ); //应用数据
		//////////////////////////////////////////////////////////////////////////
		int c = cvWaitKey(33);  
		if (c == ESC_KEY) {  
			break;  
		}  
	}  

	delete []x;
	cvReleaseImage(&frame);
	cvReleaseImage(&img);
	cvReleaseCapture(&capture);  
	cvDestroyWindow(windowTitle);  
}

/////////////////////////////////把视频中的图像切割成分子层9、组织层4、个体层1、原子层16共30个图像////////////////////////////////////
void CutImage()
{
	int key;
	char* fileName = "D:\\工程项目程序中心\\智能设计\\训练样本\\VID_20130628_154729.3gp";//指定视频

	CvCapture* capture=NULL;

	cout<<"\n按1=切割摄像头，0=指定视频\n"<<endl;
	cin>>key;
	if (1 == key)
	{
		//获取摄像头   
		capture = cvCreateCameraCapture(0);  
	}else if (0 == key)
	{
		capture = cvCreateFileCapture(fileName);	  
	}

	char* windowTitle = "Vedio";  
	int ESC_KEY = 27;  
	cvNamedWindow(windowTitle, CV_WINDOW_AUTOSIZE);  
	IplImage * frame=NULL,*img=NULL;

	vector<IplImage*> subImgs;
	int w=0,h=0,i=0;


	cvNamedWindow("20");
	cvNamedWindow("21");
	cvNamedWindow("22");
	cvNamedWindow("23");


	cvNamedWindow("30");
	cvNamedWindow("31");
	cvNamedWindow("32");
	cvNamedWindow("33");
	cvNamedWindow("34");
	cvNamedWindow("35");
	cvNamedWindow("36");
	cvNamedWindow("37");
	cvNamedWindow("38");

	cvNamedWindow("40");
	cvNamedWindow("41");
	cvNamedWindow("42");
	cvNamedWindow("43");
	cvNamedWindow("44");
	cvNamedWindow("45");
	cvNamedWindow("46");
	cvNamedWindow("47");
	cvNamedWindow("48");
	cvNamedWindow("49");
	cvNamedWindow("410");
	cvNamedWindow("411");
	cvNamedWindow("412");
	cvNamedWindow("413");
	cvNamedWindow("414");
	cvNamedWindow("415");


	double *x = new double[In];
	double check_is=0;


	while (1) {  
		frame = cvQueryFrame(capture);  
		if (!frame) {  
			break;  
		}  
		img = frame;

		cvSmooth(frame,img,CV_GAUSSIAN,3,3,0,0);
		cvShowImage(windowTitle, img);  
		///////////////////////////////切割开始///////////////////////////////////////////

		SplitImage(img,3,subImgs);	//分割图像 3 分子层切块

		cvShowImage("30",subImgs[0]);
		cvShowImage("31",subImgs[1]);
		cvShowImage("32",subImgs[2]);
		cvShowImage("33",subImgs[3]);
		cvShowImage("34",subImgs[4]);
		cvShowImage("35",subImgs[5]);
		cvShowImage("36",subImgs[6]);
		cvShowImage("37",subImgs[7]);
		cvShowImage("38",subImgs[8]);

		////////////////////////////////检测 3 原子层//////////////////////////////////
		for (i = 0;i<subImgs.size();i++)
		{
			FeatureExtr(subImgs[i],x);
		
			check_is = Result(x);
			printf("\n3_%d:\t%lf\n",i,check_is);
			if (check_is > 0.90)
			{
				printf("\a");
				getchar();
				goto to1;
			}			
		}

		//////////////////////////////////////////////////////////////////

		subImgs.clear();



		SplitImage(img,2,subImgs);	//分割图像 2 组织层切块

			cvShowImage("20",subImgs[0]);
			cvShowImage("21",subImgs[1]);
			cvShowImage("22",subImgs[2]);
			cvShowImage("23",subImgs[3]);

			////////////////////////////////检测 2 组织层//////////////////////////////////
			for (i = 0;i<subImgs.size();i++)
			{
				FeatureExtr(subImgs[i],x);

				check_is = Result(x);
				printf("\n2_%d:\t%lf\n",i,check_is);
				if (check_is > 0.90)
				{
					printf("\a");
					getchar();
					goto to1;
				}			
			}
			//////////////////////////////////////////////////////////////////

			//cvWaitKey(500);
				//cout<<subImgs.size()<<endl;
				subImgs.clear();
				//cout<<subImgs.size()<<endl;


		//////////////////////////////////检测 1 个体层////////////////////////////////////
			
					FeatureExtr(img,x);

					check_is = Result(x);
					printf("\n1_0：\t%lf\n",check_is);
					if (check_is > 0.90)
					{
						printf("\a");
						getchar();
						goto to1;
					}			
			

		////////////////////////////////////////////////////////////////////////////

			
			SplitImage(img,4,subImgs);	//分割图像 4 原子层切块
			cvShowImage("40",subImgs[0]);
			cvShowImage("41",subImgs[1]);
			cvShowImage("42",subImgs[2]);
			cvShowImage("43",subImgs[3]);
			cvShowImage("44",subImgs[4]);
			cvShowImage("45",subImgs[5]);
			cvShowImage("46",subImgs[6]);
			cvShowImage("47",subImgs[7]);
			cvShowImage("48",subImgs[8]);
			cvShowImage("49",subImgs[9]);
			cvShowImage("410",subImgs[10]);
			cvShowImage("411",subImgs[11]);
			cvShowImage("412",subImgs[12]);
			cvShowImage("413",subImgs[13]);
			cvShowImage("414",subImgs[14]);
			cvShowImage("415",subImgs[15]);

			////////////////////////////////检测 4 原子层//////////////////////////////////
			for (i = 0;i<subImgs.size();i++)
			{
				FeatureExtr(subImgs[i],x);

				check_is = Result(x);
				printf("\n4_%d:\t%lf\n",i,check_is);
				if (check_is > 0.90)
				{
					printf("\a");
					getchar();
					goto to1;
				}			
			}
			//////////////////////////////////////////////////////////////////
to1:			subImgs.clear();
			
				
	
	

		//////////////////////////////////////////////////////////////////////////
		int c = cvWaitKey(25);  
		if (c == ESC_KEY) {  
			break;  
		}  
	}  

	cvReleaseImage(&frame);
	cvReleaseImage(&img);
	cvReleaseCapture(&capture);  
	cvDestroyWindow(windowTitle);  

	cvDestroyWindow("20");
	cvDestroyWindow("21");
	cvDestroyWindow("22");
	cvDestroyWindow("23");

	cvDestroyWindow("30");
	cvDestroyWindow("31");
	cvDestroyWindow("32");
	cvDestroyWindow("33");
	cvDestroyWindow("34");
	cvDestroyWindow("35");
	cvDestroyWindow("36");
	cvDestroyWindow("37");
	cvDestroyWindow("38");


	cvDestroyWindow("40");
	cvDestroyWindow("41");
	cvDestroyWindow("42");
	cvDestroyWindow("43");
	cvDestroyWindow("44");

	cvDestroyWindow("45");
	cvDestroyWindow("46");
	cvDestroyWindow("47");
	cvDestroyWindow("48");
	cvDestroyWindow("49");
	cvDestroyWindow("410");
	cvDestroyWindow("411");
	cvDestroyWindow("412");
	cvDestroyWindow("413");
	cvDestroyWindow("414");
	cvDestroyWindow("415");
	delete []x;
}

////////////////////////////////图像分块/////////////////////////////////////////////////////////
void SplitImage(const IplImage *imgSrc, const int n, vector<IplImage*> &subImgs)
{
	IplImage * img = cvCloneImage(imgSrc);
	int subWidth  = cvFloor(img->width/n);
	int subHeight = cvFloor(img->height/n);
	
	for(int y=0; y<n; y++)//from top to bottom, left to right
	{
		for(int x=0; x<n; x++)
		{
			cvSetImageROI(img, cvRect(x*subWidth, y*subHeight, subWidth, subHeight));
			IplImage * roiImg = cvCreateImage(cvSize(subWidth,subHeight),img->depth,3);
			cvCopy(img, roiImg, 0);
			subImgs.push_back(roiImg);
			cvResetImageROI(img);
		}
	}
	cvReleaseImage(&img);
}

//////////////////////////////////图像分块训练//////////////////////////////////////////////////////
void CutImageSample()
{
	char* fileName = "D:\\工程项目程序中心\\智能设计\\训练样本\\VID_20130619_080033.3gp";//F:\\视屏区\\swglxh.mp4  
	char* windowTitle = "Vedio";  
	int ESC_KEY = 27;  
	int key = -1,i=0;
	FILE * fp = fopen("sample.txt","a+");
	FILE * fpr = fopen("sampleres.txt","a+");

	cvNamedWindow(windowTitle, CV_WINDOW_AUTOSIZE);
	cvNamedWindow("block", CV_WINDOW_AUTOSIZE);

	CvCapture *capture = cvCreateFileCapture(fileName);  
	IplImage* frame=NULL,*img=NULL;  
	double * feature = new double[In];//特征值存储准备
	vector<IplImage*> subImgs;

	while (1) {  
		frame = cvQueryFrame(capture);  
		if (!frame) {  
			break;  
		}  
		img = frame;

		cvSmooth(frame,img,CV_GAUSSIAN,3,3,0,0);
		cvShowImage(windowTitle, img);  

		////////////////////////////////////////////////////////////////////	
		
		cout<<"（1_0样本:正样本按1，负样本按0，不能作为样本的按2,8中途退出,9=直接下一帧）：\n"<<endl;
		cin>>key;
		if (1 == key)//正样本处理
		{
			FeatureExtr(img,feature);//求取特征值

			for (i=0;i<In;i++)
			{
				fprintf(fp,"%lf\t",feature[i]);
			}
			fprintf(fp,"\n");
			fprintf(fpr,"%d\n",key);

		}else if (0 == key)//负样本处理
		{
			FeatureExtr(img,feature);//求取特征值

			for (i=0;i<In;i++)
			{
				fprintf(fp,"%lf\t",feature[i]);
			}
			fprintf(fp,"\n");
			fprintf(fpr,"%d\n",key);

		}else if (8 == key)
		{
			goto to7;;
		}
		else if (9 == key)
		{
			continue;
		}else{//不可作为样本处理

		}
		for (int lay=2;lay<5;lay++)
		{
			SplitImage(img,lay,subImgs);
			for (int block=0;block<lay*lay;block++)
			{
				printf("block=%d\t",block);
				cvShowImage("block",subImgs[block]);
				cvWaitKey(500);
				printf("（%d_%d样本:正样本按1，负样本按0，不能作为样本的按2,8中途退出）：\n",lay,block);
				cin>>key;
				if (1 == key)//正样本处理
				{
					FeatureExtr(subImgs[block],feature);//求取特征值

					for (i=0;i<In;i++)
					{
						fprintf(fp,"%lf\t",feature[i]);
					}
					fprintf(fp,"\n");
					fprintf(fpr,"%d\n",key);

				}else if (0 == key)//负样本处理
				{
					FeatureExtr(subImgs[block],feature);//求取特征值

					for (i=0;i<In;i++)
					{
						fprintf(fp,"%lf\t",feature[i]);
					}
					fprintf(fp,"\n");
					fprintf(fpr,"%d\n",key);

				}else if (8 == key)
				{
					goto to6;
				}else{//不可作为样本处理

				}

			}

	to6:		subImgs.clear();
		}
		



		////////////////////////////////////////////////////////////////////


		int c = cvWaitKey(33);  
		if (c == ESC_KEY) {  
			break;  
		}  
	}  
to7:	subImgs.clear();
	delete []feature;
	fclose(fp);
	fclose(fpr);
	cvReleaseImage(&frame);
	cvReleaseImage(&img);
	cvReleaseCapture(&capture);  
	cvDestroyWindow(windowTitle);  
	cvDestroyWindow("block");  


}


/////////////////////////////////读取指定神经网络///////////////////////////////////////////////////
void ReadNeuron(int key,NeuronNet &net)
{
	int i=0,j=0;
	FILE * fpneron = NULL;
	FILE * fpmixmax = NULL;
	//////////////////////////////////////////////////////////////////////////////
	/*指定选择  1=盲道识别网络；

	*/
	//////////////////////////////////////////////////////////////////////////////
	if (1 == key)
	{
		fpneron = fopen("neuron.txt","a+");
		fpmixmax = fopen("MaxMin.txt","a+");
	}else if (2==key)
	{
		fpneron = fopen("neuron\\road1\\neuron.txt","a+");
		fpmixmax = fopen("neuron\\road1\\MaxMin.txt","a+");
	}else if (4 == key)
	{
		fpneron = fopen("neuron\\road4\\neuron.txt","a+");
		fpmixmax = fopen("neuron\\road4\\MaxMin.txt","a+");
	}
else{
		exit;
	}
	

	for (i = 0; i < Neuron; ++i)
		for (j = 0; j < In; ++j){
			fscanf(fpneron,"%lf ",&net.w[i][j]);
		}
		fscanf(fpmixmax,"\n\n\n\n");
		for (i = 0; i < Neuron; ++i)
			for (j = 0; j < Out; ++j){
				fscanf(fpneron,"%lf ",&net.v[j][i]);
			}

			fclose(fpneron);

			for(i=0; i<In; i++)
			{
				fscanf(fpmixmax,"%lf %lf ",&net.Maxin[i],&net.Minin[i]);
			}
			fscanf(fpmixmax,"\n\n\n\n");
			for(i=0; i<Out; i++)
			{
				fscanf(fpmixmax,"%lf %lf ",&net.Maxout[i],&net.Minout[i]);
			}
			fclose(fpmixmax);
}

void ReadNeuron(int key,NeuronNet &net,const char *neur_filename,const char *MaxMin_filename)
{
	int i=0,j=0;
	FILE * fpneron = NULL;
	FILE * fpmixmax = NULL;
	//////////////////////////////////////////////////////////////////////////////
	/*指定选择  1=盲道识别网络；

	*/
	//////////////////////////////////////////////////////////////////////////////
	if (1 == key)
	{
		fpneron = fopen(neur_filename,"a+");
		fpmixmax = fopen(MaxMin_filename,"a+");
	}else if (2==key)
	{
		fpneron = fopen(neur_filename,"a+");
		fpmixmax = fopen(MaxMin_filename,"a+");
	}else if (4 == key)
	{
		fpneron = fopen(neur_filename,"a+");
		fpmixmax = fopen(MaxMin_filename,"a+");
	}
else{
		exit;
	}
	

	for (i = 0; i < Neuron; ++i)
		for (j = 0; j < In; ++j){
			fscanf(fpneron,"%lf ",&net.w[i][j]);
		}
		fscanf(fpmixmax,"\n\n\n\n");
		for (i = 0; i < Neuron; ++i)
			for (j = 0; j < Out; ++j){
				fscanf(fpneron,"%lf ",&net.v[j][i]);
			}

			fclose(fpneron);

			for(i=0; i<In; i++)
			{
				fscanf(fpmixmax,"%lf %lf ",&net.Maxin[i],&net.Minin[i]);
			}
			fscanf(fpmixmax,"\n\n\n\n");
			for(i=0; i<Out; i++)
			{
				fscanf(fpmixmax,"%lf %lf ",&net.Maxout[i],&net.Minout[i]);
			}
			fclose(fpmixmax);
}
////////////////////////////////////切割图像分层成类///////////////////////////////////////////////////
void CutImageSampleClassify()
{
	char* fileName = "D:\\工程项目程序中心\\智能设计\\训练样本\\VID_20130619_080927.3gp";//样本候选区
	char* windowTitle = "Vedio";  
	int ESC_KEY = 27;  
	int key = -1,i=0,count=0;

	cvNamedWindow(windowTitle, CV_WINDOW_AUTOSIZE);

	CvCapture *capture = cvCreateFileCapture(fileName);  
	IplImage* frame=NULL,*img=NULL;  
	double * feature = new double[In];//特征值存储准备
	vector<IplImage*> subImgs;

	while (1) {  
		frame = cvQueryFrame(capture);  
		if (!frame) {  
			break;  
		}  
		key = -1;
		count++;
		if (count > 10000)
		{
			count = 10;
		}
		img = frame;

		cvSmooth(frame,img,CV_GAUSSIAN,3,3,0,0);
		cvShowImage(windowTitle, img);  
		cvWaitKey(25);
		////////////////////////////////////////////////////////////////////	
		if (count > 40)
		{
			////////////////////////////////11111111111///////////////////////////////////////////////////
			cout<<"1_0\t1=正例，0=副例，2=不确定,9=跳过:"<<endl;
			cin>>key;
			if (1 == key)
			{
				FeatureExtr(img,feature);

				FILE * fp = fopen("neuron\\road1\\sample.txt","a+");
				FILE * fpr = fopen("neuron\\road1\\sampleres.txt","a+");
				for (i=0;i<In;i++)
				{
					fprintf(fp,"%lf\t",feature[i]);
				}
				fprintf(fp,"\n");
				fprintf(fpr,"%d\n",key);
				fclose(fp);
				fclose(fpr);

			}else if (0 == key)
			{
				FeatureExtr(img,feature);

				FILE * fp = fopen("neuron\\road1\\sample.txt","a+");
				FILE * fpr = fopen("neuron\\road1\\sampleres.txt","a+");
				for (i=0;i<In;i++)
				{
					fprintf(fp,"%lf\t",feature[i]);
				}
				fprintf(fp,"\n");
				fprintf(fpr,"%d\n",key);
				fclose(fp);
				fclose(fpr);

			}else if (2 == key)
			{
				FeatureExtr(img,feature);

				FILE * fp = fopen("neuron\\road1\\sampleNotsure.txt","a+");
				FILE * fpr = fopen("neuron\\road1\\sampleresNotsure.txt","a+");
				for (i=0;i<In;i++)
				{
					fprintf(fp,"%lf\t",feature[i]);
				}
				fprintf(fp,"\n");
				fprintf(fpr,"0.5\n");
				fclose(fp);
				fclose(fpr);

			}else if (9 == key)
			{
				//跳到下一步
			}else{

			}
		
			/////////////////////////////////////////////////////////////////////////////////

			cvNamedWindow("block", CV_WINDOW_AUTOSIZE);
			//////////////////////////////////////4444444444/////////////////////////////////
			SplitImage(img,4,subImgs);

			for (int block=0;block<16;block++)
			{
				cvShowImage("block",subImgs[block]);
				cvWaitKey(20);

				cout<<"4_"<<block<<"\t1=正例，0=副例，2=不确定,9=跳到下一层,6=跳下一帧:"<<endl;
				cin>>key;


				if (1 == key)
				{
					FeatureExtr(subImgs[block],feature);

					FILE * fp = fopen("neuron\\road4\\sample.txt","a+");
					FILE * fpr = fopen("neuron\\road4\\sampleres.txt","a+");
					for (i=0;i<In;i++)
					{
						fprintf(fp,"%lf\t",feature[i]);
					}
					fprintf(fp,"\n");
					fprintf(fpr,"%d\n",key);
					fclose(fp);
					fclose(fpr);
				}else if (0 == key)
				{
					FeatureExtr(subImgs[block],feature);

					FILE * fp = fopen("neuron\\road4\\sample.txt","a+");
					FILE * fpr = fopen("neuron\\road4\\sampleres.txt","a+");
					for (i=0;i<In;i++)
					{
						fprintf(fp,"%lf\t",feature[i]);
					}
					fprintf(fp,"\n");
					fprintf(fpr,"%d\n",key);
					fclose(fp);
					fclose(fpr);
				}else if (2 == key)
				{
					FeatureExtr(subImgs[block],feature);

					FILE * fp = fopen("neuron\\road4\\sampleNotsure.txt","a+");
					FILE * fpr = fopen("neuron\\road4\\sampleresNotsure.txt","a+");
					for (i=0;i<In;i++)
					{
						fprintf(fp,"%lf\t",feature[i]);
					}
					fprintf(fp,"\n");
					fprintf(fpr,"0.5\n");
					fclose(fp);
					fclose(fpr);

				}else if (9 == key)
				{
					block = 16;//跳到下一层
				}else if (6 == key)
				{
					//跳到下一帧
				}else{

				}
			}
			subImgs.clear();

			////////////////////////////////////33333////////////////
			SplitImage(img,3,subImgs);

			for (int block=0;block<9;block++)
			{
				cvShowImage("block",subImgs[block]);
				cvWaitKey(20);
			
				cout<<"3_"<<block<<"\t1=正例，0=副例，2=不确定,9=跳过,6=跳到下一层:"<<endl;
				cin>>key;


				if (1 == key)
				{
					FeatureExtr(subImgs[block],feature);

					FILE * fp = fopen("neuron\\road3\\sample.txt","a+");
					FILE * fpr = fopen("neuron\\road3\\sampleres.txt","a+");
					for (i=0;i<In;i++)
					{
						fprintf(fp,"%lf\t",feature[i]);
					}
					fprintf(fp,"\n");
					fprintf(fpr,"%d\n",key);
					fclose(fp);
					fclose(fpr);
				}else if (0 == key)
				{
					FeatureExtr(subImgs[block],feature);

					FILE * fp = fopen("neuron\\road3\\sample.txt","a+");
					FILE * fpr = fopen("neuron\\road3\\sampleres.txt","a+");
					for (i=0;i<In;i++)
					{
						fprintf(fp,"%lf\t",feature[i]);
					}
					fprintf(fp,"\n");
					fprintf(fpr,"%d\n",key);
					fclose(fp);
					fclose(fpr);
				}else if (2 == key)
				{
					FeatureExtr(subImgs[block],feature);

					FILE * fp = fopen("neuron\\road3\\sampleNotsure.txt","a+");
					FILE * fpr = fopen("neuron\\road3\\sampleresNotsure.txt","a+");
					for (i=0;i<In;i++)
					{
						fprintf(fp,"%lf\t",feature[i]);
					}
					fprintf(fp,"\n");
					fprintf(fpr,"0.5\n");
					fclose(fp);
					fclose(fpr);

				}else if (9 == key)
				{
					block = 16;//跳到下一层
				}else if (6 == key)
				{
					//跳到下一帧
				}else{

				}
			}
			subImgs.clear();

			/////////////////////////////222222222222//////////////////////
			SplitImage(img,2,subImgs);

			for (int block=0;block<4;block++)
			{
				cvShowImage("block",subImgs[block]);
				cvWaitKey(20);

				cout<<"2_"<<block<<"\t1=正例，0=副例，2=不确定,9=跳过,6=跳到下一层:"<<endl;
				cin>>key;


				if (1 == key)
				{
					FeatureExtr(subImgs[block],feature);

					FILE * fp = fopen("neuron\\road2\\sample.txt","a+");
					FILE * fpr = fopen("neuron\\road2\\sampleres.txt","a+");
					for (i=0;i<In;i++)
					{
						fprintf(fp,"%lf\t",feature[i]);
					}
					fprintf(fp,"\n");
					fprintf(fpr,"%d\n",key);
					fclose(fp);
					fclose(fpr);
				}else if (0 == key)
				{
					FeatureExtr(subImgs[block],feature);

					FILE * fp = fopen("neuron\\road2\\sample.txt","a+");
					FILE * fpr = fopen("neuron\\road2\\sampleres.txt","a+");
					for (i=0;i<In;i++)
					{
						fprintf(fp,"%lf\t",feature[i]);
					}
					fprintf(fp,"\n");
					fprintf(fpr,"%d\n",key);
					fclose(fp);
					fclose(fpr);
				}else if (2 == key)
				{
					FeatureExtr(subImgs[block],feature);

					FILE * fp = fopen("neuron\\road2\\sampleNotsure.txt","a+");
					FILE * fpr = fopen("neuron\\road2\\sampleresNotsure.txt","a+");
					for (i=0;i<In;i++)
					{
						fprintf(fp,"%lf\t",feature[i]);
					}
					fprintf(fp,"\n");
					fprintf(fpr,"0.5\n");
					fclose(fp);
					fclose(fpr);

				}else if (9 == key)
				{
					block = 16;//跳到下一层
				}else if (6 == key)
				{
					//跳到下一帧
				}else{

				}
			}
			subImgs.clear();


			////////////////////////////////////////////////////////////////////////
			cvDestroyWindow("block"); 
			///////////////////////////////////////////////////////////////////////////////////
		}
		////////////////////////////////////////////////////////////////////


		int c = cvWaitKey(33);  
		if (c == ESC_KEY) {  
			break;  
		}  
	}  
	subImgs.clear();
	delete []feature;
	cvReleaseImage(&frame);
	cvReleaseImage(&img);
	cvReleaseCapture(&capture);  
	cvDestroyWindow(windowTitle);  
	
}

void UseCutImage()
{
	int key;
	char* fileName = "D:\\工程项目程序中心\\智能设计\\训练样本\\VID_20130628_185300.3gp";//指定视频

	CvCapture* capture=NULL;

	cout<<"\n按1=切割摄像头，0=指定视频\n"<<endl;
	cin>>key;
	if (1 == key)
	{
		//获取摄像头   
		capture = cvCreateCameraCapture(0);  
	}else if (0 == key)
	{
		capture = cvCreateFileCapture(fileName);	  
	}

	char* windowTitle = "Vedio";  
	int ESC_KEY = 27;  
	cvNamedWindow(windowTitle, CV_WINDOW_AUTOSIZE);  
	IplImage * frame=NULL,*img=NULL;

	vector<IplImage*> subImgs;
	int i=0;
	
	double *x = new double[In];
	double check_is=0;

	NeuronNet * nnet = new NeuronNet;
	///ReadNeuron(4,*nnet,"sample.txt","MaxMin.txt");
	ReadNeuron(1,*nnet);


	printf("%lf\n",nnet->w[0][0]);
	getchar();


	while (1) {  
		frame = cvQueryFrame(capture);  
		if (!frame) {  
			break;  
		}  
		img = frame;

		cvSmooth(frame,img,CV_GAUSSIAN,3,3,0,0);
		cvShowImage(windowTitle, img);  
		cvWaitKey(5);
		///////////////////////////////切割开始///////////////////////////////////////////

		////////////////////////////////检测 2 组织层//////////////////////////////////

		//////////////////////////////////检测 1 个体层////////////////////////////////////



		SplitImage(img,4,subImgs);	//分割图像 4 原子层切块
		

		////////////////////////////////检测 4 原子层//////////////////////////////////
		for (i = 0;i<subImgs.size();i++)
		{
			cvNamedWindow("444");
			cvShowImage("444",subImgs[i]);
			cvWaitKey(10);
			FeatureExtr(subImgs[i],x);

			check_is = Result(x,*nnet);
			printf("\n4_%d:\t%lf\n",i,check_is);

			if (check_is > 0.9980)
			{
				printf("\a");
				getchar();
			
			}	
			cvDestroyWindow("444");
		}
		//////////////////////////////////////////////////////////////////
			subImgs.clear();





		//////////////////////////////////////////////////////////////////////////
		int c = cvWaitKey(20);  
		if (c == ESC_KEY) {  
			break;  
		}  
	}  

	cvReleaseImage(&frame);
	cvReleaseImage(&img);
	cvReleaseCapture(&capture);  
	cvDestroyWindow(windowTitle);  
	delete []x;
}

//////////////////////网络本地化/////////////////////////////
double Result(double * var,NeuronNet &net)
{
	int i,j;
	double sum;

	for (i = 0;i<In;i++)
	{
		var[i] = (var[i]-net.Minin[i]+1) / (net.Maxin[i]-net.Minin[i]+1); //数据归一化
	}
	//var1=(var1-Minin[0]+1)/(Maxin[0]-Minin[0]+1); //数据归一化
	//var2=(var2-Minin[1]+1)/(Maxin[1]-Minin[1]+1); //数据归一化
	for (i = 0; i < Neuron; ++i)
	{
		sum=0;
		for (j = 0;j<In;j++)
		{
			sum += var[j] * net.w[i][j];
		}
		//sum = w[i][0]*var1 + w[i][1]*var2; //出入变量变化，增加输入
		net.o[i] = 1/(1+exp(-1*sum));  //激励规则
	}
	sum=0;
	for (j = 0; j < Neuron; ++j)
		sum += net.v[0][j]*net.o[j]; //输出只有一个，如果多个，增加输出
	sum = 1/(1+exp(-1*sum));  //激励规则
	return sum*(net.Maxout[0]-net.Minout[0]+1)+net.Minout[0]-1; //数据反归一化，回来到原来的值
}

void Optimization()
{
	int key;
	char* fileName = "sampleLadder\\Ladder_1.3gp";//指定视频

	CvCapture* capture=NULL;

	cout<<"\n按1=切割摄像头，0=指定视频\n"<<endl;
	cin>>key;
	if (1 == key)
	{
		//获取摄像头   
		capture = cvCreateCameraCapture(0);  
	}else if (0 == key)
	{
		capture = cvCreateFileCapture(fileName);	  
	}

	char* windowTitle = "Vedio";  
	int ESC_KEY = 27;  
	cvNamedWindow(windowTitle, CV_WINDOW_AUTOSIZE);  
	IplImage * frame=NULL,*img=NULL;

	vector<IplImage*> subImgs;
	int i=0;

	double *x = new double[In];
	double check_is=0;

	NeuronNet * nnet = new NeuronNet;
	ReadNeuron(4,*nnet,"neuron\\road4\\neuron.txt","neuron\\road4\\MaxMin.txt");


	while (1) {  
		frame = cvQueryFrame(capture);  
		if (!frame) {  
			break;  
		}  
		img = frame;

		cvSmooth(frame,img,CV_GAUSSIAN,3,3,0,0);
		cvShowImage(windowTitle, img);  
		cvWaitKey(5);
		///////////////////////////////切割开始///////////////////////////////////////////

		////////////////////////////////检测 2 组织层//////////////////////////////////

		//////////////////////////////////检测 1 个体层////////////////////////////////////



		SplitImage(img,4,subImgs);	//分割图像 4 原子层切块


		////////////////////////////////检测 4 原子层//////////////////////////////////
		for (i = 0;i<subImgs.size();i++)
		{
			cvNamedWindow("444");
			cvShowImage("444",subImgs[i]);
			cvWaitKey(10);
			FeatureExtr(subImgs[i],x);

			check_is = Result(x,*nnet);
			printf("\n4_%d:\t%lf\n",i,check_is);

			if (check_is > 0.9980)
			{
				printf("\a");
				printf("这个图片是目标需要吗？1=yes，0=no:\n");				

				cin>>key;
				if (0 == key)
				{
					FILE * fp = fopen("neuron\\road4\\sample.txt","a+");
					FILE * fpr = fopen("neuron\\road4\\sampleres.txt","a+");
					for (int c=0;c<5;c++)
					{	
						for (i=0;i<In;i++)
						{
							fprintf(fp,"%lf\t",x[i]);
						}
						fprintf(fp,"\n");
						fprintf(fpr,"%d\n",key);
					}				
					fclose(fp);
					fclose(fpr);
				}else if (1 == key)
				{
				}else{

				}
			}	
			cvDestroyWindow("444");
		}
		//////////////////////////////////////////////////////////////////
		subImgs.clear();





		//////////////////////////////////////////////////////////////////////////
		int c = cvWaitKey(20);  
		if (c == ESC_KEY) {  
			break;  
		}  
	}  

	cvReleaseImage(&frame);
	cvReleaseImage(&img);
	cvReleaseCapture(&capture);  
	cvDestroyWindow(windowTitle);  
	delete []x;
}


void Multi_obj_identify()
{

	int key;
	char* fileName = "D:\\工程项目程序中心\\智能设计\\训练样本\\VID_20130628_185300.3gp";//指定视频

	CvCapture* capture=NULL;

	cout<<"\n按1=切割摄像头，0=指定视频\n"<<endl;
	cin>>key;
	if (1 == key)
	{
		//获取摄像头   
		capture = cvCreateCameraCapture(0);  
	}else if (0 == key)
	{
		capture = cvCreateFileCapture(fileName);	  
	}

	char* windowTitle = "Vedio";  
	int ESC_KEY = 27;  
	cvNamedWindow(windowTitle, CV_WINDOW_AUTOSIZE);  
	IplImage * frame=NULL,*img=NULL;

	vector<IplImage*> subImgs;
	int i=0,j=0;

	double *x = new double[In];
	double *feature = new double[In];
	double check_is=0;

	NeuronNet * net_Ladder = new NeuronNet;//1  阶梯 目标代码0
	ReadNeuron(4,*net_Ladder,"net\\Ladder\\4\\neuron.txt","net\\Ladder\\4\\MaxMin.txt");//数字为选择的划分层次,神经网络，大小区间文件
	/*
	int j;
	for (i = 0; i < Neuron; ++i)
		for (j = 0; j < In; ++j){
			printf("%lf ",net_Ladder->w[i][j]);
		}
		getchar();
		getchar();

		printf("\n\n");
		for (i = 0; i < Neuron; ++i)
			for (j = 0; j < Out; ++j){
				printf("%lf ",net_Ladder->v[j][i]);
			}

			printf("\n\n\n\n");
			
			for(i=0; i<In; i++)
			{
				printf("%lf %lf ",net_Ladder->Maxin[i],net_Ladder->Minin[i]);
			}
			printf("\n\n");
			for(i=0; i<Out; i++)
			{
				printf("%lf %lf ",net_Ladder->Maxout[i],net_Ladder->Minout[i]);
			}
			printf("\n\n\n\n\n");*/

	NeuronNet * net_BRoad = new NeuronNet;//2  盲道 目标代码1
	ReadNeuron(4,*net_BRoad,"net\\BRoad\\4\\neuron.txt","net\\BRoad\\4\\MaxMin.txt");//数字为选择的划分层次

	NeuronNet * net_ZebraCRO = new NeuronNet;//3  斑马线 目标代码2
	ReadNeuron(4,*net_ZebraCRO,"net\\ZebraCRO\\4\\neuron.txt","net\\ZebraCRO\\4\\MaxMin.txt");//数字为选择的划分层次
	/*
	for (i = 0; i < Neuron; ++i)
		for (j = 0; j < In; ++j){
			printf("%lf ",net_ZebraCRO->w[i][j]);
		}
		getchar();
		getchar();

		printf("\n\n");
		for (i = 0; i < Neuron; ++i)
			for (j = 0; j < Out; ++j){
				printf("%lf ",net_ZebraCRO->v[j][i]);
			}

			printf("\n\n\n\n");

			for(i=0; i<In; i++)
			{
				printf("%lf %lf ",net_ZebraCRO->Maxin[i],net_ZebraCRO->Minin[i]);
			}
			printf("\n\n");
			for(i=0; i<Out; i++)
			{
				printf("%lf %lf ",net_ZebraCRO->Maxout[i],net_ZebraCRO->Minout[i]);
			}
			printf("\n\n\n\n\n");*/

	NeuronNet * net_TrfcLights = new NeuronNet;//4  红绿灯 目标代码3
	ReadNeuron(4,*net_TrfcLights,"net\\TrfcLights\\4\\neuron.txt","net\\TrfcLights\\4\\MaxMin.txt");//数字为选择的划分层次
	
	double DetectionContainer[4][3][16];//4种识别目标， 3帧一循环 ， 没帧16个检测值
	int tally = 0;

	int type_count=0,frame_count=0,cut_count=0;
	int comp[4]={0,0,0,0};
	int foot = -1;
	
	while (1) {  
		frame = cvQueryFrame(capture);  
		if (!frame) {  
			break;  
		}  
		img = frame;

		cvSmooth(frame,img,CV_GAUSSIAN,3,3,0,0);
		cvShowImage(windowTitle, img);  
		cvWaitKey(5);
		///////////////////////////////切割开始///////////////////////////////////////////

		////////////////////////////////检测 2 组织层//////////////////////////////////

		//////////////////////////////////检测 1 个体层////////////////////////////////////



		SplitImage(img,4,subImgs);	//分割图像 4 原子层切块
		

		////////////////////////////////检测 4 原子层//////////////////////////////////
		for (i = 0;i<subImgs.size();i++)
		{
			cvNamedWindow("444");
			cvShowImage("444",subImgs[i]);
			cvWaitKey(10);
			FeatureExtr(subImgs[i],feature);
		/////////////////////////////////////////////////
			for (j=0;j<In;j++)
			{
				x[j] = feature[j];
			}

		/////////////////////阶梯/////////////////////////

			check_is = Result(x,*net_Ladder);
			//printf("\n4_%d:\t%lf\n",i,check_is);
			DetectionContainer[0][tally][i] = check_is;
			/*if (check_is > 0.9980)
			{
				printf("\a");
				printf("前方检测到阶梯,请注意行走！");			
			}*/
		/////////////////////////////////////////////////
			for (j=0;j<In;j++)
			{
				x[j] = feature[j];
			}
		/////////////////盲道/////////////////////////////
			check_is = Result(x,*net_BRoad);
			//printf("\n4_%d:BRoad\t%lf\n",i,check_is);
			//getchar();
			DetectionContainer[1][tally][i] = check_is;
		//////////////////////////////////////////////////
			for (j=0;j<In;j++)
			{
				x[j] = feature[j];
			}
		///////////////////斑马线/////////////////////////
			check_is = Result(x,*net_ZebraCRO);
			//printf("check_is_ZebraCRO=%lf\n",check_is);
			//getchar();
			DetectionContainer[2][tally][i] = check_is;
		/////////////////////////////////////////////////
			for (j=0;j<In;j++)
			{
				x[j] = feature[j];
			}
		/////////////////////红绿灯//////////////////////
			check_is = Result(x,*net_TrfcLights);
			//printf("\n4_%d:TrfcLights\t%lf\n",i,check_is);
			//getchar();
			DetectionContainer[3][tally][i] = check_is;
		/////////////////////////////////////////////////
			cvDestroyWindow("444");
		}
		//////////////////////////////////////////////////////////////////
		//cout << "放元素后容器大小为: " << subImgs.size() << "容器容量为: " << subImgs.capacity() << endl; //放元素
		subImgs.clear();
		//cout << "clear后容器大小为: " << subImgs.size() << "容器容量为: " << subImgs.capacity() << endl; //clear
		//vector<double>().swap(it);
		//cout << "swap后容器大小为: " << it.size() << "容器容量为: " << it.capacity() << endl; //swap

		tally++;//帧计数器
		//计数器工作
		if (3 == tally)
		{
			tally = 0;
			for (int j = 0;j<4;j++)
			{
				comp[j] = 0;
			}
			//memset(comp,0,16);
				for (frame_count = 0;frame_count<3;frame_count++)
				{
					for (cut_count = 0;cut_count<16;cut_count++)
					{
						if (DetectionContainer[0][frame_count][cut_count] > 0.9980)//比较阈值
						{
							comp[0]++;
							//printf("comp[0]=%d\n",comp[0]);
						}						
					}
				}
				printf("comp[0]=%d\n",comp[0]);



				for (frame_count = 0;frame_count<3;frame_count++)
				{
					for (cut_count = 0;cut_count<16;cut_count++)
					{
						if (DetectionContainer[1][frame_count][cut_count] > 0.9980)//比较阈值
						{
							comp[1]++;
							

						}
					}
				}
printf("comp[1]=%d\n",comp[1]);


				for (frame_count = 0;frame_count<3;frame_count++)
				{
					for (cut_count = 0;cut_count<16;cut_count++)
					{
						if (DetectionContainer[2][frame_count][cut_count] > 0.9980)//比较阈值
						{
							comp[2]++;
							

						}
					}
				}
printf("comp[2]=%d\n",comp[2]);


				for (frame_count = 0;frame_count<3;frame_count++)
				{
					for (cut_count = 0;cut_count<16;cut_count++)
					{
						if (DetectionContainer[3][frame_count][cut_count] > 0.9980)//比较阈值
						{
							comp[3]++;
							

						}
					}
				}
printf("comp[3]=%d\n",comp[3]);
				foot = compare_type(comp,4);
				if (0 == foot)
				{
					printf("\a");
					printf("\n前方阶梯，请注意行走！\n");
					getchar();
				}else if (1 == foot)
				{
					printf("\a");
					printf("\n前方有盲道_斑马线！\n");
					getchar();
				}else if (2 == foot)
				{
					printf("\a");
					printf("\n前方有斑马线！\n");
					getchar();
				}else if (3 == foot)
				{
					printf("\a");
					printf("\n前方有红绿灯！\n");
					getchar();
				}else{
					foot = -1;
					printf("\n未检测到目标\n");
				}
			

		}
		//////////////////////////////////////////////////////////////////////////
		int c = cvWaitKey(5);  
		if (c == ESC_KEY) {  
			break;  
		}  
	}  

	vector<IplImage *>().swap(subImgs);
	cvReleaseImage(&frame);
	cvReleaseImage(&img);
	cvReleaseCapture(&capture);  
	cvDestroyWindow(windowTitle);  
	delete []x;
	delete []net_Ladder;
	delete []net_BRoad;
	delete []net_ZebraCRO;
	delete []net_TrfcLights;
}


int compare_type(int *comp,int n)
{
	int foot = -1;
	int temp = 5;//必须要大于1次的才行
	for (int i = 0;i<n;i++)
	{
		if (comp[i] > temp)
		{
			foot = i;
			temp = comp[i];
		}
	}
	//printf("%d\n",temp);
	return foot;
}


void ONE_obj_identify()
{
	int key;
	char* fileName = "sampleLadder\\BRoad_1.3gp";//指定视频

	CvCapture* capture=NULL;

	cout<<"\n按1=切割摄像头，0=指定视频\n"<<endl;
	//cin>>key;
	key=0;
	if (1 == key)
	{
		//获取摄像头   
		capture = cvCreateCameraCapture(0);  
	}else if (0 == key)
	{
		capture = cvCreateFileCapture(fileName);	  
	}

	char* windowTitle = "Vedio";  
	int ESC_KEY = 27;  
	cvNamedWindow(windowTitle, CV_WINDOW_AUTOSIZE);  
	IplImage * frame=NULL,*img=NULL;

	vector<IplImage*> subImgs;
	int i=0,j=0;

	double *x = new double[In];
	double *feature = new double[In];
	double check_is=0;
	char way_neuron[200]={0};
	char way_MaMi[200]={0};
	strcpy(way_neuron,this_way("net\\BRoad\\4\\neuron.txt"));
	strcpy(way_MaMi,this_way("net\\BRoad\\4\\MaxMin.txt"));
	NeuronNet * net_Ladder = new NeuronNet;//1  阶梯 目标代码0
	ReadNeuron(4,*net_Ladder,way_neuron,way_MaMi);//数字为选择的划分层次,神经网络，大小区间文件
	/*
	int j;
	for (i = 0; i < Neuron; ++i)
		for (j = 0; j < In; ++j){
			printf("%lf ",net_Ladder->w[i][j]);
		}
		getchar();
		getchar();

		printf("\n\n");
		for (i = 0; i < Neuron; ++i)
			for (j = 0; j < Out; ++j){
				printf("%lf ",net_Ladder->v[j][i]);
			}

			printf("\n\n\n\n");
			
			for(i=0; i<In; i++)
			{
				printf("%lf %lf ",net_Ladder->Maxin[i],net_Ladder->Minin[i]);
			}
			printf("\n\n");
			for(i=0; i<Out; i++)
			{
				printf("%lf %lf ",net_Ladder->Maxout[i],net_Ladder->Minout[i]);
			}
			printf("\n\n\n\n\n");

	NeuronNet * net_BRoad = new NeuronNet;//2  盲道 目标代码1
	ReadNeuron(4,*net_BRoad,"net\\BRoad\\4\\neuron.txt","net\\BRoad\\4\\MaxMin.txt");//数字为选择的划分层次

	NeuronNet * net_ZebraCRO = new NeuronNet;//3  斑马线 目标代码2
	ReadNeuron(4,*net_ZebraCRO,"net\\ZebraCRO\\4\\neuron.txt","net\\ZebraCRO\\4\\MaxMin.txt");//数字为选择的划分层次
	
	for (i = 0; i < Neuron; ++i)
		for (j = 0; j < In; ++j){
			printf("%lf ",net_ZebraCRO->w[i][j]);
		}
		getchar();
		getchar();

		printf("\n\n");
		for (i = 0; i < Neuron; ++i)
			for (j = 0; j < Out; ++j){
				printf("%lf ",net_ZebraCRO->v[j][i]);
			}

			printf("\n\n\n\n");

			for(i=0; i<In; i++)
			{
				printf("%lf %lf ",net_ZebraCRO->Maxin[i],net_ZebraCRO->Minin[i]);
			}
			printf("\n\n");
			for(i=0; i<Out; i++)
			{
				printf("%lf %lf ",net_ZebraCRO->Maxout[i],net_ZebraCRO->Minout[i]);
			}
			printf("\n\n\n\n\n");

	NeuronNet * net_TrfcLights = new NeuronNet;//4  红绿灯 目标代码3
	ReadNeuron(4,*net_TrfcLights,"net\\TrfcLights\\4\\neuron.txt","net\\TrfcLights\\4\\MaxMin.txt");//数字为选择的划分层次
	*/
	double DetectionContainer[4][3][16];//4种识别目标， 3帧一循环 ， 没帧16个检测值
	int tally = 0;

	int type_count=0,frame_count=0,cut_count=0;
	int comp[4]={0,0,0,0};
	int foot = -1;
	
	while (1) {  
		frame = cvQueryFrame(capture);  
		if (!frame) {  
			break;  
		}  
		img = frame;

		cvSmooth(frame,img,CV_GAUSSIAN,3,3,0,0);
		cvShowImage(windowTitle, img);  
		//cvWaitKey(5);
		///////////////////////////////切割开始///////////////////////////////////////////

		////////////////////////////////检测 2 组织层//////////////////////////////////

		//////////////////////////////////检测 1 个体层////////////////////////////////////



		SplitImage(img,4,subImgs);	//分割图像 4 原子层切块
		

		////////////////////////////////检测 4 原子层//////////////////////////////////
		for (i = 0;i<subImgs.size();i++)
		{
			//cvNamedWindow("444");
			//cvShowImage("444",subImgs[i]);
			//cvWaitKey(10);
			FeatureExtr(subImgs[i],feature);
		/////////////////////////////////////////////////
			for (j=0;j<In;j++)
			{
				x[j] = feature[j];
			}

		/////////////////////阶梯/////////////////////////

			check_is = Result(x,*net_Ladder);
			//printf("\n4_%d:\t%lf\n",i,check_is);
			DetectionContainer[0][tally][i] = check_is;
			/*if (check_is > 0.9980)
			{
				printf("\a");
				printf("前方检测到阶梯,请注意行走！");			
			}*/
		/////////////////////////////////////////////////
		/*	for (j=0;j<In;j++)
			{
				x[j] = feature[j];
			}
		/////////////////盲道/////////////////////////////
			check_is = Result(x,*net_BRoad);
			//printf("\n4_%d:BRoad\t%lf\n",i,check_is);
			//getchar();
			DetectionContainer[1][tally][i] = check_is;
		//////////////////////////////////////////////////
			for (j=0;j<In;j++)
			{
				x[j] = feature[j];
			}
		///////////////////斑马线/////////////////////////
			check_is = Result(x,*net_ZebraCRO);
			//printf("check_is_ZebraCRO=%lf\n",check_is);
			//getchar();
			DetectionContainer[2][tally][i] = check_is;
		/////////////////////////////////////////////////
			for (j=0;j<In;j++)
			{
				x[j] = feature[j];
			}
		/////////////////////红绿灯//////////////////////
			check_is = Result(x,*net_TrfcLights);
			//printf("\n4_%d:TrfcLights\t%lf\n",i,check_is);
			//getchar();
			DetectionContainer[3][tally][i] = check_is;*/
		/////////////////////////////////////////////////
			//cvDestroyWindow("444");
		}
		//////////////////////////////////////////////////////////////////
		//cout << "放元素后容器大小为: " << subImgs.size() << "容器容量为: " << subImgs.capacity() << endl; //放元素
		subImgs.clear();
		//cout << "clear后容器大小为: " << subImgs.size() << "容器容量为: " << subImgs.capacity() << endl; //clear
		//vector<double>().swap(it);
		//cout << "swap后容器大小为: " << it.size() << "容器容量为: " << it.capacity() << endl; //swap

		tally++;//帧计数器
		//计数器工作
		if (3 == tally)
		{
			tally = 0;
			for (int j = 0;j<4;j++)
			{
				comp[j] = 0;
			}
			//memset(comp,0,16);
				for (frame_count = 0;frame_count<3;frame_count++)
				{
					for (cut_count = 0;cut_count<16;cut_count++)
					{
						if (DetectionContainer[0][frame_count][cut_count] > 0.9980)//比较阈值
						{
							comp[0]++;
							//printf("comp[0]=%d\n",comp[0]);
						}						
					}
				}
				//printf("comp[0]=%d\n",comp[0]);

/*

				for (frame_count = 0;frame_count<3;frame_count++)
				{
					for (cut_count = 0;cut_count<16;cut_count++)
					{
						if (DetectionContainer[1][frame_count][cut_count] > 0.9980)//比较阈值
						{
							comp[1]++;
							

						}
					}
				}
printf("comp[1]=%d\n",comp[1]);


				for (frame_count = 0;frame_count<3;frame_count++)
				{
					for (cut_count = 0;cut_count<16;cut_count++)
					{
						if (DetectionContainer[2][frame_count][cut_count] > 0.9980)//比较阈值
						{
							comp[2]++;
							

						}
					}
				}
printf("comp[2]=%d\n",comp[2]);


				for (frame_count = 0;frame_count<3;frame_count++)
				{
					for (cut_count = 0;cut_count<16;cut_count++)
					{
						if (DetectionContainer[3][frame_count][cut_count] > 0.9980)//比较阈值
						{
							comp[3]++;
							

						}
					}
				}
printf("comp[3]=%d\n",comp[3]);
				foot = compare_type(comp,4);
				if (0 == foot)
				{
					printf("\a");
					printf("\n前方阶梯，请注意行走！\n");
					getchar();
				}else if (1 == foot)
				{
					printf("\a");
					printf("\n前方有盲道_斑马线！\n");
					getchar();
				}else if (2 == foot)
				{
					printf("\a");
					printf("\n前方有斑马线！\n");
					getchar();
				}else if (3 == foot)
				{
					printf("\a");
					printf("\n前方有红绿灯！\n");
					getchar();
				}else{
					foot = -1;
					printf("\n未检测到目标\n");
				}*/

				if (comp[0] > 7)
				{
					//printf("\a");
					Beep(1000,500);
					printf("\n前方有盲道！\n");
					//getchar();
				}
			

		}
		//////////////////////////////////////////////////////////////////////////
		int c = cvWaitKey(5);  
		if (c == ESC_KEY) {  
			break;  
		}  
	}  

	vector<IplImage *>().swap(subImgs);
	cvReleaseImage(&frame);
	cvReleaseImage(&img);
	cvReleaseCapture(&capture);  
	cvDestroyWindow(windowTitle);  
	delete []x;
	delete []net_Ladder;
	////delete []net_BRoad;
	//delete []net_ZebraCRO;
	//delete []net_TrfcLights;
}




char * this_way(char *str)
{
	char szFilePath[MAX_PATH] = {0};
	DWORD dwError = GetModuleFileNameA(NULL, szFilePath, ARRAYSIZE(szFilePath));
	if(dwError > 0)
	{
		PathRemoveFileSpecA(szFilePath);
		sprintf(szFilePath,"%s\\%s",szFilePath,str);
		//MessageBoxA(0,szFilePath,0,0);
		return szFilePath;
		//printf(_T("The App path is: %s\n"), szFilePath);
	}
	else
	{

		MessageBoxA(0,"GetModuleFileName error",0,0);
		return 0;
	}
}
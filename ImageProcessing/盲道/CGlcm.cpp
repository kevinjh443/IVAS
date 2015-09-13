
#include "stdafx.h"
#include "CGlcm.h"
#include "cv.h"
#include "highgui.h"
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

CGlcm::CGlcm(IplImage * img)
{
	if (img->nChannels == 1)
	{
		CGlCM(img);
	}
}
CGlcm::~CGlcm()
{
	delete []PMatrix;
	delete []PMatrixH;
	delete []PMatrixLD;
	delete []PMatrixRD;
	delete []PMatrixV;
}
unsigned char ** CGlcm::cmatrix(int w,int h)
{
	unsigned char **m = new unsigned char*[h];

	for(int i = 0; i < h; i ++)
		m[i] = new unsigned char[w];

	return m;
}

/////////////////////////////////////////////////////////////////////////////////////
//功能：计算共现矩阵
//参数：LocalImage－用来计算的局部纹理区域图像
//      LocalImageWidth－局部纹理区域宽度
////////////////////////////////////////////////////////////////////////////////////
void CGlcm::ComputeMatrix(unsigned char **LocalImage, int LocalImageWidth, int LocalImageHeight)
{


	unsigned char **NewImage;
	NewImage = new unsigned char*[LocalImageHeight];

	int i,j;
	for(i=0; i<LocalImageHeight; i++)
		NewImage[i] = new unsigned char[LocalImageWidth];

	for(i=0; i<LocalImageHeight; i++)
		for(j=0; j<LocalImageWidth; j++)
			//分成GrayLayerNum个灰度级
			NewImage[i][j] = LocalImage[i][j] / (256/GrayLayerNum);

	for(i=0; i<GrayLayerNum; i++)
		for(j=0; j<GrayLayerNum; j++)
		{
			PMatrixH[i][j]  = 0;
			PMatrixLD[i][j] = 0;
			PMatrixRD[i][j] = 0;
			PMatrixV[i][j]  = 0;
		}

		//计算0度的灰度共现阵
		for(i=0; i<LocalImageHeight; i++)
		{
			for(j=0; j<LocalImageWidth-dis; j++)
			{
				PMatrixH[(unsigned int)NewImage[i][j]][(unsigned int)NewImage[i][j+dis]] += 1;
				PMatrixH[(unsigned int)NewImage[i][j+dis]][(unsigned int)NewImage[i][j]] += 1;
			}
		}

		//计算90度的灰度共现阵
		for(i=0; i<LocalImageHeight-dis; i++)
		{
			for(j=0; j<LocalImageWidth; j++)
			{
				PMatrixV[(unsigned int)NewImage[i][j]][(unsigned int)NewImage[i+dis][j]] += 1;
				PMatrixV[(unsigned int)NewImage[i+dis][j]][(unsigned int)NewImage[i][j]] += 1;
			}
		}

		//计算135度的灰度共现阵
		for(i=0; i<LocalImageHeight-dis; i++)
		{
			for(j=0; j<LocalImageWidth-dis; j++)
			{
				int newi, newj;
				newi = i+dis;
				newj = j+dis;
				PMatrixLD[(unsigned int)NewImage[i][j]][(unsigned int)NewImage[newi][newj]] += 1;
				PMatrixLD[(unsigned int)NewImage[newi][newj]][(unsigned int)NewImage[i][j]] += 1;
			}
		}

		//计算45度的灰度共现阵
		for(i=dis; i<LocalImageHeight; i++)
		{
			for(j=0; j<LocalImageWidth-dis; j++)
			{
				int newi, newj;
				newi = i-dis;
				newj = j+dis;
				PMatrixRD[(unsigned int)NewImage[i][j]][(unsigned int)NewImage[newi][newj]] += 1;
				PMatrixRD[(unsigned int)NewImage[newi][newj]][(unsigned int)NewImage[i][j]] += 1;
			}
		}
}

void CGlcm::CGlCM(IplImage * cimg){

	IplImage * pImageGray = NULL; //灰度图像
	unsigned char** arLocalImage;// = new int [cimg->width][cimg->height];
	arLocalImage = cmatrix(cimg->width, cimg->height);
	//建立灰度图像
	pImageGray = cvCreateImage(cvSize(cimg->width,cimg->height),IPL_DEPTH_8U,1);

	for(int i=0;i<cimg->height;i++) 
		for(int j=0;j<cimg->width;j++) {
			int b = ((uchar *)(cimg->imageData + i*cimg->widthStep))[j*cimg->nChannels + 0];// *= 0.114; // B
			int g = ((uchar *)(cimg->imageData + i*cimg->widthStep))[j*cimg->nChannels + 1];// *= 0.587; // G
			int r = ((uchar *)(cimg->imageData + i*cimg->widthStep))[j*cimg->nChannels + 2];// *= 0.299; // R
			((uchar *)(pImageGray->imageData + i*pImageGray->widthStep))[j*pImageGray->nChannels + 0] = (b+g+r)/3;// *= 0.114; // B
			((uchar *)(pImageGray->imageData + i*pImageGray->widthStep))[j*pImageGray->nChannels + 1] = (b+g+r)/3;// *= 0.587; // G
			((uchar *)(pImageGray->imageData + i*pImageGray->widthStep))[j*pImageGray->nChannels + 2] = (b+g+r)/3;// *= 0.299; // R
			arLocalImage[i][j] = (b+g+r)/3;
		}

		//这样共现矩阵均为GrayLayerNum×GrayLayerNum
		PMatrixH = new int*[GrayLayerNum];
		PMatrixLD= new int*[GrayLayerNum];
		PMatrixRD= new int*[GrayLayerNum];
		PMatrixV = new int*[GrayLayerNum];
		PMatrix = new int*[GrayLayerNum];

		for(int i=0; i<GrayLayerNum; i++)
		{
			PMatrixH[i] = new int[GrayLayerNum];
			PMatrixLD[i]= new int[GrayLayerNum];
			PMatrixRD[i]= new int[GrayLayerNum];
			PMatrixV[i] = new int[GrayLayerNum];
			PMatrix[i] = new int [GrayLayerNum];
		}

		ComputeMatrix(arLocalImage, cimg->width, cimg->height);

		for(int i = 0 ; i < GrayLayerNum; i++){
			for(int j = 0; j < GrayLayerNum; j++){
				PMatrix[i][j] = (PMatrixH[i][j] + PMatrixLD[i][j] + PMatrixRD[i][j] + PMatrixV[i][j])/4;
				//cout << PMatrix[i][j] << "\t";
			}
			//cout << endl;
		}
		////////////////////////////特征值提取////////////////////////
		total = 0;
		for(int i = 0 ; i < GrayLayerNum; i++)
			for(int j = 0; j < GrayLayerNum; j++)
				total += PMatrix[i][j];//总和

		means=0;
		means = total*1.0/(GrayLayerNum*GrayLayerNum);//均值

		eng = 0;
		entropy = 0;
		variance=0;
		for(int i = 0 ; i < GrayLayerNum; i++)
			for(int j = 0; j < GrayLayerNum; j++)
			{
				variance += (means - PMatrix[i][j])*(means - PMatrix[i][j]);//方差
				eng += (PMatrix[i][j]*1.0/total)*(PMatrix[i][j]*1.0/total);//能量=角二阶矩
				if (0 == PMatrix[i][j])
				{
					entropy += log((PMatrix[i][j]+1)*1.0);//熵
				}else{
					entropy += log(PMatrix[i][j]*1.0);//熵
				}
				//cout<<entropy<<endl;
			}
				

		std = 0;
		std = sqrt(variance);//标准差			
			eng = sqrt(eng);
			entropy = -entropy;


		


			/////////////////////////////SVD分解///////////////////////////
			double Matrix_temp[GrayLayerNum][GrayLayerNum];
			for(int i = 0 ; i < GrayLayerNum; i++)
				for(int j = 0; j < GrayLayerNum; j++)
				{
					Matrix_temp[i][j]=(double)PMatrix[i][j];
				}

			CvMat *Matrix1=cvCreateMat(GrayLayerNum,GrayLayerNum,CV_64FC1);
			CvMat *W=cvCreateMat(GrayLayerNum,GrayLayerNum,CV_64FC1);
			CvMat *U=cvCreateMat(GrayLayerNum,GrayLayerNum,CV_64FC1);
			CvMat *V_T=cvCreateMat(GrayLayerNum,GrayLayerNum,CV_64FC1);
			//CvMat *ResultMatrix=cvCreateMat(GrayLayerNum,GrayLayerNum,CV_64FC1);

			cvSetData(Matrix1,Matrix_temp,Matrix1->step); 
			cvSVD(Matrix1,W,U,V_T,CV_SVD_V_T);

			for(int i = 0 ; i < GrayLayerNum; i++)
				{
					SVDValue[i]=cvGet2D(W,i,i).val[0];
				}
			cvReleaseMat(&W);
			cvReleaseMat(&Matrix1);
			cvReleaseMat(&U);
			cvReleaseMat(&V_T);
			/*
			printf("\nW\n");
			PrintMatrix(W,W->rows,W->cols);

			printf("\nU\n");
			PrintMatrix(U,U->rows,U->cols);
			printf("\nV_T\n");
			PrintMatrix(V_T,V_T->rows,V_T->cols);

			printf("\nValid\n");
			cvmMul(U,W,ResultMatrix);
			cvmMul(ResultMatrix,V_T,ResultMatrix);
			PrintMatrix(ResultMatrix,ResultMatrix->rows,ResultMatrix->cols);
			*/

/*

			cvNamedWindow("image", CV_WINDOW_AUTOSIZE); //创建窗口
			cvShowImage("image", cimg); //显示图像
			cvNamedWindow("Gray",CV_WINDOW_AUTOSIZE);
			cvShowImage("Gray",pImageGray);
			cvWaitKey(0);
			cvReleaseImage(&cimg);
			cvReleaseImage(&pImageGray);
			cvDestroyAllWindows();*/
}


void CGlcm::PrintMatrix(CvMat *Matrix,int Rows,int Cols)
{
	for(int i=0;i<Rows;i++)
	{
		for(int j=0;j<Cols;j++)
		{
			cout<<cvGet2D(Matrix,i,j).val[0]<<"\t";
			//printf("%.2f ",cvGet2D(Matrix,i,j).val[0]); 
		} 
		printf("\n");
	} 
}
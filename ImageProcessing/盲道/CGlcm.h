


#pragma once

#include <math.h>
#include <cv.h>
#include "highgui.h"
#include "iostream"
#include "opencv.hpp"
using namespace std;

#define GrayLayerNum 8
#define dis 5

class CGlcm  
{  
public:  
	CGlcm(IplImage * img);  
	virtual ~CGlcm(void);  
	//int GrayLayerNum = 8; //初始化设为8个灰度层，可以修改
	//int dis = 5;
	//这样共现矩阵均为GrayLayerNum×GrayLayerNum
	int** PMatrixH;
	int** PMatrixLD;
	int** PMatrixRD;
	int** PMatrixV;
	int** PMatrix;//前四个方向灰度共生矩阵的平均值
	double SVDValue[GrayLayerNum];

	double means,//均值
		variance,//方差
		std,//标准差
		entropy,//熵
		eng,//能量
		total;//总和

public:  
	
protected:  
	unsigned char **cmatrix(int w,int h);
	void ComputeMatrix(unsigned char **LocalImage, int LocalImageWidth, int LocalImageHeight);
	void PrintMatrix(CvMat *Matrix,int Rows,int Cols);
public: 
	void CGlCM(IplImage * cimg);	

};  

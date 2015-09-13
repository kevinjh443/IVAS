


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
	//int GrayLayerNum = 8; //��ʼ����Ϊ8���ҶȲ㣬�����޸�
	//int dis = 5;
	//�������־����ΪGrayLayerNum��GrayLayerNum
	int** PMatrixH;
	int** PMatrixLD;
	int** PMatrixRD;
	int** PMatrixV;
	int** PMatrix;//ǰ�ĸ�����Ҷȹ��������ƽ��ֵ
	double SVDValue[GrayLayerNum];

	double means,//��ֵ
		variance,//����
		std,//��׼��
		entropy,//��
		eng,//����
		total;//�ܺ�

public:  
	
protected:  
	unsigned char **cmatrix(int w,int h);
	void ComputeMatrix(unsigned char **LocalImage, int LocalImageWidth, int LocalImageHeight);
	void PrintMatrix(CvMat *Matrix,int Rows,int Cols);
public: 
	void CGlCM(IplImage * cimg);	

};  

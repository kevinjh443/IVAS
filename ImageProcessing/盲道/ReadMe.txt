========================================================================
    控制台应用程序：OPCVTestFor 项目概述
========================================================================

应用程序向导已为您创建了此 OPCVTestFor 应用程序。

本文件概要介绍组成 OPCVTestFor 应用程序的每个文件的内容。


OPCVTestFor.vcxproj
    这是使用应用程序向导生成的 VC++ 项目的主项目文件，
    其中包含生成该文件的 Visual C++ 
    的版本信息，以及有关使用应用程序向导选择的平台、配置和项目功能的信息。

OPCVTestFor.vcxproj.filters
    这是使用“应用程序向导”生成的 VC++ 项目筛选器文件。 
    它包含有关项目文件与筛选器之间的关联信息。 在 IDE 
    中，通过这种关联，在特定节点下以分组形式显示具有相似扩展名的文件。
    例如，“.cpp”文件与“源文件”筛选器关联。

OPCVTestFor.cpp
    这是主应用程序源文件。

/////////////////////////////////////////////////////////////////////////////
其他标准文件：

StdAfx.h，StdAfx.cpp
    这些文件用于生成名为 OPCVTestFor.pch 的预编译头 (PCH) 文件和
    名为 StdAfx.obj 的预编译类型文件。

/////////////////////////////////////////////////////////////////////////////
其他注释：

应用程序向导使用“TODO:”注释来指示应添加或自定义的源代码部分。

/////////////////////////////////////////////////////////////////////////////





		/////////////////数据准备///////////////////////
	CvMoments * moments = new CvMoments;
	CvHuMoments * humoments = new CvHuMoments;

	

	IplImage* dst = cvCreateImage(cvGetSize(img),img->depth,1);

	////////////////////////

	IplImage * laplace = 0;
	IplImage * colorlaplace = 0;
	IplImage * planes[3] = {0,0,0};
	////////////////////////////////////////////////////////////////////////////////////////////////////////

	ColorMoment(img ,cm);
	//颜色特征颜色矩9个特征值
	////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//色彩空间转换，转换类型为CV_BGR2GRAY
	cvCvtColor(img,dst,CV_BGR2GRAY);
	cvMoments(dst,moments,0);
	cvGetHuMoments(moments,humoments);
	//hu不变矩7个特征值

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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
		cvResize(dst, dst_re, CV_INTER_LINEAR); //缩放源图像到目标图像


		CvMat * mat = cvCreateMat( dst_cvsize.height, dst_cvsize.width, CV_64FC1 );
		cvConvert( dst_re, mat );//求取特征样本数据准备

		CvMat * dst_vector = cvCloneMat(mat);
		cvSetZero(dst_vector);//创建特征向量存储矩阵并初始化


		CvMat * ProValue = cvCreateMat(dst_cvsize.height,1, CV_64FC1);//特征值矩阵

		cvEigenVV(mat,
			dst_vector,
			ProValue,1.0e-6F
			);
		//雅克比方法求特征值，一行一个特征值

	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	
		/////////////////////////////////////////////////////////////////////////////////////////////////////
	feature[0] = cm->CM1b;
	feature[1] = cm->CM1g;
	feature[2] = cm->CM1r;
	feature[3] = cm->CM2b;
	feature[4] = cm->CM2g;
	feature[5] = cm->CM2r;
	feature[6] = cm->CM3b;
	feature[7] = cm->CM3g;
	feature[8] = cm->CM3r;
	feature[9] = humoments->hu1;
	feature[10] = humoments->hu2*MulPow10;
	feature[11] = humoments->hu3*MulPow10;
	feature[12] = humoments->hu4*MulPow10;
	feature[13] = humoments->hu5*MulPow20;
	feature[14] = humoments->hu6*MulPow20;
	feature[15] = humoments->hu7*MulPow20;

	int i = 0;
	for(i=0;i<dst_cvsize.height;i++)   {  
		feature[16+i] = cvmGet(ProValue,i,0);
		//printf("%lf\n",feature[16+i]); 
	}

	//////////////////////////////////////////////////////4 特征//////////////////////////////////////////////////
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
	// cvSmooth(colorlaplace, colorlaplace, CV_GAUSSIAN, 1, 0, 0);
	//形态学滤波,闭运算
	cvDilate(colorlaplace, colorlaplace, 0, 1);//膨胀
	cvErode(colorlaplace, colorlaplace, 0, 1);//腐蚀

	//cvNamedWindow("Laplacian");


	cvCvtColor(colorlaplace,dst,CV_BGR2GRAY);
	cvMoments(dst,moments,0);
	cvGetHuMoments(moments,humoments);
	//hu不变矩7个特征值
	//cvShowImage("Laplacian",colorlaplace);
	cvWaitKey(1000);
	//cvDestroyWindow("Laplacian");
	cvReleaseImage(&laplace);
	cvReleaseImage(&planes[0]);
	cvReleaseImage(&planes[1]);
	cvReleaseImage(&planes[2]);

	//cvReleaseImage(&colorlaplace);


	feature[26] = humoments->hu1;
	feature[27] = humoments->hu2*MulPow10;
	feature[28] = humoments->hu3*MulPow10;
	feature[29] = humoments->hu4*MulPow10;
	feature[30] = humoments->hu5*MulPow20;
	feature[31] = humoments->hu6*MulPow20;
	feature[32] = humoments->hu7*MulPow20;

	///////////////////////////////////5 灰度共生矩正 特征  均值 方差 标准差 能量 熵//////////////////////
	cvCvtColor(colorlaplace,dst,CV_BGR2GRAY);
	CGlcm * hgg = new CGlcm(dst);


	feature[33] = hgg->means;
	feature[34] = hgg->variance;
	feature[35] = hgg->std;
	//feature[36] = hgg->entropy*MulPow20;
	feature[36] = hgg->eng;

	delete []hgg;
	cvReleaseImage(&colorlaplace);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	delete []moments;
	delete []humoments;
	delete []cm;
	cvReleaseImage(&dst_re);	
	cvReleaseImage(&dst);
	cvReleaseMat(&dst_vector);
	cvReleaseMat(&mat);
	cvReleaseMat(&ProValue);



	http://blog.sciencenet.cn/home.php?mod=space&uid=696950&do=blog&id=699432
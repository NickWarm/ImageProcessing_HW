#include <opencv/cv.h>
#include <opencv/highgui.h> 
#include <opencv/cxcore.h>
#include <iostream>

using namespace std;

void Convolution_operator_x(double Sigma, double Array_ImpulseResponse[], CvMat *Mat_Src, CvMat *Convolution_cropped_dx );
void Convolution_operator_y(double Sigma, double Array_ImpulseResponse[], CvMat *Mat_Src, CvMat *Convolution_cropped_dy );
void Direction(CvMat *Mat_Src,CvMat *Convolution_cropped_dy, CvMat *Convolution_cropped_dx, IplImage *src_Image, IplImage *dst_Image);
void Magnitude(CvMat *src_Convolution_cropped, CvMat *dst_magnitude );


void main()
{
	
	//input signal: Lenna
	IplImage *Lenna = cvLoadImage("Lenna.bmp",-1); //��q�D
	CvMat *Mat_Lenna =cvCreateMat(Lenna->height, Lenna->width, CV_64FC1 ); //�гy�x�}�Ŷ�
	cvConvert(Lenna, Mat_Lenna);	//�N�Ϲ��ন�x�}
	CvMat *Lenna_Convolution_cropped_dx=cvCreateMat(Mat_Lenna->rows, Mat_Lenna->cols,CV_64FC1 );
	CvMat *Lenna_Convolution_cropped_dy=cvCreateMat(Mat_Lenna->rows, Mat_Lenna->cols,CV_64FC1 );


/** Lenna **/
// Sigma1=1.0
	double Sigma1=1.0; 
	double FilterLength_Sigma1[7];  //Length = 2*(3*Signa)+1
	Convolution_operator_x(Sigma1, FilterLength_Sigma1, Mat_Lenna, Lenna_Convolution_cropped_dx);
	Convolution_operator_y(Sigma1, FilterLength_Sigma1, Mat_Lenna, Lenna_Convolution_cropped_dy);
	//cout<<CV_MAT_ELEM(*Lenna_Convolution_cropped_dx,double,100,300)<<endl;  //���լO�_�s�JLenna_Convolution_cropped_dx�A���\	
	//cout<<CV_MAT_ELEM(*Lenna_Convolution_cropped_dy,double,400,3)<<endl;  //���լO�_�s�JLenna_Convolution_cropped_dx�A���\	

	//Magnitude
	IplImage *dst_magnitude_Lenna = cvCreateImage(cvGetSize(Lenna), IPL_DEPTH_8U, 1);
	CvMat *Lenna_magnitude_dx=cvCreateMat(Mat_Lenna->rows, Mat_Lenna->cols,CV_64FC1 );
	CvMat *Lenna_magnitude_dy=cvCreateMat(Mat_Lenna->rows, Mat_Lenna->cols,CV_64FC1 );

	Magnitude(Lenna_Convolution_cropped_dx, Lenna_magnitude_dx);
	Magnitude(Lenna_Convolution_cropped_dy, Lenna_magnitude_dy);
	
	cvAdd(Lenna_magnitude_dx, Lenna_magnitude_dy, dst_magnitude_Lenna);  //cvAdd���ĪG�G�p�G��J�O�x�}�A��X�i�H�۰��ন�Ϥ�


	//�p�⨤��
	IplImage *dst_direction_Lenna = cvCreateImage(cvGetSize(Lenna), IPL_DEPTH_8U, 1);
	Direction(Mat_Lenna, Lenna_Convolution_cropped_dy, Lenna_Convolution_cropped_dx, Lenna, dst_direction_Lenna);  
	//�ڦbDirection�o�Ƶ{�����Y�h�g�FcvConvert()��x�}��Ϥ��A�̪쪺�g�k�O��Direction�Ƶ{���A�AMagnitude�Ƶ{��
	//�o�˰��|���Ӱ��D�A�p�G��Direction�A�|�]���ե�cvConvert()�ɧ�x�}��Ϥ��A�Өϱo�Ϥ��̪�������T�򥢱�
	//�ҥH��Ӧ���Magnitude�Ƶ{���A�ADirection�Ƶ{���A�p���@�Ӥ~���|���Ϥ���T��
	

	//��ܹϤ�
	cvNamedWindow("Direction Image",1);
	cvShowImage("Direction Image",dst_direction_Lenna);
	cvNamedWindow("Magnitude Image",1);
	cvShowImage("Magnitude Image",dst_magnitude_Lenna);
	cvWaitKey(0);

	cvDestroyWindow("Magnitude Image");
	//cvReleaseImage(&dst_magnitude_Lenna);  //���n�������Ŷ��A�ݷ|�٥i�H��
	cvDestroyWindow("Direction Image");
	//cvReleaseImage(&dst_direction_Lenna);  //���n�������Ŷ��A�ݷ|�٥i�H��



// Sigma3 = 3.0
	double Sigma3=3.0; 
	double FilterLength_Sigma3[19];  //Length = 2*(3*Signa)+1
	CvMat *Sigma3_Lenna_Convolution_cropped_dx=cvCreateMat(Mat_Lenna->rows, Mat_Lenna->cols,CV_64FC1 );
	CvMat *Sigma3_Lenna_Convolution_cropped_dy=cvCreateMat(Mat_Lenna->rows, Mat_Lenna->cols,CV_64FC1 );

	Convolution_operator_x(Sigma3, FilterLength_Sigma3, Mat_Lenna, Sigma3_Lenna_Convolution_cropped_dx); 
	Convolution_operator_y(Sigma3, FilterLength_Sigma3, Mat_Lenna, Sigma3_Lenna_Convolution_cropped_dy); 
	// �쥻�O�g��X�s�b�P�@�ӯx�}�̡A���o�˷|�����~�A�ҥH�t�~�Ф@�ӪŶ��x�sSigma2����X�x�}
	
	//cout<<CV_MAT_ELEM(*Sigma2_Lenna_Convolution_cropped_dx,double,100,300)<<endl;  //���\	
	//cout<<CV_MAT_ELEM(*Sigma2_Lenna_Convolution_cropped_dy,double,400,3)<<endl;	   //���\	


	//Magnitude
	Magnitude(Sigma3_Lenna_Convolution_cropped_dx, Lenna_magnitude_dx);
	Magnitude(Sigma3_Lenna_Convolution_cropped_dy, Lenna_magnitude_dy);
	
	cvAdd(Lenna_magnitude_dx, Lenna_magnitude_dy, dst_magnitude_Lenna);  //cvAdd���ĪG�G�p�G��J�O�x�}�A��X�i�H�۰��ন�Ϥ�



	//�p�⨤��
	//IplImage *dst_direction_Lenna_Sigma2 = cvCreateImage(cvGetSize(Lenna), IPL_DEPTH_8U, 1);   
	Direction(Mat_Lenna, Sigma3_Lenna_Convolution_cropped_dy, Sigma3_Lenna_Convolution_cropped_dx, Lenna, dst_direction_Lenna); 



	//��ܹϤ�
	cvNamedWindow("Direction Image",1);
	cvShowImage("Direction Image",dst_direction_Lenna);
	cvNamedWindow("Magnitude Image",1);
	cvShowImage("Magnitude Image",dst_magnitude_Lenna);
	cvWaitKey(0);
	cvDestroyWindow("Magnitude Image");
	//cvReleaseImage(&dst_magnitude_Lenna);
	cvDestroyWindow("Direction Image");
	//cvReleaseImage(&dst_direction_Lenna); 


// Sigma5 = 5.0
	double Sigma5=5.0; 
	double FilterLength_Sigma5[31];  //Length = 2*(3*Signa)+1
	CvMat *Sigma5_Lenna_Convolution_cropped_dx=cvCreateMat(Mat_Lenna->rows, Mat_Lenna->cols,CV_64FC1 );
	CvMat *Sigma5_Lenna_Convolution_cropped_dy=cvCreateMat(Mat_Lenna->rows, Mat_Lenna->cols,CV_64FC1 );

	Convolution_operator_x(Sigma5, FilterLength_Sigma5, Mat_Lenna, Sigma5_Lenna_Convolution_cropped_dx); 
	Convolution_operator_y(Sigma5, FilterLength_Sigma5, Mat_Lenna, Sigma5_Lenna_Convolution_cropped_dy); 
	
	

	//Magnitude
	Magnitude(Sigma5_Lenna_Convolution_cropped_dx, Lenna_magnitude_dx);
	Magnitude(Sigma5_Lenna_Convolution_cropped_dy, Lenna_magnitude_dy);
	
	cvAdd(Lenna_magnitude_dx, Lenna_magnitude_dy, dst_magnitude_Lenna);  //cvAdd���ĪG�G�p�G��J�O�x�}�A��X�i�H�۰��ন�Ϥ�



	//�p�⨤��
	//IplImage *dst_direction_Lenna_Sigma2 = cvCreateImage(cvGetSize(Lenna), IPL_DEPTH_8U, 1);   
	Direction(Mat_Lenna, Sigma5_Lenna_Convolution_cropped_dy, Sigma5_Lenna_Convolution_cropped_dx, Lenna, dst_direction_Lenna); 



	//��ܹϤ�
	cvNamedWindow("Direction Image",1);
	cvShowImage("Direction Image",dst_direction_Lenna);
	cvNamedWindow("Magnitude Image",1);
	cvShowImage("Magnitude Image",dst_magnitude_Lenna);
	cvWaitKey(0);
	cvDestroyWindow("Magnitude Image");
	cvReleaseImage(&dst_magnitude_Lenna);
	cvDestroyWindow("Direction Image");
	cvReleaseImage(&dst_direction_Lenna); 


/** Motorcycle **/


	//input signal: Motrocycle
	IplImage *Motorcycle = cvLoadImage("Motorcycle.bmp",-1); 
	CvMat *Mat_Motorcycle =cvCreateMat(Motorcycle->height, Motorcycle->width, CV_64FC1 ); 
	cvConvert(Motorcycle, Mat_Motorcycle);	
	CvMat *Motorcycle_Convolution_cropped_dx=cvCreateMat(Mat_Motorcycle->rows, Mat_Motorcycle->cols,CV_64FC1 );
	CvMat *Motorcycle_Convolution_cropped_dy=cvCreateMat(Mat_Motorcycle->rows, Mat_Motorcycle->cols,CV_64FC1 );

	
// Sigma1=1.0
	//double Sigma1=1.0; 
	//double FilterLength_Sigma1[7];  //Length = 2*(3*Signa)+1
	Convolution_operator_x(Sigma1, FilterLength_Sigma1, Mat_Motorcycle, Motorcycle_Convolution_cropped_dx);
	Convolution_operator_y(Sigma1, FilterLength_Sigma1, Mat_Motorcycle, Motorcycle_Convolution_cropped_dy);
	

	//Magnitude
	IplImage *dst_magnitude_Motorcycle = cvCreateImage(cvGetSize(Motorcycle), IPL_DEPTH_8U, 1);
	CvMat *Motorcycle_magnitude_dx=cvCreateMat(Mat_Motorcycle->rows, Mat_Motorcycle->cols,CV_64FC1 );
	CvMat *Motorcycle_magnitude_dy=cvCreateMat(Mat_Motorcycle->rows, Mat_Motorcycle->cols,CV_64FC1 );

	Magnitude(Motorcycle_Convolution_cropped_dx, Motorcycle_magnitude_dx);
	Magnitude(Motorcycle_Convolution_cropped_dy, Motorcycle_magnitude_dy);
	
	cvAdd(Motorcycle_magnitude_dx, Motorcycle_magnitude_dy, dst_magnitude_Motorcycle);  


	//�p�⨤��
	IplImage *dst_direction_Motorcycle = cvCreateImage(cvGetSize(Motorcycle), IPL_DEPTH_8U, 1);
	Direction(Mat_Motorcycle, Motorcycle_Convolution_cropped_dy, Motorcycle_Convolution_cropped_dx, Motorcycle, dst_direction_Motorcycle);  
	

	//��ܹϤ�
	cvNamedWindow("Direction Image",1);
	cvShowImage("Direction Image",dst_direction_Motorcycle);
	cvNamedWindow("Magnitude Image",1);
	cvShowImage("Magnitude Image",dst_magnitude_Motorcycle);
	cvWaitKey(0);

	cvDestroyWindow("Magnitude Image");
	//cvReleaseImage(&dst_magnitude_Motorcycle);  //���n�������Ŷ��A�ݷ|�٥i�H��
	cvDestroyWindow("Direction Image");
	//cvReleaseImage(&dst_direction_Motorcycle);  //���n�������Ŷ��A�ݷ|�٥i�H��


// Sigma3 = 3.0
	//double Sigma3=3.0; 
	//double FilterLength_Sigma3[19];  //Length = 2*(3*Signa)+1
	CvMat *Sigma3_Motorcycle_Convolution_cropped_dx=cvCreateMat(Mat_Motorcycle->rows, Mat_Motorcycle->cols,CV_64FC1 );
	CvMat *Sigma3_Motorcycle_Convolution_cropped_dy=cvCreateMat(Mat_Motorcycle->rows, Mat_Motorcycle->cols,CV_64FC1 );

	Convolution_operator_x(Sigma3, FilterLength_Sigma3, Mat_Motorcycle, Sigma3_Motorcycle_Convolution_cropped_dx); 
	Convolution_operator_y(Sigma3, FilterLength_Sigma3, Mat_Motorcycle, Sigma3_Motorcycle_Convolution_cropped_dy); 
	
	
	//Magnitude
	Magnitude(Sigma3_Motorcycle_Convolution_cropped_dx, Motorcycle_magnitude_dx);
	Magnitude(Sigma3_Motorcycle_Convolution_cropped_dy, Motorcycle_magnitude_dy);
	
	cvAdd(Motorcycle_magnitude_dx, Motorcycle_magnitude_dy, dst_magnitude_Motorcycle);  



	//�p�⨤��  
	Direction(Mat_Motorcycle, Sigma3_Motorcycle_Convolution_cropped_dy, Sigma3_Motorcycle_Convolution_cropped_dx, Motorcycle, dst_direction_Motorcycle); 



	//��ܹϤ�
	cvNamedWindow("Direction Image",1);
	cvShowImage("Direction Image",dst_direction_Motorcycle);
	cvNamedWindow("Magnitude Image",1);
	cvShowImage("Magnitude Image",dst_magnitude_Motorcycle);
	cvWaitKey(0);
	cvDestroyWindow("Magnitude Image");
	//cvReleaseImage(&dst_magnitude_Motorcycle);
	cvDestroyWindow("Direction Image");
	//cvReleaseImage(&dst_direction_Motorcycle); 

// Sigma5 = 5.0

	CvMat *Sigma5_Motorcycle_Convolution_cropped_dx=cvCreateMat(Mat_Motorcycle->rows, Mat_Motorcycle->cols,CV_64FC1 );
	CvMat *Sigma5_Motorcycle_Convolution_cropped_dy=cvCreateMat(Mat_Motorcycle->rows, Mat_Motorcycle->cols,CV_64FC1 );

	Convolution_operator_x(Sigma5, FilterLength_Sigma5, Mat_Motorcycle, Sigma5_Motorcycle_Convolution_cropped_dx); 
	Convolution_operator_y(Sigma5, FilterLength_Sigma5, Mat_Motorcycle, Sigma5_Motorcycle_Convolution_cropped_dy); 

	//Magnitude
	Magnitude(Sigma5_Motorcycle_Convolution_cropped_dx, Motorcycle_magnitude_dx);
	Magnitude(Sigma5_Motorcycle_Convolution_cropped_dy, Motorcycle_magnitude_dy);
	cvAdd(Motorcycle_magnitude_dx, Motorcycle_magnitude_dy, dst_magnitude_Motorcycle);  


	//�p�⨤��  
	Direction(Mat_Motorcycle, Sigma5_Motorcycle_Convolution_cropped_dy, Sigma5_Motorcycle_Convolution_cropped_dx, Motorcycle, dst_direction_Motorcycle); 

	//��ܹϤ�
	cvNamedWindow("Direction Image",1);
	cvShowImage("Direction Image",dst_direction_Motorcycle);
	cvNamedWindow("Magnitude Image",1);
	cvShowImage("Magnitude Image",dst_magnitude_Motorcycle);
	cvWaitKey(0);
	cvDestroyWindow("Magnitude Image");
	//cvReleaseImage(&dst_magnitude_Motorcycle);
	cvDestroyWindow("Direction Image");
	//cvReleaseImage(&dst_direction_Motorcycle); 
}



void Convolution_operator_x(double Sigma, double Array_ImpulseResponse[], CvMat *Mat_Src , CvMat *Convolution_cropped_dx)
{
	//double Array_ImpulseResponse[7];

//Impulse Response (Filter)
	//�гy�@���}�C�Ӷi��impulse response
	CvMat Filter_Row = cvMat((int)(6*Sigma+1), 1, CV_64FC1, Array_ImpulseResponse);  //Sigma���@�w�|�O���
	
	//��Jimpulse response���@���L��������� (���O����x���L�P��y���L)
	//��x���L��impulse response (Filter_Row)��J�x�}���U�Ӥ�������
	int index=0;
	for(int x=(-Filter_Row.rows+1)/2;x< (Filter_Row.rows+1)/2;x++)  //x�b�W���ܼơA���F�קK�~�H���}�C�̪����ޭȡA�ҥH int x
		//�������G�����pSigma=1�A�himpulse response�����׬�6*1++1=7�A�̷Ӱ������G���ϧΨӬݡA
		//���������G���d��x=-3~3�A�p�G�ΰ}�C���ޭȪ��ߺD�q�s�}�l��x=0~7�A�h�������G�u�|�e�@�b�A�P�ҭn������
	{
		double a = (double)-x/pow(Sigma,2);
		double b = exp(-pow( (double)x, 2 ) / ( 2 * pow( Sigma, 2 ) ));
		double f_of_x = a*b;
		cvSetReal1D(&Filter_Row, index, f_of_x);
		index++;
	}

	//impulse response mirroring
	CvMat *FlipFilter_Row= cvCreateMat((int)6*Sigma+1, 1, CV_64FC1); //�гy�O�����m�Ӧs��½��᪺ impulse response matrix
	cvFlip(&Filter_Row, FlipFilter_Row,-1 );
	//cvFlip(&Filter_Row, FlipFilter_Row,0 );
	//�쥻�o�˼g�A�̫�@�ӰѼƳ]0�A0������½��A���o�˰��u��½���x�b����V(row)�A�S½���y�b����V
	//�ҥH�Ѽ����ӧ��-1�G�����[����½��



//END: Impulse Response 

	//source mat zero padding�G
	//�ӫ���@�U�A����ڭn�b�O���餤�гy�Ŷ��Ӧs��zeropadding�᪺�x�}(�@���}�C)�A�]���ڳ̫�n��e���ɹL���s�A�h��
	CvMat *Mat_Src_RowPadded = cvCreateMat(Mat_Src->rows + (Filter_Row.rows-1)*2, Mat_Src->cols, CV_64FC1);  
	cvSetZero(Mat_Src_RowPadded);
	//����input image �g�Lzero padding�᪺�x�}�A��cvCreateMat()�Х߯x�}�ɡA�x�}�̪������ƭȬ��s

	//��impulse response ���Lx�Ainput image��x�ɹs (row�ɹs)	
	//for(int i=0;i< Mat_Src->cols;i++)  //���ϸɹs�A�ҥH���׭������ӬO�ݭ�Ϫ��x�}���e�j�p
	for(int j=0;j<Mat_Src->rows;j++)
	{
		for(int i=0;i< Mat_Src->cols;i++)
		//for(int j=0;j<Mat_Src->rows;j++)
		{
			CV_MAT_ELEM( *Mat_Src_RowPadded, double, j+ (Filter_Row.rows-1),i)
				=CV_MAT_ELEM(*Mat_Src, double, j, i);
			//cout<<CV_MAT_ELEM(*Mat_Src,double,j,i)<<endl;
		}
	}
//END: zero padding
	

//Convolution

	CvMat *Mat_Convolution_noncropped_dx=cvCreateMat(Mat_Src->rows + (Filter_Row.rows-1), Mat_Src->cols,CV_64FC1 );
	//CvMat *Mat_Convolution_noncropped_dx=cvCreateMat(Mat_Src->rows, Mat_Src->cols,CV_64FC1 );  
	//�쥻�O�W�����Ѫ��g�k�A���o�˼g�|�����D�A�S�ɨ�s�A�ҥH�令 cvCreateMat(Mat_Src->rows + (Filter_Row.rows-1), Mat_Src->cols,CV_64FC1 )

	//Matrixs do convolution�Aimpulse response�i��time shift   
	for(int i=0; i < Mat_Convolution_noncropped_dx->cols ;i++)  
	{
		for(int j=0; j < Mat_Convolution_noncropped_dx->rows ;j++ )
		{
			//impulse response�@���}�C�̪��ȳ��n�Pinput signal�ۦ��ò֥[
			double sum=0;
			for(int index_Filter=0 ; index_Filter < FlipFilter_Row->rows ;index_Filter++) 
			{
				sum = sum + cvGetReal1D(FlipFilter_Row, index_Filter) * CV_MAT_ELEM(*Mat_Src_RowPadded, double, j+index_Filter, i);  //FlipFliper�̪������Q��CV_MAT_ELEM()�h�X�ݡA���O���ѤF�A��]�ݬd
			}
			CV_MAT_ELEM(*Mat_Convolution_noncropped_dx, double ,j,i)= sum; //��convoluton�B��᪺���G��JMat_Convolution_noncroped_dx�x�}
			//cout<<CV_MAT_ELEM(*Mat_Convolution_noncropped_dx,double,j,i)<<endl;
		}
	}





/*
	//����convolution��O�_���F��
	for(int i=0;i<Mat_Convolution_noncropped_dx->rows;i++)
	{
		for(int j=0;j<Mat_Convolution_noncropped_dx->cols;j++)
		{

			cout<<CV_MAT_ELEM(*Mat_Convolution_noncropped_dx,double,i,j)<<endl;
		}
	}
*/

	//cropped convolution result
	//�гy�@�ӵ��ū᪺�x�}
	//CvMat *Convolution_cropped_dx=cvCreateMat(Mat_Src->rows, Mat_Src->cols,CV_64FC1 );
	cvSetZero(Convolution_cropped_dx);
	//cout<<CV_MAT_ELEM(*Convolution_cropped_dx,double,4,5)<<endl;

	//��x����
	for(int i=0;i<Mat_Convolution_noncropped_dx->rows;i++)
	{
		for(int j=0;j<Mat_Convolution_noncropped_dx->cols;j++)
		{
			int Cropped_Front=(FlipFilter_Row->rows-1)/2;
			if( (i - Cropped_Front >= 0) && (i - Cropped_Front) < (Mat_Convolution_noncropped_dx->rows - FlipFilter_Row->rows+ 1)  )
			{
				CV_MAT_ELEM(*Convolution_cropped_dx, double,i-Cropped_Front,j)=CV_MAT_ELEM(*Mat_Convolution_noncropped_dx, double,i,j);  
				//cout<<"1."<<CV_MAT_ELEM(*Convolution_cropped_dx,double,i-Cropped_Front,j)<<endl;		//���ռƦr�O�_���Q��J�i�h�x�}�̡A���\
				//cout << "2."<<CV_MAT_ELEM(*Mat_Convolution_noncropped_dx, double,i,j)	<< endl;		
			}	
		}	
	}
//END: Convolution
}






void Convolution_operator_y(double Sigma, double Array_ImpulseResponse[], CvMat *Mat_Src, CvMat *Convolution_cropped_dy )
{

//Impulse Response (Filter)
	//�гy�@���}�C�Ӷi��impulse response
	CvMat Filter_Column = cvMat(1, (int)(6*Sigma+1), CV_64FC1, Array_ImpulseResponse);
	
	//��y���L��impulse response (Filter_Column)
	int index=0;
	for(int y=-(Filter_Column.cols-1)/2 ; y< (Filter_Column.cols+1)/2 ; y++)  //y�b�W���ܼ�
	{
		double a = (double)-y/pow(Sigma,2);
		double b = exp(-pow( (double)y, 2 ) / ( 2 * pow( Sigma, 2 ) ));
		double f_of_y = a*b;
		cvSetReal1D(&Filter_Column, index, f_of_y);
		index++;
	}

	//impulse response mirroring
	CvMat *FlipFilter_Column= cvCreateMat(1, (int)6*Sigma+1,  CV_64FC1); 
	cvFlip(&Filter_Column, FlipFilter_Column,-1 );
//END: Impulse Response 

//source mat zero padding�G
	CvMat *Mat_Src_ColumePadded = cvCreateMat(Mat_Src->rows , Mat_Src->cols+ (Filter_Column.cols-1)*2, CV_64FC1);  
	cvSetZero(Mat_Src_ColumePadded);

	//��impulse response ���Ly�Ainput image��y�ɹs (row�ɹs)	
	for(int i=0;i< Mat_Src->cols;i++)  //���ϸɹs�A�ҥH���׭������ӬO�ݭ�Ϫ��x�}���e�j�p
	{
		for(int j=0;j<Mat_Src->rows;j++)
		{
			CV_MAT_ELEM( *Mat_Src_ColumePadded, double, j,i+ (Filter_Column.cols-1))
				=CV_MAT_ELEM(*Mat_Src, double, j, i);
			//cout<<CV_MAT_ELEM(*Mat_Src,double,j,i)<<endl;
		}
	}
//END: zero padding
	


//Convolution
	//�гy�@�ӪŶ��A��convolution�᪺�x�} (y���L�A�|���h���h�l���s)
	CvMat *Mat_Convolution_noncropped_dy = cvCreateMat(Mat_Src->rows , Mat_Src->cols+ (Filter_Column.cols-1), CV_64FC1); 
	//CvMat *Mat_Src_RowPadded = cvCreateMat(Mat_Src->rows + (Filter_Row.rows-1)*2, Mat_Src->cols, CV_64FC1);

	//Matrixs do convolution
	for(int i=0; i < Mat_Convolution_noncropped_dy->cols ;i++)  
	{
		for(int j=0; j < Mat_Convolution_noncropped_dy->rows ;j++ )
		{
			//impulse response�@���}�C�̪��ȳ��n�Pinput signal�ۦ��ò֥[
			double sum=0;
			for(int index_Filter=0 ; index_Filter < FlipFilter_Column->cols ;index_Filter++) 
			{
				sum = sum + cvGetReal1D(FlipFilter_Column, index_Filter)*CV_MAT_ELEM(*Mat_Src_ColumePadded, double, j, i+index_Filter);  //FlipFliper�̪������Q��CV_MAT_ELEM()�h�X�ݡA���O���ѤF�A��]�ݬd
			}
			CV_MAT_ELEM(*Mat_Convolution_noncropped_dy, double ,j,i)= sum; //��convoluton�B��᪺���G��JMat_Convolution_noncroped_dx�x�}
			//cout<<CV_MAT_ELEM(*Mat_Convolution_noncropped_dy,double,j,i)<<endl;
		}
	}

	//cropped convolution result
	//�гy�@�ӵ��ū᪺�x�}
	//CvMat *Convolution_cropped_dy=cvCreateMat(Mat_Src->rows, Mat_Src->cols,CV_64FC1 );
	cvSetZero(Convolution_cropped_dy);
	//cout<<CV_MAT_ELEM(*Convolution_cropped_dx,double,4,5)<<endl;

	//��y����
	for(int i=0;i<Mat_Convolution_noncropped_dy->rows;i++)
	{
		for(int j=0;j<Mat_Convolution_noncropped_dy->cols;j++)
		{
			int Cropped_Front=(FlipFilter_Column->cols-1)/2;
			
			if( (j - Cropped_Front >= 0) && (j - Cropped_Front) < (Mat_Convolution_noncropped_dy->cols - FlipFilter_Column->cols+ 1)  )
			{
				CV_MAT_ELEM(*Convolution_cropped_dy, double,i,j-Cropped_Front)=CV_MAT_ELEM(*Mat_Convolution_noncropped_dy, double,i,j);  
				//cout<<"1."<<CV_MAT_ELEM(*Convolution_cropped_dy,double,i,j-Cropped_Front)<<endl;		//���ռƦr�O�_���Q��J�i�h�x�}�̡A���\
				//cout << "2."<<CV_MAT_ELEM(*Mat_Convolution_noncropped_dy, double,i,j)	<< endl;		
			}	
		}	
	}
//END: Convolution


}


void Direction(CvMat *Mat_Src,CvMat *Convolution_cropped_dy, CvMat *Convolution_cropped_dx, IplImage *src_Image, IplImage *dst_Image)
{
	CvMat *Direction_Mat = cvCreateMat(src_Image->height, src_Image->width, CV_64FC1);
	for(int i=0;i<Mat_Src->cols;i++)
	{
		for(int j=0;j<Mat_Src->rows;j++)
		{
			double y=CV_MAT_ELEM(*Convolution_cropped_dy, double,j,i) ;
			double x=CV_MAT_ELEM(*Convolution_cropped_dx, double,j,i);

			double atan = (( atan2(y,x) + CV_PI)/(2*CV_PI))*255;
			CV_MAT_ELEM(*Direction_Mat, double, j, i) = atan;
		}
	}

	//IplImage *dst_Image = cvCreateImage(cvGetSize(src_Image), IPL_DEPTH_8U, 1);
	cvConvert(Direction_Mat, dst_Image);

}

void Magnitude(CvMat *src_Convolution_cropped, CvMat *dst_magnitude )
{
	//Magnitude�Adx
	CvScalar s; 
	for(int i=0 ;i<src_Convolution_cropped->cols ;i++)
	{
		for(int j=0 ;j<src_Convolution_cropped->rows ;j++)
		{
			// �̪�O����A��convolution��w���Ū����x�}(cropped)�����ন�Ϥ��A�A�h�������
			// ���OConvolution��|���t�ȡA�p�G��cvConvert()�ন�Ϥ��A�h�쥻�p��s���|�j������0�A�W�L255���|�j������255
			// �|�Ҩӻ�   �x�}�̪���T�p�G��-50�A������ȫᬰ50�C�p�G����Ϥ��A���M��ȡA���� -50�|�ܬ�0�A�p���@�ӷ|�ֱ��\�h�쥻�Ϥ�����T
			s=cvGet2D(src_Convolution_cropped,j,i);  //��l��CvScalar
			s.val[0] = fabs( s.val[0] );
			cvSet2D(dst_magnitude,j,i,s);
			//cout<<CV_MAT_ELEM(*dst_magnitude,double,j,i)<<endl;
		}
	}

}


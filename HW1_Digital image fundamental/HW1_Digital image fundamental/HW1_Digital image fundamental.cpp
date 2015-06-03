#include <opencv\cv.h>
#include <opencv\cxcore.h>
#include <opencv\highgui.h>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

// parameters of Lenna
CvPoint center_Lenna = cvPoint(256,256);  // cvPoint( 寬,高)
double sigma_x_Lenna = 150, sigma_y_Lenna =150;

// parameters of Girl
CvPoint center_Girl = cvPoint(350,200);
double sigma_x_Girl = 150, sigma_y_Girl =150;

// parameters of Monet
CvPoint center_Monet = cvPoint(0,0);
double sigma_x_Monet =400, sigma_y_Monet = 300;

// parameters of Object
CvPoint center_Object = cvPoint(0,0);
double sigma_x_Object =300, sigma_y_Object = 400;

// parameters of Lin
CvPoint center_Lin = cvPoint(280,350);
double sigma_x_Lin =200, sigma_y_Lin = 200;


void Multiply_1Channel(IplImage* Original, IplImage* Lighting, IplImage* Output, CvPoint center_Gaussian, double sigma_x, double sigma_y){

	
	//x,y: 笛卡兒座標系  x',y': OpenCv座標系
	//y'=x, x'=-y
	//double y_realx = center_Gaussian.x;
	//double x_realNy =-center_Gaussian.y; //Ny means negative y
	//作業中的參數已經轉換過座標，所以以上方法略
	int x_cvAxis = center_Gaussian.y;
	int y_cvAxis = center_Gaussian.x;

	CvScalar scalar_original, scalar_illumination, scalar_output; 
	for(int i=0;i < Original->height ;i++)
	{
		for(int j=0; j < Original->width ;j++)
		{
			
			double m = (i-x_cvAxis)*(i-x_cvAxis)/(2*sigma_x*sigma_x);
			double n = (j-y_cvAxis)*(j-y_cvAxis)/(2*sigma_y*sigma_y);
			double illumination = exp(-(m+n));
			scalar_original = cvGet2D(Original,i,j);
			scalar_illumination.val[0] = 255*illumination;
			scalar_output.val[0] = scalar_original.val[0]*illumination;
			cvSet2D(Lighting,i,j,scalar_illumination);
			cvSet2D(Output,i,j,scalar_output);

		}
	}
	
	cvNamedWindow("Original",1);
	cvNamedWindow("Lighting",1);
	cvNamedWindow("Output",1);

	cvShowImage("Original",Original);
	cvShowImage("Lighting",Lighting);
	cvShowImage("Output",Output);

	cvWaitKey(0);

	cvDestroyWindow("Original");
	cvDestroyWindow("Lighting");
	cvDestroyWindow("Output");

	cvReleaseImage(&Original);
	cvReleaseImage(&Lighting);
	cvReleaseImage(&Output);
}

void Multiply_3Channel(IplImage* Original, IplImage* Lighting, IplImage* Output, CvPoint center_Gaussian, double sigma_x, double sigma_y){

	int x_cvAxis = center_Gaussian.y;
	int y_cvAxis = center_Gaussian.x;

	CvScalar scalar_original, scalar_illumination, scalar_output; 
	for(int i=0;i < Original->height ;i++)
	{
		for(int j=0; j < Original->width ;j++)
		{
			
			double m = (i-x_cvAxis)*(i-x_cvAxis)/(2*sigma_x*sigma_x);
			double n = (j-y_cvAxis)*(j-y_cvAxis)/(2*sigma_y*sigma_y);
			double illumination = exp(-(m+n));
			scalar_original = cvGet2D(Original,i,j);
			scalar_illumination.val[0] = 255*illumination;
			scalar_illumination.val[1] = 255*illumination;
			scalar_illumination.val[2] = 255*illumination;
			scalar_output.val[0] = scalar_original.val[0]*illumination;
			scalar_output.val[1] = scalar_original.val[1]*illumination;
			scalar_output.val[2] = scalar_original.val[2]*illumination;
			cvSet2D(Lighting,i,j,scalar_illumination);
			cvSet2D(Output,i,j,scalar_output);

		}
	}


	cvNamedWindow("Original_3Channel",1);
	cvNamedWindow("Lighting_3Channel",1);
	cvNamedWindow("Output_3Channel",1);

	cvShowImage("Original_3Channel",Original);
	cvShowImage("Lighting_3Channel",Lighting);
	cvShowImage("Output_3Channel",Output);

	cvWaitKey(0);

	cvDestroyWindow("Original_3Channel");
	cvDestroyWindow("Lighting_3Channel");
	cvDestroyWindow("Output_3Channel");

	cvReleaseImage(&Original);
	cvReleaseImage(&Lighting);
	cvReleaseImage(&Output);
}


void Recovered_1Channel(IplImage* Original, IplImage* Picture_Recoverd, CvPoint center_Gaussian, double sigma_x, double sigma_y){
	int x_cvAxis = center_Gaussian.y;
	int y_cvAxis = center_Gaussian.x;

	CvScalar scalar_original, scalar_recovered; 
	for(int i=0;i < Original->height ;i++)
	{
		for(int j=0; j < Original->width ;j++)
		{
			
			double m = (i-x_cvAxis)*(i-x_cvAxis)/(2*sigma_x*sigma_x);
			double n = (j-y_cvAxis)*(j-y_cvAxis)/(2*sigma_y*sigma_y);
			double illumination = exp(-(m+n));
			scalar_original = cvGet2D(Original,i,j);
			scalar_recovered.val[0] = scalar_original.val[0]/illumination;
			cvSet2D(Picture_Recoverd,i,j,scalar_recovered);
			
		}
	}

	cvNamedWindow("Original",1);
	cvNamedWindow("Recovered",1);

	cvShowImage("Original",Original);
	cvShowImage("Recovered",Picture_Recoverd);

	cvWaitKey(0);

	cvDestroyWindow("Original");
	cvDestroyWindow("Recovered");

	cvReleaseImage(&Original);
	cvReleaseImage(&Picture_Recoverd);

}

void Recovered_3Channel(IplImage* Original, IplImage* Picture_Recoverd, CvPoint center_Gaussian, double sigma_x, double sigma_y){
	int x_cvAxis = center_Gaussian.y;
	int y_cvAxis = center_Gaussian.x;

	CvScalar scalar_original, scalar_recovered; 
	for(int i=0;i < Original->height ;i++)
	{
		for(int j=0; j < Original->width ;j++)
		{
			
			double m = (i-x_cvAxis)*(i-x_cvAxis)/(2*sigma_x*sigma_x);
			double n = (j-y_cvAxis)*(j-y_cvAxis)/(2*sigma_y*sigma_y);
			double illumination = exp(-(m+n));
			scalar_original = cvGet2D(Original,i,j);
			scalar_recovered.val[0] = scalar_original.val[0]/illumination;
			scalar_recovered.val[1] = scalar_original.val[1]/illumination;
			scalar_recovered.val[2] = scalar_original.val[2]/illumination;
			cvSet2D(Picture_Recoverd,i,j,scalar_recovered);
			
		}
	}

	cvNamedWindow("Original",1);
	cvNamedWindow("Recovered",1);

	cvShowImage("Original",Original);
	cvShowImage("Recovered",Picture_Recoverd);

	cvWaitKey(0);

	cvDestroyWindow("Original");
	cvDestroyWindow("Recovered");

	cvReleaseImage(&Original);
	cvReleaseImage(&Picture_Recoverd);
}

void main(){
	// Lenna	
	IplImage* Lenna=cvLoadImage("Lenna.bmp",0);
	IplImage* Lighting_Lenna = cvCreateImage(cvSize(Lenna->width,Lenna->height),IPL_DEPTH_8U,1);	
	IplImage* Output_Lenna = cvCreateImage(cvSize(Lenna->width,Lenna->height),IPL_DEPTH_8U,1);
	
	// Girl
	IplImage* Girl=cvLoadImage("Girl.bmp",1);
	IplImage* Lighting_Girl=cvCreateImage(cvSize(Girl->width,Girl->height),IPL_DEPTH_8U,3);
	IplImage* Output_Girl=cvCreateImage(cvSize(Girl->width,Girl->height),IPL_DEPTH_8U,3);

	// Monet
	IplImage* Monet=cvLoadImage("Monet.bmp",1);
	IplImage* Lighting_Monet=cvCreateImage(cvSize(Monet->width,Monet->height),IPL_DEPTH_8U,3);
	IplImage* Output_Monet=cvCreateImage(cvSize(Monet->width,Monet->height),IPL_DEPTH_8U,3);

	// Object
	IplImage* Object=cvLoadImage("Object.bmp",0);
	IplImage* Object_recovered=cvCreateImage(cvSize(Object->width,Object->height),IPL_DEPTH_8U,1);

	// Lin
	IplImage* Lin=cvLoadImage("Lin.bmp",1);
	IplImage* Lin_recovered=cvCreateImage(cvSize(Lin->width,Lin->height),IPL_DEPTH_8U,3);


	Multiply_1Channel(Lenna, Lighting_Lenna, Output_Lenna, center_Lenna, sigma_x_Lenna, sigma_y_Lenna);
	Multiply_3Channel(Girl, Lighting_Girl, Output_Girl, center_Girl, sigma_x_Girl, sigma_y_Girl);
	Multiply_3Channel(Monet, Lighting_Monet, Output_Monet, center_Monet, sigma_x_Monet, sigma_y_Monet);
	Recovered_1Channel(Object, Object_recovered, center_Object, sigma_x_Object, sigma_y_Object);
	Recovered_3Channel(Lin,  Lin_recovered, center_Lin, sigma_x_Lin, sigma_y_Lin);
}




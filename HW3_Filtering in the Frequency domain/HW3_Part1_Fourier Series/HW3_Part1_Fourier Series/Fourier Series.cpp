#include <iostream>
#include "opencv/cv.h"
#include "opencv/cxcore.h"
#include "opencv/highgui.h"


using namespace std;

IplImage *Image1;
CvPoint FromPoint1, ToPoint1, FromPoint2, ToPoint2;  //ø�s�y�жb
CvPoint FromPoint3, ToPoint3; //ø y=x
CvPoint FromPoint_dst, ToPoint_dst;
CvScalar Color, Color3, ColorLine;
int Thickness;

void Plot_FourierSeries(int k);
void main()
{

	Plot_FourierSeries(5);

	cvNamedWindow("cvLine shift",1);
    cvShowImage("cvLine shift",Image1);
	cvWaitKey(0);

	Plot_FourierSeries(10);

	cvNamedWindow("cvLine shift",1);
    cvShowImage("cvLine shift",Image1);
	cvWaitKey(0);

	Plot_FourierSeries(20);
	cvNamedWindow("cvLine shift",1);
    cvShowImage("cvLine shift",Image1);
	cvWaitKey(0);

	Plot_FourierSeries(30);
	cvNamedWindow("cvLine shift",1);
    cvShowImage("cvLine shift",Image1);
	cvWaitKey(0);

}

void Plot_FourierSeries(int k)
{
	CvSize ImageSize1 = cvSize(500,500);
	Image1 = cvCreateImage(ImageSize1, IPL_DEPTH_8U,3);
	
	Thickness=2;
	
	Image1->origin=1;


	//x, y�b
	FromPoint1 = cvPoint(250,0); 
	ToPoint1 = cvPoint(250,500); 
	FromPoint2 = cvPoint(0,250);
	ToPoint2 = cvPoint(500,250);
	Color = CV_RGB(0,0,0);
	cvLine(Image1, FromPoint1, ToPoint1, Color, Thickness,CV_AA,0);
	cvLine(Image1, FromPoint2, ToPoint2, Color, Thickness,CV_AA,0);


	// y=x���ϧ�
	FromPoint3 = cvPoint(250,250);
	ToPoint3 = cvPoint(500,500);
	Color3 = CV_RGB(0,255,0);
	cvLine(Image1, FromPoint3, ToPoint3, Color3, Thickness,CV_AA,0);

	

	//Fourier Series coefficient
	FromPoint_dst = cvPoint(250,250);
	ColorLine = CV_RGB(0,0,255);
	
/* �ƾǹB�� */
	double Ao, An, Bn ;  //  �Y��
	double  y, sum;      //  output
	//float AnTest;
	// n=1~5
	y=0;
	//sum=0;   
	//����o�˼g�A�]���C��x�I���n���on=1~10��An�PBn�A�p�G��bfor�~���A�e�@��x�I��sum�N�|�v�T�U�@��x�I��sum�A�z�ҷ�M���p�⵲�G�N����
	Ao=CV_PI/4;

	//int k=5;						 
	for(double x=0; x<CV_PI ;x+=0.0001)
	{
		ToPoint_dst = cvPoint(250+x*CV_PI*20 ,250+y*CV_PI*20);
		sum=0;
		for(double n=1;n<=k;n++)  
		{
		 An=(cos(n*CV_PI)-1)/(CV_PI*pow(n, 2));
		 //cout<<An<<endl;
		 
		 Bn=-cos(n*CV_PI)/n;
		 sum = sum + (An*cos(n*x)+Bn*sin(n*x));
		 //cout<<sum<<endl;
		}

		y= Ao+sum;
		cvLine(Image1, FromPoint_dst, ToPoint_dst,ColorLine, 1.5,CV_AA,0);
		FromPoint_dst=ToPoint_dst;
		
	}
	
}
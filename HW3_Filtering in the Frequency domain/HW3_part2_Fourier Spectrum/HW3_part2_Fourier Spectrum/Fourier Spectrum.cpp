#include <opencv\cv.h>
#include <opencv\cxcore.h>
#include <opencv\highgui.h>
#include <iostream>
using namespace std;


void FourierSpectrum(IplImage *Image, IplImage *Image_dst, IplImage *Image_dst_Phase);
void main()
{
	IplImage *Image=0, *Image2=0, *Image3=0, *Image_dst, *Image_dst_Phase;  
	Image = cvLoadImage("Rectangle1.bmp",-1);
	Image2 = cvLoadImage("Rectangle2.bmp",-1);
	Image3 = cvLoadImage("Rectangle3.bmp",-1);

	//把|F(u,v)|與角度運算後的F64的圖片轉回U8，U8格式才能顯示正常圖片
	Image_dst = cvCreateImage(cvGetSize(Image), IPL_DEPTH_8U, 1);
	Image_dst_Phase = cvCreateImage(cvGetSize(Image), IPL_DEPTH_8U, 1);
	

/* Rectangle1 */
	FourierSpectrum(Image, Image_dst, Image_dst_Phase);
	
	// Log Fourier Spectrum
	cvNamedWindow("Log Image",1);
	cvShowImage("Log Image",Image_dst);
	cvWaitKey(0);
	cvDestroyWindow("Log Image");
	
	// Phase Angle
	cvNamedWindow("Phase Image",1);
	cvShowImage("Phase Image",Image_dst_Phase);
	cvWaitKey(0);
	cvDestroyWindow("Phase Image");
	

/* Rectangle2 */	
	FourierSpectrum(Image2, Image_dst, Image_dst_Phase);
	
	// Log Fourier Spectrum
	cvNamedWindow("Log Image",1);
	cvShowImage("Log Image",Image_dst);
	cvWaitKey(0);
	cvDestroyWindow("Log Image");
	
	// Phase Angle
	cvNamedWindow("Phase Image",1);
	cvShowImage("Phase Image",Image_dst_Phase);
	cvWaitKey(0);
	cvDestroyWindow("Phase Image");

	
/* Rectangle3 */	
	FourierSpectrum(Image3, Image_dst, Image_dst_Phase);
	
	// Log Fourier Spectrum
	cvNamedWindow("Log Image",1);
	cvShowImage("Log Image",Image_dst);
	cvWaitKey(0);
	cvDestroyWindow("Log Image");
	
	// Phase Angle
	cvNamedWindow("Phase Image",1);
	cvShowImage("Phase Image",Image_dst_Phase);
	cvWaitKey(0);
	cvDestroyWindow("Phase Image");


	cvReleaseImage(&Image_dst);
	cvReleaseImage(&Image_dst_Phase);
}

void FourierSpectrum(IplImage *Image, IplImage *Image_dst, IplImage *Image_dst_Phase)
{
	IplImage *Image_Re=0, *Image_Im=0, *Image_Freq, *Image_Phase;

	// 為了進行運算 (平方、開根號、算角度)，所以要把原本圖片的U8轉成F64
	Image_Re = cvCreateImage(cvGetSize(Image), IPL_DEPTH_64F, 1); //創造單通道，實部，為了要把U8轉成F64，所以用IPL_DEPTH_64F
	Image_Im = cvCreateImage(cvGetSize(Image), IPL_DEPTH_64F, 1); //虛部
	Image_Freq = cvCreateImage(cvGetSize(Image), IPL_DEPTH_64F, 2);  //DFT後，數值有實部與虛部，第一個通道放實部的參數，第二個通道放虛部的參數，到這一步僅有創造出空間，Image_Freq還沒有內容物

	//角度運算的圖
	Image_Phase = cvCreateImage(cvGetSize(Image), IPL_DEPTH_64F, 1); 


	cvConvert(Image, Image_Re);


	//下面這兩條其實可略，因為虛部一開始就是零，就算用Image_Re經過cvMerge()，還是跟原來的圖片資訊一樣，但這想法還沒寫過要試試看
	cvSetZero(Image_Im); //初始化虛部
	cvMerge(Image_Re, Image_Im, 0, 0, Image_Freq ); //原圖的資訊已被分為實部與虛部，可準備進行DFT的操作，不過此時低頻在圖片的中央，高頻在圖片的四個角落，該先把時域F64的圖片資訊乘上(-1)^(x+y)，如此一來負列轉換後的頻域資訊也會跟著進行平移，使得高頻移動到圖片中央


	// 將高頻訊號平移到圖片中央
	for(int i=0;i<Image->height;i++)
	{
		for(int j=0;j<Image->width;j++)
		{
			CV_IMAGE_ELEM(Image_Freq, double,i,j*2)=pow(-1.0,i+j)*CV_IMAGE_ELEM(Image_Freq, double, i, j*2);
		}
	}


	cvDFT(Image_Freq, Image_Freq, CV_DXT_FORWARD);
	

	//算角度方法一：不需要通過分割，直接從複數去操作，成功
	/*for(int i=0;i<Image->height;i++)
	{
		for(int j=0;j<Image->width;j++)
		{
			double Real = CV_IMAGE_ELEM(Image_Freq, double, i, j*2);
			double Imaginary = CV_IMAGE_ELEM(Image_Freq, double, i, j*2+1);
			double atan = atan2(Imaginary, Real);
			CV_IMAGE_ELEM(Image_Phase, double,i,j)=(atan+CV_PI)*255/(2*CV_PI);  
		}
	}*/


//算|F(u,v)|與F(u,v)的角度
//原本的Image_Freq是一個雙通道的圖，第一通道是實部，第二通道是虛部，|F(u,v)| = sqrt(Re^2 + Im^2)，要拆成實部一張圖，虛部一張圖


	//分割成實部虛部
	cvSplit(Image_Freq, Image_Re, Image_Im,0,0);

	//算角度方法二：將複數分割成實部、虛部
	for(int i=0;i<Image->height;i++)
	{
		for(int j=0;j<Image->width;j++)
		{
			double Real = CV_IMAGE_ELEM(Image_Re, double, i, j);
			double Imaginary = CV_IMAGE_ELEM(Image_Im, double, i, j);
			double atan = atan2(Imaginary, Real);
			CV_IMAGE_ELEM(Image_Phase, double,i,j)=(atan+CV_PI)*255/(2*CV_PI);  
		}
	}

	

	cvConvert(Image_Phase, Image_dst_Phase);

	/*cvNamedWindow("Phase Image",1);
	cvShowImage("Phase Image",Image_dst_Phase);
	cvWaitKey(0);
	cvDestroyWindow("Phase Image");
	cvReleaseImage(&Image_dst_Phase);*/



	// 計算log(1 + Mag)
	cvPow(Image_Re, Image_Re, 2.0);  //  Re^2
	cvPow(Image_Im, Image_Im, 2.0);  //  Im^2
	cvAdd(Image_Re, Image_Im, Image_Re);  // Re^2 + Im^2
	cvPow(Image_Re, Image_Re, 0.5);  // |F(u,v)| = sqrt(Re^2 + Im^2)
	
	cvAddS(Image_Re, cvScalar(1), Image_Re);  // 1 + Mag
	cvLog(Image_Re, Image_Re);  // log(1 + Mag)



	//印出log(1 + Mag)，印出的結果皆大於0，所以正規畫0~255時，全部的數值應該要減掉(最小值*255)
	/*for(int i=0;i<Image->height;i++)
	{
		for(int j=0;j<Image->width;j++)
		{
			cout<<CV_IMAGE_ELEM(Image_Re, double,i,j*2)<<endl;
		}
	}*/



	// Normalize to 0~255
	double MinValue=0, MaxValue=0, Scale=0, Shift=0;
	cvMinMaxLoc(Image_Re, &MinValue, &MaxValue);  // 取出最大值與最小值

	/*cout<<MinValue<<endl;
	cout<<MaxValue<<endl;*/

	Scale = 255/(MaxValue-MinValue);// 把原本圖片裡的資訊比例化，並乘上255使之成為U8的格式，U8: 0~255
	Shift = -MinValue*Scale;  //在圖片資訊為F64時，做了數學運算 log(1 + Mag)，因為「1 + Mag」裡面這個+1的動作，使得圖片資訊轉回U8時可能會超出0~255，所以才要做Shift的動作來扣除原本加出去的，使圖片資訊的範圍維持在0~255


	//system("pause");

	cvConvertScale(Image_Re, Image_dst, Scale, Shift);

	/*for(int i=0;i<Image->height;i++)
	{
		for(int j=0;j<Image->width;j++)
		{
			cout<<CV_IMAGE_ELEM(Image_dst, double,i,j*2)<<endl;
		}
	}*/

	cvReleaseImage(&Image_Re);
	cvReleaseImage(&Image_Im);
	cvReleaseImage(&Image_Freq);
	cvReleaseImage(&Image_Phase);
}
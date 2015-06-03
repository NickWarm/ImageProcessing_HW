#include <opencv\cv.h>
#include <opencv\cxcore.h>
#include <opencv\highgui.h>

using namespace std;

void Lowpass_Gaussian(IplImage *Image, IplImage *Image_dst, int Do);
void Highpass_Gaussian(IplImage *Image, IplImage *Image_dst, int Do);
void main()
{

	IplImage *Image, *Image2, *Image_dst;
	Image = cvLoadImage("Jenny_Gray.bmp",-1);
	Image2 = cvLoadImage("Baboon_Gray.bmp",-1);
	Image_dst = cvCreateImage(cvGetSize(Image), IPL_DEPTH_8U, 1);
	
	// 參數
	int Do1=30, Do2=60;

/* Jenny_Gray */
	//Lowpass Filtering，Do1=30
	Lowpass_Gaussian(Image, Image_dst,Do1);
	cvNamedWindow("Lowpass Image",1);
	cvShowImage("Lowpass Image",Image_dst);
	cvWaitKey(0);
	cvDestroyWindow("Lowpass Image");

	//Highpass Filtering，Do1=30
	Highpass_Gaussian(Image, Image_dst,Do1);
	cvNamedWindow("Highpass Image",1);
	cvShowImage("Highpass Image",Image_dst);
	cvWaitKey(0);
	cvDestroyWindow("Highpass Image");

	//Lowpass Filtering，Do1=60
	Lowpass_Gaussian(Image, Image_dst,Do2);
	cvNamedWindow("Lowpass Image",1);
	cvShowImage("Lowpass Image",Image_dst);
	cvWaitKey(0);
	cvDestroyWindow("Lowpass Image");

	//Highpass Filtering，Do1=60
	Highpass_Gaussian(Image, Image_dst,Do2);
	cvNamedWindow("Highpass Image",1);
	cvShowImage("Highpass Image",Image_dst);
	cvWaitKey(0);
	cvDestroyWindow("Highpass Image");

/* Baboon_Gray */	
	//Lowpass Filtering，Do1=30
	Lowpass_Gaussian(Image2, Image_dst,Do1);
	cvNamedWindow("Lowpass Image",1);
	cvShowImage("Lowpass Image",Image_dst);
	cvWaitKey(0);
	cvDestroyWindow("Lowpass Image");

	//Highpass Filtering，Do1=30
	Highpass_Gaussian(Image2, Image_dst,Do1);
	cvNamedWindow("Highpass Image",1);
	cvShowImage("Highpass Image",Image_dst);
	cvWaitKey(0);
	cvDestroyWindow("Highpass Image");

	//Lowpass Filtering，Do1=60
	Lowpass_Gaussian(Image2, Image_dst,Do2);
	cvNamedWindow("Lowpass Image",1);
	cvShowImage("Lowpass Image",Image_dst);
	cvWaitKey(0);
	cvDestroyWindow("Lowpass Image");

	//Highpass Filtering，Do1=60
	Highpass_Gaussian(Image2, Image_dst,Do2);
	cvNamedWindow("Highpass Image",1);
	cvShowImage("Highpass Image",Image_dst);
	cvWaitKey(0);
	cvDestroyWindow("Highpass Image");


	//釋放記憶體
	cvReleaseImage(&Image);
	cvReleaseImage(&Image2);
	cvReleaseImage(&Image_dst);
}

void Lowpass_Gaussian(IplImage *Image, IplImage *Image_dst, int Do)
{
	IplImage *Image_Re, *Image_Im, *Image_Freq;

	Image_Re = cvCreateImage(cvGetSize(Image), IPL_DEPTH_64F, 1);
	Image_Im = cvCreateImage(cvGetSize(Image), IPL_DEPTH_64F, 1);
	Image_Freq = cvCreateImage(cvGetSize(Image), IPL_DEPTH_64F, 2);
	//Image_dst = cvCreateImage(cvGetSize(Image), IPL_DEPTH_8U, 1);

	cvConvert(Image, Image_Re);
	cvZero(Image_Im);
	cvMerge(Image_Re, Image_Im,0,0, Image_Freq);

	//高頻移到圖片中心
	for(int i=0;i<Image->height;i++)
	{
		for(int j=0;j<Image->width;j++)
		{
			CV_IMAGE_ELEM(Image_Freq, double, i, j*2) = pow(-1.0,i+j)*CV_IMAGE_ELEM(Image_Freq, double, i, j*2);
			//CV_IMAGE_ELEM(Image_Freq, double, i, j*2+1) = pow(-1.0,i+j)*CV_IMAGE_ELEM(Image_Freq, double, i, j*2+1);
			//第二行可不寫，因為虛部一開始就設為零，乘上(-1)^(i+j)後依舊是零
		}
	}

	// discreate time Fourier transform
	cvDFT(Image_Freq,Image_Freq, CV_DXT_FORWARD);

	// F(u,v)*H(u,v)，time domain進行convolution轉換到frequency domain為相乘
	//int Do=30;
	double Xc = Image->height/2;
	double Yc = Image->width/2;
	for(int i=0;i<Image->height;i++)
	{
		for(int j=0;j<Image->width;j++)
		{
			double squareX = pow(i-Xc, 2);
			double squareY = pow(j-Yc, 2);
			CV_IMAGE_ELEM(Image_Freq, double,i, j*2)=exp(-(squareX + squareY)/(2*Do*Do))*CV_IMAGE_ELEM(Image_Freq, double,i, j*2);
			CV_IMAGE_ELEM(Image_Freq, double,i, j*2+1)=exp(-(squareX + squareY)/(2*Do*Do))*CV_IMAGE_ELEM(Image_Freq, double,i, j*2+1);
		}
	}


	//Inverse Fourier transform：頻域轉時域 
	//因為f(u,v)=(1/MN)*{inverse F(u,v)}，實際上反轉換後的圖片資訊會比原圖大MN倍  (MN為輸入圖片的大小)，
	cvDFT(Image_Freq,Image_Freq, CV_DXT_INVERSE);


	//Normalize to 0~255，方法一：圖片資訊除MN
	double M = Image->height;
	double N = Image->width;
	cvConvertScale(Image_Freq,Image_Freq,1/(M*N));


	//Normalize to 0~255，方法二：圖片資訊除MN (用Image->imageSize取得圖片大小)，失敗，因為imageSize = height*widthStep，而widthStep = width*nChannels的寬度大小
	//而Image_Freq是雙通道的，所以算出來的imageSize會變成兩倍大
	//cvConvertScale(Image_Freq,Image_Freq,1/Image->imageSize);  


	//反轉換後，時域的訊號只會有實部的資訊，所以要分割取實部
	cvSplit(Image_Freq, Image_Re,0,0,0);

	//Normalize to 0~255，方法二：圖片資訊除MN (用Image->imageSize取得圖片大小)，失敗，原因不明
	//cvConvertScale(Image_Re,Image_Re,1/Image->imageSize);

	//Normalize to 0~255，方法二：圖片資訊除MN (用Image->imageSize取得圖片大小)，程式成功可RUN，把1/Image->imageSize改成1.0/Image->imageSize即可，但是原因可能要查API才知道
	//cvConvertScale(Image_Re,Image_Re,1.0/Image->imageSize);

	//討論後的原因：
	//64位元的單通道寬度(widthStep)是8位元的8倍，而imageSize = height*width*nChannels的寬度大小，
	//imageSize = height*width*nChannels的寬度大小，原圖從U8轉換到F64，64位元的單通道寬度(widthStep)是8位元的8倍
	//由於是先正規化，之後才從F64轉回U8，在正規化時(F64)用Image_Re->imageSize所取出的圖片大小，比轉換後的圖片大小(U8)大了8倍
	//所以按照這順序寫的話，應該用如下的寫法
	//cvConvertScale(Image_Re,Image_Re,8.0/(Image_Re->imageSize));

	//使用imageSize來正規化討論後較佳的寫法：先把圖片從F64轉回U8後再正規劃，就不用考慮"64位元的單通道寬度(widthStep)是8位元的8倍"
	


	//將圖片中心高頻移回四個角落
	for(int i=0;i<Image->height;i++)
	{
		for(int j=0;j<Image->width;j++)
		{
			CV_IMAGE_ELEM(Image_Re, double, i, j) = pow(-1.0,i+j)*CV_IMAGE_ELEM(Image_Re, double, i, j);
			//cout<<CV_IMAGE_ELEM(Image_Re, double, i, j)<<endl;
		}
	}

	


	//Normalize to 0~255，方法三：頻譜的做法
	//double MinValue=0, MaxValue=0, Scale=0, Shift=0;
	//cvMinMaxLoc(Image_Re, &MinValue, &MaxValue);  // 取出最大值與最小值
	//Scale = 255/((MaxValue-MinValue));// 把原本圖片裡的資訊比例化，並乘上255使之成為U8的格式，U8: 0~255
	////Shift = -MinValue*Scale;
	//cvConvertScale(Image_Re, Image_dst, Scale);
	////cvConvertScale(Image_Re, Image_dst, Scale, Shift);


	//F64轉回U8 (方法一與三的做法)
	cvConvert(Image_Re, Image_dst);
	

	//錯誤的方法二
	//Normalize to 0~255，方法二：使用imageSize來正規化，先把圖片從F64轉回U8後再正規劃，就不用考慮"64位元的單通道寬度(widthStep)是8位元的8倍"
	//cvConvert(Image_Re, Image_dst);
	//cvConvertScale(Image_dst,Image_dst,1.0/(Image_dst->imageSize));
	//失敗，原因：用cvConvert()先轉換型態，輸出會被限制在0~255，F64的圖像資訊大都超過255，所以會造成圖片資訊遺失

	//正確的方法二
	//Normalize to 0~255，方法二：使用imageSize來正規化，先把圖片從F64轉回U8後再正規劃，就不用考慮"64位元的單通道寬度(widthStep)是8位元的8倍"
	//從F64轉U8  並正規化改用cvConvertScale()，成功
	//原因：cvConvertScale()這條函式作用的效果依序為：先正規化再平移再轉形態
	//cvConvertScale(Image_Re,Image_dst,1.0/(Image_dst->imageSize));

	

	//釋放記憶體
	cvReleaseImage(&Image_Re);
	cvReleaseImage(&Image_Im);
	cvReleaseImage(&Image_Freq);

}



void Highpass_Gaussian(IplImage *Image, IplImage *Image_dst, int Do)
{
	IplImage *Image_Re, *Image_Im, *Image_Freq;

	Image_Re = cvCreateImage(cvGetSize(Image), IPL_DEPTH_64F, 1);
	Image_Im = cvCreateImage(cvGetSize(Image), IPL_DEPTH_64F, 1);
	Image_Freq = cvCreateImage(cvGetSize(Image), IPL_DEPTH_64F, 2);
	//Image_dst = cvCreateImage(cvGetSize(Image), IPL_DEPTH_8U, 1);

	cvConvert(Image, Image_Re);
	cvZero(Image_Im);
	cvMerge(Image_Re, Image_Im,0,0, Image_Freq);

	//高頻移到圖片中心
	for(int i=0;i<Image->height;i++)
	{
		for(int j=0;j<Image->width;j++)
		{
			CV_IMAGE_ELEM(Image_Freq, double, i, j*2) = pow(-1.0,i+j)*CV_IMAGE_ELEM(Image_Freq, double, i, j*2);
			//CV_IMAGE_ELEM(Image_Freq, double, i, j*2+1) = pow(-1.0,i+j)*CV_IMAGE_ELEM(Image_Freq, double, i, j*2+1);
			//第二行可不寫，因為虛部一開始就設為零，乘上(-1)^(i+j)後依舊是零
		}
	}

	// discreate time Fourier transform
	cvDFT(Image_Freq,Image_Freq, CV_DXT_FORWARD);

	// F(u,v)*H(u,v)，time domain進行convolution轉換到frequency domain為相乘
	//int Do=30;
	double Xc = Image->height/2;
	double Yc = Image->width/2;
	for(int i=0;i<Image->height;i++)
	{
		for(int j=0;j<Image->width;j++)
		{
			double squareX = pow(i-Xc, 2);
			double squareY = pow(j-Yc, 2);
			CV_IMAGE_ELEM(Image_Freq, double,i, j*2)=(1-exp(-(squareX + squareY)/(2*Do*Do)))*CV_IMAGE_ELEM(Image_Freq, double,i, j*2);
			CV_IMAGE_ELEM(Image_Freq, double,i, j*2+1)=(1-exp(-(squareX + squareY)/(2*Do*Do)))*CV_IMAGE_ELEM(Image_Freq, double,i, j*2+1);
		}
	}


	//Inverse Fourier transform：頻域轉時域 
	//因為f(u,v)=(1/MN)*{inverse F(u,v)}，實際上反轉換後的圖片資訊會比原圖大MN倍  (MN為輸入圖片的大小)，
	cvDFT(Image_Freq,Image_Freq, CV_DXT_INVERSE);


	//反轉換後，時域的訊號只會有實部的資訊，所以要分割取實部
	cvSplit(Image_Freq, Image_Re,0,0,0);


	//將圖片中心高頻移回四個角落
	for(int i=0;i<Image->height;i++)
	{
		for(int j=0;j<Image->width;j++)
		{
			CV_IMAGE_ELEM(Image_Re, double, i, j) = pow(-1.0,i+j)*CV_IMAGE_ELEM(Image_Re, double, i, j);
			//cout<<CV_IMAGE_ELEM(Image_Re, double, i, j)<<endl;
		}
	}

	
	//Normalize to 0~255：使用imageSize來正規化，先把圖片從F64轉回U8後再正規劃，就不用考慮"64位元的單通道寬度(widthStep)是8位元的8倍"
	//cvConvertScale()這條函式作用的效果依序為：先正規化再平移再轉形態
	cvConvertScale(Image_Re,Image_dst,1.0/(Image_dst->imageSize));

	

	//釋放記憶體
	cvReleaseImage(&Image_Re);
	cvReleaseImage(&Image_Im);
	cvReleaseImage(&Image_Freq);

}
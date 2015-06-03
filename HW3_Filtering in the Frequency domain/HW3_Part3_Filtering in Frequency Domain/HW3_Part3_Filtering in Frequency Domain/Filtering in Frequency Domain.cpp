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
	
	// �Ѽ�
	int Do1=30, Do2=60;

/* Jenny_Gray */
	//Lowpass Filtering�ADo1=30
	Lowpass_Gaussian(Image, Image_dst,Do1);
	cvNamedWindow("Lowpass Image",1);
	cvShowImage("Lowpass Image",Image_dst);
	cvWaitKey(0);
	cvDestroyWindow("Lowpass Image");

	//Highpass Filtering�ADo1=30
	Highpass_Gaussian(Image, Image_dst,Do1);
	cvNamedWindow("Highpass Image",1);
	cvShowImage("Highpass Image",Image_dst);
	cvWaitKey(0);
	cvDestroyWindow("Highpass Image");

	//Lowpass Filtering�ADo1=60
	Lowpass_Gaussian(Image, Image_dst,Do2);
	cvNamedWindow("Lowpass Image",1);
	cvShowImage("Lowpass Image",Image_dst);
	cvWaitKey(0);
	cvDestroyWindow("Lowpass Image");

	//Highpass Filtering�ADo1=60
	Highpass_Gaussian(Image, Image_dst,Do2);
	cvNamedWindow("Highpass Image",1);
	cvShowImage("Highpass Image",Image_dst);
	cvWaitKey(0);
	cvDestroyWindow("Highpass Image");

/* Baboon_Gray */	
	//Lowpass Filtering�ADo1=30
	Lowpass_Gaussian(Image2, Image_dst,Do1);
	cvNamedWindow("Lowpass Image",1);
	cvShowImage("Lowpass Image",Image_dst);
	cvWaitKey(0);
	cvDestroyWindow("Lowpass Image");

	//Highpass Filtering�ADo1=30
	Highpass_Gaussian(Image2, Image_dst,Do1);
	cvNamedWindow("Highpass Image",1);
	cvShowImage("Highpass Image",Image_dst);
	cvWaitKey(0);
	cvDestroyWindow("Highpass Image");

	//Lowpass Filtering�ADo1=60
	Lowpass_Gaussian(Image2, Image_dst,Do2);
	cvNamedWindow("Lowpass Image",1);
	cvShowImage("Lowpass Image",Image_dst);
	cvWaitKey(0);
	cvDestroyWindow("Lowpass Image");

	//Highpass Filtering�ADo1=60
	Highpass_Gaussian(Image2, Image_dst,Do2);
	cvNamedWindow("Highpass Image",1);
	cvShowImage("Highpass Image",Image_dst);
	cvWaitKey(0);
	cvDestroyWindow("Highpass Image");


	//����O����
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

	//���W����Ϥ�����
	for(int i=0;i<Image->height;i++)
	{
		for(int j=0;j<Image->width;j++)
		{
			CV_IMAGE_ELEM(Image_Freq, double, i, j*2) = pow(-1.0,i+j)*CV_IMAGE_ELEM(Image_Freq, double, i, j*2);
			//CV_IMAGE_ELEM(Image_Freq, double, i, j*2+1) = pow(-1.0,i+j)*CV_IMAGE_ELEM(Image_Freq, double, i, j*2+1);
			//�ĤG��i���g�A�]���곡�@�}�l�N�]���s�A���W(-1)^(i+j)����¬O�s
		}
	}

	// discreate time Fourier transform
	cvDFT(Image_Freq,Image_Freq, CV_DXT_FORWARD);

	// F(u,v)*H(u,v)�Atime domain�i��convolution�ഫ��frequency domain���ۭ�
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


	//Inverse Fourier transform�G�W����ɰ� 
	//�]��f(u,v)=(1/MN)*{inverse F(u,v)}�A��ڤW���ഫ�᪺�Ϥ���T�|���ϤjMN��  (MN����J�Ϥ����j�p)�A
	cvDFT(Image_Freq,Image_Freq, CV_DXT_INVERSE);


	//Normalize to 0~255�A��k�@�G�Ϥ���T��MN
	double M = Image->height;
	double N = Image->width;
	cvConvertScale(Image_Freq,Image_Freq,1/(M*N));


	//Normalize to 0~255�A��k�G�G�Ϥ���T��MN (��Image->imageSize���o�Ϥ��j�p)�A���ѡA�]��imageSize = height*widthStep�A��widthStep = width*nChannels���e�פj�p
	//��Image_Freq�O���q�D���A�ҥH��X�Ӫ�imageSize�|�ܦ��⭿�j
	//cvConvertScale(Image_Freq,Image_Freq,1/Image->imageSize);  


	//���ഫ��A�ɰ쪺�T���u�|���곡����T�A�ҥH�n���Ψ��곡
	cvSplit(Image_Freq, Image_Re,0,0,0);

	//Normalize to 0~255�A��k�G�G�Ϥ���T��MN (��Image->imageSize���o�Ϥ��j�p)�A���ѡA��]����
	//cvConvertScale(Image_Re,Image_Re,1/Image->imageSize);

	//Normalize to 0~255�A��k�G�G�Ϥ���T��MN (��Image->imageSize���o�Ϥ��j�p)�A�{�����\�iRUN�A��1/Image->imageSize�令1.0/Image->imageSize�Y�i�A���O��]�i��n�dAPI�~���D
	//cvConvertScale(Image_Re,Image_Re,1.0/Image->imageSize);

	//�Q�׫᪺��]�G
	//64�줸����q�D�e��(widthStep)�O8�줸��8���A��imageSize = height*width*nChannels���e�פj�p�A
	//imageSize = height*width*nChannels���e�פj�p�A��ϱqU8�ഫ��F64�A64�줸����q�D�e��(widthStep)�O8�줸��8��
	//�ѩ�O�����W�ơA����~�qF64��^U8�A�b���W�Ʈ�(F64)��Image_Re->imageSize�Ҩ��X���Ϥ��j�p�A���ഫ�᪺�Ϥ��j�p(U8)�j�F8��
	//�ҥH���ӳo���Ǽg���ܡA���ӥΦp�U���g�k
	//cvConvertScale(Image_Re,Image_Re,8.0/(Image_Re->imageSize));

	//�ϥ�imageSize�ӥ��W�ưQ�׫���Ϊ��g�k�G����Ϥ��qF64��^U8��A���W���A�N���ΦҼ{"64�줸����q�D�e��(widthStep)�O8�줸��8��"
	


	//�N�Ϥ����߰��W���^�|�Ө���
	for(int i=0;i<Image->height;i++)
	{
		for(int j=0;j<Image->width;j++)
		{
			CV_IMAGE_ELEM(Image_Re, double, i, j) = pow(-1.0,i+j)*CV_IMAGE_ELEM(Image_Re, double, i, j);
			//cout<<CV_IMAGE_ELEM(Image_Re, double, i, j)<<endl;
		}
	}

	


	//Normalize to 0~255�A��k�T�G�W�Ъ����k
	//double MinValue=0, MaxValue=0, Scale=0, Shift=0;
	//cvMinMaxLoc(Image_Re, &MinValue, &MaxValue);  // ���X�̤j�ȻP�̤p��
	//Scale = 255/((MaxValue-MinValue));// ��쥻�Ϥ��̪���T��ҤơA�í��W255�Ϥ�����U8���榡�AU8: 0~255
	////Shift = -MinValue*Scale;
	//cvConvertScale(Image_Re, Image_dst, Scale);
	////cvConvertScale(Image_Re, Image_dst, Scale, Shift);


	//F64��^U8 (��k�@�P�T�����k)
	cvConvert(Image_Re, Image_dst);
	

	//���~����k�G
	//Normalize to 0~255�A��k�G�G�ϥ�imageSize�ӥ��W�ơA����Ϥ��qF64��^U8��A���W���A�N���ΦҼ{"64�줸����q�D�e��(widthStep)�O8�줸��8��"
	//cvConvert(Image_Re, Image_dst);
	//cvConvertScale(Image_dst,Image_dst,1.0/(Image_dst->imageSize));
	//���ѡA��]�G��cvConvert()���ഫ���A�A��X�|�Q����b0~255�AF64���Ϲ���T�j���W�L255�A�ҥH�|�y���Ϥ���T��

	//���T����k�G
	//Normalize to 0~255�A��k�G�G�ϥ�imageSize�ӥ��W�ơA����Ϥ��qF64��^U8��A���W���A�N���ΦҼ{"64�줸����q�D�e��(widthStep)�O8�줸��8��"
	//�qF64��U8  �å��W�Ƨ��cvConvertScale()�A���\
	//��]�GcvConvertScale()�o���禡�@�Ϊ��ĪG�̧Ǭ��G�����W�ƦA�����A��κA
	//cvConvertScale(Image_Re,Image_dst,1.0/(Image_dst->imageSize));

	

	//����O����
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

	//���W����Ϥ�����
	for(int i=0;i<Image->height;i++)
	{
		for(int j=0;j<Image->width;j++)
		{
			CV_IMAGE_ELEM(Image_Freq, double, i, j*2) = pow(-1.0,i+j)*CV_IMAGE_ELEM(Image_Freq, double, i, j*2);
			//CV_IMAGE_ELEM(Image_Freq, double, i, j*2+1) = pow(-1.0,i+j)*CV_IMAGE_ELEM(Image_Freq, double, i, j*2+1);
			//�ĤG��i���g�A�]���곡�@�}�l�N�]���s�A���W(-1)^(i+j)����¬O�s
		}
	}

	// discreate time Fourier transform
	cvDFT(Image_Freq,Image_Freq, CV_DXT_FORWARD);

	// F(u,v)*H(u,v)�Atime domain�i��convolution�ഫ��frequency domain���ۭ�
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


	//Inverse Fourier transform�G�W����ɰ� 
	//�]��f(u,v)=(1/MN)*{inverse F(u,v)}�A��ڤW���ഫ�᪺�Ϥ���T�|���ϤjMN��  (MN����J�Ϥ����j�p)�A
	cvDFT(Image_Freq,Image_Freq, CV_DXT_INVERSE);


	//���ഫ��A�ɰ쪺�T���u�|���곡����T�A�ҥH�n���Ψ��곡
	cvSplit(Image_Freq, Image_Re,0,0,0);


	//�N�Ϥ����߰��W���^�|�Ө���
	for(int i=0;i<Image->height;i++)
	{
		for(int j=0;j<Image->width;j++)
		{
			CV_IMAGE_ELEM(Image_Re, double, i, j) = pow(-1.0,i+j)*CV_IMAGE_ELEM(Image_Re, double, i, j);
			//cout<<CV_IMAGE_ELEM(Image_Re, double, i, j)<<endl;
		}
	}

	
	//Normalize to 0~255�G�ϥ�imageSize�ӥ��W�ơA����Ϥ��qF64��^U8��A���W���A�N���ΦҼ{"64�줸����q�D�e��(widthStep)�O8�줸��8��"
	//cvConvertScale()�o���禡�@�Ϊ��ĪG�̧Ǭ��G�����W�ƦA�����A��κA
	cvConvertScale(Image_Re,Image_dst,1.0/(Image_dst->imageSize));

	

	//����O����
	cvReleaseImage(&Image_Re);
	cvReleaseImage(&Image_Im);
	cvReleaseImage(&Image_Freq);

}
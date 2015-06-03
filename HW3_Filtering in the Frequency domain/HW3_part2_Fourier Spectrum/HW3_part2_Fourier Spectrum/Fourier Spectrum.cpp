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

	//��|F(u,v)|�P���׹B��᪺F64���Ϥ���^U8�AU8�榡�~����ܥ��`�Ϥ�
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

	// ���F�i��B�� (����B�}�ڸ��B�⨤��)�A�ҥH�n��쥻�Ϥ���U8�নF64
	Image_Re = cvCreateImage(cvGetSize(Image), IPL_DEPTH_64F, 1); //�гy��q�D�A�곡�A���F�n��U8�নF64�A�ҥH��IPL_DEPTH_64F
	Image_Im = cvCreateImage(cvGetSize(Image), IPL_DEPTH_64F, 1); //�곡
	Image_Freq = cvCreateImage(cvGetSize(Image), IPL_DEPTH_64F, 2);  //DFT��A�ƭȦ��곡�P�곡�A�Ĥ@�ӳq�D��곡���ѼơA�ĤG�ӳq�D��곡���ѼơA��o�@�B�Ȧ��гy�X�Ŷ��AImage_Freq�٨S�����e��

	//���׹B�⪺��
	Image_Phase = cvCreateImage(cvGetSize(Image), IPL_DEPTH_64F, 1); 


	cvConvert(Image, Image_Re);


	//�U���o������i���A�]���곡�@�}�l�N�O�s�A�N���Image_Re�g�LcvMerge()�A�٬O���Ӫ��Ϥ���T�@�ˡA���o�Q�k�٨S�g�L�n�ոլ�
	cvSetZero(Image_Im); //��l�Ƶ곡
	cvMerge(Image_Re, Image_Im, 0, 0, Image_Freq ); //��Ϫ���T�w�Q�����곡�P�곡�A�i�ǳƶi��DFT���ާ@�A���L���ɧC�W�b�Ϥ��������A���W�b�Ϥ����|�Ө����A�ӥ���ɰ�F64���Ϥ���T���W(-1)^(x+y)�A�p���@�ӭt�C�ഫ�᪺�W���T�]�|��۶i�業���A�ϱo���W���ʨ�Ϥ�����


	// �N���W�T��������Ϥ�����
	for(int i=0;i<Image->height;i++)
	{
		for(int j=0;j<Image->width;j++)
		{
			CV_IMAGE_ELEM(Image_Freq, double,i,j*2)=pow(-1.0,i+j)*CV_IMAGE_ELEM(Image_Freq, double, i, j*2);
		}
	}


	cvDFT(Image_Freq, Image_Freq, CV_DXT_FORWARD);
	

	//�⨤�פ�k�@�G���ݭn�q�L���ΡA�����q�Ƽƥh�ާ@�A���\
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


//��|F(u,v)|�PF(u,v)������
//�쥻��Image_Freq�O�@�����q�D���ϡA�Ĥ@�q�D�O�곡�A�ĤG�q�D�O�곡�A|F(u,v)| = sqrt(Re^2 + Im^2)�A�n��곡�@�i�ϡA�곡�@�i��


	//���Φ��곡�곡
	cvSplit(Image_Freq, Image_Re, Image_Im,0,0);

	//�⨤�פ�k�G�G�N�ƼƤ��Φ��곡�B�곡
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



	// �p��log(1 + Mag)
	cvPow(Image_Re, Image_Re, 2.0);  //  Re^2
	cvPow(Image_Im, Image_Im, 2.0);  //  Im^2
	cvAdd(Image_Re, Image_Im, Image_Re);  // Re^2 + Im^2
	cvPow(Image_Re, Image_Re, 0.5);  // |F(u,v)| = sqrt(Re^2 + Im^2)
	
	cvAddS(Image_Re, cvScalar(1), Image_Re);  // 1 + Mag
	cvLog(Image_Re, Image_Re);  // log(1 + Mag)



	//�L�Xlog(1 + Mag)�A�L�X�����G�Ҥj��0�A�ҥH���W�e0~255�ɡA�������ƭ����ӭn�(�̤p��*255)
	/*for(int i=0;i<Image->height;i++)
	{
		for(int j=0;j<Image->width;j++)
		{
			cout<<CV_IMAGE_ELEM(Image_Re, double,i,j*2)<<endl;
		}
	}*/



	// Normalize to 0~255
	double MinValue=0, MaxValue=0, Scale=0, Shift=0;
	cvMinMaxLoc(Image_Re, &MinValue, &MaxValue);  // ���X�̤j�ȻP�̤p��

	/*cout<<MinValue<<endl;
	cout<<MaxValue<<endl;*/

	Scale = 255/(MaxValue-MinValue);// ��쥻�Ϥ��̪���T��ҤơA�í��W255�Ϥ�����U8���榡�AU8: 0~255
	Shift = -MinValue*Scale;  //�b�Ϥ���T��F64�ɡA���F�ƾǹB�� log(1 + Mag)�A�]���u1 + Mag�v�̭��o��+1���ʧ@�A�ϱo�Ϥ���T��^U8�ɥi��|�W�X0~255�A�ҥH�~�n��Shift���ʧ@�Ӧ����쥻�[�X�h���A�ϹϤ���T���d������b0~255


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
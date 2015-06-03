#include <opencv\cv.h>
#include <opencv\cxcore.h>
#include <opencv\highgui.h>

using namespace std;

void Image_RGB_to_HSI(IplImage* image, IplImage *Image_H, IplImage *Image_S, IplImage *Image_I);
void Image_HSI_to_RGB(IplImage *Image_H, IplImage *Image_S, IplImage *Image_I, IplImage *dst_Image);
void RotateH_and_MultiplyS(IplImage *HSIimg_H, IplImage *HSIimg_S, IplImage *HSIimg_I, double theta, double multiple);

void main()
{
/*Strawberry.bmp*/
	IplImage* image0 = cvLoadImage("Strawberry.bmp",-1); //input
	IplImage *dst_Image0 = cvCreateImage(cvGetSize(image0),IPL_DEPTH_8U,3);

	// HSI (double)
	IplImage *HSIimg_H0, *HSIimg_S0, *HSIimg_I0;
	HSIimg_H0 = cvCreateImage(cvGetSize(image0),IPL_DEPTH_64F,1); // 將運算過後double型別的H, S, I放入圖片裡，所以創建圖片時要用IPL_DEPTH_64F (0.0--1.0之間)，最後再透過cvConvertScale轉成可輸出的圖片格式 (IPL_DEPTH_8U)
	HSIimg_S0 = cvCreateImage(cvGetSize(image0),IPL_DEPTH_64F,1); 
	HSIimg_I0 = cvCreateImage(cvGetSize(image0),IPL_DEPTH_64F,1); 

	Image_RGB_to_HSI(image0, HSIimg_H0, HSIimg_S0, HSIimg_I0); 

/*Renoir*/
	IplImage* image = cvLoadImage("Renoir.bmp",-1); //input
	IplImage *dst_Image = cvCreateImage(cvGetSize(image),IPL_DEPTH_8U,3); //output

	// HSI (double)
	IplImage *HSIimg_H, *HSIimg_S, *HSIimg_I;
	HSIimg_H = cvCreateImage(cvGetSize(image),IPL_DEPTH_64F,1); // 將運算過後double型別的H, S, I放入圖片裡，所以創建圖片時要用IPL_DEPTH_64F (0.0--1.0之間)，最後再透過cvConvertScale轉成可輸出的圖片格式 (IPL_DEPTH_8U)
	HSIimg_S = cvCreateImage(cvGetSize(image),IPL_DEPTH_64F,1); 
	HSIimg_I = cvCreateImage(cvGetSize(image),IPL_DEPTH_64F,1); 
	
	Image_RGB_to_HSI(image, HSIimg_H, HSIimg_S, HSIimg_I); 
	Image_HSI_to_RGB(HSIimg_H, HSIimg_S, HSIimg_I, dst_Image);

	RotateH_and_MultiplyS(HSIimg_H, HSIimg_S, HSIimg_I, 90,1);  //H rotate 90
	Image_HSI_to_RGB(HSIimg_H, HSIimg_S, HSIimg_I, dst_Image);
	
	RotateH_and_MultiplyS(HSIimg_H, HSIimg_S, HSIimg_I, 90,1); //H rotate 180 (90+90=180
	Image_HSI_to_RGB(HSIimg_H, HSIimg_S, HSIimg_I, dst_Image);

	RotateH_and_MultiplyS(HSIimg_H, HSIimg_S, HSIimg_I, 90,1); //H rotate 270 (180+90=270
	Image_HSI_to_RGB(HSIimg_H, HSIimg_S, HSIimg_I, dst_Image);

	RotateH_and_MultiplyS(HSIimg_H, HSIimg_S, HSIimg_I, -270,0.25); //H 歸零，S*0.25
	Image_HSI_to_RGB(HSIimg_H, HSIimg_S, HSIimg_I, dst_Image);

	RotateH_and_MultiplyS(HSIimg_H, HSIimg_S, HSIimg_I, 0,(0.5/0.25)); // (S*0.25)*(0.5/0.25)
	Image_HSI_to_RGB(HSIimg_H, HSIimg_S, HSIimg_I, dst_Image);

	RotateH_and_MultiplyS(HSIimg_H, HSIimg_S, HSIimg_I, 0,(0.75/0.5)); // (S*0.5)*(0.75/0.5)
	Image_HSI_to_RGB(HSIimg_H, HSIimg_S, HSIimg_I, dst_Image);


/*Kodak_Q60*/
	IplImage* image2 = cvLoadImage("Kodak_Q60.bmp",-1); //input
	IplImage *dst_Image2 = cvCreateImage(cvGetSize(image2),IPL_DEPTH_8U,3); //output
	IplImage *HSIimg_H2, *HSIimg_S2, *HSIimg_I2;  // HSI (double)

	HSIimg_H2 = cvCreateImage(cvGetSize(image2),IPL_DEPTH_64F,1); // 將運算過後double型別的H, S, I放入圖片裡，所以創建圖片時要用IPL_DEPTH_64F (0.0--1.0之間)，最後再透過cvConvertScale轉成可輸出的圖片格式 (IPL_DEPTH_8U)
	HSIimg_S2 = cvCreateImage(cvGetSize(image2),IPL_DEPTH_64F,1); 
	HSIimg_I2 = cvCreateImage(cvGetSize(image2),IPL_DEPTH_64F,1);

	Image_RGB_to_HSI(image2, HSIimg_H2, HSIimg_S2, HSIimg_I2);
	Image_HSI_to_RGB(HSIimg_H2, HSIimg_S2, HSIimg_I2, dst_Image2);

	RotateH_and_MultiplyS(HSIimg_H2, HSIimg_S2, HSIimg_I2, 90,1);  //H rotate 90
	Image_HSI_to_RGB(HSIimg_H2, HSIimg_S2, HSIimg_I2, dst_Image2);
	
	RotateH_and_MultiplyS(HSIimg_H2, HSIimg_S2, HSIimg_I2, 90,1); //H rotate 180 (90+90=180
	Image_HSI_to_RGB(HSIimg_H2, HSIimg_S2, HSIimg_I2, dst_Image2);

	RotateH_and_MultiplyS(HSIimg_H2, HSIimg_S2, HSIimg_I2, 90,1); //H rotate 270 (180+90=270
	Image_HSI_to_RGB(HSIimg_H2, HSIimg_S2, HSIimg_I2, dst_Image2);

	RotateH_and_MultiplyS(HSIimg_H2, HSIimg_S2, HSIimg_I2, -270,0.25); //H 歸零，S*0.25
	Image_HSI_to_RGB(HSIimg_H2, HSIimg_S2, HSIimg_I2, dst_Image2);

	RotateH_and_MultiplyS(HSIimg_H2, HSIimg_S2, HSIimg_I2, 0,(0.5/0.25)); // (S*0.25)*(0.5/0.25)
	Image_HSI_to_RGB(HSIimg_H2, HSIimg_S2, HSIimg_I2, dst_Image2);

	RotateH_and_MultiplyS(HSIimg_H2, HSIimg_S2, HSIimg_I2, 0,(0.75/0.5)); // (S*0.5)*(0.75/0.5)
	Image_HSI_to_RGB(HSIimg_H2, HSIimg_S2, HSIimg_I2, dst_Image2);

}


void Image_RGB_to_HSI(IplImage* image, IplImage *HSIimg_H, IplImage *HSIimg_S, IplImage *HSIimg_I)
{
	// RGB
	uchar* data = (uchar*)image->imageData;
	int step = image->widthStep/sizeof(uchar); 
	int channels = image->nChannels;


	// HSI (double)
	double *data_H, *data_S, *data_I;
	data_H = (double*)HSIimg_H->imageData;
	data_S = (double*)HSIimg_S->imageData;
	data_I = (double*)HSIimg_I->imageData;
	int step_HSI = HSIimg_H->widthStep/sizeof(double);  


	// HSI (uchar)
	IplImage *Image_H, *Image_S, *Image_I;
	Image_H = cvCreateImage(cvGetSize(image),IPL_DEPTH_8U,1);
	Image_S = cvCreateImage(cvGetSize(image),IPL_DEPTH_8U,1);
	Image_I = cvCreateImage(cvGetSize(image),IPL_DEPTH_8U,1);


	//訪問圖片像素
	double b, g, r, H, S, I, num, den, x, theta, min_rgb;
	for(int i=0 ;i<image->height;i++)
		for(int j=0;j<image->width;j++)
		{
			/*讀取RGB圖片資訊並正規化到0~1之間*/
			b=(double) data[i*step+j*channels+0]/255;
			g=(double) data[i*step+j*channels+1]/255;
			r=(double) data[i*step+j*channels+2]/255;

			if(b==g && g==r)
			{
				H=-1.0; //寫0.0僅是提醒H, S, I為double型別
				S=0.0;
				I=(1.0/3)*(r+g+b);
				//cout<<I<<endl;
				data_H[i*step_HSI+j*1+0]=H;
				data_S[i*step_HSI+j*1+0]=S;
				data_I[i*step_HSI+j*1+0]=I;
			}
			else
			{
				num = 0.5*((r-g)+(r-b)); // numerator 分子
				den =sqrt((r-g)*(r-g)+(r-b)*(g-b)); //denominator 分母
				x= num/den;
			
				if(x<-1.0) 
					x=-1.0; 
				if(x>1.0)  
					x=1.0; 

				theta = (180.0/CV_PI)*acos(x);
				//cout<<theta<<endl;
				if(b<=g)
					data_H[i*step_HSI+j*1+0]=theta;
				else
					data_H[i*step_HSI+j*1+0]=360.0-theta;
				//cout<<(float)data_H[i*step_HSI+j*1+0]<<endl;

				min_rgb = __min(__min(r,g),b);
				S=1.0-(3.0/(r+g+b))*min_rgb;
				I=(1.0/3)*(r+g+b);
				//cout<<S<<endl;
				data_S[i*step_HSI+j*1+0]=S;
				data_I[i*step_HSI+j*1+0]=I;
				//cout<<(double)data_S[i*step_HSI+j*1+0]<<endl;
			}
		}

	// H=0~360 (H*255/360)，S,I=0~1 (S*255, I*255)，double轉成uchar
	cvConvertScale(HSIimg_H, Image_H, 255.0/360, 0);
	cvConvertScale(HSIimg_S, Image_S, 255.0, 0);
	cvConvertScale(HSIimg_I, Image_I, 255.0, 0);

	cvNamedWindow("HSI Image_H");
	cvNamedWindow("HSI Image_S");
	cvNamedWindow("HSI Image_I");
	
	cvShowImage("HSI Image_H", Image_H);
	cvShowImage("HSI Image_S", Image_S);
	cvShowImage("HSI Image_I", Image_I);
	
	cvWaitKey(0);

	cvDestroyWindow("HSI Image_H");
	cvDestroyWindow("HSI Image_S");
	cvDestroyWindow("HSI Image_I");

	cvReleaseImage(&image);
}

void Image_HSI_to_RGB(IplImage *HSIimg_H, IplImage *HSIimg_S, IplImage *HSIimg_I, IplImage *dst_Image)
{
	double  *dataH, *dataS, *dataI;
	dataH = (double*)HSIimg_H->imageData; // 單通
	dataS = (double*)HSIimg_S->imageData;
	dataI = (double*)HSIimg_I->imageData;
	int step = HSIimg_H->widthStep/sizeof(double); 
	int channels = HSIimg_H->nChannels;


	//R, G, B (單通
	IplImage *B = cvCreateImage(cvGetSize(HSIimg_H),IPL_DEPTH_64F,1);
	IplImage *G = cvCloneImage(B);
	IplImage *R = cvCloneImage(B);

	double *dataB, *dataG, *dataR;
	dataB = (double*)B->imageData;
	dataG = (double*)G->imageData;
	dataR = (double*)R->imageData;
	int step_RGB = B->widthStep/sizeof(double);
	int channels_RGB = B->nChannels;

	double b, g, r, H, S, I, num, den;
	for(int i=0 ;i<HSIimg_H->height;i++)
		for(int j=0;j<HSIimg_H->width;j++)
		{
			H= dataH[i*step+j*channels];
			S= dataS[i*step+j*channels];
			I= dataI[i*step+j*channels];
			//cout<<I<<endl;

			if(H==-1.0)
			{
				r=I;
				g=I;
				b=I;

				dataB[i*step_RGB+j*channels_RGB] = b;
				dataG[i*step_RGB+j*channels_RGB] = g;
				dataR[i*step_RGB+j*channels_RGB] = r;
				//cout<<(double)dataB[i*step_RGB+j*channels_RGB]<<endl;
				//cout<<b<<endl;
			}
			else if(H>0.0 && H<120.0)
			{
				b = I*(1-S);
				num = S*cos(H*(CV_PI/180));    // 分子
				den = cos((60-H)*(CV_PI/180)); // 分母
				r = I*(1+(num/den));
				g = 3*I-(r+b);
				
				dataB[i*step_RGB+j*channels_RGB] = b;
				dataG[i*step_RGB+j*channels_RGB] = g; 
				dataR[i*step_RGB+j*channels_RGB] = r; 
				//cout<<(double)dataB[i*step_RGB+j*channels_RGB]<<endl;
			}
			else if(H>=120 && H<240)
			{
				H=H-120;
				r=I*(1-S);
				num = S*cos(H*(CV_PI/180));    // 分子
				den = cos((60-H)*(CV_PI/180)); // 分母
				g=I*(1+(num/den));
				b=3*I-(r+g);

				dataB[i*step_RGB+j*channels_RGB] = b;
				dataG[i*step_RGB+j*channels_RGB] = g; 
				dataR[i*step_RGB+j*channels_RGB] = r; 
				//cout<<(double)data_BGR[i*step_RGB+j*channels_RGB+2]<<endl;
			}
			else
			{
				H=H-240;
				g=I*(1-S);
				num = S*cos(H*(CV_PI/180));    // 分子
				den = cos((60-H)*(CV_PI/180)); // 分母
				b=I*(1+(num/den));
				r=3*I-(g+b);

				dataB[i*step_RGB+j*channels_RGB] = b;
				dataG[i*step_RGB+j*channels_RGB] = g; 
				dataR[i*step_RGB+j*channels_RGB] = r; 
				//cout<<(double)data_BGR[i*step_RGB+j*channels_RGB+0]<<endl;
			}
		}
	
	////64F 測試成功 ( 雖然滿奇怪的
	//IplImage *Bimg = cvCreateImage(cvGetSize(dst_Image),IPL_DEPTH_64F,3);
	//IplImage *Gimg = cvCloneImage(Bimg);
	//IplImage *Rimg = cvCloneImage(Bimg);
	//IplImage *RGBimg = cvCreateImage(cvGetSize(dst_Image),IPL_DEPTH_64F,3);

	//cvMerge(B,0,0,0, Bimg);
	//cvMerge(0,G,0,0, Gimg);
	//cvMerge(0,0,R,0, Rimg);
	//cvMerge(B, G, R,0, RGBimg);
	//
	//cvNamedWindow("(64F)B Image");
	//cvShowImage("(64F)B Image", Bimg);
	//cvNamedWindow("(64F)G Image");
	//cvShowImage("(64F)G Image", Gimg);
	//cvNamedWindow("(64F)R Image");
	//cvShowImage("(64F)R Image", Rimg);
	//cvNamedWindow("(64F)RGB Image");
	//cvShowImage("(64F)RGB Image", RGBimg);


	// 8U
	IplImage *Bimg = cvCreateImage(cvGetSize(dst_Image),IPL_DEPTH_8U,1);
	IplImage *Gimg = cvCloneImage(Bimg);
	IplImage *Rimg = cvCloneImage(Bimg);
	
	cvConvertScale(B,Bimg, 255.0, 0);
	cvConvertScale(G,Gimg, 255.0, 0);
	cvConvertScale(R,Rimg, 255.0, 0);
	
	cvMerge(Bimg, Gimg, Rimg,0, dst_Image);

	cvNamedWindow("(8U)RGB Image");
	cvShowImage("(8U)RGB Image", dst_Image);

	//// R, G, B單通道各自show
	//IplImage *B3img = cvCreateImage(cvGetSize(dst_Image),IPL_DEPTH_8U,3);
	//IplImage *G3img = cvCloneImage(B3img);
	//IplImage *R3img = cvCloneImage(B3img);

	//cvMerge(Bimg, 0, 0,0, B3img);
	//cvMerge(0, Gimg, 0,0, G3img);
	//cvMerge(0, 0, Rimg,0, R3img);

	//cvNamedWindow("(8U)B Image");
	//cvShowImage("(8U)B Image", B3img);
	//cvNamedWindow("(8U)G Image");
	//cvShowImage("(8U)G Image", G3img);
	//cvNamedWindow("(8U)R Image");
	//cvShowImage("(8U)R Image", R3img);

	cvWaitKey(0);
	
	cvDestroyWindow("(8U)B Image");
	cvDestroyWindow("(8U)G Image");
	cvDestroyWindow("(8U)R Image");
	cvDestroyWindow("(8U)RGB Image");

}

void RotateH_and_MultiplyS(IplImage *HSIimg_H, IplImage *HSIimg_S, IplImage *HSIimg_I, double theta, double multiple)
{
	// HSI (double)
	double *data_H, *data_S, *data_I;
	data_H = (double*)HSIimg_H->imageData;
	data_S = (double*)HSIimg_S->imageData;
	data_I = (double*)HSIimg_I->imageData;
	int step_HSI = HSIimg_H->widthStep/sizeof(double);  

	double H, S, I;
	for(int i=0 ;i<HSIimg_H->height;i++)
		for(int j=0;j<HSIimg_H->width;j++)
		{
			data_H[i*step_HSI+j*1+0]=data_H[i*step_HSI+j*1]+theta;
			data_S[i*step_HSI+j*1+0]=data_S[i*step_HSI+j*1]*multiple;
			data_I[i*step_HSI+j*1+0]=data_I[i*step_HSI+j*1];
		}
}

void RGB_to_HSI(unsigned char R, unsigned char G, unsigned char B, double &H, double &S, double &I)
{
	double b, g, r,  num, den, x, theta, min_rgb;
	//double	H, S, I;
	
	
	b=(double) R/255;
	g=(double) G/255;
	r=(double) B/255;

	if(b==g && g==r)
			{
				H=-1.0; 
				S=0.0;
				I=(1.0/3)*(r+g+b);
			}
	else
			{
				num = 0.5*((r-g)+(r-b)); // numerator 分子
				den =sqrt((r-g)*(r-g)+(r-b)*(g-b)); //denominator 分母
				x= num/den;
			
				if(x<-1.0) 
					x=-1.0; 
				if(x>1.0)  
					x=1.0; 

				theta = (180.0/CV_PI)*acos(x);
				//cout<<theta<<endl;
				if(b<=g)
					H=theta;
				else
					H=360.0-theta;
				
				min_rgb = __min(__min(r,g),b);
				S=1.0-(3.0/(r+g+b))*min_rgb;
				I=(1.0/3)*(r+g+b);
				//cout<<S<<endl;
			}
	H=H*(255/360);
	S=S*255;
	I=I*255;
}

void HSI_to_RGB(double H, double S, double I, uchar &R, uchar &G, uchar &B)
{
	double b, g, r, num, den;
	if(H==-1.0)
			{
				r=I;
				g=I;
				b=I;
			}
			else if(H>0.0 && H<120.0)
			{
				b = I*(1-S);
				num = S*cos(H*(CV_PI/180));    // 分子
				den = cos((60-H)*(CV_PI/180)); // 分母
				r = I*(1+(num/den));
				g = 3*I-(r+b);
			}
			else if(H>=120 && H<240)
			{
				H=H-120;
				r=I*(1-S);
				num = S*cos(H*(CV_PI/180));    // 分子
				den = cos((60-H)*(CV_PI/180)); // 分母
				g=I*(1+(num/den));
				b=3*I-(r+g);
			}
			else
			{
				H=H-240;
				g=I*(1-S);
				num = S*cos(H*(CV_PI/180));    // 分子
				den = cos((60-H)*(CV_PI/180)); // 分母
				b=I*(1+(num/den));
				r=3*I-(g+b);
			}

		int rr, gg, bb;
		rr = cvRound(255*r);
		gg = cvRound(255*g);
		bb = cvRound(255*b);

		if(rr<0)
		{
			rr=0;
		}
		else if(rr > 255)
		{
			rr=255;
		}
		else if(gg<0)
		{
			gg=0;
		}
		else if(gg>255)
		{
			gg=255;
		}
		else if(bb<0)
		{
			bb=0;
		}
		else if(bb>255)
		{
			bb=255;
		}


		R = (uchar)rr;
		G = (uchar)gg;
		B = (uchar)bb;

}
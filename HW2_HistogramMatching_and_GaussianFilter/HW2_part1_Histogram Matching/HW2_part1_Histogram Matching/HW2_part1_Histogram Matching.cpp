#include <opencv\cv.h>
#include<opencv\highgui.h>
# include <iostream>
#include <stdio.h>



int HistogramBins =256;
float HistogramRange1[2]={0,255};
float *HistogramRange[1]={&HistogramRange1[0]};

//Part.1
void HistogramMatching(int HistogramBins,  
	IplImage *src_Image, IplImage *dst_Image, 
	CvHistogram *source_Histogram, CvHistogram *specified_Histogram, IplImage *EqualizeHistogramImage);

//Other
void ReleaseStorageSpace(IplImage *src_Image, IplImage *dst_Image, 
	IplImage *OriginalHistogramImage, IplImage *EqualizeHistogramImage);
void DrawHistogramImage(CvHistogram *Histogram, IplImage *HistogramImage, int HistogramBin);
void ShowImage(IplImage *src_Image, IplImage *dst_Image, 
	IplImage *OriginalHistogramImage,IplImage *EqualizeHistogramImage);
void ShowImage2(IplImage *src_Image, IplImage *dst_Image);
void ReleaseStorageSpace(IplImage *src_Image, IplImage *dst_Image, 
	IplImage OriginalHistogramImage, IplImage EqualizeHistogramImage);

int main()
{
	


/*** Part.1 ***/

/* �Ĥ@�p�D�A��ϻP���G�P���w�Ϥ����n��ܪ���� */
	//Pollen1
	IplImage * Pollen1 =  cvLoadImage( "Pollen1.bmp", -1 );  //src
	IplImage * Pollen1_dst =  cvCreateImage(cvGetSize(Pollen1),IPL_DEPTH_8U,1); //dst
	CvHistogram * Histogram_Pollen1 = cvCreateHist( 1,&HistogramBins, CV_HIST_ARRAY, HistogramRange, 1 );
	CvHistogram * EqualizeHistogram_Pollen1 = cvCreateHist( 1, &HistogramBins, CV_HIST_ARRAY, HistogramRange, 1 );
	IplImage * HistogramImage_Pollen1 = cvCreateImage( cvSize(256,500), IPL_DEPTH_8U, 1 );
	IplImage * EqualizeHistogramImage_Pollen1 = cvCreateImage( cvSize(256,500), IPL_DEPTH_8U, 1 );
	HistogramImage_Pollen1->origin=1;			//�ഫ����Ϫ��y�Э��I�쥪�U��
	EqualizeHistogramImage_Pollen1->origin=1;
	cvCalcHist(&Pollen1,Histogram_Pollen1);		//�p�⪽��Ϩ���ܹϤ�
	DrawHistogramImage(Histogram_Pollen1,HistogramImage_Pollen1,HistogramBins);
	ShowImage(Pollen1, NULL, HistogramImage_Pollen1, NULL);

	//Pollen2
	IplImage * Pollen2 = cvLoadImage( "Pollen2.bmp", -1 );   
	IplImage * Pollen2_dst = cvCreateImage(cvGetSize(Pollen2),IPL_DEPTH_8U,1);
	CvHistogram * Histogram_Pollen2 = cvCreateHist( 1,&HistogramBins, CV_HIST_ARRAY, HistogramRange, 1 );
	CvHistogram * EqualizeHistogram_Pollen2 = cvCreateHist( 1, &HistogramBins, CV_HIST_ARRAY, HistogramRange, 1 );
	IplImage * HistogramImage_Pollen2 = cvCreateImage( cvSize(256,500), IPL_DEPTH_8U, 1 );
	IplImage * EqualizeHistogramImage_Pollen2 = cvCreateImage( cvSize(256,500), IPL_DEPTH_8U, 1 );
	HistogramImage_Pollen2->origin=1;
	EqualizeHistogramImage_Pollen2->origin=1;
	cvCalcHist(&Pollen2,Histogram_Pollen2);		
	DrawHistogramImage(Histogram_Pollen2,HistogramImage_Pollen2,HistogramBins);
	ShowImage(Pollen2, NULL, HistogramImage_Pollen2, NULL);

	//Goldhill�Aspecified image
	IplImage * Goldhill = cvLoadImage( "Goldhill.bmp", -1 ); //specified
	CvHistogram * Histogram_Goldhill = cvCreateHist( 1,&HistogramBins, CV_HIST_ARRAY, HistogramRange, 1 );
	IplImage * HistogramImage_Goldhill = cvCreateImage( cvSize(256,500), IPL_DEPTH_8U, 1 );
	HistogramImage_Goldhill->origin=1;
	cvCalcHist(&Goldhill,Histogram_Goldhill);
	DrawHistogramImage(Histogram_Goldhill,HistogramImage_Goldhill,HistogramBins);
	ShowImage(Goldhill, NULL, HistogramImage_Goldhill, NULL);

	//Pollen1 be processed by histogram matching
	cvCalcHist(&Pollen1,Histogram_Pollen1);
	DrawHistogramImage(Histogram_Pollen1,HistogramImage_Pollen1,HistogramBins);
	HistogramMatching(HistogramBins, Pollen1, Pollen1_dst, 
		 Histogram_Pollen1,Histogram_Goldhill, EqualizeHistogramImage_Pollen1);  
	ShowImage(Pollen1, Pollen1_dst, HistogramImage_Pollen1, EqualizeHistogramImage_Pollen1);

	//Pollen2 be processed by histogram matching
	cvCalcHist(&Pollen2,Histogram_Pollen2);
	DrawHistogramImage(Histogram_Pollen2,HistogramImage_Pollen2,HistogramBins);
	HistogramMatching(HistogramBins, Pollen2, Pollen2_dst, 
		 Histogram_Pollen2,Histogram_Goldhill, EqualizeHistogramImage_Pollen2);
	ShowImage(Pollen2, Pollen2_dst, HistogramImage_Pollen2, EqualizeHistogramImage_Pollen2);

	ReleaseStorageSpace(Pollen1, Pollen1_dst, HistogramImage_Pollen1, EqualizeHistogramImage_Pollen1);
	ReleaseStorageSpace(Pollen2, Pollen2_dst, HistogramImage_Pollen2, EqualizeHistogramImage_Pollen2);

	cvDestroyWindow("Original Image");
	cvDestroyWindow("Processed Image");
	cvDestroyWindow("Original Histogram");
	cvDestroyWindow("Processed Histogram");


/* �ĤG�p�D�A������ܭ�ϻP���G������� */
	//Jenny
	IplImage * Jenny = cvLoadImage( "Jenny.bmp", -1 );	//src
	IplImage * Jenny_Blue = cvCreateImage( cvGetSize( Jenny ), 8, 1 ); //src
	IplImage * Jenny_Green = cvCreateImage( cvGetSize( Jenny ), 8, 1 );
	IplImage * Jenny_Red = cvCreateImage( cvGetSize( Jenny ), 8, 1 );
	cvSplit( Jenny, Jenny_Blue, Jenny_Green, Jenny_Red, 0 );	//��T�q�D����Ϥ��Φ���q�D

	CvHistogram *Histogram_Jenny_Blue = cvCreateHist( 1,&HistogramBins, CV_HIST_ARRAY, HistogramRange, 1 );
	CvHistogram *Histogram_Jenny_Green = cvCreateHist( 1,&HistogramBins, CV_HIST_ARRAY, HistogramRange, 1 );
	CvHistogram *Histogram_Jenny_Red = cvCreateHist( 1,&HistogramBins, CV_HIST_ARRAY, HistogramRange, 1 );
	cvCalcHist(&Jenny_Blue, Histogram_Jenny_Blue);
	cvCalcHist(&Jenny_Green, Histogram_Jenny_Green);
	cvCalcHist(&Jenny_Red, Histogram_Jenny_Red);

	//Jenny_dst
	IplImage * Jenny_Blue_dst = cvCreateImage( cvGetSize( Jenny ), 8, 1 );
	IplImage * Jenny_Green_dst = cvCreateImage( cvGetSize( Jenny ), 8, 1 );
	IplImage * Jenny_Red_dst = cvCreateImage( cvGetSize( Jenny ), 8, 1 );
	IplImage * Jenny_dst = cvCreateImage( cvGetSize( Jenny ), 8, 3 );  //�`�N

//Train�Aspecified piccture1
	IplImage * Train = cvLoadImage( "Train.bmp", -1); //specified
	CvHistogram *Histogram_Train = cvCreateHist( 1,&HistogramBins, CV_HIST_ARRAY, HistogramRange, 1 );
	
	//IplImage *HistogramImage_Train_B = cvCreateImage( cvSize(256,500), IPL_DEPTH_8U, 1 );
	//IplImage *HistogramImage_Train_G = cvCreateImage( cvSize(256,500), IPL_DEPTH_8U, 1 );
	//IplImage *HistogramImage_Train_R = cvCreateImage( cvSize(256,500), IPL_DEPTH_8U, 1 );
	//IplImage *HistogramImage_Train = cvCreateImage( cvSize(256,500), IPL_DEPTH_8U, 3 );

	//����Ϫ��p��u��γ�q�D���A�ҥH�ڦp�G������Train.bmp�o�i�m�⪺��(�T�q�D)��JcvCalcHist{}�N�|�L�kŪ�A���ӧ�쥻�T�q�D���Ϥ��Φ�B�BG�BR�~���J
	//cvCalcHist(&Train, Histogram_Train);
	
	//�p��XTrain�����
	IplImage * Train_Blue = cvCreateImage( cvGetSize( Train ), 8, 1 );
	IplImage * Train_Green = cvCreateImage( cvGetSize( Train ), 8, 1 );
	IplImage * Train_Red = cvCreateImage( cvGetSize( Train ), 8, 1 );
	cvSplit(Train, Train_Blue, Train_Green, Train_Red, 0);

	CvHistogram *Histogram_Train_Blue = cvCreateHist( 1,&HistogramBins, CV_HIST_ARRAY, HistogramRange, 1 );
	CvHistogram *Histogram_Train_Green = cvCreateHist( 1,&HistogramBins, CV_HIST_ARRAY, HistogramRange, 1 );
	CvHistogram *Histogram_Train_Red = cvCreateHist( 1,&HistogramBins, CV_HIST_ARRAY, HistogramRange, 1 );

	cvCalcHist(&Train_Blue, Histogram_Train_Blue);
	cvCalcHist(&Train_Green, Histogram_Train_Green);
	cvCalcHist(&Train_Red, Histogram_Train_Red);

	//Histogram Match: processed by Train
	HistogramMatching(HistogramBins, Jenny_Blue, Jenny_Blue_dst, Histogram_Jenny_Blue, Histogram_Train_Blue, NULL);
	HistogramMatching(HistogramBins, Jenny_Green, Jenny_Green_dst, Histogram_Jenny_Green, Histogram_Train_Green, NULL);
	HistogramMatching(HistogramBins, Jenny_Red, Jenny_Red_dst, Histogram_Jenny_Red, Histogram_Train_Red, NULL);

	cvMerge(Jenny_Blue_dst, Jenny_Green_dst, Jenny_Red_dst, 0, Jenny_dst);
	ShowImage2(Jenny, Jenny_dst);

// prodessed by Train.  END
	
//Gollum�Aspecified piccture2
	IplImage * Gollum = cvLoadImage( "Gollum.bmp", -1); //specified
	CvHistogram *Histogram_Gollum = cvCreateHist( 1,&HistogramBins, CV_HIST_ARRAY, HistogramRange, 1 );
	
	//�p��XGollum�����
	IplImage * Gollum_Blue = cvCreateImage( cvGetSize( Gollum ), 8, 1 );
	IplImage * Gollum_Green = cvCreateImage( cvGetSize( Gollum ), 8, 1 );
	IplImage * Gollum_Red = cvCreateImage( cvGetSize( Gollum ), 8, 1 );
	cvSplit(Gollum, Gollum_Blue, Gollum_Green, Gollum_Red, 0);

	CvHistogram *Histogram_Gollum_Blue = cvCreateHist( 1,&HistogramBins, CV_HIST_ARRAY, HistogramRange, 1 );
	CvHistogram *Histogram_Gollum_Green = cvCreateHist( 1,&HistogramBins, CV_HIST_ARRAY, HistogramRange, 1 );
	CvHistogram *Histogram_Gollum_Red = cvCreateHist( 1,&HistogramBins, CV_HIST_ARRAY, HistogramRange, 1 );

	cvCalcHist(&Gollum_Blue, Histogram_Gollum_Blue);
	cvCalcHist(&Gollum_Green, Histogram_Gollum_Green);
	cvCalcHist(&Gollum_Red, Histogram_Gollum_Red);

	//Histogram Match: processed by Train
	HistogramMatching(HistogramBins, Jenny_Blue, Jenny_Blue_dst, Histogram_Jenny_Blue, Histogram_Gollum_Blue, NULL);
	HistogramMatching(HistogramBins, Jenny_Green, Jenny_Green_dst, Histogram_Jenny_Green, Histogram_Gollum_Green, NULL);
	HistogramMatching(HistogramBins, Jenny_Red, Jenny_Red_dst, Histogram_Jenny_Red, Histogram_Gollum_Red, NULL);

	cvMerge(Jenny_Blue_dst, Jenny_Green_dst, Jenny_Red_dst, 0, Jenny_dst);
	ShowImage2(Jenny, Jenny_dst);


// prodessed by Gollum.  END

//Goldhill�Aspecified piccture3
	//�Ĥ@�p�D�w�g�w�q�L�F
	//IplImage *Goldhill = cvLoadImage( "Goldhill.bmp", -1); //specified
	//CvHistogram *Histogram_Goldhill = cvCreateHist( 1,&HistogramBins, CV_HIST_ARRAY, HistogramRange, 1 );
	
	//�p��XGollum�����
	CvHistogram *Histogram_Goldhill_Blue = cvCreateHist( 1,&HistogramBins, CV_HIST_ARRAY, HistogramRange, 1 );
	CvHistogram *Histogram_Goldhill_Green = cvCreateHist( 1,&HistogramBins, CV_HIST_ARRAY, HistogramRange, 1 );
	CvHistogram *Histogram_Goldhill_Red = cvCreateHist( 1,&HistogramBins, CV_HIST_ARRAY, HistogramRange, 1 );
	
	cvCalcHist(&Goldhill, Histogram_Goldhill_Blue);
	cvCalcHist(&Goldhill, Histogram_Goldhill_Green);
	cvCalcHist(&Goldhill, Histogram_Goldhill_Red);

	//Histogram Match: processed by Gollum
	HistogramMatching(HistogramBins, Jenny_Blue, Jenny_Blue_dst, Histogram_Jenny_Blue, Histogram_Goldhill_Blue, NULL);
	HistogramMatching(HistogramBins, Jenny_Green, Jenny_Green_dst, Histogram_Jenny_Green, Histogram_Goldhill_Green, NULL);
	HistogramMatching(HistogramBins, Jenny_Red, Jenny_Red_dst, Histogram_Jenny_Red, Histogram_Goldhill_Red, NULL);

	cvMerge(Jenny_Blue_dst, Jenny_Green_dst, Jenny_Red_dst, 0, Jenny_dst);
	ShowImage2(Jenny, Jenny_dst);

/*** Part.2 ***/

}



void DrawHistogramImage(CvHistogram *Histogram, IplImage *HistogramImage, int HistogramBin)
{
/*	// �ĪG���n
	CvPoint Point1;
    CvPoint Point2;

    for(int i=0;i<HistogramBins;i++)
    {
        Point1=cvPoint(i,(int)cvQueryHistValue_1D(Histogram,i));
        Point2=cvPoint(i,0);
        cvLine(HistogramImage,Point1,Point2,CV_RGB(127,127,127));

    }
*/

	for(int i = 0; i < HistogramBins; i++ )
	{
		CvPoint Point1 = cvPoint( i, 0 );
		CvPoint Point2 = cvPoint( i , (int)cvQueryHistValue_1D( Histogram, i )/60 );
		cvLine( HistogramImage, Point1, Point2, CV_RGB(127,127,127), 4, 8, 0 );
	}
}

void HistogramMatching(int HistogramBins,  
	IplImage *src_Image, IplImage *dst_Image, 
	CvHistogram *source_Histogram, CvHistogram *specified_Histogram, IplImage *EqualizeHistogramImage)
{
	float src_CDF=0, specified_CDF=0;
	float CDFArray_src[256], CDFArray_specified[256];
	uchar LookupTableData[256] ;
	CvMat LookupTableMatrix;
	
	//Probability Density Function (PDF) 
	cvNormalizeHist(source_Histogram,1);
	cvNormalizeHist(specified_Histogram,1);
	
	//Cumulative Distribution Function (CDF)
	for(int i=0;i<HistogramBins;i++)
	{
		// ���o��Ϫ�CDF
		src_CDF=src_CDF+cvQueryHistValue_1D(source_Histogram,i);
		CDFArray_src[i]=(float)cvRound(255*src_CDF); // �ҥ�����  s=T(r)=(L-1)*CDF
		
		// ���o���w�Ϫ�CDF
		specified_CDF=specified_CDF+cvQueryHistValue_1D(specified_Histogram,i);
		CDFArray_specified[i]=(float)cvRound(255*specified_CDF);

		//��i�ϳq�L s=T(r)�A���U�۪�s�A�{�b�i��ǰt
		int j=0; //���F��ǰt����A�令CDFArray_specified[j]

/*      
		// fail
		for(int i=0;i<HistogramBins;i++)
		{
			if(CDFArray_src[i]<=CDFArray_specified[j])
				LookupTableData[i]=j;
			else if(CDFArray_src[i]>CDFArray_specified[j])
				j++;
			else if( (CDFArray_specified[j]-CDFArray_src[i])>(CDFArray_src[i]-CDFArray_specified[j-1]) )
				LookupTableData[i]=j-1;
			else
				LookupTableData[i]=j;
		}
*/

		for( int i = 0; i < HistogramBins; i++ )
		{
			if( CDFArray_src[i]<=CDFArray_specified[j] ) 
				LookupTableData[i]=j;
			else
				{
				while( CDFArray_src[i]>CDFArray_specified[j] ) j++;
				if(  CDFArray_specified[j]-CDFArray_src[i]>CDFArray_src[i]-CDFArray_specified[j-1]  ) 
					LookupTableData[i]=j-1;
				else 
					LookupTableData[i]=j;
				}
		}

	}

	//���Ʃ�J�x�}�A���ͩҭn���Ϥ�
	//cvSetData(LookupTableMatrix,LookupTableData,CV_AUTOSTEP);  //�o�Ͱ��|�̨S�F��i�������D�A��]�ݬd
	LookupTableMatrix=cvMat(1, HistogramBins, CV_8UC1, LookupTableData);
	cvLUT(src_Image, dst_Image, &LookupTableMatrix); 

	//�p���ø�s�����
	cvCalcHist(&dst_Image,specified_Histogram);
	DrawHistogramImage(specified_Histogram,EqualizeHistogramImage,HistogramBins);

}



void ShowImage(IplImage *src_Image, IplImage *dst_Image, 
	IplImage *OriginalHistogramImage,IplImage *EqualizeHistogramImage)
{
	cvNamedWindow("Original Image",1);
	cvShowImage("Original Image",src_Image);
	cvNamedWindow("Processed Image",1);
	cvShowImage("Processed Image",dst_Image);
	cvNamedWindow("Original Histogram",1);
	cvShowImage("Original Histogram",OriginalHistogramImage);
	cvNamedWindow("Processed Histogram",1);
	cvShowImage("Processed Histogram",EqualizeHistogramImage);

	cvWaitKey(0);
/*
	cvDestroyWindow("Source Image");
	cvReleaseImage(&src_Image);
	cvDestroyWindow("Processed Image");
	cvReleaseImage(&dst_Image);
	cvDestroyWindow("Source Histogram");
	cvReleaseImage(&OriginalHistogramImage);
	cvDestroyWindow("Processed Histogram");
	cvReleaseImage(&EqualizeHistogramImage);
*/

}

void ShowImage2(IplImage *src_Image, IplImage *dst_Image)
{
	cvNamedWindow("Original Image",1);
	cvShowImage("Original Image",src_Image);
	cvNamedWindow("Processed Image",1);
	cvShowImage("Processed Image",dst_Image);
	

	cvWaitKey(0);

}

void ReleaseStorageSpace(IplImage *src_Image, IplImage *dst_Image, 
	IplImage *OriginalHistogramImage, IplImage *EqualizeHistogramImage)
{
	cvDestroyWindow("Source Image");
	cvReleaseImage(&src_Image);
	cvDestroyWindow("Processed Image");
	cvReleaseImage(&dst_Image);
	cvDestroyWindow("Source Histogram");
	cvReleaseImage(&OriginalHistogramImage);
	cvDestroyWindow("Processed Histogram");
	cvReleaseImage(&EqualizeHistogramImage);
}

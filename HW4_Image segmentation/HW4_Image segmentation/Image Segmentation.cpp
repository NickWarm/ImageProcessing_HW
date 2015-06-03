#include <opencv\cv.h>
#include <opencv\cxcore.h>
#include <opencv\highgui.h>

using namespace std;

//CvPoint2D32f Point1, Point2, Point3, Point4, Point5, Point6;
void Edge_detection(IplImage *image);
void Rotate_Image(IplImage *image, CvPoint2D32f Point1, CvPoint2D32f Point2,CvPoint2D32f Point3,CvPoint2D32f Point4);
CvPoint2D32f Point1, Point2, Point3, Point4, Point5, Point6;
void main()
{
	//CD.bmp
	IplImage *image = cvLoadImage("CD.bmp",-1);
	Edge_detection(image);
	Rotate_Image(image, Point1, Point3, Point2, Point4);

	//CD1.bmp
	IplImage *image1 = cvLoadImage("CD1.bmp",-1);
	Edge_detection(image1);
	Rotate_Image(image1, Point1, Point2, Point3, Point4);

	//CD2.bmp
	IplImage *image2 = cvLoadImage("CD2.bmp",-1);
	Edge_detection(image2);
	Rotate_Image(image2, Point3, Point4, Point1, Point2);

	//CD3.bmp
	IplImage *image3 = cvLoadImage("CD3.bmp",-1);
	Edge_detection(image3);
	Rotate_Image(image3, Point4, Point3, Point2, Point1);

	//CD4.bmp
	IplImage *image4 = cvLoadImage("CD4.bmp",-1);
	Edge_detection(image4);
	Rotate_Image(image4, Point2, Point1, Point6, Point4);

	//CD5.bmp
	IplImage *image5 = cvLoadImage("CD5.bmp",-1);
	Edge_detection(image5);
	Rotate_Image(image5, Point2, Point4, Point1, Point3);

	
}

void Edge_detection(IplImage *image)
{
	//Canny ��t����
	IplImage *Canny_image = cvCreateImage(cvGetSize(image),IPL_DEPTH_8U,1); 
	cvCvtColor(image, Canny_image, CV_BGR2GRAY);
	cvCanny(image, Canny_image,50,150,3);

	//Hough Transform �˴����u
	CvMemStorage *storage = cvCreateMemStorage(0); 
	CvSeq *lines = cvHoughLines2(Canny_image, storage, CV_HOUGH_PROBABILISTIC, 1, CV_PI/180, 80,200,10);  
	//cout<< lines->total <<endl;  //�g���աACvSeq *lines�̦s�F�������u  


	//�bEdge_image�W�e�X�˴��쪺���u�A�ǥѭp�B���������A�qCvPoint�����u�W�����I
	IplImage *Edge_image = cvCloneImage(image);
	//CvPoint2D32f Point1, Point2, Point3, Point4, Point5, Point6;
	int k=0;  //�O�B���A�O���O�ĴX���u
	for(int i=0;i<lines->total;i++)
	{
		CvPoint *line = (CvPoint*)cvGetSeqElem(lines,i);
		cvLine(Edge_image,line[0], line[1],CV_RGB(255,0,0),3,8);  //�N��쪺���u�аO������

		//���o�|�ӳ��I
		if(k==0)
		{
			Point1=cvPointTo32f(line[0]);
			Point2=cvPointTo32f(line[1]);
		}
		if(k==1)
		{
			Point3=cvPointTo32f(line[0]);
			Point4=cvPointTo32f(line[1]);
		}
		if(k==2)
		{
			Point5=cvPointTo32f(line[0]);
			Point6=cvPointTo32f(line[1]);
		}
		k++;
	}

	//�L�X�|�ӳ��I�y��
	cout << Point1.x  <<"," << Point1.y << endl;
	cout << Point2.x  <<"," << Point2.y << endl;
	cout << Point3.x  <<"," << Point3.y << endl;
	cout << Point4.x  <<"," << Point4.y << endl;
	cout << Point5.x  <<"," << Point5.y << endl;
	cout << Point6.x  <<"," << Point6.y << endl; //�L�X��o�{�ۦP���Ix,y�ǷL���t1�A�ۦP�I�R�h�@��
 
	cvNamedWindow("Canny Image");
	cvShowImage("Canny Image", Canny_image);
	cvNamedWindow("Edge Image");
	cvShowImage("Edge Image", Edge_image);
	cvWaitKey(0);
	cvReleaseImage(&Canny_image);
	cvReleaseImage(&Edge_image);
	cvDestroyWindow("Canny Image");
	cvDestroyWindow("Edge Image");
}


void Rotate_Image(IplImage *image, CvPoint2D32f Point1, CvPoint2D32f Point2,CvPoint2D32f Point3,CvPoint2D32f Point4)
{
	IplImage *dst_image = cvCloneImage(image);
	CvPoint2D32f srcTri[4], dstTri[4];
	CvMat *warp_mat = cvCreateMat(3,3,CV_64FC1);

	//��l�Ϥ����|�ӳ��I
	srcTri[0].x = Point1.x;
	srcTri[0].y = Point1.y;
	srcTri[1].x = Point2.x;
	srcTri[1].y = Point2.y;
	srcTri[2].x = Point3.x;
	srcTri[2].y = Point3.y;
	srcTri[3].x = Point4.x;
	srcTri[3].y = Point4.y;

	//��X�Ϥ����|�ӳ��I
	dstTri[0].x = 0;
	dstTri[0].y = 0;
	dstTri[1].x = image-> width-1;
	dstTri[1].y = 0;
	dstTri[2].x = 0;
	dstTri[2].y = image-> height-1;
	dstTri[3].x = image-> width-1;
	dstTri[3].y = image-> height-1;

	cvGetPerspectiveTransform(srcTri, dstTri, warp_mat );
	cvWarpPerspective( image, dst_image, warp_mat );  

	cvNamedWindow("Result");
	cvShowImage("Result",dst_image);
	cvWaitKey(0);
	cvReleaseImage(&dst_image);
	cvDestroyWindow("Result");
}
#include <opencv\cv.h>
#include <opencv\cxcore.h>
#include <opencv\highgui.h>
#include <iostream>

using namespace std;

void Phones_detecting(IplImage *image);

int main(int argc, char*argv[])
{

	if(argc!=2)
	{
		printf("Usage: DIP10179008 Filename\n");
		exit(1);
	}

	
	IplImage *image1 = cvLoadImage(argv[1],-1);
	Phones_detecting(image1);

	return 0;
}


void Phones_detecting(IplImage *image)
{
	int region[9][2] = {{428,266},{250,269},{77,260},  // ROI 左上角的座標
					   {397,158},{251,152},{122,158},
					   {379,79},{262,79},{145,80}};
	int summ[9];
	for(int i=0;i<9;i++)
	{
		/* step.1: 取得ROI */	
		int roi_width, roi_heigh;
		roi_width = 110;
		roi_heigh = 50;

		cvSetImageROI(image, cvRect(region[i][0], region[i][1], roi_width, roi_heigh)); //設定ROI，置入原圖

		/* step.2: 算RGB中R的平均值 */
		CvMat *ColorAVG_1D = cvCreateMat(1,roi_width,CV_64FC3);
		IplImage *AVGimage = cvCreateImage(cvSize(110, 255),IPL_DEPTH_8U,3);

		cvReduce(image, ColorAVG_1D, 0, CV_REDUCE_AVG); //計算色彩平均
		
		//繪圖
		cvZero(AVGimage);
		AVGimage->origin=1;
		int sum=0;
		for(int i=0;i<roi_width*0.25;i++)
		{
			CvPoint point_buttom = cvPoint(i,0);
			CvPoint point_top = cvPoint(i,(int)cvGet1D(ColorAVG_1D,i).val[1]);
		
			cvLine(AVGimage, point_buttom, point_top, CV_RGB(255,255,255));
			sum+= (int)cvGet1D(ColorAVG_1D,i).val[1];  //算出白色面積，與未屏蔽的圖做比較，來判斷ROI內是否被遮蔽
			
		}
		summ[i]=sum;
		sum=0;
		//cout<<"R["<<i<<"]= "<<summ[i]<<endl;
	}
	
	// 未屏蔽的ROI:(BGR中測G)			屏蔽時的ROI								Threshold閥值
	// R8=2476   R7=3376   R6=3283	|	R8=1249		R7=1957		R6=2678	 |	R8=1300		R7=2000		R6=2700
	// R5=3921   R4=3576   R3=2657	|	R5=3669		R4=3179		R3=2146	 |	R5=3700		R4=3200		R3=2200	
	// R2=3110   R1=2265   R0=4870	|	R2=2632		R1=1980		R0=4423  | 	R2=2700		R1=2000		R0=4500

	// 測試修改R8的閥值
	// R8=2200		R7=2000		R6=2700
	// R5=3700		R4=3200		R3=2200
	// R2=2700		R1=2000		R0=4500


	//用圖形標記不同手機的ROI，閥值：小於閥值=1，否則為零
	if(summ[0]<4500)
		summ[0]=1;
	else
		summ[0]=0;

	if(summ[1]<2000)
		summ[1]=1;
	else
		summ[1]=0;

	if(summ[2]<2700)
		summ[2]=1;
	else
		summ[2]=0;

	if(summ[3]<2200)
		summ[3]=1;
	else
		summ[3]=0;
	
	if(summ[4]<3200)
		summ[4]=1;
	else
		summ[4]=0;
	
	if(summ[5]<3700)
		summ[5]=1;
	else
		summ[5]=0;

	if(summ[6]<2700)
		summ[6]=1;
	
	else
		summ[6]=0;
	
	if(summ[7]<2000)
		summ[7]=1;
	else
		summ[7]=0;
	
	if(summ[8]<2200)
		summ[8]=1;
	else
		summ[8]=0;


	//判斷手機：用九宮格不同的圖形(看1)，來定義不同的手機
	if(summ[0]==0 && summ[1]==0 && summ[2]==0 && summ[3]==0 && summ[4]==0 && summ[5]==0 && summ[6]==0 && summ[7]==0 && summ[8]==0)
		cout<<"System Idle"<<endl; 
	if(summ[0]==1 && summ[1]==0 && summ[2]==0 && summ[3]==1 && summ[4]==1 && summ[5]==0 && summ[6]==1 && summ[7]==1 && summ[8]==1)
		cout<<"Apple iPhone 5S"<<endl; 
	if(summ[0]==0 && summ[1]==1 && summ[2]==0 && summ[3]==0 && summ[4]==1 && summ[5]==1 && summ[6]==0 && summ[7]==0 && summ[8]==1)
		cout<<"HTC One Max"<<endl; 
	if(summ[0]==0 && summ[1]==0 && summ[2]==1 && summ[3]==0 && summ[4]==1 && summ[5]==1 && summ[6]==0 && summ[7]==0 && summ[8]==1)
		cout<<"Samsung Galaxy Note 3"<<endl; 
	if(summ[0]==1 && summ[1]==0 && summ[2]==1 && summ[3]==1 && summ[4]==1 && summ[5]==1 && summ[6]==1 && summ[7]==1 && summ[8]==1)
		cout<<"ASUS Fonepad Note 6"<<endl; 
	if(summ[0]==0 && summ[1]==0 && summ[2]==0 && summ[3]==0 && summ[4]==1 && summ[5]==0 && summ[6]==0 && summ[7]==1 && summ[8]==1)
		cout<<"Nokia Lumia 1520"<<endl;
	if(summ[0]==0 && summ[1]==0 && summ[2]==0 && summ[3]==0 && summ[4]==0 && summ[5]==1 && summ[6]==0 && summ[7]==0 && summ[8]==1)
		cout<<"Koobee Max X7"<<endl;
	if(summ[0]==0 && summ[1]==0 && summ[2]==0 && summ[3]==0 && summ[4]==0 && summ[5]==0 && summ[6]==1 && summ[7]==0 && summ[8]==0)
		cout<<"Nexus 5"<<endl;
	if(summ[0]==1 && summ[1]==1 && summ[2]==1 && summ[3]==0 && summ[4]==1 && summ[5]==1 && summ[6]==1 && summ[7]==1 && summ[8]==1)
		cout<<"Sony Xperia"<<endl;
	if(summ[0]==1 && summ[1]==0 && summ[2]==1 && summ[3]==0 && summ[4]==1 && summ[5]==1 && summ[6]==1 && summ[7]==0 && summ[8]==1)
		cout<<"LG G Pro Lite"<<endl;
	if(summ[0]==0 && summ[1]==0 && summ[2]==0 && summ[3]==0 && summ[4]==1 && summ[5]==1 && summ[6]==1 && summ[7]==1 && summ[8]==1)
		cout<<"Intermediate"<<endl;
	if(summ[0]==0 && summ[1]==0 && summ[2]==0 && summ[3]==1 && summ[4]==1 && summ[5]==1 && summ[6]==1 && summ[7]==1 && summ[8]==1)
		cout<<"Intermediate"<<endl;
	if(summ[0]==1 && summ[1]==0 && summ[2]==0 && summ[3]==0 && summ[4]==1 && summ[5]==1 && summ[6]==0 && summ[7]==0 && summ[8]==0)
		cout<<"Intermediate"<<endl;
	if(summ[0]==0 && summ[1]==0 && summ[2]==0 && summ[3]==0 && summ[4]==1 && summ[5]==1 && summ[6]==0 && summ[7]==1 && summ[8]==1)
		cout<<"Intermediate"<<endl;

	
	/*cvResetImageROI(image);
	cvShowImage("original image",image);
    cvWaitKey(0);*/
}


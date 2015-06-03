#include <opencv/cv.h>
#include <opencv/highgui.h> 
#include <opencv/cxcore.h>
#include <iostream>

using namespace std;

void Convolution_operator_x(double Sigma, double Array_ImpulseResponse[], CvMat *Mat_Src, CvMat *Convolution_cropped_dx );
void Convolution_operator_y(double Sigma, double Array_ImpulseResponse[], CvMat *Mat_Src, CvMat *Convolution_cropped_dy );
void Direction(CvMat *Mat_Src,CvMat *Convolution_cropped_dy, CvMat *Convolution_cropped_dx, IplImage *src_Image, IplImage *dst_Image);
void Magnitude(CvMat *src_Convolution_cropped, CvMat *dst_magnitude );


void main()
{
	
	//input signal: Lenna
	IplImage *Lenna = cvLoadImage("Lenna.bmp",-1); //單通道
	CvMat *Mat_Lenna =cvCreateMat(Lenna->height, Lenna->width, CV_64FC1 ); //創造矩陣空間
	cvConvert(Lenna, Mat_Lenna);	//將圖像轉成矩陣
	CvMat *Lenna_Convolution_cropped_dx=cvCreateMat(Mat_Lenna->rows, Mat_Lenna->cols,CV_64FC1 );
	CvMat *Lenna_Convolution_cropped_dy=cvCreateMat(Mat_Lenna->rows, Mat_Lenna->cols,CV_64FC1 );


/** Lenna **/
// Sigma1=1.0
	double Sigma1=1.0; 
	double FilterLength_Sigma1[7];  //Length = 2*(3*Signa)+1
	Convolution_operator_x(Sigma1, FilterLength_Sigma1, Mat_Lenna, Lenna_Convolution_cropped_dx);
	Convolution_operator_y(Sigma1, FilterLength_Sigma1, Mat_Lenna, Lenna_Convolution_cropped_dy);
	//cout<<CV_MAT_ELEM(*Lenna_Convolution_cropped_dx,double,100,300)<<endl;  //測試是否存入Lenna_Convolution_cropped_dx，成功	
	//cout<<CV_MAT_ELEM(*Lenna_Convolution_cropped_dy,double,400,3)<<endl;  //測試是否存入Lenna_Convolution_cropped_dx，成功	

	//Magnitude
	IplImage *dst_magnitude_Lenna = cvCreateImage(cvGetSize(Lenna), IPL_DEPTH_8U, 1);
	CvMat *Lenna_magnitude_dx=cvCreateMat(Mat_Lenna->rows, Mat_Lenna->cols,CV_64FC1 );
	CvMat *Lenna_magnitude_dy=cvCreateMat(Mat_Lenna->rows, Mat_Lenna->cols,CV_64FC1 );

	Magnitude(Lenna_Convolution_cropped_dx, Lenna_magnitude_dx);
	Magnitude(Lenna_Convolution_cropped_dy, Lenna_magnitude_dy);
	
	cvAdd(Lenna_magnitude_dx, Lenna_magnitude_dy, dst_magnitude_Lenna);  //cvAdd的效果：如果輸入是矩陣，輸出可以自動轉成圖片


	//計算角度
	IplImage *dst_direction_Lenna = cvCreateImage(cvGetSize(Lenna), IPL_DEPTH_8U, 1);
	Direction(Mat_Lenna, Lenna_Convolution_cropped_dy, Lenna_Convolution_cropped_dx, Lenna, dst_direction_Lenna);  
	//我在Direction這副程式裡頭多寫了cvConvert()把矩陣轉圖片，最初的寫法是先Direction副程式，再Magnitude副程式
	//這樣做會有個問題，如果先Direction，會因為調用cvConvert()時把矩陣轉圖片，而使得圖片裡的部分資訊遺失掉
	//所以後來成先Magnitude副程式，再Direction副程式，如此一來才不會有圖片資訊遺失
	

	//顯示圖片
	cvNamedWindow("Direction Image",1);
	cvShowImage("Direction Image",dst_direction_Lenna);
	cvNamedWindow("Magnitude Image",1);
	cvShowImage("Magnitude Image",dst_magnitude_Lenna);
	cvWaitKey(0);

	cvDestroyWindow("Magnitude Image");
	//cvReleaseImage(&dst_magnitude_Lenna);  //不要急著釋放空間，待會還可以用
	cvDestroyWindow("Direction Image");
	//cvReleaseImage(&dst_direction_Lenna);  //不要急著釋放空間，待會還可以用



// Sigma3 = 3.0
	double Sigma3=3.0; 
	double FilterLength_Sigma3[19];  //Length = 2*(3*Signa)+1
	CvMat *Sigma3_Lenna_Convolution_cropped_dx=cvCreateMat(Mat_Lenna->rows, Mat_Lenna->cols,CV_64FC1 );
	CvMat *Sigma3_Lenna_Convolution_cropped_dy=cvCreateMat(Mat_Lenna->rows, Mat_Lenna->cols,CV_64FC1 );

	Convolution_operator_x(Sigma3, FilterLength_Sigma3, Mat_Lenna, Sigma3_Lenna_Convolution_cropped_dx); 
	Convolution_operator_y(Sigma3, FilterLength_Sigma3, Mat_Lenna, Sigma3_Lenna_Convolution_cropped_dy); 
	// 原本是寫輸出存在同一個矩陣裡，但這樣會有錯誤，所以另外創一個空間儲存Sigma2的輸出矩陣
	
	//cout<<CV_MAT_ELEM(*Sigma2_Lenna_Convolution_cropped_dx,double,100,300)<<endl;  //成功	
	//cout<<CV_MAT_ELEM(*Sigma2_Lenna_Convolution_cropped_dy,double,400,3)<<endl;	   //成功	


	//Magnitude
	Magnitude(Sigma3_Lenna_Convolution_cropped_dx, Lenna_magnitude_dx);
	Magnitude(Sigma3_Lenna_Convolution_cropped_dy, Lenna_magnitude_dy);
	
	cvAdd(Lenna_magnitude_dx, Lenna_magnitude_dy, dst_magnitude_Lenna);  //cvAdd的效果：如果輸入是矩陣，輸出可以自動轉成圖片



	//計算角度
	//IplImage *dst_direction_Lenna_Sigma2 = cvCreateImage(cvGetSize(Lenna), IPL_DEPTH_8U, 1);   
	Direction(Mat_Lenna, Sigma3_Lenna_Convolution_cropped_dy, Sigma3_Lenna_Convolution_cropped_dx, Lenna, dst_direction_Lenna); 



	//顯示圖片
	cvNamedWindow("Direction Image",1);
	cvShowImage("Direction Image",dst_direction_Lenna);
	cvNamedWindow("Magnitude Image",1);
	cvShowImage("Magnitude Image",dst_magnitude_Lenna);
	cvWaitKey(0);
	cvDestroyWindow("Magnitude Image");
	//cvReleaseImage(&dst_magnitude_Lenna);
	cvDestroyWindow("Direction Image");
	//cvReleaseImage(&dst_direction_Lenna); 


// Sigma5 = 5.0
	double Sigma5=5.0; 
	double FilterLength_Sigma5[31];  //Length = 2*(3*Signa)+1
	CvMat *Sigma5_Lenna_Convolution_cropped_dx=cvCreateMat(Mat_Lenna->rows, Mat_Lenna->cols,CV_64FC1 );
	CvMat *Sigma5_Lenna_Convolution_cropped_dy=cvCreateMat(Mat_Lenna->rows, Mat_Lenna->cols,CV_64FC1 );

	Convolution_operator_x(Sigma5, FilterLength_Sigma5, Mat_Lenna, Sigma5_Lenna_Convolution_cropped_dx); 
	Convolution_operator_y(Sigma5, FilterLength_Sigma5, Mat_Lenna, Sigma5_Lenna_Convolution_cropped_dy); 
	
	

	//Magnitude
	Magnitude(Sigma5_Lenna_Convolution_cropped_dx, Lenna_magnitude_dx);
	Magnitude(Sigma5_Lenna_Convolution_cropped_dy, Lenna_magnitude_dy);
	
	cvAdd(Lenna_magnitude_dx, Lenna_magnitude_dy, dst_magnitude_Lenna);  //cvAdd的效果：如果輸入是矩陣，輸出可以自動轉成圖片



	//計算角度
	//IplImage *dst_direction_Lenna_Sigma2 = cvCreateImage(cvGetSize(Lenna), IPL_DEPTH_8U, 1);   
	Direction(Mat_Lenna, Sigma5_Lenna_Convolution_cropped_dy, Sigma5_Lenna_Convolution_cropped_dx, Lenna, dst_direction_Lenna); 



	//顯示圖片
	cvNamedWindow("Direction Image",1);
	cvShowImage("Direction Image",dst_direction_Lenna);
	cvNamedWindow("Magnitude Image",1);
	cvShowImage("Magnitude Image",dst_magnitude_Lenna);
	cvWaitKey(0);
	cvDestroyWindow("Magnitude Image");
	cvReleaseImage(&dst_magnitude_Lenna);
	cvDestroyWindow("Direction Image");
	cvReleaseImage(&dst_direction_Lenna); 


/** Motorcycle **/


	//input signal: Motrocycle
	IplImage *Motorcycle = cvLoadImage("Motorcycle.bmp",-1); 
	CvMat *Mat_Motorcycle =cvCreateMat(Motorcycle->height, Motorcycle->width, CV_64FC1 ); 
	cvConvert(Motorcycle, Mat_Motorcycle);	
	CvMat *Motorcycle_Convolution_cropped_dx=cvCreateMat(Mat_Motorcycle->rows, Mat_Motorcycle->cols,CV_64FC1 );
	CvMat *Motorcycle_Convolution_cropped_dy=cvCreateMat(Mat_Motorcycle->rows, Mat_Motorcycle->cols,CV_64FC1 );

	
// Sigma1=1.0
	//double Sigma1=1.0; 
	//double FilterLength_Sigma1[7];  //Length = 2*(3*Signa)+1
	Convolution_operator_x(Sigma1, FilterLength_Sigma1, Mat_Motorcycle, Motorcycle_Convolution_cropped_dx);
	Convolution_operator_y(Sigma1, FilterLength_Sigma1, Mat_Motorcycle, Motorcycle_Convolution_cropped_dy);
	

	//Magnitude
	IplImage *dst_magnitude_Motorcycle = cvCreateImage(cvGetSize(Motorcycle), IPL_DEPTH_8U, 1);
	CvMat *Motorcycle_magnitude_dx=cvCreateMat(Mat_Motorcycle->rows, Mat_Motorcycle->cols,CV_64FC1 );
	CvMat *Motorcycle_magnitude_dy=cvCreateMat(Mat_Motorcycle->rows, Mat_Motorcycle->cols,CV_64FC1 );

	Magnitude(Motorcycle_Convolution_cropped_dx, Motorcycle_magnitude_dx);
	Magnitude(Motorcycle_Convolution_cropped_dy, Motorcycle_magnitude_dy);
	
	cvAdd(Motorcycle_magnitude_dx, Motorcycle_magnitude_dy, dst_magnitude_Motorcycle);  


	//計算角度
	IplImage *dst_direction_Motorcycle = cvCreateImage(cvGetSize(Motorcycle), IPL_DEPTH_8U, 1);
	Direction(Mat_Motorcycle, Motorcycle_Convolution_cropped_dy, Motorcycle_Convolution_cropped_dx, Motorcycle, dst_direction_Motorcycle);  
	

	//顯示圖片
	cvNamedWindow("Direction Image",1);
	cvShowImage("Direction Image",dst_direction_Motorcycle);
	cvNamedWindow("Magnitude Image",1);
	cvShowImage("Magnitude Image",dst_magnitude_Motorcycle);
	cvWaitKey(0);

	cvDestroyWindow("Magnitude Image");
	//cvReleaseImage(&dst_magnitude_Motorcycle);  //不要急著釋放空間，待會還可以用
	cvDestroyWindow("Direction Image");
	//cvReleaseImage(&dst_direction_Motorcycle);  //不要急著釋放空間，待會還可以用


// Sigma3 = 3.0
	//double Sigma3=3.0; 
	//double FilterLength_Sigma3[19];  //Length = 2*(3*Signa)+1
	CvMat *Sigma3_Motorcycle_Convolution_cropped_dx=cvCreateMat(Mat_Motorcycle->rows, Mat_Motorcycle->cols,CV_64FC1 );
	CvMat *Sigma3_Motorcycle_Convolution_cropped_dy=cvCreateMat(Mat_Motorcycle->rows, Mat_Motorcycle->cols,CV_64FC1 );

	Convolution_operator_x(Sigma3, FilterLength_Sigma3, Mat_Motorcycle, Sigma3_Motorcycle_Convolution_cropped_dx); 
	Convolution_operator_y(Sigma3, FilterLength_Sigma3, Mat_Motorcycle, Sigma3_Motorcycle_Convolution_cropped_dy); 
	
	
	//Magnitude
	Magnitude(Sigma3_Motorcycle_Convolution_cropped_dx, Motorcycle_magnitude_dx);
	Magnitude(Sigma3_Motorcycle_Convolution_cropped_dy, Motorcycle_magnitude_dy);
	
	cvAdd(Motorcycle_magnitude_dx, Motorcycle_magnitude_dy, dst_magnitude_Motorcycle);  



	//計算角度  
	Direction(Mat_Motorcycle, Sigma3_Motorcycle_Convolution_cropped_dy, Sigma3_Motorcycle_Convolution_cropped_dx, Motorcycle, dst_direction_Motorcycle); 



	//顯示圖片
	cvNamedWindow("Direction Image",1);
	cvShowImage("Direction Image",dst_direction_Motorcycle);
	cvNamedWindow("Magnitude Image",1);
	cvShowImage("Magnitude Image",dst_magnitude_Motorcycle);
	cvWaitKey(0);
	cvDestroyWindow("Magnitude Image");
	//cvReleaseImage(&dst_magnitude_Motorcycle);
	cvDestroyWindow("Direction Image");
	//cvReleaseImage(&dst_direction_Motorcycle); 

// Sigma5 = 5.0

	CvMat *Sigma5_Motorcycle_Convolution_cropped_dx=cvCreateMat(Mat_Motorcycle->rows, Mat_Motorcycle->cols,CV_64FC1 );
	CvMat *Sigma5_Motorcycle_Convolution_cropped_dy=cvCreateMat(Mat_Motorcycle->rows, Mat_Motorcycle->cols,CV_64FC1 );

	Convolution_operator_x(Sigma5, FilterLength_Sigma5, Mat_Motorcycle, Sigma5_Motorcycle_Convolution_cropped_dx); 
	Convolution_operator_y(Sigma5, FilterLength_Sigma5, Mat_Motorcycle, Sigma5_Motorcycle_Convolution_cropped_dy); 

	//Magnitude
	Magnitude(Sigma5_Motorcycle_Convolution_cropped_dx, Motorcycle_magnitude_dx);
	Magnitude(Sigma5_Motorcycle_Convolution_cropped_dy, Motorcycle_magnitude_dy);
	cvAdd(Motorcycle_magnitude_dx, Motorcycle_magnitude_dy, dst_magnitude_Motorcycle);  


	//計算角度  
	Direction(Mat_Motorcycle, Sigma5_Motorcycle_Convolution_cropped_dy, Sigma5_Motorcycle_Convolution_cropped_dx, Motorcycle, dst_direction_Motorcycle); 

	//顯示圖片
	cvNamedWindow("Direction Image",1);
	cvShowImage("Direction Image",dst_direction_Motorcycle);
	cvNamedWindow("Magnitude Image",1);
	cvShowImage("Magnitude Image",dst_magnitude_Motorcycle);
	cvWaitKey(0);
	cvDestroyWindow("Magnitude Image");
	//cvReleaseImage(&dst_magnitude_Motorcycle);
	cvDestroyWindow("Direction Image");
	//cvReleaseImage(&dst_direction_Motorcycle); 
}



void Convolution_operator_x(double Sigma, double Array_ImpulseResponse[], CvMat *Mat_Src , CvMat *Convolution_cropped_dx)
{
	//double Array_ImpulseResponse[7];

//Impulse Response (Filter)
	//創造一維陣列來進行impulse response
	CvMat Filter_Row = cvMat((int)(6*Sigma+1), 1, CV_64FC1, Array_ImpulseResponse);  //Sigma不一定會是整數
	
	//填入impulse response的一次微分高斯函數 (分別有對x偏微與對y偏微)
	//對x偏微的impulse response (Filter_Row)放入矩陣內各個元素的值
	int index=0;
	for(int x=(-Filter_Row.rows+1)/2;x< (Filter_Row.rows+1)/2;x++)  //x軸上的變數，為了避免誤以為陣列裡的索引值，所以 int x
		//高斯分佈的假如Sigma=1，則impulse response的長度為6*1++1=7，依照高斯分佈的圖形來看，
		//有高斯分佈的範圍為x=-3~3，如果用陣列索引值的習慣從零開始取x=0~7，則高斯分佈只會占一半，與所要的不符
	{
		double a = (double)-x/pow(Sigma,2);
		double b = exp(-pow( (double)x, 2 ) / ( 2 * pow( Sigma, 2 ) ));
		double f_of_x = a*b;
		cvSetReal1D(&Filter_Row, index, f_of_x);
		index++;
	}

	//impulse response mirroring
	CvMat *FlipFilter_Row= cvCreateMat((int)6*Sigma+1, 1, CV_64FC1); //創造記憶體位置來存放翻轉後的 impulse response matrix
	cvFlip(&Filter_Row, FlipFilter_Row,-1 );
	//cvFlip(&Filter_Row, FlipFilter_Row,0 );
	//原本這樣寫，最後一個參數設0，0為水平翻轉，但這樣做只有翻轉到x軸的方向(row)，沒翻轉到y軸的方向
	//所以參數應該改用-1：水平加垂直翻轉



//END: Impulse Response 

	//source mat zero padding：
	//來思索一下，為何我要在記憶體中創造空間來存放zeropadding後的矩陣(一維陣列)，因為我最後要把前面補過的零再去掉
	CvMat *Mat_Src_RowPadded = cvCreateMat(Mat_Src->rows + (Filter_Row.rows-1)*2, Mat_Src->cols, CV_64FC1);  
	cvSetZero(Mat_Src_RowPadded);
	//此為input image 經過zero padding後的矩陣，用cvCreateMat()創立矩陣時，矩陣裡的元素數值為零

	//對impulse response 偏微x，input image的x補零 (row補零)	
	//for(int i=0;i< Mat_Src->cols;i++)  //對原圖補零，所以長度限制應該是看原圖的矩陣長寬大小
	for(int j=0;j<Mat_Src->rows;j++)
	{
		for(int i=0;i< Mat_Src->cols;i++)
		//for(int j=0;j<Mat_Src->rows;j++)
		{
			CV_MAT_ELEM( *Mat_Src_RowPadded, double, j+ (Filter_Row.rows-1),i)
				=CV_MAT_ELEM(*Mat_Src, double, j, i);
			//cout<<CV_MAT_ELEM(*Mat_Src,double,j,i)<<endl;
		}
	}
//END: zero padding
	

//Convolution

	CvMat *Mat_Convolution_noncropped_dx=cvCreateMat(Mat_Src->rows + (Filter_Row.rows-1), Mat_Src->cols,CV_64FC1 );
	//CvMat *Mat_Convolution_noncropped_dx=cvCreateMat(Mat_Src->rows, Mat_Src->cols,CV_64FC1 );  
	//原本是上面註解的寫法，但這樣寫會有問題，沒補到零，所以改成 cvCreateMat(Mat_Src->rows + (Filter_Row.rows-1), Mat_Src->cols,CV_64FC1 )

	//Matrixs do convolution，impulse response進行time shift   
	for(int i=0; i < Mat_Convolution_noncropped_dx->cols ;i++)  
	{
		for(int j=0; j < Mat_Convolution_noncropped_dx->rows ;j++ )
		{
			//impulse response一維陣列裡的值都要與input signal相成並累加
			double sum=0;
			for(int index_Filter=0 ; index_Filter < FlipFilter_Row->rows ;index_Filter++) 
			{
				sum = sum + cvGetReal1D(FlipFilter_Row, index_Filter) * CV_MAT_ELEM(*Mat_Src_RowPadded, double, j+index_Filter, i);  //FlipFliper裡的元素想用CV_MAT_ELEM()去訪問，但是失敗了，原因待查
			}
			CV_MAT_ELEM(*Mat_Convolution_noncropped_dx, double ,j,i)= sum; //把convoluton運算後的結果放入Mat_Convolution_noncroped_dx矩陣
			//cout<<CV_MAT_ELEM(*Mat_Convolution_noncropped_dx,double,j,i)<<endl;
		}
	}





/*
	//驗證convolution後是否有東西
	for(int i=0;i<Mat_Convolution_noncropped_dx->rows;i++)
	{
		for(int j=0;j<Mat_Convolution_noncropped_dx->cols;j++)
		{

			cout<<CV_MAT_ELEM(*Mat_Convolution_noncropped_dx,double,i,j)<<endl;
		}
	}
*/

	//cropped convolution result
	//創造一個裁剪後的矩陣
	//CvMat *Convolution_cropped_dx=cvCreateMat(Mat_Src->rows, Mat_Src->cols,CV_64FC1 );
	cvSetZero(Convolution_cropped_dx);
	//cout<<CV_MAT_ELEM(*Convolution_cropped_dx,double,4,5)<<endl;

	//對x裁剪
	for(int i=0;i<Mat_Convolution_noncropped_dx->rows;i++)
	{
		for(int j=0;j<Mat_Convolution_noncropped_dx->cols;j++)
		{
			int Cropped_Front=(FlipFilter_Row->rows-1)/2;
			if( (i - Cropped_Front >= 0) && (i - Cropped_Front) < (Mat_Convolution_noncropped_dx->rows - FlipFilter_Row->rows+ 1)  )
			{
				CV_MAT_ELEM(*Convolution_cropped_dx, double,i-Cropped_Front,j)=CV_MAT_ELEM(*Mat_Convolution_noncropped_dx, double,i,j);  
				//cout<<"1."<<CV_MAT_ELEM(*Convolution_cropped_dx,double,i-Cropped_Front,j)<<endl;		//測試數字是否有被輸入進去矩陣裡，成功
				//cout << "2."<<CV_MAT_ELEM(*Mat_Convolution_noncropped_dx, double,i,j)	<< endl;		
			}	
		}	
	}
//END: Convolution
}






void Convolution_operator_y(double Sigma, double Array_ImpulseResponse[], CvMat *Mat_Src, CvMat *Convolution_cropped_dy )
{

//Impulse Response (Filter)
	//創造一維陣列來進行impulse response
	CvMat Filter_Column = cvMat(1, (int)(6*Sigma+1), CV_64FC1, Array_ImpulseResponse);
	
	//對y偏微的impulse response (Filter_Column)
	int index=0;
	for(int y=-(Filter_Column.cols-1)/2 ; y< (Filter_Column.cols+1)/2 ; y++)  //y軸上的變數
	{
		double a = (double)-y/pow(Sigma,2);
		double b = exp(-pow( (double)y, 2 ) / ( 2 * pow( Sigma, 2 ) ));
		double f_of_y = a*b;
		cvSetReal1D(&Filter_Column, index, f_of_y);
		index++;
	}

	//impulse response mirroring
	CvMat *FlipFilter_Column= cvCreateMat(1, (int)6*Sigma+1,  CV_64FC1); 
	cvFlip(&Filter_Column, FlipFilter_Column,-1 );
//END: Impulse Response 

//source mat zero padding：
	CvMat *Mat_Src_ColumePadded = cvCreateMat(Mat_Src->rows , Mat_Src->cols+ (Filter_Column.cols-1)*2, CV_64FC1);  
	cvSetZero(Mat_Src_ColumePadded);

	//對impulse response 偏微y，input image的y補零 (row補零)	
	for(int i=0;i< Mat_Src->cols;i++)  //對原圖補零，所以長度限制應該是看原圖的矩陣長寬大小
	{
		for(int j=0;j<Mat_Src->rows;j++)
		{
			CV_MAT_ELEM( *Mat_Src_ColumePadded, double, j,i+ (Filter_Column.cols-1))
				=CV_MAT_ELEM(*Mat_Src, double, j, i);
			//cout<<CV_MAT_ELEM(*Mat_Src,double,j,i)<<endl;
		}
	}
//END: zero padding
	


//Convolution
	//創造一個空間，放convolution後的矩陣 (y偏微，尚未去掉多餘的零)
	CvMat *Mat_Convolution_noncropped_dy = cvCreateMat(Mat_Src->rows , Mat_Src->cols+ (Filter_Column.cols-1), CV_64FC1); 
	//CvMat *Mat_Src_RowPadded = cvCreateMat(Mat_Src->rows + (Filter_Row.rows-1)*2, Mat_Src->cols, CV_64FC1);

	//Matrixs do convolution
	for(int i=0; i < Mat_Convolution_noncropped_dy->cols ;i++)  
	{
		for(int j=0; j < Mat_Convolution_noncropped_dy->rows ;j++ )
		{
			//impulse response一維陣列裡的值都要與input signal相成並累加
			double sum=0;
			for(int index_Filter=0 ; index_Filter < FlipFilter_Column->cols ;index_Filter++) 
			{
				sum = sum + cvGetReal1D(FlipFilter_Column, index_Filter)*CV_MAT_ELEM(*Mat_Src_ColumePadded, double, j, i+index_Filter);  //FlipFliper裡的元素想用CV_MAT_ELEM()去訪問，但是失敗了，原因待查
			}
			CV_MAT_ELEM(*Mat_Convolution_noncropped_dy, double ,j,i)= sum; //把convoluton運算後的結果放入Mat_Convolution_noncroped_dx矩陣
			//cout<<CV_MAT_ELEM(*Mat_Convolution_noncropped_dy,double,j,i)<<endl;
		}
	}

	//cropped convolution result
	//創造一個裁剪後的矩陣
	//CvMat *Convolution_cropped_dy=cvCreateMat(Mat_Src->rows, Mat_Src->cols,CV_64FC1 );
	cvSetZero(Convolution_cropped_dy);
	//cout<<CV_MAT_ELEM(*Convolution_cropped_dx,double,4,5)<<endl;

	//對y裁剪
	for(int i=0;i<Mat_Convolution_noncropped_dy->rows;i++)
	{
		for(int j=0;j<Mat_Convolution_noncropped_dy->cols;j++)
		{
			int Cropped_Front=(FlipFilter_Column->cols-1)/2;
			
			if( (j - Cropped_Front >= 0) && (j - Cropped_Front) < (Mat_Convolution_noncropped_dy->cols - FlipFilter_Column->cols+ 1)  )
			{
				CV_MAT_ELEM(*Convolution_cropped_dy, double,i,j-Cropped_Front)=CV_MAT_ELEM(*Mat_Convolution_noncropped_dy, double,i,j);  
				//cout<<"1."<<CV_MAT_ELEM(*Convolution_cropped_dy,double,i,j-Cropped_Front)<<endl;		//測試數字是否有被輸入進去矩陣裡，成功
				//cout << "2."<<CV_MAT_ELEM(*Mat_Convolution_noncropped_dy, double,i,j)	<< endl;		
			}	
		}	
	}
//END: Convolution


}


void Direction(CvMat *Mat_Src,CvMat *Convolution_cropped_dy, CvMat *Convolution_cropped_dx, IplImage *src_Image, IplImage *dst_Image)
{
	CvMat *Direction_Mat = cvCreateMat(src_Image->height, src_Image->width, CV_64FC1);
	for(int i=0;i<Mat_Src->cols;i++)
	{
		for(int j=0;j<Mat_Src->rows;j++)
		{
			double y=CV_MAT_ELEM(*Convolution_cropped_dy, double,j,i) ;
			double x=CV_MAT_ELEM(*Convolution_cropped_dx, double,j,i);

			double atan = (( atan2(y,x) + CV_PI)/(2*CV_PI))*255;
			CV_MAT_ELEM(*Direction_Mat, double, j, i) = atan;
		}
	}

	//IplImage *dst_Image = cvCreateImage(cvGetSize(src_Image), IPL_DEPTH_8U, 1);
	cvConvert(Direction_Mat, dst_Image);

}

void Magnitude(CvMat *src_Convolution_cropped, CvMat *dst_magnitude )
{
	//Magnitude，dx
	CvScalar s; 
	for(int i=0 ;i<src_Convolution_cropped->cols ;i++)
	{
		for(int j=0 ;j<src_Convolution_cropped->rows ;j++)
		{
			// 最初是打算，把convolution後已裁剪的的矩陣(cropped)直接轉成圖片，再去取絕對值
			// 但是Convolution後會有負值，如果用cvConvert()轉成圖片，則原本小於零的會強迫等於0，超過255的會強迫等於255
			// 舉例來說   矩陣裡的資訊如果有-50，取絕對值後為50。如果先轉圖片再取決對值，那麼 -50會變為0，如此一來會少掉許多原本圖片的資訊
			s=cvGet2D(src_Convolution_cropped,j,i);  //初始化CvScalar
			s.val[0] = fabs( s.val[0] );
			cvSet2D(dst_magnitude,j,i,s);
			//cout<<CV_MAT_ELEM(*dst_magnitude,double,j,i)<<endl;
		}
	}

}


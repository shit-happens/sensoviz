#include "stdafx.h"
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <cmath> 
#include <string>
#include <opencv2/imgproc/imgproc.hpp>

#include "region_transformation.h"

using namespace std;
using namespace cv;

//Mat point_harris(Mat img, int blockSize, int ksize, double alpha)
//{
//	Mat result, img32bit;
//	cvtColor(img, img, CV_BGR2GRAY);
//	img.convertTo(img32bit, CV_32FC1);
//	cornerHarris(img32bit, result, blockSize, ksize, alpha, BORDER_DEFAULT);
//	return result;
//}

int adjustHeight(int Height, int Row)
{
	int adjustedHeight = 0;
	double minDiff = pow(10, 10);
	for (int i = 0; i < Row; i++)
	{
		if (Row % (i + 1) == 0)
		{
			int diff = abs(Height - (i + 1));
			if (diff <= minDiff)
			{
				minDiff = diff;
				adjustedHeight = (i + 1);
			}
		}
	}
	return adjustedHeight;
}

int adjustWidth(int Width, int Col)
{
	int adjustedWidth = 0;
	double minDiff = pow(10, 10);
	for (int i = 0; i < Col; i++)
	{
		if (Col % (i + 1) == 0)
		{
			int diff = abs(Width - (i + 1));
			if (diff <= minDiff)
			{
				minDiff = diff;
				adjustedWidth = (i + 1);
			}
		}
	}
	return adjustedWidth;
}
int main()
{
	region_transformation obj;
	Mat img = imread("C:/Users/Anshit Vishwakarma/Desktop/Sensovision/2.png");
	Mat result1 = obj.fill_up(img);
	imshow("result1", result1);
	imshow("Image", img);
	waitKey(0);
	return 0;

}



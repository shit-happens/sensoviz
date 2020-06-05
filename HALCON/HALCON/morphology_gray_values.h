#pragma once
#include "stdafx.h"
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <cmath>

using namespace std;
using namespace cv;

#ifndef morphology_gray_values_H
#define morphology_gray_values_H

class morphology_gray_values
{
public:
	Mat gray_erosion(Mat img, int SE_Shape, Size SE_size);
	Mat gray_erosion_rect(Mat img, int MaskHeight, int MaskWidth);
	Mat gray_erosion_shape(Mat img, int SE_Shape, int MaskHeight, int MaskWidth);
	Mat gray_dilation(Mat img, int SE_Shape, Size SE_size);
	Mat gray_dilation_rect(Mat img, int MaskHeight, int MaskWidth);
	Mat gray_dilation_shape(Mat img, int SE_Shape, int MaskHeight, int MaskWidth);
	Mat gray_opening(Mat img, int SE_Shape, Size SE_size);
	Mat gray_opening_rect(Mat img, int MaskHeight, int MaskWidth);
	Mat gray_opening_shape(Mat img, int SE_Shape, int MaskHeight, int MaskWidth);
	Mat gray_closing(Mat img, int SE_Shape, Size SE_size);
	Mat gray_closing_rect(Mat img, int MaskHeight, int MaskWidth);
	Mat gray_closing_shape(Mat img, int SE_Shape, int MaskHeight, int MaskWidth);
	Mat gray_tophat(Mat img, int SE_Shape, Size SE_size);
	Mat gray_bothat(Mat img, int SE_Shape, Size SE_size);
	Mat gray_range_rect(Mat img, int MaskWidth, int MaskHeight);
	Mat gray_disc_se(int width, int height, int Smax);
	void boundary(Mat img);
};

#endif // !morphology_gray_values_H





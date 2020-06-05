#pragma once
#include "stdafx.h"
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <cmath>

using namespace std;
using namespace cv;

#ifndef smoothing_H
#define smoothing_H

class smoothing
{
public:
	Mat bilateral_filter(Mat img, int d, double sigmaColor, double sigmaSpace);
	Mat gauss_filter(Mat img, Size ksize, double sigmaX, double sigmaY);
	Mat sigma_image(Mat img, int maskwidth, int maskheight, int Sigma);
	Mat median_image(Mat img, string masktype, int radius);
	Mat median_rect(Mat img, int maskwidth, int maskheight);
	Mat median_seperate(Mat img, int maskwidth, int maskheight);
	Mat mean_image(Mat img, int maskwidth, int maskheight);
	Mat mean_sp(Mat img, int maskwidth, int maskheight, int MinThresh, int MaxThresh);
};

#endif // !smoothing_H




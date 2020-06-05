#pragma once
#include "stdafx.h"
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <cmath>

using namespace std;
using namespace cv;

#ifndef arithmetic_H
#define arithetic_H

class arithmetic
{
public:
	Mat arithmetic::abs_diff_image(Mat img1, Mat img2, float mult);
	Mat arithmetic::abs_image(Mat img);
	Mat arithmetic::min_image(Mat img1, Mat img2);
	Mat arithmetic::max_image(Mat img1, Mat img2);
	Mat arithmetic::log_image(Mat img);
	Mat arithmetic::pow_image(Mat img, int exp);
	Mat arithmetic::invert_image(Mat img);
	Mat arithmetic::sin_image(Mat img);
	Mat arithmetic::cos_image(Mat img);
	Mat arithmetic::tan_image(Mat img);
	Mat arithmetic::add_image(Mat img1, Mat img2, float mult, float add);
	Mat arithmetic::sub_image(Mat ImageMinuend, Mat ImageSubtrahend, float mult, float add);
	Mat arithmetic::mult_image(Mat img1, Mat img2, float mult, float add);
	Mat arithmetic::div_image(Mat img1, Mat img2, float mult, float add);
	Mat arithmetic::sqrt_image(Mat img);
	Mat arithmetic::exp_image(Mat img);
	Mat arithmetic::asin_image(Mat img);
	Mat arithmetic::acos_image(Mat img);
	Mat arithmetic::atan_image(Mat img);
	Mat arithmetic::atan2_image(Mat imgY, Mat imgX);

};



#endif // !arithmetic_H




#pragma once
#include "stdafx.h"
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <cmath>

using namespace std;
using namespace cv;

#ifndef bit_H
#define bit_H

class bit
{
//private:
	
public:
	int* GetBinaryDigits(int N);
	Mat bit_and(Mat mask1, Mat mask2);
	Mat bit_or(Mat mask1, Mat mask2);
	Mat bit_xor(Mat mask1, Mat mask2);
	Mat bit_not(Mat mask);
	Mat bit_mask(Mat mask, int BitMask);
	Mat bit_lshift(Mat mask, int shift);
	Mat bit_rshift(Mat mask, int shift);
	Mat bit_slice(Mat mask, int bit);
};

#endif // !bit_H




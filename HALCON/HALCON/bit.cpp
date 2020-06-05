#include "stdafx.h"
#include "bit.h"

Mat bit::bit_and(Mat mask1, Mat mask2)
{
	Mat result;
	bitwise_and(mask1, mask2, result);
	return result;
}
Mat bit::bit_or(Mat mask1, Mat mask2)
{
	Mat result;
	bitwise_or(mask1, mask2, result);
	return result;
}
Mat bit::bit_xor(Mat mask1, Mat mask2)
{
	Mat result;
	bitwise_xor(mask1, mask2, result);
	return result;
}
Mat bit::bit_not(Mat mask)
{
	bitwise_not(mask, mask);
	return mask;
}

Mat bit::bit_mask(Mat mask, int BitMask)
{
	for (int i = 0; i < mask.rows; ++i)
		for (int j = 0; j < mask.cols; ++j)
		{
			mask.at<uchar>(i, j) = (mask.at<uchar>(i, j)) & BitMask;
		}
	return mask;
}

Mat bit::bit_lshift(Mat mask, int shift)
{
	for (int i = 0; i < mask.rows; ++i)
		for (int j = 0; j < mask.cols; ++j)
		{
			mask.at<uchar>(i, j) = min((mask.at<uchar>(i, j))*(pow(2, shift)), double(255));
		}
	return mask;
}

Mat bit::bit_rshift(Mat mask, int shift)
{
	for (int i = 0; i < mask.rows; ++i)
		for (int j = 0; j < mask.cols; ++j)
		{
			mask.at<uchar>(i, j) = min((mask.at<uchar>(i, j)) / (pow(2, shift)), double(255));
		}
	return mask;
}

int* bit::GetBinaryDigits(int N) {

	#define ull unsigned long long int
	// To store the binary number 
	ull B_Number = 0;
	int cnt = 0;
	while (N != 0) {
		int rem = N % 2;
		ull c = pow(10, cnt);
		B_Number += rem * c;
		N /= 2;

		// Count used to store exponent value 
		cnt++;
	}

	int* array = new int[cnt];
	for (int i = 0; i < cnt && B_Number != 0; i++) {
		array[i] = B_Number % 10;
		B_Number /= 10;
	}
	return array;
}

Mat bit::bit_slice(Mat mask, int bit)
{
	Mat ImageSlice(mask.rows, mask.cols, CV_8UC1, Scalar(0));
	for (int i = 0; i < mask.rows; ++i)
		for (int j = 0; j < mask.cols; ++j)
		{
			int* bin_digits = GetBinaryDigits(mask.at<uchar>(i, j));
			ImageSlice.at<uchar>(i, j) = bin_digits[8 - bit];
		}
	return ImageSlice;
}

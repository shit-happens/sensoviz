#include "stdafx.h"
#include "arithmetic.h"
#include <cmath>
#include <opencv2/imgproc/imgproc.hpp>

Mat arithmetic::abs_diff_image(Mat img1, Mat img2, float mult)
{
	Mat ImageResult(img1.rows, img2.cols, CV_32FC1, Scalar(0));
	for (int i = 0; i < img1.rows; i++)
	{
		for (int j = 0; j < img1.cols; j++)
		{
			ImageResult.at<float>(i, j) = abs(img1.at<float>(i, j) - img2.at<float>(i, j))*mult;
		}
	}
	return ImageResult;
}
Mat arithmetic::abs_image(Mat img)
{
	Mat gray;
	cvtColor(img, gray, CV_BGR2GRAY);
	return gray;
}
Mat arithmetic::min_image(Mat img1, Mat img2)
{
	Mat ImageMin(img1.rows, img1.cols, CV_32FC1, Scalar(0));
	for (int i = 0; i < img1.rows; ++i)
		for (int j = 0; j < img1.cols; ++j)
		{
			ImageMin.at<float>(i, j) = min((img1.at<float>(i, j)), (img2.at<float>(i, j)));
		}
	return ImageMin;
}
Mat arithmetic::max_image(Mat img1, Mat img2)
{
	Mat ImageMax(img1.rows, img1.cols, CV_32FC1, Scalar(0));
	for (int i = 0; i < img1.rows; ++i)
		for (int j = 0; j < img1.cols; ++j)
		{
			ImageMax.at<float>(i, j) = max((img1.at<float>(i, j)), (img2.at<float>(i, j)));
		}
	return ImageMax;
}
Mat arithmetic::log_image(Mat img)
{
	Mat LogImage(img.rows, img.cols, CV_32FC1, Scalar(0));
	for (int i = 0; i < img.rows; ++i)
		for (int j = 0; j < img.cols; ++j)
		{
			LogImage.at<float>(i, j) = log(img.at<float>(i, j));
		}
	return LogImage;
}
Mat arithmetic::pow_image(Mat img, int exp)
{
	Mat PowImage(img.rows, img.cols, CV_32FC1, Scalar(0));
	for (int i = 0; i < img.rows; i++) {
		for (int j = 0; j < img.cols; j++) {
			int pixel = img.at<float>(i, j);
			PowImage.at<float>(i, j) = max(255,int(pow(pixel,exp)));
		}
	}
	return PowImage;
}
Mat arithmetic::invert_image(Mat img)
{
	Mat InvertImage(img.rows, img.cols, CV_8UC1, Scalar(0));
	for (int i = 0; i < img.rows; i++) {
		for (int j = 0; j < img.cols; j++) {
			InvertImage.at<float>(i, j) = 255 - img.at<float>(i, j);
		}
	}
	return InvertImage;
}
Mat arithmetic::sin_image(Mat img)
{
	Mat SinImage(img.rows, img.cols, CV_32FC1, Scalar(0));
	for (int i = 0; i < img.rows; ++i)
		for (int j = 0; j < img.cols; ++j)
		{
			SinImage.at<float>(i, j) = sin(img.at<float>(i, j));
		}
	return SinImage;
}
Mat arithmetic::cos_image(Mat img)
{
	Mat CosImage(img.rows, img.cols, CV_32FC1, Scalar(0));
	for (int i = 0; i < img.rows; ++i)
		for (int j = 0; j < img.cols; ++j)
		{
			CosImage.at<float>(i, j) = cos(img.at<float>(i, j));
		}
	return CosImage;
}
Mat arithmetic::tan_image(Mat img)
{
	Mat TanImage(img.rows, img.cols, CV_32FC1, Scalar(0));
	for (int i = 0; i < img.rows; ++i)
		for (int j = 0; j < img.cols; ++j)
		{
			TanImage.at<float>(i, j) = tan(img.at<float>(i, j));
		}
	return TanImage;
}
Mat arithmetic::add_image(Mat img1, Mat img2, float mult, float add)
{
	Mat ImageResult(img1.rows, img1.cols, CV_32FC1, Scalar(0));
	for (int i = 0; i < img1.rows; i++)
	{
		for (int j = 0; j < img1.cols; j++)
		{
			ImageResult.at<float>(i, j) = ((img1.at<float>(i, j) + img2.at<float>(i, j))*mult + add);
		}
	}
	return ImageResult;
}
Mat arithmetic::sub_image(Mat ImageMinuend, Mat ImageSubtrahend, float mult, float add)
{
	Mat ImageResult(ImageMinuend.rows, ImageMinuend.cols, CV_32FC1, Scalar(0));
	for (int i = 0; i < ImageMinuend.rows; i++)
	{
		for (int j = 0; j < ImageMinuend.cols; j++)
		{
			ImageResult.at<float>(i, j) = (ImageMinuend.at<float>(i, j) - ImageSubtrahend.at<float>(i, j))*mult + add;
		}
	}
	return ImageResult;
}
Mat arithmetic::mult_image(Mat img1, Mat img2, float mult, float add)
{
	Mat ImageResult(img1.rows, img1.cols, CV_32FC1, Scalar(0));
	for (int i = 0; i < img1.rows; i++)
	{
		for (int j = 0; j < img1.cols; j++)
		{
			ImageResult.at<float>(i, j) = (img1.at<float>(i, j) * img2.at<float>(i, j))*mult + add;
		}
	}
	return ImageResult;
}
Mat arithmetic::div_image(Mat img1, Mat img2, float mult, float add)
{
	Mat ImageResult(img1.rows, img1.cols, CV_32FC1, Scalar(0));
	for (int i = 0; i < img1.rows; i++)
	{
		for (int j = 0; j < img1.cols; j++)
		{
			ImageResult.at<float>(i, j) = (img1.at<float>(i, j) / img2.at<float>(i, j))*mult + add;
		}
	}
	return ImageResult;
}
Mat arithmetic::sqrt_image(Mat img)
{
	Mat SqrtImage(img.rows, img.cols, CV_32FC1, Scalar(0));
	for (int i = 0; i < img.rows; ++i)
		for (int j = 0; j < img.cols; ++j)
		{
			SqrtImage.at<float>(i, j) = sqrt(img.at<float>(i, j));
		}
	return SqrtImage;
}
Mat arithmetic::exp_image(Mat img)
{
	Mat expImage(img.rows, img.cols, CV_32FC1, Scalar(0));
	for (int i = 0; i < img.rows; ++i)
		for (int j = 0; j < img.cols; ++j)
		{
			expImage.at<float>(i, j) = exp(img.at<float>(i, j));
		}
	return expImage;
}
Mat arithmetic::asin_image(Mat img)
{
	Mat aSinImage(img.rows, img.cols, CV_32FC1, Scalar(0));
	for (int i = 0; i < img.rows; ++i)
		for (int j = 0; j < img.cols; ++j)
		{
			if (img.at<float>(i, j) > 1 && img.at<float>(i, j) < -1)
			{
				img.at<float>(i, j) = 0;
			}
			aSinImage.at<float>(i, j) = asin(img.at<float>(i, j));
		}
	return aSinImage;
}
Mat arithmetic::acos_image(Mat img)
{
	Mat aCosImage(img.rows, img.cols, CV_32FC1, Scalar(0));
	for (int i = 0; i < img.rows; ++i)
		for (int j = 0; j < img.cols; ++j)
		{
			if (img.at<float>(i, j) > 1 && img.at<float>(i, j) < -1)
			{
				img.at<float>(i, j) = 0;
			}
			aCosImage.at<float>(i, j) = acos(img.at<float>(i, j));
		}
	return aCosImage;
}
Mat arithmetic::atan_image(Mat img)
{
	Mat aTanImage(img.rows, img.cols, CV_32FC1, Scalar(0));
	for (int i = 0; i < img.rows; ++i)
		for (int j = 0; j < img.cols; ++j)
		{
			aTanImage.at<float>(i, j) = atan(img.at<float>(i, j));
		}
	return aTanImage;
}
Mat arithmetic::atan2_image(Mat imgY, Mat imgX)
{
	Mat atan2Image(imgX.rows, imgX.cols, CV_32FC1, Scalar(0));
	for (int i = 0; i < imgX.rows; ++i)
		for (int j = 0; j < imgX.cols; ++j)
		{
			atan2Image.at<float>(i, j) = atan2((imgY.at<float>(i, j)),(imgX.at<float>(i, j)));
		}
	return atan2Image;
}




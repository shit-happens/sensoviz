#include "stdafx.h"
#include "morphology_gray_values.h"
#include <opencv2/imgproc/imgproc.hpp>

Mat morphology_gray_values::gray_erosion(Mat img, int SE_Shape, Size SE_size)
{
	Mat dst;
	// Preparing the kernel matrix object
	Mat kernel = getStructuringElement(SE_Shape, SE_size);
	//SE_Shape = MORPH_RECT, MORPH_ELLIPSE, MORPH_CROSS
	//CV_SHAPE_CUSTOM ~ custom structuring element (OpenCV 1.x API)
	erode(img, dst, kernel);
	return dst;
}

Mat morphology_gray_values::gray_erosion_rect(Mat img, int MaskHeight, int MaskWidth)
{
	if (MaskHeight % 2 == 0) {
		MaskHeight += 1;
	}
	if (MaskWidth % 2 == 0) {
		MaskWidth += 1;
	}
	Mat ImageMin = Mat(img.rows, img.cols, CV_8UC1, Scalar(0));
	Mat kernel = getStructuringElement(MORPH_RECT, Size(MaskWidth, MaskHeight));
	erode(img, ImageMin, kernel);
	return ImageMin;
}

Mat morphology_gray_values::gray_erosion_shape(Mat img, int SE_Shape, int MaskHeight, int MaskWidth)
{
	if (MaskHeight % 2 == 0) {
		MaskHeight += 1;
	}
	if (MaskWidth % 2 == 0) {
		MaskWidth += 1;
	}
	Mat ImageMin = Mat(img.rows, img.cols, CV_8UC1, Scalar(0));
	Mat kernel = getStructuringElement(SE_Shape, Size(MaskWidth, MaskHeight));
	erode(img, ImageMin, kernel);
	return ImageMin;
}

Mat morphology_gray_values::gray_dilation(Mat img, int SE_Shape, Size SE_size)
{
	Mat dst;
	// Preparing the kernel matrix object
	Mat kernel = getStructuringElement(SE_Shape, SE_size);
	//SE_Shape = MORPH_RECT, MORPH_ELLIPSE, MORPH_CROSS
	//CV_SHAPE_CUSTOM ~ custom structuring element (OpenCV 1.x API)
	dilate(img, dst, kernel);
	return dst;
}

Mat morphology_gray_values::gray_dilation_rect(Mat img, int MaskHeight, int MaskWidth)
{
	if (MaskHeight % 2 == 0) {
		MaskHeight += 1;
	}
	if (MaskWidth % 2 == 0) {
		MaskWidth += 1;
	}
	Mat ImageMax = Mat(img.rows, img.cols, CV_8UC1, Scalar(0));
	Mat kernel = getStructuringElement(MORPH_RECT, Size(MaskWidth, MaskHeight));
	dilate(img, ImageMax, kernel);
	return ImageMax;
}

Mat morphology_gray_values::gray_dilation_shape(Mat img, int SE_Shape, int MaskHeight, int MaskWidth)
{
	if (MaskHeight % 2 == 0) {
		MaskHeight += 1;
	}
	if (MaskWidth % 2 == 0) {
		MaskWidth += 1;
	}
	Mat ImageMax = Mat(img.rows, img.cols, CV_8UC1, Scalar(0));
	Mat kernel = getStructuringElement(SE_Shape, Size(MaskWidth, MaskHeight));
	dilate(img, ImageMax, kernel);
	return ImageMax;
}

Mat morphology_gray_values::gray_opening(Mat img, int SE_Shape, Size SE_size)
{
	Mat dst;
	// Preparing the kernel matrix object
	Mat kernel = getStructuringElement(SE_Shape, SE_size);
	//SE_Shape = MORPH_RECT, MORPH_ELLIPSE, MORPH_CROSS
	//CV_SHAPE_CUSTOM ~ custom structuring element (OpenCV 1.x API)
	morphologyEx(img, dst, MORPH_OPEN, kernel);
	return dst;
}

Mat morphology_gray_values::gray_opening_rect(Mat img, int MaskHeight, int MaskWidth)
{
	if (MaskHeight % 2 == 0) {
		MaskHeight += 1;
	}
	if (MaskWidth % 2 == 0) {
		MaskWidth += 1;
	}
	Mat ImageOpening = Mat(img.rows, img.cols, CV_8UC1, Scalar(0));
	Mat kernel = getStructuringElement(MORPH_RECT, Size(MaskWidth, MaskHeight));
	morphologyEx(img, ImageOpening, MORPH_OPEN, kernel);
	return ImageOpening;
}

Mat morphology_gray_values::gray_opening_shape(Mat img, int SE_Shape, int MaskHeight, int MaskWidth)
{
	if (MaskHeight % 2 == 0) {
		MaskHeight += 1;
	}
	if (MaskWidth % 2 == 0) {
		MaskWidth += 1;
	}
	Mat ImageOpening = Mat(img.rows, img.cols, CV_8UC1, Scalar(0));
	Mat kernel = getStructuringElement(SE_Shape, Size(MaskWidth, MaskHeight));
	morphologyEx(img, ImageOpening, MORPH_OPEN, kernel);
	return ImageOpening;
}

Mat morphology_gray_values::gray_closing(Mat img, int SE_Shape, Size SE_size)
{
	Mat dst;
	// Preparing the kernel matrix object
	Mat kernel = getStructuringElement(SE_Shape, SE_size);
	//SE_Shape = MORPH_RECT, MORPH_ELLIPSE, MORPH_CROSS
	//CV_SHAPE_CUSTOM ~ custom structuring element (OpenCV 1.x API)
	morphologyEx(img, dst, MORPH_CLOSE, kernel);
	return dst;
}

Mat morphology_gray_values::gray_closing_rect(Mat img, int MaskHeight, int MaskWidth)
{
	if (MaskHeight % 2 == 0) {
		MaskHeight += 1;
	}
	if (MaskWidth % 2 == 0) {
		MaskWidth += 1;
	}
	Mat ImageClosing = Mat(img.rows, img.cols, CV_8UC1, Scalar(0));
	Mat kernel = getStructuringElement(MORPH_RECT, Size(MaskWidth, MaskHeight));
	morphologyEx(img, ImageClosing, MORPH_CLOSE, kernel);
	return ImageClosing;
}

Mat morphology_gray_values::gray_closing_shape(Mat img, int SE_Shape, int MaskHeight, int MaskWidth)
{
	if (MaskHeight % 2 == 0) {
		MaskHeight += 1;
	}
	if (MaskWidth % 2 == 0) {
		MaskWidth += 1;
	}
	Mat ImageClosing = Mat(img.rows, img.cols, CV_8UC1, Scalar(0));
	Mat kernel = getStructuringElement(SE_Shape, Size(MaskWidth, MaskHeight));
	morphologyEx(img, ImageClosing, MORPH_CLOSE, kernel);
	return ImageClosing;
}

Mat morphology_gray_values::gray_tophat(Mat img, int SE_Shape, Size SE_size)
{
	Mat dst;
	// Preparing the kernel matrix object
	Mat kernel = getStructuringElement(SE_Shape, SE_size);
	//SE_Shape = MORPH_RECT, MORPH_ELLIPSE, MORPH_CROSS
	//CV_SHAPE_CUSTOM ~ custom structuring element (OpenCV 1.x API)
	morphologyEx(img, dst, MORPH_TOPHAT, kernel);
	return dst;
}

Mat morphology_gray_values::gray_bothat(Mat img, int SE_Shape, Size SE_size)
{
	Mat dst;
	// Preparing the kernel matrix object
	Mat kernel = getStructuringElement(SE_Shape, SE_size);
	//SE_Shape = MORPH_RECT, MORPH_ELLIPSE, MORPH_CROSS
	//CV_SHAPE_CUSTOM ~ custom structuring element (OpenCV 1.x API)
	morphologyEx(img, dst, MORPH_BLACKHAT, kernel);
	return dst;
}

Mat morphology_gray_values::gray_range_rect(Mat img, int MaskWidth, int MaskHeight)
{
	Mat meanImg = Mat(img.rows, img.cols, CV_8UC1, Scalar(0));

	if (MaskHeight % 2 == 0) {
		MaskHeight += 1;
	}
	if (MaskWidth % 2 == 0) {
		MaskWidth += 1;
	}

	int halfwidth = int(MaskWidth / 2);
	int halfheight = int(MaskHeight / 2);
	for (int i = 0; i < img.rows; i++) {
		for (int j = 0; j < img.cols; j++) {
			int max = -1;
			int min = 100000;
			for (int x = i - halfwidth; x <= i + halfwidth; x++) {
				for (int y = j - halfheight; y <= j + halfheight; y++) {
					if ((x < 0) || (x >= img.rows) || (y < 0) || (y >= img.cols))
						continue;
					if (img.at<uchar>(x, y) > max)
					{
						max = img.at<uchar>(x, y);
					}

					if (img.at<uchar>(x, y) < min)
					{
						min = img.at<uchar>(x, y);
					}

					}
				}

			meanImg.at<uchar>(i, j) = max - min;
			}
		}
	return meanImg;
}

Mat morphology_gray_values::gray_disc_se(int width, int height, int Smax)
{
	Mat kernel = getStructuringElement(MORPH_ELLIPSE, Size(width, height));

	for (int i = 0; i < kernel.rows; i++) {
		for (int j = 0; j < kernel.cols; j++) {

			if (kernel.at<uchar>(i, j) > Smax)
			{
				kernel.at<uchar>(i, j) = Smax;
			}
			
		}
	}
	return kernel;
}

void morphology_gray_values::boundary(Mat img)
{
	Mat edges;
	Mat outImage(img.rows, img.cols, CV_8UC3, Scalar(0, 0, 0));
	cvtColor(img, img, CV_BGR2GRAY);
	Canny(img, edges, 100, 200, 3);
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	RNG rng(12345);

	/*findContours(edges, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_NONE, Point(0, 0));
	drawContours(img, contours[0], 0, (0, 255, 0), 3);
	drawContours(img, contours[1], 1, (0, 0, 255), 3);*/

	findContours(edges, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);

	if (!contours.empty() && !hierarchy.empty()) {

		// loop through the contours/hierarchy
		for (int i = 0; i<contours.size(); i++) {

			// look for hierarchy[i][3]!=-1, ie hole boundaries
			if (hierarchy[i][3] != -1) {
				// random colour
				Scalar colour(255,0,0);
				drawContours(outImage, contours, i, colour, 3);
			}
			if (hierarchy[i][2] != -1) {
				// random colour
				Scalar colour(0, 255, 0);
				drawContours(outImage, contours, i, colour, 3);
			}
			if (hierarchy[i][1] != -1) {
				// random colour
				Scalar colour(0, 255, 0);
				drawContours(outImage, contours, i, colour, 3);
			}
		}
	}

	imshow("contours", outImage);
	waitKey(0);
//	return contours;
}
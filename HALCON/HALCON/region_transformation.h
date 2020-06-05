#pragma once
#include "stdafx.h"
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <cmath>

using namespace std;
using namespace cv;

#ifndef region_transformation_H
#define region_transformation_H

class region_transformation
{
public:
	Mat clip_region(Mat img, int row1, int col1, int row2, int col2);
	Mat clip_region_rel(Mat mask, int top, int bottom, int left, int right);
	Mat fill_up(Mat mask);
	Mat fill_up_shape(Mat mask, string Feature, double Min, double Max);
	Mat partition_rectangle(Mat img, int Width, int Height);
	Mat partition_dynamic(Mat img, double Distance, double Percent);
	Mat distance_transform(Mat mask, bool Foreground);
	Mat shape_trans(Mat mask, string Type);

private:
	double getOuterRadius(Mat mask);
	double getArea(Mat mask);
	double getRa(Mat mask);
	double find_compactness(Mat mask, double area);
	double getAnisometry(Mat mask);
	double find_convexity(Mat mask);
	double getRb(Mat mask);
	Point getCentroid(Mat mask);
	void getSecondOrderMoments(Mat mask, double* moments);
	double getPhi(Mat mask);
	int adjustHeight(int Height, int Row);
	int adjustWidth(int Width, int Col);
	void drawLine(int x, Mat img, vector<Point>cnt);
	int verticalExtent(int x, int Rows, vector<Point>cnt);
};

#endif // !region_transformation_H




#pragma once
#include "stdafx.h"
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <cmath>

using namespace std;
using namespace cv;

#ifndef region_features_H
#define region_features_H


class region_features
{
public:

	double find_circularity(Mat mask, double area);
	void find_convexity(Mat mask);
	double find_compactness(Mat mask, double area);
	double getstructurefactor(Mat mask, double ra, double rb, double area);
	double getArea(Mat mask);
	Point getCentroid(Mat mask);
	void getSecondOrderMoments(Mat mask, double* moments);
	double getRa(Mat mask);
	double getRb(Mat mask);
	double getPhi(Mat mask);
	double get_max_dist(vector<Point> contour);
	double getAnisometry(Mat mask);
	double getBulkiness(Mat mask);
	double getStructfactor(Mat mask);
	double getOuterRadius(Mat mask);
	double getInnerRadius(Mat mask);
	int getCol1(Mat mask);
	int getCol2(Mat mask);
	int getRow1(Mat mask);
	int getRow2(Mat mask);
	int getCenterRow(Mat mask);
	int getCenterCol(Mat mask);
	int getWidth(Mat mask);
	int getHeight(Mat mask);
};

#endif // !region_features_H
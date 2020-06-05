#pragma once
#include "stdafx.h"
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <cmath>

using namespace std;
using namespace cv;

#ifndef OneD_Measurement_H
#define OneD_Measurement_H

class OneD_Measurement
{
public:
	void gen_measure_arc(Mat img, int Cx, int Cy, int Radius, double startAngle, double endAngle);
	void gen_measure_rectangle(Mat img, int Cx, int Cy, double phi, int length, int breadth);
};
#endif // !OneD_Measurement_H




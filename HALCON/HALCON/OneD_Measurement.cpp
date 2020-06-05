#include "stdafx.h"
#include "OneD_Measurement.h"


void OneD_Measurement::gen_measure_arc(Mat img, int Cx, int Cy, int Radius, double startAngle, double endAngle)
{
	Mat Measure_Arc = Mat(img.rows, img.cols, CV_8UC3, Scalar(255, 255, 255));
	Mat gray; Mat canny_output;
	Mat mask1 = Mat(img.rows, img.cols, CV_8UC1, Scalar(0));
	Mat mask2 = Mat(img.rows, img.cols, CV_8UC1, Scalar(0));
	ellipse(mask1, Point(Cx, Cy), Size(Radius, Radius), 0, startAngle, endAngle, Scalar(255), 1);
	ellipse(Measure_Arc, Point(Cx, Cy), Size(Radius, Radius), 0, startAngle, endAngle, Scalar(255, 0, 0), 1);

	cvtColor(img, gray, COLOR_BGR2GRAY);
	Canny(gray, canny_output, 50, 200);
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	RNG rng(12345);
	findContours(canny_output, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE, Point(0, 0));
	findContours(canny_output, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
	drawContours(mask2, contours, 0, Scalar(255), 1);
	drawContours(Measure_Arc, contours, 0, Scalar(0, 255, 0), 1);

	Mat result; int num = 0;
	bitwise_and(mask1, mask2, result);

	vector<Point> AllPoints;
	for (int i = 0; i < result.rows; i++) {
		for (int j = 0; j < result.cols; j++) {
			if (result.at<uchar>(i, j) == 255)
			{
				AllPoints.push_back(Point(j, i));
				num++;
			}
		}
	}
	struct Dist {
		bool operator()(const Point& a, const Point &b) {
			return sqrt((a.x - b.x)*(a.x - b.x) + (a.y - b.y)*(a.y - b.y)) < 2;
		}
	};

	vector<int> P_labels;
	vector<int> A;
	int numberOfPoints = partition(AllPoints, P_labels, Dist());
	partition(AllPoints, A, Dist());
	cout << "Number of Intersection points: " << numberOfPoints << endl;

	A.erase(unique(A.begin(), A.end()), A.end());

	vector<Point> IntersectionPoints;
	for (int i = 0; i < A.size(); i++)
	{
		int sumX = 0;
		int sumY = 0;
		int count = 0;
		for (int j = 0; j < P_labels.size(); j++)
		{
			if (P_labels[j] == A[i])
			{
				sumX = sumX + AllPoints[j].x;
				sumY = sumY + AllPoints[j].y;
				count++;
			}
		}
		int mX = sumX / count;
		int mY = sumY / count;
		string label = "P" + to_string(i + 1);
		IntersectionPoints.push_back(Point(mX, mY));
		circle(Measure_Arc, Point(mX, mY), 2, Scalar(0, 0, 255), -1);
		putText(Measure_Arc, label, Point(mX + 2, mY), FONT_HERSHEY_PLAIN, 1.0, CV_RGB(0, 0, 0), 1.0);
	}


	imshow("mask1", mask1);
	imshow("mask2", mask2);
	imshow("result", result);
	imshow("Measurements", Measure_Arc);
	waitKey(0);

	for (int i = 0; i < IntersectionPoints.size(); i++)
	{
		for (int j = 0; j < IntersectionPoints.size(); j++)
		{
			if (i < j)
			{
				Point p1 = IntersectionPoints[i];
				Point p2 = IntersectionPoints[j];
				double dist = sqrt((p1.x - p2.x)*(p1.x - p2.x) + (p1.y - p2.y)*(p1.y - p2.y));
				double theta = (2 * asin(dist / (2 * Radius)))*(180 / CV_PI);
				double La = (2 * CV_PI*Radius)*(theta / 360);
				cout << "Angle between points P" + to_string(i + 1) + " and P" + to_string(j + 1) + " : " << theta << endl;
				cout << "Length of Arc between points P" + to_string(i + 1) + " and P" + to_string(j + 1) + " : " << La << endl;
				cout << endl;
			}
		}
	}
}


void OneD_Measurement::gen_measure_rectangle(Mat img, int Cx, int Cy, double phi, int length, int breadth)
{
	//(Cx,Cy)- Centre of ROI
	//phi- Angle of Major axis
	//length- Length of ROI
	//breadth- Breadth of ROI
	RotatedRect rRect(Point(Cx, Cy), Size(length, breadth), phi);
	// matrices we'll use
	Mat M, rotated, cropped;
	// get angle and size from the bounding box
	float angle = rRect.angle;
	Size rect_size = rRect.size;
	// get the rotation matrix
	M = getRotationMatrix2D(rRect.center, angle, 1.0);
	// perform the affine transformation
	warpAffine(img, rotated, M, img.size(), INTER_CUBIC);
	// crop the resulting image
	getRectSubPix(rotated, rect_size, rRect.center, cropped);

	Mat cropped_gray;
	cvtColor(cropped, cropped_gray, CV_BGR2GRAY);
	GaussianBlur(cropped_gray, cropped_gray, Size(7, 7), 0);
	Canny(cropped_gray, cropped_gray, 50, 100);
	dilate(cropped_gray, cropped_gray, Mat(), Point(-1, -1), 1);
	erode(cropped_gray, cropped_gray, Mat(), Point(-1, -1), 1);

	vector<Vec4i> lines; vector<Vec4i> P_lines; int count = 0;
	HoughLinesP(cropped_gray, lines, 1, CV_PI / 180, 1, 0, 0);

	for (size_t i = 0; i < lines.size(); i++)
	{
		Vec4i l = lines[i];
		Point p1, p2;
		p1 = Point(l[0], l[1]);
		p2 = Point(l[2], l[3]);
		//calculate angle in radian,  if you need it in degrees just do angle * 180 / PI
		float angle = atan2(p1.y - p2.y, p1.x - p2.x);
		angle = (angle * 180) / CV_PI;
		if (abs(angle - 90) < 1)
		{
			//			line(cropped, p1, p2, Scalar(255, 0, 0), 2);
			P_lines.push_back(l);
			count++;
		}
	}

	struct isEqual {
		bool operator()(const Vec4i& _l1, const Vec4i& _l2)
		{
			Vec4i l1(_l1), l2(_l2);

			float mx1 = (l1[0] + l1[2]) / 2;
			float mx2 = (l2[0] + l2[2]) / 2;

			//float my1 = (l1[1] + l1[3]) / 2;
			//float my2 = (l2[1] + l2[3]) / 2;

			if (abs(mx1 - mx2) > 1) //Threshold to differentiate between perpendicular lines (adjustable)
				return false;

			return true;
		}
	};
	vector<int> L_labels;
	int numberOfLines = partition(P_lines, L_labels, isEqual());
	cout << "Number of Perpendicular Lines: " << numberOfLines << endl;
	vector<int> A = L_labels;
	set<int> s(A.begin(), A.end());
	A.assign(s.begin(), s.end());

	vector<Vec4i> PerpendicularLines;
	for (int i = 0; i < A.size(); i++)
	{
		int x1 = 0; int x2 = 0;
		int count = 0;
		for (int j = 0; j < L_labels.size(); j++)
		{
			Vec4i line = P_lines[j];
			int x_min = min(line[0], line[2]);
			int x_max = max(line[0], line[2]);
			if (L_labels[j] == A[i])
			{
				x1 = x1 + x_min;
				x2 = x2 + x_max;
				count++;
			}
		}
		int mx1 = x1 / count;
		int mx2 = x2 / count;
		int my1 = 0;
		int my2 = cropped.rows;
		PerpendicularLines.push_back(Vec4i(mx1, my1, mx2, my2));
		string label = "L" + to_string(i + 1);
		line(cropped, Point(mx1, my1), Point(mx2, my2), Scalar(255, 0, 0), 2);
		putText(cropped, label, Point(int((mx1 + mx2) / 2) + 1, int((my1 + my2) / 2)), FONT_HERSHEY_PLAIN, 1.0, CV_RGB(0, 0, 0), 1.0);
	}

	for (int i = 0; i < PerpendicularLines.size(); i++)
	{
		for (int j = 0; j < PerpendicularLines.size(); j++)
		{
			if (i < j)
			{
				Vec4i L1 = PerpendicularLines[i];
				Vec4i L2 = PerpendicularLines[j];
				double dist = abs(int((L1[0] + L1[2]) / 2) - int((L2[0] + L2[2]) / 2));
				cout << "Distance between lines L" + to_string(i + 1) + " and L" + to_string(j + 1) + " : " << dist << endl;
				cout << endl;
			}
		}
	}

	imshow("result", cropped);
	waitKey(0);
}

#include "stdafx.h"
#include "region_transformation.h"

Mat region_transformation::shape_trans(Mat mask, string Type)
{
	Mat canny_output;
	cvtColor(mask, mask, CV_BGR2GRAY);
	Mat RegionTrans = Mat(mask.rows, mask.cols, CV_8UC3, Scalar(0,0,0));
	Canny(mask, canny_output, 50, 200);
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	RNG rng(12345);
	findContours(canny_output, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE, Point(0, 0));

	if (Type == "convex")
	{
		// create hull array for convex hull points
		vector< vector<Point> > hull(contours.size());
		for (int i = 0; i < contours.size(); i++)
			convexHull(Mat(contours[i]), hull[i], false);

		for (int i = 0; i < contours.size(); i++){
			Scalar color_contours = Scalar(0, 255, 0); // green - color for contours
			Scalar color = Scalar(255, 0, 0); // blue - color for convex hull
											  // draw ith contour
			drawContours(RegionTrans, contours, i, color_contours, 1, 8, vector<Vec4i>(), 0, Point());
			// draw ith convex hull
			drawContours(RegionTrans, hull, i, color, 1, 8, vector<Vec4i>(), 0, Point());
	}

	}

	else if (Type == "ellipse")
	{
		RotatedRect box = fitEllipse(contours[0]);
		ellipse(RegionTrans, box, Scalar(255, 0, 0));
	}

	else if (Type == "inner_rectangle1")
	{
		int max_width = 0, max_height = 0, max_area = 0, width, height;
		Point max_p1, max_p2;
		for (Point p1 : contours[0]){
		    for (Point p2 : contours[0]){
		        width = abs(p2.x - p1.x);
		        height = abs(p2.y - p1.y);
		        if( (width*height) > max_area ){
		            max_area = width*height;
		            max_width = width;
		            max_height = height;
		            max_p1 = p1;
		            max_p2 = p2;
		        }
		    }
		}
		rectangle(RegionTrans, max_p1, max_p2, Scalar(255, 0, 0));
	}

	else if (Type == "outer_circle")
	{
		double outer_radius = -1;
		Point max_dist_p1, max_dist_p2;
		double max_dist = -1, dist;
		for (Point p1 : contours[0]) {
			for (Point p2 : contours[0]) {
				dist = sqrt(pow((p1.x - p2.x), 2) + pow((p1.y - p2.y), 2));
				if (max_dist < dist) {
					max_dist = dist;
					max_dist_p1 = p1;
					max_dist_p2 = p2;
				}
			}
		}
		int Cx = (max_dist_p1.x + max_dist_p2.x) / 2;
		int Cy = (max_dist_p1.y + max_dist_p2.y) / 2;
		outer_radius = max_dist / 2;

		circle(RegionTrans, Point(Cx, Cy), outer_radius, Scalar(255, 0, 0), 1);
	}

	else if (Type == "rectangle1")
	{
		int min_x; int min_y; int max_x; int max_y;
		vector<int> X; vector<int> Y;
		for (Point p : contours[0]) {
			X.push_back(p.x);
			Y.push_back(p.y);
		}
		min_x = *min_element(X.begin(), X.end());
		max_x = *max_element(X.begin(), X.end());
		min_y = *min_element(Y.begin(), Y.end());
		max_y = *max_element(Y.begin(), Y.end());

		rectangle(RegionTrans, Point(min_x, min_y), Point(max_x, max_y), Scalar(255, 0, 0));
	}

	else if (Type == "rectangle2")
	{
		RotatedRect rect = minAreaRect(contours[0]);
		Point2f vertices[4];
		rect.points(vertices);
		for (int i = 0; i < 4; i++)
		{
			line(RegionTrans, vertices[i], vertices[(i + 1) % 4], Scalar(255,0,0), 1);
		}
	}
	return RegionTrans;
}

Mat region_transformation::distance_transform(Mat mask, bool Foreground)
{
	cvtColor(mask, mask, CV_BGR2GRAY);
	Mat result = Mat(mask.rows, mask.cols, CV_8UC1, Scalar(0));
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(mask, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE, Point(0, 0));
	for (int i = 0; i < mask.cols; i++) {
		for (int j = 0; j < mask.rows; j++) {
			if (Foreground == true)
			{
				if (pointPolygonTest(contours[0], Point(j, i), false) >= 0)
				{
					result.at<uchar>(j, i) = int(pointPolygonTest(contours[0], Point(j, i), true));
				}
			}
			else
			{
				if (pointPolygonTest(contours[0], Point(j, i), false) < 0)
				{
					result.at<uchar>(j, i) = int(pointPolygonTest(contours[0], Point(j, i), true));
				}
			}
		}
	}
	return result;
}


Mat region_transformation::partition_rectangle(Mat img, int Width, int Height)
{
	int Row = img.rows;
	int Col = img.cols;
	double imgArea = (Row)*(Col);
	double rectArea = (Width)*(Height);
	Mat Partitioned = img.clone();
	double numOfRectangles = 0;
	if (imgArea < rectArea)
	{
		return img;
	}
	if (imgArea >= 1.5 * rectArea)
	{
		Height = adjustHeight(Height, Row);
		Width = adjustWidth(Width, Col);
		cout << "Height: " << Height << endl;
		cout << "Width: " << Width << endl;
		int numberOfHorizontalLines = int(Row / Height);
		int numberOfVerticalLines = int(Col / Width);
		numOfRectangles = (numberOfHorizontalLines + 1) * (numberOfVerticalLines + 1);
		for (int i = 0; i < numberOfHorizontalLines; i++) {
			int y = (Height)*(i + 1);
			line(Partitioned, Point(0, y), Point(Col, y), Scalar(0, 255, 0), 1);
		}
		for (int j = 0; j < numberOfVerticalLines; j++) {
			int x = (Width)*(j + 1);
			line(Partitioned, Point(x, 0), Point(x, Row), Scalar(0, 255, 0), 1);
		}
	}
	return Partitioned;
}

Mat region_transformation::partition_dynamic(Mat img, double Distance, double Percent)
{
	Mat gray; Mat canny_output; vector<vector<Point> > contours; vector<Vec4i> hierarchy;
	Mat Partitioned = img.clone();
	cvtColor(img, gray, COLOR_BGR2GRAY);
	Canny(gray, canny_output, 50, 200);
	findContours(canny_output, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE, Point(0, 0));
	drawContours(Partitioned, contours, 0, Scalar(0, 255, 0), 2);
	vector<Point> contour = contours[0];
	vector<int> X;
	for (Point P : contour)
	{
		X.push_back(P.x);
	}

	int Width = abs(max_element(X.begin(), X.end()) - min_element(X.begin(), X.end()));
	int n = round(Width / Distance);
	Distance = (Width / n);

	int Rows = img.rows;
	int x = *min_element(X.begin(), X.end()) + Distance;
	while (x < *max_element(X.begin(), X.end()))
	{
		int del = Distance*Percent*0.01;
		int minimumElement = 10000000000000000;
		int XPartitioned = 0;
		for (int u = x - del; u <= x + del; u++)
		{
			if (verticalExtent(u, Rows, contour) < minimumElement)
			{
				minimumElement = verticalExtent(u, Rows, contour);
				XPartitioned = u;
			}
			drawLine(XPartitioned, Partitioned, contour);
		}
		x = x + Distance;
	}
	return Partitioned;
}

Mat region_transformation::clip_region(Mat img, int row1, int col1, int row2, int col2)
{
	Rect myROI(row1, col1, row2, col2);
	return img(myROI);
}

Mat region_transformation::clip_region_rel(Mat mask, int top, int bottom, int left, int right) {
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	RNG rng(12345);
	findContours(mask, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE, Point(0, 0));

	Rect boundRect = boundingRect(contours[0]);
	int x1 = boundRect.x;
	int y1 = boundRect.y;
	int w = boundRect.width;
	int h = boundRect.height;
	Rect roi1(x1, y1, w, h);
	Mat cropped_img = mask(roi1);

	Rect roi2(left, top, abs(left - right), abs(top - bottom));
	return cropped_img(roi2);
}

Mat region_transformation::fill_up(Mat mask)
{
	cvtColor(mask, mask, CV_BGR2GRAY);
	Mat des; vector<vector<Point> > contours; vector<Vec4i> hierarchy; Mat result;
	bitwise_not(mask, des);
	findContours(des, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE, Point(0, 0));

	for (int i = 0; i < contours.size(); i++)
	{
		drawContours(des, contours, -1, Scalar(255), -1);
	}

	bitwise_not(des, result);
	return result;
}

Mat region_transformation::fill_up_shape(Mat mask, string Feature, double Min, double Max)
{
	double featureValue = 0;
	if (Feature == "anisometry")
	{
		featureValue = getAnisometry(mask);
	}
	else if (Feature == "area")
	{
		featureValue = getArea(mask);
	}
	else if (Feature == "compactness")
	{
		double area = getArea(mask);
		featureValue = find_compactness(mask, area);
	}
	else if (Feature == "convexity")
	{
		featureValue = find_convexity(mask);
	}
	else if (Feature == "outer_circle")
	{
		featureValue = find_convexity(mask);
	}
	else if (Feature == "phi")
	{
		featureValue = getPhi(mask);
	}
	else if (Feature == "ra")
	{
		featureValue = getRa(mask);
	}
	else if (Feature == "rb")
	{
		featureValue = getRb(mask);
	}

	if (featureValue >= Min && featureValue <= Max)
	{
		Mat des; vector<vector<Point> > contours; vector<Vec4i> hierarchy; Mat result;
		bitwise_not(mask, des);
		findContours(des, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE, Point(0, 0));
		drawContours(des, contours, -1, Scalar(255), -1);

		bitwise_not(des, result);
		return result;
	}
}

double region_transformation::getOuterRadius(Mat mask) {
	double outer_radius = -1;
	Point max_dist_p1, max_dist_p2;
	double max_dist = -1, dist;

	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(mask, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE, Point(0, 0));
	for each(Point p1 in contours[0]) {
		for each(Point p2 in contours[0]) {
			dist = sqrt(pow((p1.x - p2.x), 2) + pow((p1.y - p2.y), 2));
			if (max_dist < dist) {
				max_dist = dist;
				max_dist_p1 = p1;
				max_dist_p2 = p2;
			}
		}
	}
	outer_radius = max_dist / 2;
	return outer_radius;
}

double region_transformation::getArea(Mat mask) {
	double area = -1;
	for (int i = 0; i < mask.rows; i++) {
		for (int j = 0; j < mask.cols; j++) {
			if (mask.at<uchar>(i, j) != 0) {
				area += 1;
			}
		}
	}
	return area;
}

double region_transformation::getRa(Mat mask) {
	double ra = -1;
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(mask, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE, Point(0, 0));
	Moments mu = moments(contours[0], false);

	double sum = mu.m02 + mu.m20 + (2 * mu.m11);
	sum = 1;
	double v02 = mu.m02 / sum;
	double v20 = mu.m20 / sum;
	double v11 = mu.m11 / sum;

	cout << "02:  " << mu.m02 << "  20:  " << mu.m20 << "   11:  " << mu.m11 << endl;

	ra = 0.5 * sqrt(8 * (v20 + v02 + sqrt(pow(v20 - v02, 2) + 4 * pow(v11, 2))));
	return ra;
}

double region_transformation::find_compactness(Mat mask, double area) {
	double c_compactness;
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	RNG rng(12345);
	findContours(mask, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE, Point(0, 0));

	double contlength = arcLength(contours[0], true);
	double c_dash_compactness = (contlength * contlength) / (area * 4 * 3.1416);
	c_compactness = max(c_dash_compactness, double(1));
	return c_compactness;
}

double region_transformation::getAnisometry(Mat mask) {
	double anisometry = -1;
	double ra = getRa(mask);
	double rb = getRb(mask);
	anisometry = ra / rb;
	return anisometry;
}

double region_transformation::find_convexity(Mat mask)
{
	int thresh = 100;
	Mat gray;
	cvtColor(mask, gray, COLOR_BGR2GRAY);
	Mat canny_output;
	Canny(gray, canny_output, thresh, thresh * 2);
	vector<vector<Point> > contours;
	findContours(canny_output, contours, RETR_TREE, CHAIN_APPROX_SIMPLE);
	vector<Point> hull;
	convexHull(contours[0], hull);

	double FC = contourArea(hull);
	double F0 = contourArea(contours[0], false);
	double result = (F0 / FC);
	return result;
}

double region_transformation::getRb(Mat mask) {
	double rb = -1;
	double secondOrderMoments[9] = { 0 };
	getSecondOrderMoments(mask, secondOrderMoments);

	double M02 = secondOrderMoments[6];
	double M11 = secondOrderMoments[4];
	double M20 = secondOrderMoments[2];
	double M00 = secondOrderMoments[0];

	double sum = M00;
	M02 = M02 / sum;
	M11 = M11 / sum;
	M20 = M20 / sum;

	rb = 0.5 * sqrt(8 * (M20 + M02 - sqrt(pow(M20 - M02, 2) + 4 * pow(M11, 2))));
	return rb;
}

Point region_transformation::getCentroid(Mat mask) {
	Point centroid(-1, -1);
	double sumx = 0, sumy = 0, count = 0;
	for (int i = 0; i < mask.rows; i++) {
		for (int j = 0; j < mask.cols; j++) {
			if (mask.at<uchar>(i, j) != 0) {
				sumx += i;
				sumy += j;
				count++;
			}
		}
	}
	centroid.x = int(sumx / count);
	centroid.y = int(sumy / count);
	return centroid;
}

void region_transformation::getSecondOrderMoments(Mat mask, double* moments) {
	Point centroid = getCentroid(mask);
	int i0 = centroid.x, j0 = centroid.y;
	double moment = 0;
	for (int x = 0; x < 3; x++) {
		for (int y = 0; y < 3; y++) {
			moment = 0;
			for (int i = 0; i < mask.rows; i++) {
				for (int j = 0; j < mask.cols; j++) {
					//   moment += double(mask.at<uchar>(i,j)) * pow(i,x) * pow(j,y);
					if (mask.at<uchar>(i, j) != 0)
						moment += (pow((i0 - i), x) * pow((j0 - j), y));
				}
			}
			if (x == 0 && y == 0)   moments[0] = moment;
			else if (x == 0 && y == 1)   moments[1] = moment;
			else if (x == 0 && y == 2)   moments[2] = moment;
			else if (x == 1 && y == 0)   moments[3] = moment;
			else if (x == 1 && y == 1)   moments[4] = moment;
			else if (x == 1 && y == 2)   moments[5] = moment;
			else if (x == 2 && y == 0)   moments[6] = moment;
			else if (x == 2 && y == 1)   moments[7] = moment;
			else if (x == 2 && y == 2)   moments[8] = moment;
		}
	}
	return;
}

double region_transformation::getPhi(Mat mask) {
	double phi = -1;
	double secondOrderMoments[9] = { 0 };
	getSecondOrderMoments(mask, secondOrderMoments);

	double M02 = secondOrderMoments[6];
	double M11 = secondOrderMoments[4];
	double M20 = secondOrderMoments[2];
	double M00 = secondOrderMoments[0];

	double sum = M00;
	M02 = M02 / sum;
	M11 = M11 / sum;
	M20 = M20 / sum;

	phi = -0.5 * atan2(2 * M11, (M02 - M20));
	return phi;
}

int region_transformation::adjustHeight(int Height, int Row)
{
	int adjustedHeight = 0;
	double minDiff = pow(10, 10);
	for (int i = 0; i < Row; i++)
	{
		if (Row % (i + 1) == 0)
		{
			int diff = abs(Height - (i + 1));
			if (diff <= minDiff)
			{
				minDiff = diff;
				adjustedHeight = (i + 1);
			}
		}
	}
	return adjustedHeight;
}

int region_transformation::adjustWidth(int Width, int Col)
{
	int adjustedWidth = 0;
	double minDiff = pow(10, 10);
	for (int i = 0; i < Col; i++)
	{
		if (Col % (i + 1) == 0)
		{
			int diff = abs(Width - (i + 1));
			if (diff <= minDiff)
			{
				minDiff = diff;
				adjustedWidth = (i + 1);
			}
		}
	}
	return adjustedWidth;
}

int region_transformation::verticalExtent(int x, int Rows, vector<Point>cnt)
{
	int count = 0;
	for (int y = 0; y <= Rows; y++)
	{
		if (pointPolygonTest(cnt, Point(x, y), false) >= 0)
		{
			count++;
		}
	}
	return count;
}

void region_transformation::drawLine(int x, Mat img, vector<Point>cnt)
{
	for (int y = 0; y <= img.rows; y++)
	{
		if (pointPolygonTest(cnt, Point(x, y), false) >= 0)
		{
			img.at<Vec3b>(Point(x, y)) = Vec3b(255, 0, 0);
		}
	}
}
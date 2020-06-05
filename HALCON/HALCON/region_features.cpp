#include "stdafx.h"
#include "region_features.h"

double region_features::find_circularity(Mat mask, double area) {
	double c_circ = 0;

	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	RNG rng(12345);
	findContours(mask, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE, Point(0, 0));

	Moments mu = moments(contours[0], true);
	int cx = int(mu.m10 / mu.m00);
	int cy = int(mu.m01 / mu.m00);

	double max_dist = 0;
	for (uint i = 0; i < contours[0].size(); i++) {
		int x = contours[0][i].x;
		int y = contours[0][i].y;
		double dist = sqrt(pow((x - cx), 2) + pow((y - cy), 2));
		if (dist > max_dist) max_dist = dist;
	}

	double c_dash_circ = area / (max_dist * max_dist * 3.1415);
	c_circ = min(double(1), c_dash_circ);
	return c_circ;
}

double region_features::find_compactness(Mat mask, double area) {
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


double region_features::getstructurefactor(Mat mask, double ra, double rb, double area) {
	double structurefactor = ((ra / rb) * (3.1415 * (ra * rb) / area)) - 1;
	return structurefactor;
}

double region_features::getRa(Mat mask) {
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

double region_features::getArea(Mat mask) {
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

Point region_features::getCentroid(Mat mask) {
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

void region_features::getSecondOrderMoments(Mat mask, double* moments) {
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

double region_features::getRb(Mat mask) {
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

double region_features::getPhi(Mat mask) {
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


int region_features::getRow1(Mat mask) {
	int row1 = mask.rows;
	for (int i = 0; i < mask.rows; i++) {
		for (int j = 0; j < mask.cols; j++) {
			if (mask.at<uchar>(i, j) != 0) {
				if (i < row1) row1 = i;
			}
		}
	}
	return row1;
}

int region_features::getCol1(Mat mask) {
	int col1 = mask.cols;
	for (int i = 0; i < mask.rows; i++) {
		for (int j = 0; j < mask.cols; j++) {
			if (mask.at<uchar>(i, j) != 0) {
				if (j < col1) col1 = j;
			}
		}
	}
	return col1;
}

int region_features::getRow2(Mat mask) {
	int row2 = -1;  // bottom right row
	for (int i = 0; i < mask.rows; i++) {
		for (int j = 0; j < mask.cols; j++) {
			if (mask.at<uchar>(i, j) != 0) {
				if (i > row2) row2 = i;
			}
		}
	}
	return row2;
}

int region_features::getCol2(Mat mask) {
	int col2 = -1;
	for (int i = 0; i < mask.rows; i++) {
		for (int j = 0; j < mask.cols; j++) {
			if (mask.at<uchar>(i, j) != 0) {
				if (j > col2) col2 = j;
			}
		}
	}
	return col2;
}

int region_features::getCenterRow(Mat mask) {
	int CenterRow = int((getRow1(mask) + getRow2(mask) / 2));
	return CenterRow;
}

int region_features::getCenterCol(Mat mask) {
	int CenterCol = int((getCol1(mask) + getCol2(mask)) / 2);
	return CenterCol;
}

int region_features::getWidth(Mat mask) {
	int width = int((getRow2(mask) - getRow1(mask)) / 2);
	return width;
}

int region_features::getHeight(Mat mask) {
	int height = int((getCol2(mask) - getCol1(mask)) / 2);
	return height;
}

double region_features::get_max_dist(vector<Point> contour) {
	double max_dist = 0;
	double dist;
	for each(Point p1 in contour) {
		for each(Point p2 in contour) {
			dist = sqrt(pow((p1.x - p2.x), 2) + pow((p1.y - p2.y), 2));
			if (max_dist < dist)   max_dist = dist;
		}
	}
	return max_dist;
}

double region_features::getAnisometry(Mat mask) {
	double anisometry = -1;
	double ra = getRa(mask);
	double rb = getRb(mask);
	anisometry = ra / rb;
	return anisometry;
}

double region_features::getBulkiness(Mat mask) {
	double bulkiness = -1;
	double ra = getRa(mask);
	double rb = getRb(mask);
	double area = getArea(mask);
	bulkiness = ra * rb * 3.1415 / area;
	return bulkiness;
}

double region_features::getStructfactor(Mat mask) {
	double structfactor = -1;
	double anisometry = getAnisometry(mask);
	double bulkiness = getBulkiness(mask);
	structfactor = (anisometry * bulkiness) - 1;
	return structfactor;
}

double region_features::getOuterRadius(Mat mask) {
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

void region_features::find_convexity(Mat mask)
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
	float result = (F0 / FC);
	cout << result << endl;
}

double region_features::getInnerRadius(Mat mask)
{
	vector<Mat> channels;
	split(mask, channels);

	// On these lines, the input image is padded
	Mat gray(mask.rows + 20, mask.cols + 20, CV_8UC1);
	gray = 255;
	channels[0].copyTo(gray(Rect(10, 10, mask.cols, mask.rows)));

	vector<vector<Point> > contours;
	findContours(gray, contours, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);
	Mat drawing(gray.size(), CV_8UC3);
	Mat draw2 = drawing.clone();

	drawContours(drawing, contours, 0, Scalar(20, 175, 20), 1, CV_AA);

	double dist, maxdist = -1;

	for (int i = 0; i< mask.cols; i++)
	{
		for (int j = 0; j< mask.rows; j++)
		{

			dist = pointPolygonTest(contours[0], Point(i, j), true);
			if (dist > maxdist)
			{
				maxdist = dist;
			}
		}
	}
	return maxdist;

}



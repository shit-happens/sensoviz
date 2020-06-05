#include "stdafx.h"
#include "smoothing.h"
#include <opencv2/imgproc/imgproc.hpp>

Mat smoothing::bilateral_filter(Mat img, int d, double sigmaColor, double sigmaSpace) {
	cvtColor(img, img, CV_BGR2GRAY);
	Mat bilateral_image = Mat(img.rows, img.cols, CV_8UC1, Scalar(0));
	bilateralFilter(img, bilateral_image, d, sigmaColor, sigmaSpace, BORDER_DEFAULT);
	return bilateral_image;
}

Mat smoothing::gauss_filter(Mat img, Size ksize, double sigmaX, double sigmaY) {
	cvtColor(img, img, CV_BGR2GRAY);
	Mat gauss_image = Mat(img.rows, img.cols, CV_8UC1, Scalar(0));
	GaussianBlur(img, gauss_image, ksize, sigmaX, sigmaY, BORDER_DEFAULT);
	return gauss_image;
}

Mat smoothing::sigma_image(Mat img, int maskwidth, int maskheight, int Sigma) {
	cvtColor(img, img, CV_BGR2GRAY);
	Mat sigmaImg = Mat(img.rows, img.cols, CV_8UC1, Scalar(0));
	int halfwidth = int(maskwidth / 2);
	int halfheight = int(maskheight / 2);
	for (int i = 0; i < img.rows; i++) {
		for (int j = 0; j < img.cols; j++) {
			double mean = 0;
			double std = 0;
			double count = 0;
			for (int x = i - halfwidth; x <= i + halfwidth; x++) {
				for (int y = j - halfheight; y <= j + halfheight; y++) {
					if ((x < 0) || (x >= img.rows) || (y < 0) || (y >= img.cols))
						continue;
					mean += img.at<uchar>(x, y);
					count++;
				}
			}

			mean = mean / count;
			for (int x = i - halfwidth; x <= i + halfwidth; x++) {
				for (int y = j - halfheight; y <= j + halfheight; y++) {
					if ((x < 0) || (x >= img.rows) || (y < 0) || (y >= img.cols))
						continue;
					std += pow(abs(img.at<uchar>(x, y) - mean), 2);
				}
			}
			std = std / count;
			std = sqrt(std);

			double value = 0;
			int num = 0;
			for (int x = i - halfwidth; x <= i + halfwidth; x++) {
				for (int y = j - halfheight; y <= j + halfheight; y++) {
					if ((x < 0) || (x >= img.rows) || (y < 0) || (y >= img.cols))
						continue;
					if (int(abs(img.at<uchar>(x, y) - img.at<uchar>(i, j))) << int(Sigma*std)) {
						value = value + img.at<uchar>(x, y);
						num++;
					}
				}
			}
			sigmaImg.at<uchar>(i, j) = int(value / num);
		}
	}
	return sigmaImg;
}

Mat smoothing::median_image(Mat img, string masktype, int radius) {
	cvtColor(img, img, CV_BGR2GRAY);
	Mat medianImg = Mat(img.rows, img.cols, CV_8UC1, Scalar(0));
	if (masktype == "circle") {
		for (int i = 0; i < img.rows; i++) {
			for (int j = 0; j < img.cols; j++) {

				int count = 0;
				for (int x = i - radius; x <= i + radius; x++) {
					for (int y = j - radius; y <= j + radius; y++) {
						if ((x < 0) || (x >= img.rows) || (y < 0) || (y >= img.cols))
							continue;
						int dist = sqrt(pow(abs(x - i), 2) + pow(abs(y - j), 2));
						if (dist <= radius) {
							count++;
						}
					}
				}
				int idx = 0;
				int arr_size = count;
				int* arr = new int[arr_size] {-1};
				for (int x = i - radius; x <= i + radius; x++) {
					for (int y = j - radius; y <= j + radius; y++) {
						if ((x < 0) || (x >= img.rows) || (y < 0) || (y >= img.cols))
							continue;
						int dist = sqrt(pow(abs(x - i), 2) + pow(abs(y - j), 2));
						if (dist <= radius) {
							arr[idx] = int(img.at<uchar>(x, y));
							idx++;
						}
					}
				}

				sort(arr, arr + count);
				int median_idx = int(((count - 1) / 2) + 1);
				medianImg.at<uchar>(i, j) = int(arr[median_idx]);
			}
		}
		return medianImg;
	}

	else if (masktype == "square") {
		int edge = (2 * radius) + 1;
		int halfwidth = int(edge / 2);
		int halfheight = int(edge / 2);
		for (int i = 0; i < img.rows; i++) {
			for (int j = 0; j < img.cols; j++) {
				int count = 0;
				for (int x = i - halfwidth; x <= i + halfwidth; x++) {
					for (int y = j - halfheight; y <= j + halfheight; y++) {
						if ((x < 0) || (x >= img.rows) || (y < 0) || (y >= img.cols))
							continue;
						count++;
					}
				}

				int idx = 0;
				int arr_size = count;
				int* arr = new int[arr_size] {-1};
				for (int x = i - halfwidth; x <= i + halfwidth; x++) {
					for (int y = j - halfheight; y <= j + halfheight; y++) {
						if ((x < 0) || (x >= img.rows) || (y < 0) || (y >= img.cols))
							continue;
						arr[idx] = int(img.at<uchar>(x, y));
						idx++;
					}
				}

				sort(arr, arr + count);
				int median_idx = int(((count - 1) / 2) + 1);
				medianImg.at<uchar>(i, j) = int(arr[median_idx]);

			}
		}
		return medianImg;
	}
}

Mat smoothing::median_rect(Mat img, int maskwidth, int maskheight) {
	int halfwidth = int(maskwidth / 2);
	int halfheight = int(maskheight / 2);
	Mat medianImg = Mat(img.rows, img.cols, CV_8UC1, Scalar(0));
	for (int i = 0; i < img.rows; i++) {
		for (int j = 0; j < img.cols; j++) {
			int count = 0;
			for (int x = i - halfwidth; x <= i + halfwidth; x++) {
				for (int y = j - halfheight; y <= j + halfheight; y++) {
					if ((x < 0) || (x >= img.rows) || (y < 0) || (y >= img.cols))
						continue;
					count++;
				}
			}

			int idx = 0;
			int arr_size = count;
			int* arr = new int[arr_size] {-1};
			for (int x = i - halfwidth; x <= i + halfwidth; x++) {
				for (int y = j - halfheight; y <= j + halfheight; y++) {
					if ((x < 0) || (x >= img.rows) || (y < 0) || (y >= img.cols))
						continue;
					arr[idx] = int(img.at<uchar>(x, y));
					idx++;
				}
			}

			sort(arr, arr + count);
			int median_idx = int(((count - 1) / 2) + 1);
			medianImg.at<uchar>(i, j) = int(arr[median_idx]);

		}
	}
	return medianImg;
}

Mat smoothing::median_seperate(Mat img, int maskwidth, int maskheight) {
	Mat img1 = img.clone();
	cvtColor(img1, img1, CV_BGR2GRAY);
	Mat imgH = median_rect(img1, maskwidth, 1);
	Mat imgHV = median_rect(imgH, 1, maskheight);

	Mat img2 = img.clone();
	cvtColor(img2, img2, CV_BGR2GRAY);
	Mat imgV = median_rect(img2, 1, maskheight);
	Mat imgVH = median_rect(imgV, maskwidth, 1);

	Mat medianImg = Mat(img.rows, img.cols, CV_8UC1, Scalar(0));
	for (int i = 0; i < img.rows; i++) {
		for (int j = 0; j < img.cols; j++) {
			medianImg.at<uchar>(i, j) = int(((imgVH.at<uchar>(i, j)) + (imgHV.at<uchar>(i, j))) / 2);
		}
	}

	return medianImg;
}

Mat smoothing::mean_image(Mat img, int maskwidth, int maskheight) {
	cvtColor(img, img, CV_BGR2GRAY);
	Mat meanImg = Mat(img.rows, img.cols, CV_8UC1, Scalar(0));

	int halfwidth = int(maskwidth / 2);
	int halfheight = int(maskheight / 2);
	for (int i = 0; i < img.rows; i++) {
		for (int j = 0; j < img.cols; j++) {
			double count = 0;
			double sum = 0;
			for (int x = i - halfwidth; x <= i + halfwidth; x++) {
				for (int y = j - halfheight; y <= j + halfheight; y++) {
					if ((x < 0) || (x >= img.rows) || (y < 0) || (y >= img.cols))
						continue;
					sum += img.at<uchar>(i, j);
					count++;
				}
			}
			meanImg.at<uchar>(i, j) = int(sum / count);
		}
	}
	return meanImg;
}

Mat smoothing::mean_sp(Mat img, int maskwidth, int maskheight, int MinThresh, int MaxThresh) {
	cvtColor(img, img, CV_BGR2GRAY);
	Mat meanImg = Mat(img.rows, img.cols, CV_8UC1, Scalar(0));

	int halfwidth = int(maskwidth / 2);
	int halfheight = int(maskheight / 2);
	for (int i = 0; i < img.rows; i++) {
		for (int j = 0; j < img.cols; j++) {
			double count = 0;
			double sum = 0;
			for (int x = i - halfwidth; x <= i + halfwidth; x++) {
				for (int y = j - halfheight; y <= j + halfheight; y++) {
					if ((x < 0) || (x >= img.rows) || (y < 0) || (y >= img.cols))
						continue;
					if (int(img.at<uchar>(x, y)) > MinThresh && int(img.at<uchar>(x, y)) < MaxThresh) {
						sum += img.at<uchar>(i, j);
						count++;
					}
				}
			}

			if (count == 0) {
				meanImg.at<uchar>(i, j) = int(img.at<uchar>(i, j));
			}
			else {
				meanImg.at<uchar>(i, j) = int(sum / count);
			}
		}
	}
	return meanImg;
}



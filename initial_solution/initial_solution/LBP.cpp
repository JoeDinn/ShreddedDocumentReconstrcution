#include "stdafx.h"
#include "LBP.h"

std::vector<double> LBP::get_values(cv::Mat & image)
{
	std::vector<double> hist(256);
	for (int i = 1; i<image.rows - 1; i++) {
		for (int j = 1; j<image.cols - 1; j++) {
			uchar center = image.at<uchar>(i, j);
			unsigned char code = 0;
			code |= (image.at<uchar>(i - 1, j - 1) > center) << 7;
			code |= (image.at<uchar>(i - 1, j) > center) << 6;
			code |= (image.at<uchar>(i - 1, j + 1) > center) << 5;
			code |= (image.at<uchar>(i, j + 1) > center) << 4;
			code |= (image.at<uchar>(i + 1, j + 1) > center) << 3;
			code |= (image.at<uchar>(i + 1, j) > center) << 2;
			code |= (image.at<uchar>(i + 1, j - 1) > center) << 1;
			code |= (image.at<uchar>(i, j - 1) > center) << 0;
			hist[code] ++;
		}
	}
	return hist;
}

LBP::LBP()
	:Filter(256)

{
}


LBP::~LBP()
{
}

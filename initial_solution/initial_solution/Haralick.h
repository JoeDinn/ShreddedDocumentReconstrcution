#pragma once
#include "Filter.h"
#include "haralick_feat.h"

class Haralick :
	public Filter
{
public:
	std::vector<double> get_values(cv::Mat &image);
	Haralick();
	~Haralick();
};


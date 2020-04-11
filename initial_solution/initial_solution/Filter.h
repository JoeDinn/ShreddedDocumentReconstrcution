#pragma once
#include <vector>
#include "utilities.h"
class Filter
{
public:
	int number_filters;
	virtual std::vector<double>get_values(cv::Mat &image) =0;
	Filter(int number_filters_) :number_filters(number_filters_) {};
	~Filter();
};


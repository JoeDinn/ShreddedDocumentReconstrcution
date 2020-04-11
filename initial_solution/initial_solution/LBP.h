#pragma once
#include "Filter.h"
class LBP :
	public Filter
{
public:
	std::vector<double> get_values(cv::Mat & image);
	LBP();
	~LBP();
};


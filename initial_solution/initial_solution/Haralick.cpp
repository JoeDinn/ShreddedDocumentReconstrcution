#include "stdafx.h"
#include "Haralick.h"


std::vector<double> Haralick::get_values(cv::Mat &image)
{
	HaralickExtractor extract;
	if (image.channels() > 1)
	{
		cv::cvtColor(image, image, cv::COLOR_BGR2GRAY);
	}
	
	std::vector<int> deltax({ 1 });
	std::vector<int> deltay({ 0 });
	return extract.getFeaturesFromImage(image, deltax, deltay);
}

Haralick::Haralick():
	Filter(12)
{
}


Haralick::~Haralick()
{
}

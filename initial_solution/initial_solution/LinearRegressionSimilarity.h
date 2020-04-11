#pragma once
#include "Similarity.h"
#include "Filter.h"
#include "alglib/dataanalysis.h"
#include <fstream>//For writing files

class LinearRegressionSimilarity :
	public Similarity
{
	alglib::linearmodel model;
	Filter &filter;
public:
	std::vector<double> process(Fragment & left_image, Fragment & right_image);
	long double cost(Fragment & left_image, Fragment & right_image);
	LinearRegressionSimilarity(Filter &filter_, std::string dataset);
	~LinearRegressionSimilarity();
};


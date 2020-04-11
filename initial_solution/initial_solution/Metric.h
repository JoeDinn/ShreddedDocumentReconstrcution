#pragma once
#include "utilities.h"
class Metric
{
public:
	Metric();
	virtual double evaluate(Solution &solution, GroundTruth &ground_truth)= 0;
	~Metric();
	bool correct_allignment(Fragment &left_fragment, Fragment &right_fragment, GroundTruth & ground_truth);
	int number_correct_pairs(Solution &solution, GroundTruth &ground_truth);
};


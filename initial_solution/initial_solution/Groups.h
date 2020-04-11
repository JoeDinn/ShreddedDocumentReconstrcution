#pragma once
#include "Metric.h"
class Groups :
	public Metric
{
	int ngroups(Solution &solution, GroundTruth &ground_truth);
public:
	double evaluate(Solution &solution, GroundTruth &ground_truth);
	Groups();
	~Groups();
};


#pragma once
#include "utilities.h"
#include "Optimiser.h"
class SimAneal :
	public Optimiser
{
	std::string left_most_name;
	double initial_temperature;
	int temperature_length;
	double cooling_factor;
	int num_non_improve;
public:
	SimAneal(Similarity &similarity_, double initial_temperature_ = 0.1, int temperature_length_ = 10000, double cooling_factor_ = 0.00001, int num_non_improve_ = 100000000);
	~SimAneal();
	Solution optimise(Solution fragments);
};
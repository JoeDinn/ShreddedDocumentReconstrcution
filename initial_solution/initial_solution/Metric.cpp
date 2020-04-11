#include "stdafx.h"
#include "Metric.h"
//#define DEBUG

Metric::Metric()
{
}


Metric::~Metric()
{
}

bool Metric::correct_allignment(Fragment & left_fragment, Fragment & right_fragment, GroundTruth & ground_truth)
{

	for (int index = 0; index < ground_truth.size() - 1; ++index)
	{
		if (ground_truth[index] == left_fragment.name)
		{
			return ground_truth[index + 1] == right_fragment.name;
		}
	}
	return false;//If true position is last item (or doesnt exist)
}

int Metric::number_correct_pairs(Solution & solution, GroundTruth & ground_truth)
{
	assert(solution.size() == ground_truth.size());
	
	int number_correct_pairs{};
	for (int left_index = 0; left_index < solution.size() - 1; left_index++)
	{
		number_correct_pairs += correct_allignment(solution[left_index], solution[left_index + 1], ground_truth);
	}
#ifdef DEBUG
	std::cout << "number correct pairs: " << number_correct_pairs << std::endl;
#endif // DEBUG
	return number_correct_pairs;
}

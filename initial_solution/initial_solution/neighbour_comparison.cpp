#include "stdafx.h"
#include "neighbour_comparison.h"
//#define DEBUG

double neighbour_comparison::evaluate(Solution & solution, GroundTruth & ground_truth)
{
	//NC = #{correct pairs}/(n-1) (from Reassembling Shredded Document Stripes Using Word-path Metric and Greedy Composition Optimal Matching Solver)
#ifdef DEBUG
	std::cout << number_correct_pairs(solution, ground_truth) << std::endl;
	std::cout << (solution.size() - 1) << std::endl;
	std::cout << number_correct_pairs(solution, ground_truth) / (solution.size() - 1) << std::endl;
#endif // DEBUG
	return ((double)number_correct_pairs(solution,ground_truth)) / (solution.size() - 1);
}



neighbour_comparison::neighbour_comparison()
{
}


neighbour_comparison::~neighbour_comparison()
{
}

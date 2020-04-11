#include "stdafx.h"
#include "SimilarityTester.h"
#include <fstream>//For writing files
#include <vector>
#define DEBUG
typedef std::pair<std::string, long double> Fragment_cost;
double SimilarityTester::test(Solution &correct_solution, Similarity & similarity)
{
	double total{ 0 };
	for (int index = 0; index < (correct_solution.size() - 1); index++)
	{
		std::vector<Fragment_cost> costs{};
		//iterate over each item
		for (int next = 0; next < correct_solution.size();  next++)
		{
			if (next != index)
			{//calculate cost
				costs.emplace_back(Fragment_cost(correct_solution[next].name,similarity.compare(correct_solution[index], correct_solution[next])));
			}
		}
		//sort costs
		std::sort(costs.begin(), costs.end(), [](auto &left, auto &right) {
			return left.second < right.second;
		});
		//output position of correct next item
		for (int i=0; i< (costs.size()-1); ++i)
		{
			if (costs[i].first == correct_solution[index + 1].name)
			{
				total += i;
#ifdef DEBUG
				std::cout << correct_solution[index].name << " Correct match at position: " << i << " Cost: " << costs[i].second << " Best match:" << costs[0].first << " With score: " << costs[0].second << std::endl;
#endif // DEBUG
			}
		}
	}
#ifdef DEBUG
	std::cout << "Average: " << total/(correct_solution.size() - 1) << std::endl;
#endif // DEBUG
	return total / (correct_solution.size() - 1);
}

SimilarityTester::SimilarityTester()
{
}


SimilarityTester::~SimilarityTester()
{
}

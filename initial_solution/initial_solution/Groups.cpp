#include "stdafx.h"
#include "Groups.h"


int Groups::ngroups(Solution & solution, GroundTruth & ground_truth)
{
	//ngroups(x) = n-(sum{x(i+1) == x(i) + 1})
//	int number_groups{ solution.size() - number_correct_pairs(solution,ground_truth) };

}

double Groups::evaluate(Solution & solution, GroundTruth & ground_truth)
{
	//accuracy(x) = (n-ngroups(x))/n-1
	return (solution.size()- ngroups(solution,ground_truth))/(solution.size()-1);
}

Groups::Groups()
{
}


Groups::~Groups()
{
}

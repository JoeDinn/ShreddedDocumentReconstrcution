#pragma once
#include "utilities.h"
#include "Similarity.h"
class SimilarityTester
{
public:
	double test(Solution &correct_solution,Similarity &similarity);
	SimilarityTester();
	~SimilarityTester();
};


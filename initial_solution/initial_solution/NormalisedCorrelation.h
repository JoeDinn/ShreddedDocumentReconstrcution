#pragma once
#include "Similarity.h"
class NormalisedCorrelation :
	public Similarity
{
public:
	long double cost(Fragment & left_image, Fragment & right_image);
	NormalisedCorrelation();
	~NormalisedCorrelation();
};


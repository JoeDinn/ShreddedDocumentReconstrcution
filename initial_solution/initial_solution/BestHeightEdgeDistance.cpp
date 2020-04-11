#include "stdafx.h"
#include "BestHeightEdgeDistance.h"


long double BestHeightEdgeDistance::cost(Fragment & left_image, Fragment & right_image)
{
	//Iterate through each possibility and return index of best result.
	int best_right_start{};
	double best_score{ std::numeric_limits<double>::max() };

	for (int right_start = -floor(left_image.image.cols / 4); right_start < floor(left_image.image.cols / 4); right_start++)
	{
		double score{ 0 };
		int total_rows = MIN(left_image.image.rows, right_image.image.rows) - abs(right_start);
		for (int row = 0; row < total_rows; row++)
		{
			int left_row = MAX(row, row + right_start);
			int right_row = MAX(row, row - right_start);
			score += abs(left_image(left_row, left_image.last_pixel[left_row] - 1) - right_image(right_row, right_image.first_pixel[right_row]));
		}
		score = score / total_rows;
		if (score < best_score)
		{
			best_score = score;
			best_right_start = right_start;
		}
	}
	return best_score;
}

BestHeightEdgeDistance::BestHeightEdgeDistance() :
	edge_distance()
{
}


BestHeightEdgeDistance::~BestHeightEdgeDistance()
{
}

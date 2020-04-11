#include "stdafx.h"
#include "NormalisedCorrelation.h"
//define DEBUG

long double NormalisedCorrelation::cost(Fragment & left_image, Fragment & right_image)
{
#ifdef DEBUG
	std::cout << left_image.name << " " << right_image.name << std::endl;
#endif // DEBUG


		//Iterate through each possibility and best score
	int best_right_start{};
	long double best_score{ std::numeric_limits<long double>::max() };

	for (int right_start = -floor(left_image.image.rows / 4); right_start < floor(left_image.image.rows / 4); right_start++)
	{
		long double score{ 0 };
		int total_rows = MIN(left_image.image.rows,right_image.image.rows) - abs(right_start);

		//Compute left mean
		long double left_mean{ 0 };
		for (int row = 0; row < total_rows; row++)
		{
			int left_row = MAX(row, row + right_start);
			left_mean += left_image(left_row, left_image.last_pixel[left_row] - 1);
		}
		left_mean = left_mean / total_rows;
		//Compute right mean
		long double right_mean{ 0 };
		for (int row = 0; row < total_rows; row++)
		{
			int right_row = MAX(row, row - right_start);
#ifdef DEBUG
			if (right_image.name == "D01911")
			{
				std::cout << row << " " << right_row << " " << std::endl;
				std::cout << right_image(right_row, right_image.first_pixel[right_row]) << std::endl;
			}
			
#endif // DEBUG			
			right_mean += right_image(right_row, right_image.first_pixel[right_row]);
		}
		right_mean = right_mean / total_rows;

		//Compute top of equation
		for (int row = 0; row < total_rows; row++)
		{
			int left_row = MAX(row, row + right_start);
			int right_row = MAX(row, row - right_start);

			long double left_top = (left_image(left_row, left_image.last_pixel[left_row] - 1) - left_mean);
			long double right_top = (right_image(right_row, right_image.first_pixel[right_row]) - right_mean);

			score += left_top * right_top;
		}

		//Compute bottom
		long double left_bottom{ 0 };
		long double right_bottom{ 0 };
		for (int row = 0; row < total_rows; row++)
		{
			int left_row = MAX(row, row + right_start);
			int right_row = MAX(row, row - right_start);

			left_bottom += pow(left_image(left_row, left_image.last_pixel[left_row] - 1) - left_mean,2);
			right_bottom += pow(right_image(right_row, right_image.first_pixel[right_row]) - right_mean,2);
		}
		

		//std::cout << "mean l:  " << left_mean << " mean r: " << right_mean <<  " top: " << score << " Bottom l: " << left_bottom << " Bottom r: " <<  right_bottom << std::endl;
		
		score = 1-( score / sqrt(left_bottom * right_bottom));
		//std::cout << "score: " << score << std::endl;
		if (score < best_score)
		{
			best_score = score;
			best_right_start = right_start;
		}
	}
	return best_score;
}

NormalisedCorrelation::NormalisedCorrelation()
{
}


NormalisedCorrelation::~NormalisedCorrelation()
{
}
/*int best_height_match(Fragment & left_image, Fragment & right_image,long double *cost)
{
	
}*/
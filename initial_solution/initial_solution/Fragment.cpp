#include "stdafx.h"
#include "Fragment.h"
#include <iostream>
#include <fstream>

void Fragment::get_row_average()
{
	row_average = std::vector<double>(image.rows);
	for (int row = 0; row < image.rows; row++)
	{
		double current_row_total{ 0 };
		for (int col = first_pixel[row]; col < last_pixel[row]; col++)
		{
			current_row_total += (*this)(row, col);
		}
		row_average[row] = current_row_total / (last_pixel[row] - first_pixel[row]);
	}
}

Fragment::Fragment(std::string root_,std::string name_):
<<<<<<< Updated upstream
	name(name_), image(cv::imread(root_ + "/strips/" + name_ + ".png", cv::IMREAD_GRAYSCALE)), total_pixels() 
=======
	name(name_), image(cv::imread(root_ + "/strips/" + name_ + ".png", cv::IMREAD_GRAYSCALE)) ,is_thresholded(false),centre()
>>>>>>> Stashed changes

{
	cv::Mat mask{ cv::imread(root_ + "/masks/" + name_ + ".png", cv::IMREAD_GRAYSCALE) };
	first_pixel = std::vector<int>(image.rows);
	last_pixel = std::vector<int>(image.rows);
	if (!mask.empty())//If mask found
	{
		//Find first and last pixels
		for (int i{}; i < image.rows; ++i)
		{
			bool find_first{ true };
			bool find_last{ false };
			for (int j{}; j < image.cols; ++j)
			{
				uchar pixel_value{ mask.at<uchar>(i, j) };
				switch (pixel_value)
				{
				case 255:
					if (find_first)
					{
						first_pixel[i] = j;
						find_first = false;
						find_last = true;
					}
					break;
				case 0:
					if (find_last)
					{
						last_pixel[i] = j;
						find_last = false;
					}
					break;
				default:
					break;
				}
			}
			if (find_last)
			{
				last_pixel[i] = image.cols+1;
			}

		}
	}
	else//assume whole image
	{
		for (int i{}; i < image.rows; ++i)
		{
			first_pixel[i] = 0;
			last_pixel[i] = image.cols+1;
		}
	}
	//Get average for each row
	get_row_average();

}

Fragment::Fragment(const Fragment & fragment) :
<<<<<<< Updated upstream
	image(fragment.image), name(fragment.name), first_pixel(fragment.first_pixel), last_pixel(fragment.last_pixel), total_pixels()
=======
	image(fragment.image), name(fragment.name), first_pixel(fragment.first_pixel), last_pixel(fragment.last_pixel),centre(fragment.centre) , is_thresholded(false),row_average(fragment.row_average)
{
}

Fragment::Fragment(cv::Mat image_, std::string name_, std::vector<int> first_pixel_, std::vector<int> last_pixel_, std::vector<int> centre_):
	image(image_), name(name_), first_pixel(first_pixel_), last_pixel(last_pixel_),centre(centre_) , is_thresholded(false)
>>>>>>> Stashed changes
{
	get_row_average();
}


<<<<<<< Updated upstream
Fragment::Fragment() :
	total_pixels()
{
}

=======
Fragment::Fragment() :is_thresholded(false), first_pixel(), last_pixel(), centre()
{
}

double Fragment::average()
{
	double sum{ 0 };
	int pixel_total{ 0 };
	for (int row = 0; row < image.rows; row++)
	{
		for (int col = first_pixel[row]; col < last_pixel[row]; col++)
		{
			pixel_total++;
			sum += (*this)(row, col);
		}
	}
	return sum / pixel_total;

}

Fragment Fragment::get_thresholded()
{
	if (not(is_thresholded))
	{
		threshold();
	}
	
	return Fragment(thresholded,name,first_pixel,last_pixel,centre);
}

>>>>>>> Stashed changes


void Fragment::threshold()
{
	int hist[256]{};
	colour_histogram(hist);
	
		int total_pixels{};
		for (int i{}; i < 256; ++i)
		{
			total_pixels += hist[i];
		}


		//Get threshold using otsu

		int best_threshold{};
		int top = 256;
		int sum_lower = 0;
		unsigned long long weight_lower = 0;//Probability of lower class
		unsigned long long maximum_intra_class_var = 0;
		long double sum1{};
		for (int i = 0; i < top; i++) sum1 += (i * hist[i]);


		for (int current_threshold = 0; current_threshold < top; ++current_threshold)
		{
			unsigned long long weight_upper = total_pixels - weight_lower;
			if (weight_lower > 0 and weight_upper > 0)
			{
				long double mean_upper{ (sum1 - sum_lower) / weight_upper };
				long double mean_lower{ (double)sum_lower / weight_lower };

				

				unsigned long long intra_class_var = weight_lower * weight_upper * pow(mean_lower - mean_upper, 2);



				if (intra_class_var >= maximum_intra_class_var)
				{
					best_threshold = current_threshold;
					maximum_intra_class_var = intra_class_var;
				}
			}
			weight_lower = weight_lower + hist[current_threshold];//Increment probability of lower class
			sum_lower = sum_lower + (current_threshold *  hist[current_threshold]);

		}

	

	//threshold

	for (int i{}; i < image.rows; ++i)
	{
		for (int j{ first_pixel[i] }; j < last_pixel[i]; ++j)
		{
			(*this)(i, j) = ((int)(*this)(i, j) < best_threshold) ? 0 : 255;

		}
	}

}



void Fragment::colour_histogram(int hist[256])
{

	//get pixel values in each row
	for (int row{}; row < image.rows; ++row)
	{
		for (int col{ first_pixel[row] }; col < last_pixel[row]; ++col)
		{
			++hist[(int)(*this)(row, col)];
		}
		
	}
	
}

void Fragment::get_text_lines(bool *is_text_line)
{
	for (int row{}; row < image.rows; ++row)
	{
		is_text_line[row] = false;
		for (int col{ first_pixel[row] }; col < last_pixel[row]; ++col)
		{
			if ((int)(*this)(row, col) == 0)
			{
				is_text_line[row] = true;
				//std::cout << name  << ": Row " << row << "Has pixels (first at: " << col << " )" << std::endl;
				break;
			}
		}
	}
}


Fragment::~Fragment()
{

}

uchar & Fragment::operator()(int row, int col)
{
	return image.at<uchar>(row, col);
}


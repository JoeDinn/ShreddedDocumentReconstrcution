#include "stdafx.h"
#include "utilities.h"
<<<<<<< Updated upstream
#define DEBUG

=======
#include <limits>
#include <fstream>//For writing files
//#define DEBUG
>>>>>>> Stashed changes
void output(Solution &solution)
{
	std::cout << std::endl << "Size" << solution.size() << std::endl;
	std::vector<cv::Mat> images(solution.size());
<<<<<<< Updated upstream
	for (int i{ 0 }; i < solution.size(); ++i)
=======
	int most_rows{ 0 };
	for (int i{ 0 }; i < solution.size(); ++i)
	{
		most_rows = MAX(solution[i].image.rows, most_rows);
		images[i] = solution[i].image;
	}//Pad if necessary
	for (int i{ 0 }; i < solution.size(); ++i)
	{
		if (images[i].rows < most_rows) {
			cv::Mat pad(cv::Mat::zeros(most_rows - images[i].rows, images[i].cols, images[i].type()));
			cv::vconcat(images[i], pad, images[i]);
		}
		
	}

	cv::Mat combined;
	
	

	cv::hconcat(images, combined);
	cv::imwrite("D:/ocr/output.png", combined);
	cv::namedWindow("Display window", cv::WINDOW_NORMAL); // Create a window for display.
	cv::imshow("Display window", combined); // Show our image inside it.
	cv::waitKey(0); // Wait for a keystroke in the window
}
void stitch(Solution &solution)
{
	std::cout << std::endl << "Size" << solution.size() << std::endl;
	Fragment image(solution[0]);
	for (int i{ 1 }; i < solution.size(); ++i)
>>>>>>> Stashed changes
	{
		images[i] = solution[i].img;
	}

<<<<<<< Updated upstream

	cv::Mat combined;
	cv::hconcat(images, combined);
	cv::namedWindow("Display window", cv::WINDOW_NORMAL); // Create a window for display.
	cv::imshow("Display window", combined); // Show our image inside it.
	cv::waitKey(0); // Wait for a keystroke in the window
=======
	cv::imwrite("D:/ocr/stitched.png", image.image);
	cv::namedWindow("Display window", cv::WINDOW_NORMAL); 
	cv::imshow("Display window", image.image); 
	cv::waitKey(); 
>>>>>>> Stashed changes
}

void log(Solution &solution)
{
	for(Fragment fragment:solution)
	{
		std::cout << ", " << fragment.name;
	}
	std::cout << std::endl;
}

void shuffle(Solution & solution)
{
	for (int i = 0; i < solution.size()-1; ++i)
	{
		int j{ random::rand_int(i,solution.size()-1)};
		solution.swap(i, j);
#ifdef DEBUG
		std::cout << i << " <--> " << j << std::endl;
#endif // DEBUG
	}
#ifdef DEBUG
	std::cout << "done\n";
#endif // DEBUG
}



int left_most(Solution & solution)
{
	int left_most_index{};
	double  best_whiteness{-100};
	for (int index{}; index < solution.size(); ++index)
	{
		double whiteness{ avg_first(solution[index])};//whiteness_score
#ifdef DEBUG
		std::cout << solution[index].name << " " << whiteness << std::endl;
#endif // DEBUG
		if (whiteness > best_whiteness)
		{
			left_most_index = index;
			best_whiteness = whiteness;
		}
	}
#ifdef DEBUG
	std::cout << "Best: " << solution[left_most_index].name << " " << best_whiteness << std::endl;
#endif // DEBUG
	return left_most_index;
}

<<<<<<< Updated upstream
long long int whiteness_score(Fragment & fragment)
{
	long long int whiteness{};
	long int dim{};
	for (int i{}; i < fragment.img.rows; ++i)
=======

Fragment combine(Fragment &left_image, Fragment &right_image, int right_start)
{
	int max_cols{};

	int total_rows = MIN(left_image.image.rows, right_image.image.rows) - abs(right_start);
#ifdef DEBUG
	std::cout << "total rows: " << total_rows << std::endl;
#endif // DEBUG
	for (int row = 0; row < total_rows; row++)
	{
		int left_row = MAX(row, row + right_start);
		int right_row = MAX(row, row - right_start);
		max_cols = MAX(left_image.last_pixel[left_row] + right_image.last_pixel[right_row] - right_image.first_pixel[right_row], max_cols);
	}		
#ifdef DEBUG
	std::cout << "Max cols: " << max_cols << std::endl;
#endif // DEBUG
	std::vector<int> first_pixel(total_rows);
	std::vector<int> last_pixel( total_rows );
	std::vector<int> centre( total_rows );
	cv::Mat combined_image(total_rows, max_cols, left_image.image.type());
	combined_image = 0;

	for (int row = 0; row < total_rows; row++)
	{
		int left_row = MAX(row, row + right_start);
		int right_row = MAX(row, row - right_start);

		first_pixel[row] = left_image.first_pixel[left_row];
		last_pixel[row] = (left_image.last_pixel[left_row] + right_image.last_pixel[right_row]) - right_image.first_pixel[right_row];
		centre[row] = left_image.last_pixel[left_row];

		for (int col = 0; col < left_image.last_pixel[left_row]; col++)
		{
			combined_image.at<uchar>(row, col) = left_image(left_row, col);
		}
			
		for (int col = right_image.first_pixel[right_row]; col < right_image.last_pixel[right_row]; col++)
		{
			combined_image.at<uchar>(row, col + (left_image.last_pixel[left_row] - right_image.first_pixel[right_row])) = right_image(right_row, col);
		}
		
	}
	return Fragment(combined_image, left_image.name + right_image.name, first_pixel, last_pixel, centre);
}

Fragment combine_whole(Fragment &left_image, Fragment &right_image, int right_start)
{
	cv::Mat combined_image;
	cv::Mat left_padded(left_image.image.rows + abs(right_start), left_image.image.cols , left_image.image.type());
	cv::Mat right_padded(left_image.image.rows + abs(right_start), right_image.image.cols, left_image.image.type());
	cv::Mat pad = cv::Mat::zeros(abs(right_start), left_image.image.cols, left_image.image.type());    // 3 cols, 4 rows
	if (right_start > 0)//left image starts at 0
	{
		//Pad left below
		cv::vconcat(left_padded, pad, left_padded);
		//Pad right above
		cv::vconcat(pad, right_padded, right_padded);
	}
	else if (right_start < 0)//Right image starts above
	{
		//Pad left above
		cv::vconcat(pad, left_padded, left_padded);
		//Pad right below
		cv::vconcat(right_padded, pad, right_padded);
	}
	else
	{
		return combine(left_image,right_image);
	}
	cv::hconcat(left_padded, right_padded, combined_image);


	return Fragment(combined_image, left_image.name + right_image.name, left_image.first_pixel, left_image.last_pixel);//Last and first dont matter
}


int best_height_match(Fragment & left_image, Fragment & right_image,long double *cost)
{
	//Iterate through each possibility and return index of best result.
	int best_right_start{};
	double best_score{ std::numeric_limits<double>::max() };

	for (int right_start = -floor(left_image.image.cols/4); right_start < floor(left_image.image.cols / 4); right_start++)
	{
		double score{0};
		int total_rows = MIN(left_image.image.rows, right_image.image.rows) - abs(right_start);
		for (int row = 0; row < total_rows; row++)
		{
			int left_row = MAX(row, row + right_start);
			int right_row = MAX(row, row - right_start);
			//std::cout << left_image.name << " " << right_image.name << "  " <<  left_row << " " << right_row;
			score += abs(left_image.row_average[left_row] - right_image.row_average[right_row]);
			//score += abs(left_image(left_row, left_image.last_pixel[left_row]-1) - right_image(right_row,right_image.first_pixel[right_row]));
		}
		score = score / total_rows;
		if (score < best_score)
		{
			best_score = score;
			best_right_start = right_start;
		}
	}
	if(cost) *cost = best_score;
	return best_right_start;
}

Solution load(std::string root, std::string dir)
{
	std::ifstream file(root + "/" + dir + ".txt");
	std::string str;
	std::getline(file, str);
	
	int number_files = std::stoi(str);

	
	Solution images(number_files);
	int image_counter = 0;
	while (std::getline(file, str))
	{
		images[image_counter] =  Fragment(root , str) ; // read the file
		if (images[image_counter].image.empty() or images[image_counter].average() > 254) // if invalid just overwrite
		{
			//delete?
			std::cout << "could not open or find the image" << std::endl;
		}
		else ++image_counter;
	}
	if (image_counter < number_files)
	{
		images.resize(image_counter);
	}
	return images;
}

Pix * mat_to_pix(cv::Mat & image)
{

	Pix *pixd = pixCreate(image.size().width, image.size().height, 8);
	for (int y = 0; y<image.rows; y++) {
		for (int x = 0; x<image.cols; x++) {
			pixSetPixel(pixd, x, y, (l_uint32)image.at<uchar>(y, x));
		}
	}
	return pixd;

}




int binarise(int value) { return (value != 255); }
double avg_first(Fragment & fragment)
{
	Fragment thresholded{ fragment.get_thresholded() };
	double row_count{ 0 };
	double avg_first_index{ 0 };
	for (int row = 0; row < fragment.image.rows; row++)
	{
		bool found{ false };
		for (int col = fragment.first_pixel[row]; col < fragment.last_pixel[row] and not(found); ++col)
		{
			if (binarise(thresholded(row, col)))//If 1
			{
#ifdef DEBUG
				std::cout <<  row<< ": " <<  (double)col - fragment.first_pixel[row]  << " " << ((double)col - fragment.first_pixel[row]) / (fragment.last_pixel[row] - fragment.first_pixel[row]) <<std::endl;
#endif // DEBUG
				avg_first_index += ((double)col - fragment.first_pixel[row]) / (fragment.last_pixel[row] - fragment.first_pixel[row]);
				found = true;
				row_count++;
			}
		}
	}
#ifdef DEBUG
	std::cout << avg_first_index/ row_count << std::endl;
#endif // DEBUG
	return avg_first_index / row_count;
}
//Convert 255 to 0 and 0 to 1
double centroid_x(Fragment & fragment)
{
	
	//m(1,0)/m(0,0)
	//sum(sum(x*f(x,y)))/ sum(sum(f(x,y)))
	Fragment  thresholded{ fragment.get_thresholded() };
	//Get total
	int m_0_0{ 0 };
	double m_1_0{ 0 };
	for (int row = 0; row < thresholded.image.rows; row++)
	{
		for (int col = thresholded.first_pixel[row]; col < thresholded.last_pixel[row]; col++)
		{
			m_0_0 += binarise(thresholded(row, col));
			m_1_0 += binarise(thresholded(row, col)) * (row - thresholded.first_pixel[row]);
		}
	}
	std::cout << fragment.name << " " << m_1_0 / m_0_0 << std::endl;
	return m_1_0 / m_0_0;


}

double whiteness_score(Fragment & fragment)
{
	long int total_pixels{ 0 };
	//Get total number of pixels and average pixel value
	long long unsigned int total_value{0};
	for (int i{}; i < fragment.image.rows; ++i)
>>>>>>> Stashed changes
	{
		for (int j{ fragment.first_pixel[i] }; j < fragment.last_pixel[i]; ++j)
		{
			total_value += fragment(i, j);
		}
		total_pixels += (fragment.last_pixel[i] - fragment.first_pixel[i]);
	}
	double mean = total_value / total_pixels;
	std::cout << total_pixels << " " << total_value << " " << mean << std::endl;
	//cv::Mat whiteness_image(fragment.image.rows, fragment.image.cols, fragment.image.type());/////
	long double whiteness{};
	for (int i{}; i < fragment.image.rows; ++i)
	{
		int row_pixels = fragment.last_pixel[i] - fragment.first_pixel[i];
		for (int j{ fragment.first_pixel[i] }; j < fragment.last_pixel[i]; ++j)
		{
			whiteness += (((long double)fragment(i, j) )  * (fragment.last_pixel[i] - j) / row_pixels ) / total_value;
			//whiteness_image.at<uchar>(i, j) = ((long double)fragment(i, j)) *pow((fragment.last_pixel[i] - j) / row_pixels, 1);
			//std::cout << ((long double)(long double)fragment(i, j) *(fragment.last_pixel[i] - j) / row_pixels) << " ";
			//std::cout <<  "(" << (int)fragment(i, j) << ","  << (fragment.last_pixel[i] - j) << ">" << ((int)fragment(i, j) * (fragment.last_pixel[i] - j)) / row_dim << ") ";
		}
		//std::cout << std::endl;
		//std::cout << i << " " << total_pixels << " " << whiteness <<  std::endl;
	}
	//std::cout << fragment.name << " " << whiteness / fragment.image.rows << std::endl;
	//cv::normalize(whiteness_image, whiteness_image, 0, 255, cv::NORM_MINMAX, -1, cv::Mat());
	//cv::hconcat(whiteness_image, fragment.image, whiteness_image);
	//cv::namedWindow("whiteness_image",cv::WINDOW_NORMAL);
	//cv::imshow("whiteness_image", whiteness_image);
	//cv::waitKey();
	std::cout  << fragment.name << " " << whiteness << std::endl;
	return whiteness ;
}




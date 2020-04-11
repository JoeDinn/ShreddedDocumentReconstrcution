#pragma once
#include <opencv2/core.hpp>
#include <opencv2/imagecodecs.hpp>
#include <opencv2/highgui.hpp>
#include <unordered_set>

struct Fragment
{
	
	std::string name;
	cv::Mat image;
<<<<<<< Updated upstream
	int *first_pixel;
	int *last_pixel;
	int total_pixels;
	Fragment(std::string root_,std::string name_);
	Fragment(const Fragment &fragment);
	Fragment();
=======
	bool is_thresholded;
	cv::Mat thresholded;
	std::vector<int> first_pixel;
	std::vector<int> last_pixel;
	std::vector<int> centre;
	std::vector<double> row_average;
	void get_row_average();
	Fragment(std::string root_,std::string name_);
	Fragment(const Fragment &fragment);
	Fragment(cv::Mat image_, std::string name_, std::vector<int> first_pixel_, std::vector<int> last_pixel_, std::vector<int> centre_ = std::vector<int>());
	Fragment();
	double average();
	Fragment get_thresholded();
>>>>>>> Stashed changes
	void threshold();
	void colour_histogram(int hist[256]);
	void get_text_lines(bool *is_text_line);
	uchar &operator()(int row,int col);
	~Fragment();

};


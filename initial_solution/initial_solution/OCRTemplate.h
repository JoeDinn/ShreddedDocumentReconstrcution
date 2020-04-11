#pragma once
#include "Similarity.h"


struct SearchBox
{
	int left_most;
	int right_most;
	int top_most;
	int bottom_most;
	bool in_left_strip;
	bool in_right_strip;
	SearchBox(int left_most_, int right_most_, int top_most_, int bottom_most_) :
		left_most(left_most_), right_most(right_most_), top_most(top_most_), bottom_most(bottom_most_), in_left_strip(false), in_right_strip(false) {}
	int height() { return bottom_most - top_most; }
	int width() { return right_most - left_most; }
	SearchBox operator+(SearchBox &box2);
	void operator+=(SearchBox &box2);
};


class OCRTemplate:
	public Similarity
{
private:
	void grow_region(Fragment &image, cv::Mat &visited, int seed_row, int seed_col, SearchBox &box);
	void pad(cv::Mat &image);
	std::vector<SearchBox> get_candidate_characters(Fragment &combined_image);
	//bool is_not_valid_character(SearchBox &box);
protected:
	int largest_height;
	int largest_width;
	int smallest_width;
	int smallest_height;
	virtual double char_score(cv::Mat &image)=0;
	bool between(double a, double b, double c);
public:
	OCRTemplate();
	~OCRTemplate();
	long double cost(Fragment & left_image, Fragment & right_image);
};


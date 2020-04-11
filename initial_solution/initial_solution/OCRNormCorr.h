#pragma once
#include "OCRTemplate.h"
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>


struct Character
{
	char symbol;
	cv::Mat best_image;
	int best_index;
	std::vector<cv::Mat> images;
	std::vector<float> confidences;
	Character(char symbol_) : symbol(symbol_), images(), confidences() {}
	Character(char symbol_, const cv::Mat &best_image_) :symbol(symbol_), best_image(best_image_) {};
	Character() {};
	friend bool operator<(Character &char_1, Character &char_2);
	void add(const cv::Mat &image, float confidence);
};

class OCRNormCorr :
	public OCRTemplate
{
	int current_area;
	std::unordered_map<char, Character> characters;
	double correlation(cv::Mat &character_template, cv::Mat &image);
	void set_max_min();
protected:
	double char_score(cv::Mat & image);
public:
	OCRNormCorr(Solution &images);
	OCRNormCorr(std::string image_file);
	~OCRNormCorr();
};
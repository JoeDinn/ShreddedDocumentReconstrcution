#include "stdafx.h"
#include "OCRNormCorr.h"
#include <fstream>//For writing files
#include <limits>//For smallest double
//#define DEBUG
//#define DEBUGLOAD

void OCRNormCorr::set_max_min()
{

	largest_height = 0;
	largest_width = 0;
	smallest_height = 100;
	smallest_width = 100;

	for (std::pair<const char, Character> &character : characters)
	{
		largest_height = MAX(largest_height, character.second.best_image.rows);
		largest_width = MAX(largest_width, character.second.best_image.cols);
		smallest_height = MIN(smallest_height, character.second.best_image.rows);
		smallest_width = MIN(smallest_width, character.second.best_image.cols);
		//std::cout <<  character.second.image.rows << " " << character.second.image.cols << std::endl;
		//std::cout << (3.0 / 4) * character.second.image.rows << " " << (3.0 / 4)* character.second.image.cols  << std::endl;
#ifdef DEBUGLOAD

		std::cout << character.first << " " << character.second.confidence << " Height: " << character.second.image.rows << " Width: " << character.second.image.cols << std::endl;
		std::string name(1, character.first);
		name += std::to_string(character.second.confidence);
		cv::namedWindow(name, cv::WINDOW_NORMAL);
		cv::imshow(name, character.second.image);
		//
#endif // DEBUG
	}
#ifdef DEBUGLOAD
	cv::waitKey();
	std::cout << largest_height << " " << largest_width  << " " <<  smallest_height<< " " << smallest_width << std::endl;
#endif // DEBUG

}

OCRNormCorr::OCRNormCorr(Solution & images)
{
	tesseract::TessBaseAPI tess;
	//Initialise data structure

	//For each image
	for (Fragment image : images)
	{
#ifdef DEBUGLOAD
		std::cout << image.name << std::endl;
#endif // DEBUG

		//Pre process - make bg white?
		for (int row = 0; row < image.image.rows; row++)
		{
			for (int col = 0; col < image.first_pixel[row]; col++)
			{
				image(row, col) = 255;
			}
			for (int col = image.last_pixel[row]; col < image.image.cols; col++)
			{
				image(row, col) = 255;
			}
		}
		Fragment thresholded(image.get_thresholded());//For edge

		//Detect all the characters
		tess.Init("D:/Program Files/Tesseract-OCR/tessdata", "eng", tesseract::OEM_DEFAULT);//adjust
		//tess.SetPageSegMode(tesseract::PSM_SPARSE_TEXT);
		tess.SetVariable("tessedit_char_whitelist", "1234567890abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ");
		tess.SetImage(mat_to_pix(image.image));
		tess.SetSourceResolution(70);
		tess.Recognize(0);
		tesseract::ResultIterator* ri = tess.GetIterator();
		tesseract::PageIteratorLevel level = tesseract::RIL_SYMBOL;
		if (ri != 0) {
			do {
				//For each character
				if (not (ri->Empty(tesseract::PageIteratorLevel::RIL_SYMBOL)))
				{
					const char* symbol = ri->GetUTF8Text(level);
					float confidence = ri->Confidence(level);
					int left, top, right, bottom;
					ri->BoundingBox(level, &left, &top, &right, &bottom);

#ifdef DEBUGLOAD
					printf("symbol: '%s';  \tconf: %.2f; BoundingBox: %d,%d,%d,%d;\n",
						symbol, confidence, left, top, right, bottom);
#endif // DEBUG
					//Reject if touching edge boundary
					bool edge{ false };
					for (int row = top; row < MIN(bottom+1, image.image.rows); row++)
					{
						if ((image.first_pixel[row] >= left and (int)thresholded(row, image.first_pixel[row]) == 0)//Left edge
						or (image.last_pixel[row]-1 <= right and (int)thresholded(row, image.last_pixel[row] - 1) == 0))//Right edge
						{
							edge = true;
						}
					}
					if (!edge)
					{
						//If not already there create new character
						characters.emplace(*symbol, Character(*symbol));

						characters[*symbol].add(
							cv::Mat(image.image,
								cv::Range(top, MIN(bottom + 1, image.image.rows)), //rows
								cv::Range(left, MIN(right + 1, image.image.cols))),//cols
							confidence);
					}
					
					delete[] symbol;
				}
			} while (ri->Next(level));
		}
		tess.End();
	}
	long double mean_width{ 0 };
	long double mean_height{ 0 };
	int total_characters{ 0 };
	//Get mean dimensions
	for (std::pair<char, Character> character : characters)
	{
		for (cv::Mat image : character.second.images)
		{
			mean_width += image.cols;
			mean_height += image.rows;
			total_characters++;
		}
	}
	mean_width = mean_width / total_characters;
	mean_height = mean_height / total_characters;
	for (std::pair<const char, Character> &character : characters)
	{
		character.second.best_index = -1 ;
		double best_score{ 0 };
		for (int index = 0; index < character.second.confidences.size(); index++)
		{
			if (character.second.confidences[index] > best_score and
				between(mean_height*0.5, character.second.images[index].rows, mean_height*1.5) and//and not too tall or short
				between(mean_width*0.5, character.second.images[index].cols, mean_width*1.5))//and not too wide or thin)
			{
				character.second.best_index = index;
				best_score = character.second.confidences[index];
			}
		}
		if (character.second.best_index != -1)
		{
			character.second.best_image = character.second.images[character.second.best_index];
		}
		character.second.images.clear();
		character.second.confidences.clear();
	}
	//Delete bad 
	for (auto it = characters.cbegin(); it != characters.cend() /* not hoisted */; /* no increment */)
	{
		int best_index{ it->second.best_index };
		if (best_index == -1)
		{
			it = characters.erase(it++);    // or "it = m.erase(it)" since C++11
		}
		else
		{
			++it;
		}
	}

	set_max_min();



}

OCRNormCorr::OCRNormCorr(std::string image_file)
{
	std::ifstream file(image_file);
	std::string character;
	//For each set of data
	while (std::getline(file, character))
	{
#ifdef DEBUGLOAD
		//std::cout << character << std::endl;
#endif // DEBUG
		std::string character_file_name{ character };
		if (isupper(character_file_name[0]))
		{
			std::transform(character_file_name.begin(), character_file_name.end(), character_file_name.begin(), std::ptr_fun<int, int>(std::toupper));
			character_file_name += "_cap";
		}
		character_file_name = "letters/"  + character_file_name +".png";
		char key = character[0];
		//characters.emplace(key,Character(cv::imread(character_file_name,cv::IMREAD_GRAYSCALE),1));////FIX
	}
	set_max_min();

}

OCRNormCorr::~OCRNormCorr()
{
}

double OCRNormCorr::char_score(cv::Mat & image)
{
	//For each character
	double best_score{ std::numeric_limits<double>::min() };
#ifdef DEBUG
	cv::Mat best_template;
	char best_char;
#endif // DEBUG

	for (std::pair<char, Character> character : characters)
	{
		double character_score = correlation(character.second.best_image, image);
#ifdef DEBUG
		//std::cout << character.first << " " << std::endl;
		//std::cout << character_score << std::endl;
		if (character_score > best_score)
		{
			character.second.best_image.copyTo(best_template);
			best_char = character.first;
		}
#endif // DEBUG

		best_score = MAX(character_score, best_score);
	}
#ifdef DEBUG
	std::cout << "char: " << best_char << " score: " << best_score << std::endl;
	cv::namedWindow("character",cv::WINDOW_NORMAL);
	cv::imshow("character", image);
	cv::namedWindow("best template", cv::WINDOW_NORMAL);
	cv::imshow("best template", best_template);
	cv::waitKey();
#endif // DEBUG

	return -best_score;
}

double OCRNormCorr::correlation(cv::Mat & character_template, cv::Mat & image)
{
	int result_cols = (image.cols - character_template.cols) + 1;
	int result_rows = (image.rows - character_template.rows) + 1;

	//std::cout << image.cols << " " << character_template.cols << " " << result_cols << std::endl;
	//std::cout << image.rows << " " << character_template.rows << " " << result_rows << std::endl;

	cv::Mat result(result_rows, result_cols, image.type());

	cv::matchTemplate(image, character_template, result, cv::TM_CCOEFF_NORMED);//TM_CCOEFF_NORMED

	cv::Point minLoc;
	cv::Point maxLoc;
	double minVal;
	double best_score;
	cv::minMaxLoc(result, &minVal, &best_score, &minLoc, &maxLoc, cv::Mat());//best score is maximum

	return (best_score);//
}


void Character::add(const cv::Mat &image, float confidence)
{
	images.emplace_back(image);
	confidences.emplace_back(confidence);
}

bool operator<(Character &char_1, Character &char_2)
{
	return char_1.symbol < char_2.symbol;
}

#include "stdafx.h"
#include "OCRLine.h"
//#define DEBUG

long double OCRLine::cost(Fragment & left_image, Fragment & right_image)
{
	//Stitch
	int best_height{ best_height_match(left_image,right_image) };
	Fragment combined_image{ combine(left_image,right_image,best_height) };
	//Threshold
	Fragment combined_image_thresholded = combined_image.get_thresholded();
	//Get lines
	bool *is_text_line{ new bool[combined_image_thresholded.image.rows] };
	combined_image_thresholded.get_text_lines(is_text_line);

	long double score{ 0 };
	int number_of_lines{ 0 };
	//For each line
	for (int row = 0; row < combined_image_thresholded.image.rows; row++)
	{
		if (is_text_line[row])
		{
			//Line of text found
			int left{ combined_image_thresholded.first_pixel[row] };
			int right{ combined_image_thresholded.last_pixel[row] - 1 };
			int top{ row };
			//Iterate over each row of line of text
			while (is_text_line[row])
			{
				//Update furthest left and right
				int row_first = combined_image_thresholded.first_pixel[row];
				int row_last = combined_image_thresholded.last_pixel[row] - 1;
				left = MIN(left, row_first);
				right = MAX(right, row_last);				

				++row;
			}
			int bottom{ row - 1 };
#ifdef DEBUG
			std::cout << "Found line at " << row << " left: " << left << " Right: " << right << " top: " << top << " bottom: " << bottom << std::endl;
#endif // DEBUG
			//Make image of line
			cv::Mat line_image(combined_image.image,
				cv::Range(top, bottom + 1), // rows
				cv::Range(left, right + 1));// cols
			score += line_score(line_image);
			number_of_lines++;
		}
	}


	return score/ number_of_lines;
}

int OCRLine::line_score(cv::Mat & image)
{
	tess.SetImage(mat_to_pix(image));
	tess.SetSourceResolution(70);

	// Get the text
	char* character = tess.GetUTF8Text();
	int* confidence = tess.AllWordConfidences();//Confidence score in range 0,100
#ifdef DEBUG
	std::cout << "character: " << character << " confidence: " << *confidence << std::endl;
	//cv::imwrite("d:/ocr/garbage.png", image);
	cv::namedWindow("char", cv::WINDOW_NORMAL);
	cv::imshow("char", image);
	cv::waitKey();
#endif // DEBUG

	if ((*confidence) == -1)//Failed to identify 
	{
		return 0;
	}
	return -*confidence;
}

OCRLine::OCRLine()
{
	tess.Init("D:/Program Files/Tesseract-OCR/tessdata", "eng", tesseract::OEM_DEFAULT);//adjust
	tess.SetPageSegMode(tesseract::PSM_SINGLE_LINE);//Detect single line
}


OCRLine::~OCRLine()
{
}

#pragma once
<<<<<<< Updated upstream
#include "Similarity.h"
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
#include <vector>
=======
#include "OCRTemplate.h"
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>


>>>>>>> Stashed changes
class OCRCharacter :
	public OCRTemplate
{
<<<<<<< Updated upstream
public:
	long double cost(Fragment & left_mage, Fragment & right_image);
	int char_score(cv::Mat &image);
	void grow_region(Fragment &image, cv::Mat &visited, int row, int col, int &left_most, int &right_most);

=======
	tesseract::TessBaseAPI tess;
protected:
	double char_score(cv::Mat &image);
public:
>>>>>>> Stashed changes
	OCRCharacter();
	~OCRCharacter();
};


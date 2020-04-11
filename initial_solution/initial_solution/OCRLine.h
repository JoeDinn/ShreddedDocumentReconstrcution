#pragma once
#include "Similarity.h"
#include "Hungarian.h"
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>

class OCRLine :
	public Similarity
{
	tesseract::TessBaseAPI tess;
public:
	long double cost(Fragment & left_image, Fragment & right_image);
	int line_score(cv::Mat &image);
	OCRLine();
	~OCRLine();
};


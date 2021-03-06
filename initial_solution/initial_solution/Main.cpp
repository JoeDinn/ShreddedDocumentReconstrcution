#include "stdafx.h"
#include "stdlib.h"
#include "utilities.h"

#include <iostream>
#include <string>
#include <fstream>
//Optimisers
#include "SimAneal.h"
#include "Hungarian.h"
#include "Greedy.h"
//Similarity measures
#include "diffsquared.h"
#include "EdgeDistance.h"
#include "OCRCharacter.h"
//Evaluation metrics
#include "neighbour_comparison.h"


int main(int argc, char** argv)
{

	if (argc != 2)
	{
		std::cout << " Usage: " << argv[0] << " ImageToLoadAndDisplay" << std::endl;
		return -1;
	}

	std::string root{ argv[1] };
	std::ifstream file(root + "/" + root + ".txt");
	std::string str;
	std::getline(file, str);
	std::cout << str << std::endl;
	int number_files = std::stoi(str);

	GroundTruth ground_truth(number_files);
	Solution images(number_files);
	int image_counter = 0;
	while (std::getline(file, str))
	{
		ground_truth[image_counter] = str;
		images[image_counter] = Fragment(root, str); // read the file
		if (images[image_counter].img.empty()) // check for invalid input
		{
			std::cout << "could not open or find the image" << std::endl;
	}
		++image_counter;
	}

	

	images[1].threshold();

	OCRCharacter ocr{};
	ocr.compare(images[2], images[3]);

	//cv::imshow("Thresholded", images[1].img);
	//cv::imwrite("test_threshold.png", images[1].img);
	cv::waitKey();


	//log(images);
	shuffle(images);
	log(images);
	//output(images);

	EdgeDistance d{};
	//diffsquared d{};
	Similarity &s{ d };
	
	//sim_aneal o{s};
	//hungarian o{s};
	Greedy o(s);
	

	neighbour_comparison m{};
	

	Solution ordered_images{ o.optimise(images) };
	std::cout << "Final " << ordered_images.size() << std::endl;
	log(ordered_images);
	
	output(ordered_images);
	std::cout << "Evaluation score: " << m.evaluate(ordered_images, ground_truth);
	return 0;
}
/**/
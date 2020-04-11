#pragma once
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <random> // for std::mt19937
#include <ctime> // for std::time
#include <iostream>
#include "Fragment.h"
#include "Solution.h"
#include <leptonica/allheaders.h>


//typedef std::vector<Fragment> Solution;
typedef std::vector<std::string> GroundTruth;

namespace random
{
	// Initialize our mersenne twister with a random seed based on the clock (once at system startup)
	static std::mt19937 mersenne(static_cast<std::mt19937::result_type>(std::time(nullptr)));

	static int rand_int(int max)
	{
		std::uniform_int_distribution<> rand_gen(0, max); // we can create a distribution in any function that needs it
		return rand_gen(random::mersenne); // and then generate a random number from our global generator
	}
	static int rand_int(int min, int max)
	{
		std::uniform_int_distribution<> rand_gen(min, max); // we can create a distribution in any function that needs it
		return rand_gen(random::mersenne); // and then generate a random number from our global generator
	}

	static double rand_double(int max)
	{
		std::uniform_real_distribution<double> rand_gen(0, max); // we can create a distribution in any function that needs it
		return rand_gen(random::mersenne); // and then generate a random number from our global generator
	}
	static double rand_double(int min, int max)
	{
		std::uniform_real_distribution<double> rand_gen(min, max); // we can create a distribution in any function that needs it
		return rand_gen(random::mersenne); // and then generate a random number from our global generator
	}
}


void stitch(Solution &solution);
void output(Solution &solution);
void log(Solution &solution);

void shuffle(Solution &solution);
int left_most(Solution &solution);

<<<<<<< Updated upstream
long long int whiteness_score(Fragment &fragment);
=======
Fragment combine(Fragment &left_image, Fragment &right_image, int right_start = 0);
Fragment combine_whole(Fragment &left_image, Fragment &right_image, int right_start = 0);
int best_height_match(Fragment &left_image, Fragment &right_image,long double *cost = nullptr);

Solution load(std::string root, std::string dir);

Pix *mat_to_pix(cv::Mat &image);

double avg_first(Fragment &fragment);
double centroid_x(Fragment &fragment);

double whiteness_score(Fragment &fragment);

>>>>>>> Stashed changes

#pragma once
#include "stdafx.h"
#include "stdlib.h"
#include "utilities.h"

#include <iostream>//For console
#include <string>
#include <fstream>//For writing files
#include <chrono>//For timing
//Optimisers
#include "SimAneal.h"
#include "Hungarian.h"
#include "Greedy.h"
#include "Tabu.h"
//Similarity measures
#include "LinearRegressionSimilarity.h"
#include "EdgeDistance.h"
#include "OCRCharacter.h"
#include "OCRLine.h"
#include "OCRNormCorr.h"
#include "BestHeightEdgeDistance.h"
#include "BWBHED.h"
#include "NormalisedCorrelation.h"
//Evaluation metrics
#include "neighbour_comparison.h"
//Other
#include "Haralick.h"
#include "LBP.h"
#include "SimilarityTester.h"

struct tabu_paramters
{
	std::string file;
	int max_moves_since_improvement;
	int max_tabu_size;
	tabu_paramters(std::string file_,int max_moves_since_improvement_,	int max_tabu_size_):
		file(file_), max_moves_since_improvement(max_moves_since_improvement_), max_tabu_size(max_tabu_size_)
	{}
	tabu_paramters():file(""), max_moves_since_improvement(0), max_tabu_size(0)
	{}
};

struct sim_aneal_paramters
{
	std::string file;
	double initial_temperature;
	int temperature_length;
	double cooling_factor;
	int num_non_improve;
	sim_aneal_paramters(std::string file_, double initial_temperature_, int temperature_length_, double cooling_factor_, int num_non_improve_):
		file(file_), initial_temperature(initial_temperature_), temperature_length(temperature_length_), cooling_factor(cooling_factor_), num_non_improve(num_non_improve_)
	{}
	sim_aneal_paramters() :
		file(""), initial_temperature(0), temperature_length(0), cooling_factor(0), num_non_improve(0)
	{}
};


namespace Tester
{
	//Test all items in dataset with optimiser using metric
	void test_optimiser(Optimiser &optimiser, Metric &metric, std::string dataset, std::string out_file_name = "test_results.txt");
	//Special case of above for OCRNormCorr (optimiser hardcoded)
	void test_optimiser_OCRNormCorr(Metric &metric, std::string dataset, std::string out_file_name = "test_results.txt");
	void test_tabu_paramters_OCRNormCorr(Metric &metric, std::string dataset, std::vector<tabu_paramters> parameters_list);
	void test_sim_aneal_paramters_OCRNormCorr(Metric &metric, std::string dataset, std::vector<sim_aneal_paramters> parameters_list);

	//Test all items in dataset on similarity measure
	void test_similarity(Similarity &similarity, std::string dataset, std::string out_file_name = "test_results.txt");
	//Special case of above for OCRNormCorr
	void test_OCRNormCorr(std::string dataset, std::string out_file_name = "test_results.txt");
	
	//Test whiteness value
	void test_left_most(std::string dataset, std::string out_file_name = "test_results.txt");

	//Test stitching
	void test_stitching(std::string dataset);
}
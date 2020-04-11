#include "stdafx.h"
#include "Tester.h"
#include <iostream>//For console
#include <string>
#include <fstream>//For writing files

void Tester::test_optimiser(Optimiser &optimiser, Metric &metric, std::string dataset, std::string out_file_name)
{
	std::ofstream out_file(out_file_name);
	std::ifstream file(dataset);
	std::string root;
	std::string document;
	std::getline(file, root);
	//For each set of data
	while (std::getline(file, document))
	{
		//Load
		Solution images{ load(root + "/" + document  ,document) };
		//Generate ground truth
		GroundTruth ground_truth{};
		for (Fragment fragment : images)
		{
			ground_truth.emplace_back(fragment.name);
		}
		//Shuffle
		shuffle(images);
		//Order

		Greedy pre_optimiser(optimiser.similarity);//////////////////////////////////using greedy (change)
		Solution semi_ordered_images{ pre_optimiser.optimise(images) };
		Solution ordered_images{ optimiser.optimise(semi_ordered_images) };
		//Evaluate
		double score = metric.evaluate(ordered_images, ground_truth);
		if (out_file.is_open()) out_file << document << " " << score << std::endl;
	}
}

void Tester::test_optimiser_OCRNormCorr(Metric & metric, std::string dataset, std::string out_file_name)
{
	std::ofstream out_file(out_file_name);
	std::ifstream file(dataset);
	std::string root;
	std::string document;
	std::getline(file, root);
	std::cout << root << std::endl;
	//For each set of data
	while (std::getline(file, document))
	{
		out_file << document << std::endl;
		//Load
		Solution images{ load(root + "/" + document  ,document) };
		//Generate ground truth
		GroundTruth ground_truth{};
		for (Fragment fragment : images)
		{
			ground_truth.emplace_back(fragment.name);
		}
		OCRNormCorr ocrnc{ images };
		//Shuffle
		shuffle(images);
		//Order
		log(images);
		Tabu optimiser(ocrnc);
		//SimAneal optimiser{ ocrnc,0.2, 1000,  0.0001,  100000000, 10 };//0.5, 100,  0.00001,  100000000, 10
		Greedy pre_optimiser(ocrnc);//////////////////////////////////using greedy (change)
		Solution semi_ordered_images{ pre_optimiser.optimise(images) };
		log(semi_ordered_images);
		Solution ordered_images{ optimiser.optimise(semi_ordered_images) };
		log(ordered_images);
		//Evaluate
		double pre_score = metric.evaluate(semi_ordered_images, ground_truth);
		double score = metric.evaluate(ordered_images, ground_truth);
		if (out_file.is_open())
		{
			
			out_file << "greedy: " << pre_score << std::endl;
			out_file << "tabu: "   << score << std::endl;
		}
	}
}

void Tester::test_tabu_paramters_OCRNormCorr(Metric & metric, std::string dataset, std::vector<tabu_paramters> parameters_list)
{
	std::ifstream file(dataset);
	std::string root;
	std::string document;
	std::getline(file, root);
	std::cout << root << std::endl;
	//For each set of data
	while (std::getline(file, document))
	{
		//Load
		Solution images{ load(root + "/" + document  ,document) };
		OCRNormCorr ocrnc{ images };
		//Generate ground truth
		GroundTruth ground_truth{};
		for (Fragment fragment : images)
		{
			ground_truth.emplace_back(fragment.name);
		}
		for (tabu_paramters parameters : parameters_list)//
		{
			std::ofstream out_file(parameters.file,std::ios::app);

			out_file << document << std::endl;



			
			//Shuffle
			shuffle(images);
			//Order
			log(images);
			Tabu optimiser(ocrnc, parameters.max_moves_since_improvement, parameters.max_tabu_size);

			Greedy pre_optimiser(ocrnc);
			Solution semi_ordered_images{ pre_optimiser.optimise(images) };
			log(semi_ordered_images);
			Solution ordered_images{ optimiser.optimise(semi_ordered_images) };
			log(ordered_images);
			//Evaluate
			double pre_score = metric.evaluate(semi_ordered_images, ground_truth);
			double score = metric.evaluate(ordered_images, ground_truth);
			if (out_file.is_open())
			{

				out_file << "greedy: " << pre_score << std::endl;
				out_file << "tabu: " << score << std::endl;
			}
		}
		
	}
}

void Tester::test_sim_aneal_paramters_OCRNormCorr(Metric & metric, std::string dataset, std::vector<sim_aneal_paramters> parameters_list)
{
	std::ifstream file(dataset);
	std::string root;
	std::string document;
	std::getline(file, root);
	std::cout << root << std::endl;
	//For each set of data
	while (std::getline(file, document))
	{
		Solution images{ load(root + "/" + document  ,document) };
		OCRNormCorr ocrnc{ images };
		//Generate ground truth
		GroundTruth ground_truth{};
		for (Fragment fragment : images)
		{
			ground_truth.emplace_back(fragment.name);
		}
		for (sim_aneal_paramters parameters : parameters_list)//
		{
			std::ofstream out_file(parameters.file, std::ios::app);

			out_file << document << std::endl;
			//Load


			//Shuffle
			shuffle(images);
			//Order
			//log(images);
			SimAneal optimiser(ocrnc, parameters.initial_temperature,parameters.temperature_length,parameters.cooling_factor,parameters.num_non_improve);

			Greedy pre_optimiser(ocrnc);
			Solution semi_ordered_images{ pre_optimiser.optimise(images) };
			//log(semi_ordered_images);
			Solution ordered_images{ optimiser.optimise(semi_ordered_images) };
			//log(ordered_images);
			//Evaluate
			double pre_score = metric.evaluate(semi_ordered_images, ground_truth);
			double score = metric.evaluate(ordered_images, ground_truth);
			if (out_file.is_open())
			{
				out_file << "greedy: " << pre_score << std::endl;
				out_file << "sim_aneal: " << score << std::endl;
			}
			out_file.close();
		}

	}
}

void Tester::test_OCRNormCorr(std::string dataset, std::string out_file_name)
{
	SimilarityTester s_t{};
	std::ofstream out_file(out_file_name);
	std::ifstream file(dataset);
	std::string root;
	std::string document;
	std::getline(file, root);
	//For each set of data
	while (std::getline(file, document))
	{
		//Load
		Solution images{ load(root + "/" + document  ,document) };
		OCRNormCorr ocrnc{ images };
		//evaluate
		double avg = s_t.test(images, ocrnc);
		if (out_file.is_open()) out_file << document + "/" + document << " " << avg << std::endl;
	}

}

void Tester::test_left_most(std::string dataset, std::string out_file_name)
{
	SimilarityTester s_t{};
	std::ofstream out_file(out_file_name);
	std::ifstream file(dataset);
	std::string root;
	std::string document;
	std::getline(file, root);
	//For each set of data
	while (std::getline(file, document))
	{
		//Load
		Solution images{ load(root + "/" + document  ,document) };
		shuffle(images);

		int left_most_index = left_most(images);
		if (out_file.is_open()) out_file << document << ": " << left_most_index << " " << images[left_most_index].name << std::endl;

	}

}

void Tester::test_stitching(std::string dataset)
{
	std::ifstream file(dataset);
	std::string root;
	std::string document;
	std::getline(file, root);
	//For each set of data
	while (std::getline(file, document))
	{
		//Load
		Solution images{ load(root + "/" + document  ,document) };
		stitch(images);
	}
}

void Tester::test_similarity(Similarity & similarity, std::string dataset, std::string out_file_name)
{
	SimilarityTester s_t{};
	std::ofstream out_file(out_file_name);
	std::ifstream file(dataset);
	std::string root;
	std::string document;
	std::getline(file, root);
	//For each set of data
	while (std::getline(file, document))
	{
		//Load
		Solution images{ load(root + "/" + document  ,document) };

		//evaluate
		double avg = s_t.test(images, similarity);
		if (out_file.is_open()) out_file << document << " " << avg << std::endl;
	}

}
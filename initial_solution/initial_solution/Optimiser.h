#pragma once
#include "utilities.h"
#include "Similarity.h"
typedef std::vector<std::vector<double>> neighbourhood;
typedef std::pair<int, int> neighbour;
class Optimiser
{
protected:
	double get_updated_cost(Solution &current_solution, double current_cost, int index, int swap_index);
	double get_initial_cost(Solution &initial_solution);
	neighbourhood get_neighbourhood(Solution & solution, double current_cost);
	neighbour get_random_neighbour(Solution &current_solution);
	void update(Solution & current_solution, double & current_cost, int index, int swap_index, double neighbour_cost);
public:
	Similarity & similarity;
	Optimiser(Similarity &similarity_) : similarity(similarity_) {};
	virtual Solution optimise(Solution fragments) = 0;
};
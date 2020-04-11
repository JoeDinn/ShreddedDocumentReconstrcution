#include "stdafx.h"
#include "Tabu.h"
//#define DEBUG
bool Tabu::is_tabu(Fragment & first_fragment, Fragment & next_fragment)
{
	std::string search_name{ cannonical_tabu(first_fragment, next_fragment) };
	for (std::string name: tabu_table)
	{
		if (search_name == name)
		{
			return true;
		}
	}
	return false;
}

void Tabu::add_tabu(Fragment & first_fragment, Fragment & next_fragment)
{
	tabu_table.push_back(cannonical_tabu(first_fragment,next_fragment));
	++tabu_count;
	//Remove move from tabu list if tenure over
	if (tabu_count >= max_tabu_size)
	{
		--tabu_count;
		tabu_table.pop_front();
	}
	
}

std::string Tabu::cannonical_tabu(Fragment & first_fragment, Fragment & next_fragment)
{
	std::string min = MIN(first_fragment.name, next_fragment.name);
	std::string max = MAX(first_fragment.name, next_fragment.name);
	return min + max;
}

Tabu::Tabu(Similarity &similarity_, int max_moves_since_improvement_,int max_tabu_size_) :
	Optimiser(similarity_), max_moves_since_improvement(max_moves_since_improvement_), max_tabu_size(max_tabu_size_)
{
}

Tabu::~Tabu()
{
}

Solution Tabu::optimise(Solution fragments)
{
	Solution current_solution{ fragments };
	double current_score{ get_initial_cost(fragments) };
	Solution best_solution{ current_solution };
	double best_score{ current_score };

#ifdef DEBUG
	std::cout << "At start: " << std::endl;
	log(current_solution);/////
	std::cout << current_score << std::endl;
#endif // DEBUG
	//initialise tabulist
	tabu_count = 0;
	int moves_since_improvement{ 0 };
	while (moves_since_improvement < max_moves_since_improvement)
	{
#ifdef DEBUG
		//log(current_solution);/////
		//std::cout << current_score << std::endl;
#endif // DEBUG
		//Get neighbourhood
		neighbourhood current_neighbourhood{  get_neighbourhood(current_solution,current_score) };
		//Find best neighbour
		int best_index{ 0 };
		int best_swap_index{0};
		for (int index = 0; index < current_solution.size() -1; index++)
		{
			for (int swap_index = index + 1; swap_index < current_solution.size(); ++swap_index)
			{
				
				//If cost less than current neighbourhood best
				if (current_neighbourhood[index][swap_index]  < current_neighbourhood[best_index][best_swap_index]
					//and move is not tabu (or cost is less than the current global best)
					and (not(is_tabu(current_solution[index], current_solution[swap_index])) or current_neighbourhood[index][swap_index] < best_score))
				{
					best_index = index;
					best_swap_index = swap_index;
				}
			}
		}
#ifdef DEBUG
		//std::cout << "Swapping: " << current_solution[best_index].name << " <-> " << current_solution[best_swap_index].name << std::endl;
#endif // DEBUG

		//Add move to tabu table
		add_tabu(current_solution[best_index], current_solution[best_swap_index]);
		//Update current solution and score
		update(current_solution, current_score, best_index, best_swap_index, current_neighbourhood[best_index][best_swap_index]);
		//If better than current best global solution update
		if ((float)current_score < (float)best_score)//////hack
		{
#ifdef DEBUG
			std::cout << (current_score - best_score) << " " <<(best_score == best_score) << " improved " << best_score+1 << " -> " << current_score+1 << " since last: " << moves_since_improvement << std::endl;////
#endif // DEBUG
			best_solution = current_solution;
			best_score = current_score;
			moves_since_improvement = 0;
		}
		else
		{
			++moves_since_improvement;
		}
	}
	return best_solution;
}
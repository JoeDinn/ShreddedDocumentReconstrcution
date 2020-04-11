#include "stdafx.h"
#include "SimAneal.h"
//#define DEBUG
#define PLOT
#ifdef PLOT 
#include <fstream>//For writing files
#endif // PLOT 



SimAneal::SimAneal(Similarity &similarity_, double initial_temperature_, int temperature_length_, double cooling_factor_, int num_non_improve_) :
	Optimiser(similarity_),
	initial_temperature(initial_temperature_), temperature_length(temperature_length_), cooling_factor(cooling_factor_), num_non_improve(num_non_improve_)
{
}


SimAneal::~SimAneal()
{
}



Solution SimAneal::optimise(Solution fragments)
{
	Solution current_solution{ fragments };
	




	left_most_name = fragments[left_most(fragments)].name;




	double current_cost{ get_initial_cost(current_solution) };
#ifdef DEBUG
	std::cout << "left most name" << left_most_name << std::endl;
	std::cout << "At start " << current_cost << std::endl;
#endif // DEBUG
	double temperature{ initial_temperature };
	
	Solution best_solution{ current_solution };
	long double best_cost{ current_cost };

	double q;
	int count_non_improve{ 0 };
#ifdef PLOT 
	std::ofstream f("results.txt");
	int plot_frequency = (initial_temperature / cooling_factor) / 1000;
	std::cout << plot_frequency << std::endl;
	int plot_count{ 0 };
#endif // PLOT
	while ((count_non_improve < num_non_improve) and (temperature > 0))
	{
#ifdef DEBUG
		int uphills{ 0 };
#endif // DEBUG



		for (int i{}; i < temperature_length; ++i)
		{
			neighbour rand_neighbour = get_random_neighbour(current_solution);
			int index = rand_neighbour.first;
			int swap_index = rand_neighbour.second;
			long double neighbour_cost{ get_updated_cost(current_solution, current_cost, index,swap_index) };

#ifdef DEBUG
			//std::cout << "Current: " << current_cost << " neighbour: " << neighbour_cost << " is left most: " << (current_solution[0].name == left_most_name )<< std::endl;
#endif // DEBUG

			long double cost_change{ neighbour_cost - current_cost };
			if (cost_change <= 0)
			{
				update(current_solution, current_cost, index, swap_index, neighbour_cost);
			}
			else
			{
				q =  random::rand_double(1);

				if (q < exp( -cost_change / temperature))
				{
#ifdef DEBUG
					uphills++;
#endif // DEBUG
					update(current_solution, current_cost, index, swap_index, neighbour_cost);
				}
			}
			if (current_cost < best_cost)
			{
#ifdef DEBUG
				std::cout << "Current: " << best_cost << " New: " << current_cost << " left most in place: " << (current_penalty == 0) << " since last: " << count_non_improve << " Temp: " << temperature <<  std::endl;
#endif // DEBUG
				best_solution = current_solution;
				best_cost = current_cost;
				count_non_improve = 0;
			}
			else count_non_improve += 1;//////////////////////////////////////////////////////
		}
#ifdef DEBUG
		//std::cout << "Temp: " << temperature << " best score: " << best_cost << " Uphills: " << uphills << " Probability: " << std::endl;

#endif // DEBUG
#ifdef PLOT
		
		if (plot_count==0 and f.is_open())
		{
			f << plot_count << " " <<temperature << "," << best_cost << std::endl;
		}
		plot_count++;
		if (plot_count > plot_frequency)
		{
			plot_count = 0;
		}
#endif // PLOT

		temperature -= (cooling_factor * initial_temperature);
	}
#ifdef DEBUG
	std::cout << "At end " << best_cost << std::endl;
#endif // DEBUG
#ifdef PLOT
	f.close();
#endif // PLOT


	return best_solution;
}


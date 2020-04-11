#include "stdafx.h"
#include "Optimiser.h"

double Optimiser::get_updated_cost(Solution & current_solution, double current_cost, int index, int swap_index)
{
	double old_cost{};
	double new_cost{};


	if (swap_index - index != 1)
	{
		//swapping A-B-C...X-Y-Z with A-Y-C...X-B-Z
		//B at index, Y at swap_index
		//Remove old costs
		//A-B
		if (index > 0)//Not first
			old_cost += similarity.compare(current_solution[index - 1], current_solution[index]);
		//B-C
		if (index < current_solution.size() - 1)//Not last
			old_cost += similarity.compare(current_solution[index], current_solution[index + 1]);
		//X-Y
		if (swap_index - index != 1)//Not swap_index one after index
			old_cost += similarity.compare(current_solution[swap_index - 1], current_solution[swap_index]);
		//Y-Z
		if (swap_index < current_solution.size() - 1)//Not last
			old_cost += similarity.compare(current_solution[swap_index], current_solution[swap_index + 1]);

		//Add new costs
		//A-Y
		if (index > 0)//Not firstABCD - ACBD       AC CB BD
			new_cost += similarity.compare(current_solution[index - 1], current_solution[swap_index]);
		//Y-C
		if (index < current_solution.size() - 1)//Not last
			new_cost += similarity.compare(current_solution[swap_index], current_solution[index + 1]);
		//X-B
		if (swap_index - index != 1)//Not swap_index one after index
			new_cost += similarity.compare(current_solution[swap_index - 1], current_solution[index]);
		else
			new_cost += similarity.compare(current_solution[swap_index - 1], current_solution[index]);
		//B-Z
		if (swap_index < current_solution.size() - 1)//Not last
			new_cost += similarity.compare(current_solution[index], current_solution[swap_index + 1]);
	}
	else
	{
		//swapping A-B-C-D with A-C-B-D
		//B at index, C at swap_index
		//Remove old costs
		//A-B
		if (index > 0) old_cost += similarity.compare(current_solution[index - 1], current_solution[index]);
		//B-C
		if (index <	(current_solution.size() - 1)) old_cost += similarity.compare(current_solution[index], current_solution[swap_index]);
		//C-D
		if (index <	(current_solution.size() - 2)) old_cost += similarity.compare(current_solution[swap_index], current_solution[swap_index + 1]);

		//Add new costs
		//A-C
		if (index > 0 and index < (current_solution.size() - 1)) new_cost += similarity.compare(current_solution[index - 1], current_solution[swap_index]);
		//C-B
		if (index <	(current_solution.size() - 1)) new_cost += similarity.compare(current_solution[swap_index], current_solution[index]);
		//B-D
		if (index <	(current_solution.size() - 2)) new_cost += similarity.compare(current_solution[index], current_solution[swap_index + 1]);
	}

	return (current_cost + (new_cost - old_cost));
}

double Optimiser::get_initial_cost(Solution &solution)
{
	double total{ 0 };
	for (int i{ 0 }; i < solution.size() - 1; ++i)
	{
		total += similarity.compare(solution[i], solution[i + 1]);
	}
	return total;
}

neighbourhood Optimiser::get_neighbourhood(Solution & solution, double current_cost)
{
	neighbourhood solution_neighbourhood{ solution.size(), std::vector<double>(solution.size(), 0) };
	for (int index = 0; index < solution.size() - 1; ++index)
	{
		//std::cout << "index " << index << "size: " << (solution.size() - index) - 1 << std::endl;
		for (int swap_index = index + 1; swap_index < solution.size(); ++swap_index)
		{
			solution_neighbourhood[index][(swap_index)] = get_updated_cost(solution, current_cost, index, swap_index);
		}
	}
	return solution_neighbourhood;
}

neighbour Optimiser::get_random_neighbour(Solution &current_solution)
{
	int index = random::rand_int(current_solution.size() - 2);
	int swap_index = random::rand_int(index+1,current_solution.size() - 1);
	return neighbour(index, swap_index);
}

void Optimiser::update(Solution & current_solution, double & current_cost, int index,int swap_index , double neighbour_cost)
{
	current_solution.swap(index, swap_index);
	current_cost = neighbour_cost;
}
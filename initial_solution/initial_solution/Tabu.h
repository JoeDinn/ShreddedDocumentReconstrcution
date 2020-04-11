#pragma once
#include "Optimiser.h"
#include <deque>//For tabulist

class Tabu :
	public Optimiser
{
	std::deque<std::string> tabu_table;
	bool is_tabu(Fragment &first_fragment, Fragment &next_fragment);
	void add_tabu(Fragment &first_fragment, Fragment &next_fragment);
	std::string cannonical_tabu(Fragment &first_fragment, Fragment &next_fragment);
	int max_moves_since_improvement;
	int max_tabu_size;
	int tabu_count;
public:
	Tabu(Similarity &similarity_, int max_moves_since_improvement_= 100000, int max_tabu_size_= 60);
	~Tabu();
	Solution optimise(Solution fragments);
};


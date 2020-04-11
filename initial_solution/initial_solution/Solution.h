#pragma once
#include "Fragment.h"
#include <iterator>
class Solution
{
	class iterator;
	std::vector<Fragment> fragments;
	std::vector<int> indexes;
	
	class iterator : public std::iterator<std::output_iterator_tag, int>
	{
	public:
		explicit iterator(Solution & solution_, size_t index_ = 0);
		Fragment operator*() const;
		iterator & operator++();
		iterator operator++(int);
		bool operator!=(const iterator &it) const;
		
	private:
		size_t index = 0;
		Solution &solution;
	};
public:
	Solution(int size=0);
	~Solution();
	void swap(int index_1, int index_2);
	iterator begin();
	iterator end();
	size_t size();
	void resize(int size);
	Fragment &operator[](int index);
	//iterator stuff
	
};
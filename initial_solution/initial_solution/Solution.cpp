#include "stdafx.h"
#include "Solution.h"


Solution::Solution(int size):fragments(size),indexes(size)
{
	for (int i = 0; i < fragments.size(); i++)
		indexes[i] = i;
	fragments.size();
}


Solution::~Solution()
{
}

void Solution::swap(int index_1, int index_2)
{
	std::swap(indexes[index_1], indexes[index_2]);
}

size_t Solution::size()
{
	return fragments.size();
}

Fragment & Solution::operator[](int index)
{
	return fragments[indexes[index]];
}

Solution::iterator::iterator(Solution & solution_, size_t index_):solution(solution_),index(index_)
{
}

Fragment Solution::iterator::operator*() const
{
	return solution.fragments[solution.indexes[index]];
}

Solution::iterator & Solution::iterator::operator++()
{
	index++;
	return *this;
}

Solution::iterator Solution::iterator::operator++(int)
{
	return ++(*this);
}

bool Solution::iterator::operator!=(const iterator &it) const
{
	return index != it.index;
}

void Solution::resize(int size)
{
	fragments.resize(size);
	indexes.resize(size);
}

Solution::iterator Solution::begin()
{
	return Solution::iterator(*this, 0);
}
Solution::iterator Solution::end()
{
	return Solution::iterator(*this, size());
}
/*
 * +-----------------------------------------------+
 * | Project:	  Minesweeper					   |
 * | File:		  random.hpp					   |
 * | Author:	  Pham Thanh Tung				   |
 * | Student ID:  17020042						   |
 * | Class:		  QH - 2017 - I / CQ - C - A - C   |
 * +-----------------------------------------------+
 */

#ifndef RANDOM_HPP_INCLUDED
#define RANDOM_HPP_INCLUDED

#include <random>
#include <chrono>

class Random {
private:
	unsigned seed;
	std::default_random_engine generator;
	std::uniform_int_distribution<int> distribution;

public:
	Random();
	int get (int, int);
};

#endif // RANDOM_HPP_INCLUDED

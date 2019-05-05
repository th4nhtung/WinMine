/*
 * +-----------------------------------------------+
 * | Project:	  Minesweeper					   |
 * | File:		  random.cpp					   |
 * | Author:	  Pham Thanh Tung				   |
 * | Student ID:  17020042						   |
 * | Class:		  QH - 2017 - I / CQ - C - A - C   |
 * +-----------------------------------------------+
 */

#include "random.hpp"

Random::Random() {
	this->seed = std::chrono::system_clock::now().time_since_epoch().count();
	this->generator = std::default_random_engine(this->seed);
}

int Random::get (int low, int high) {
	this->distribution = std::uniform_int_distribution<int>(low, high);
	return this->distribution(this->generator);
}

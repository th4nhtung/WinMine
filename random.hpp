/*
 * +-----------------------------------------------+
 * | Project:     Minesweeper                      |
 * | Author:      Pham Thanh Tung                  |
 * | Student ID:  17020042                         |
 * | Class:       QH - 2017 - I / CQ - C - A - C   |
 * +-----------------------------------------------+
 */

/**
 * @file random.hpp
 * @author Tung Pham Thanh
 * @brief This is a header file for Random class
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
	/**
	 * @brief Construct, initialize seed and generator
	 */

	Random() {
		this->seed = std::chrono::system_clock::now().time_since_epoch().count();
		this->generator = std::default_random_engine(this->seed);
	}

	/**
	 * @brief Get a random integer in range [low, high], including both \a low and \a high
	 * @param[in] low Lower bound of range
	 * @param[in] high Upper bound of range
	 * @return A random integer in range [low, high]
	 */

	int get (int low, int high) {
		this->distribution = std::uniform_int_distribution<int>(low, high);
		return this->distribution(this->generator);
	}
};

#endif // RANDOM_HPP_INCLUDED

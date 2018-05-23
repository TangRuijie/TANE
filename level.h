#pragma once
#include <vector>
#include <iostream>
#define ATTRIBUTE_NUMBER	15

class Level {
	public:
		int l_size;
		std::vector<std::vector<int>> set;
		Level();
		void GenerateNextLevel();
		void printL();

	private:
		void combine(std::vector<int> now, int rest);
};
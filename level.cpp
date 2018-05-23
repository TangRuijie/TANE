#include "level.h"

Level::Level() {
	l_size = 1;
	for (int i = 1; i <= ATTRIBUTE_NUMBER; i++) {
		std::vector<int> temp;
		temp.push_back(i);
		set.push_back(temp);
	}
}

void Level::GenerateNextLevel() {
	l_size++;
	set.clear();
	if (l_size > ATTRIBUTE_NUMBER)
		return;
	for (int i = 1; i <= ATTRIBUTE_NUMBER; i++) {
		std::vector<int> tmp;
		tmp.push_back(i);
		combine(tmp, l_size - 1);
	}
}

void Level::printL() {
	int l = set.size();
	for (int i = 0; i < l; i++) {
		for (int j = 0; j < l_size; j++) {
			std::cout << set[i][j] << " ";
		}
		std::cout << std::endl;
	}
}

void Level::combine(std::vector<int> now, int rest) {
	if (rest == 0) {
		set.push_back(now);
		return;
	}
	int tmp_size = now.size();
	for (int i = now[tmp_size - 1] + 1; i <= ATTRIBUTE_NUMBER; i++) {
		std::vector<int> tmp_vec = now;
		tmp_vec.push_back(i);
		combine(tmp_vec, rest - 1);
	}
}
#include <iostream>
#include <fstream>
#include <iterator>
#include <algorithm>
#include <string>
#include <ctime>
#include <unordered_map>
#include "level.h"

std::vector<std::vector<int>> partition[ATTRIBUTE_NUMBER];
std::vector<std::string> data[ATTRIBUTE_NUMBER];

void ComputePartition() {
	std::ifstream infile;
	infile.open("data.txt", std::ios::in);
	if (!infile.is_open())
		std::cout << "Failed to open the file." << std::endl;
	std::string tmp_str;

	// 当前读入属性的名称
	int current_attr = 1;

	// 读入数据
	while (!infile.eof()) {
		char a = infile.get();
		if (a == ',') {
			char b = infile.get();
			if (b == ' ') {
				tmp_str.push_back(a);
				tmp_str.push_back(b);
				continue;
			}
			data[current_attr - 1].push_back(tmp_str);
			current_attr++;
			tmp_str.clear();
			tmp_str.push_back(b);
		}
		else if (a == '\n' || a == -1) {
			data[current_attr - 1].push_back(tmp_str);
			current_attr = 1;
			tmp_str.clear();
		}
		else {
			tmp_str.push_back(a);
		}
	}	

	// 各个属性分别有多少种值
	int value_num[15] = { 0 };
	for (int i = 0; i < ATTRIBUTE_NUMBER; i++)
		value_num[i] = 1;

	// 计算哈希值
	std::unordered_map<std::string, int> map[ATTRIBUTE_NUMBER];
	for (int i = 0; i < ATTRIBUTE_NUMBER; i++) {
		int tmp_size = data[i].size();
		for (int j = 0; j < tmp_size; j++) {
			if (map[i][data[i][j]] == 0) {
				map[i][data[i][j]] = value_num[i];
				value_num[i]++;
			}
		}
	}

	// 计算划分
	for (int i = 0; i < ATTRIBUTE_NUMBER; i++) {
		for (int j = 0; j < value_num[i] - 1; j++) {
			std::vector<int> tmp_vec;
			(partition[i]).push_back(tmp_vec);
		}
		int tmp_size = data[i].size();
		for (int j = 0; j < tmp_size; j++) {
			partition[i][map[i][data[i][j]] - 1].push_back(j);
		}
	}


	infile.close();
}

void ComputeDependcies(Level L) {

}

bool isHolds(std::vector<int> X, int B) {
	return true;
}

std::vector<int> ComputeRhsPlus(std::vector<int> X) {
	std::vector<int> result;
	for (int i = 1; i <= ATTRIBUTE_NUMBER; i++) {
		bool tmp_flag = true;
		for (int j = 0; j < X.size(); j++) {	
			std::vector<int> X_AB;
			for (int k = 0; k < X.size(); k++) {
				if (X[k] == i || X[k] == X[j])
					continue;
				X_AB.push_back(X[k]);
			}
			if (isHolds(X_AB, X[j])) {
				tmp_flag = false;
				break;
			}
		}
		if (tmp_flag == true)
			result.push_back(i);
	}
	return result;
}

int main() {
	Level L;
	auto start_time = clock();
	ComputePartition();
	auto end_time = clock();
	std::cout << "cost time: " << (end_time - start_time) / 1000.0 << "s" << std::endl;
	system("pause");

	return 0;
}
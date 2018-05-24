#include <iostream>
#include <fstream>
#include <iterator>
#include <algorithm>
#include <string>
#include <ctime>
#include <unordered_map>
#include <map>
#include "level.h"

std::map<std::vector<int>, std::vector<std::vector<int>>> partition_map;
std::vector<std::vector<int>> partition[ATTRIBUTE_NUMBER];
std::vector<std::string> data[ATTRIBUTE_NUMBER];
std::map<std::vector<int>, std::vector<std::vector<int>>> m_map;

void ComputePartition();
std::vector<std::vector<int>> XMultipleY(std::vector<std::vector<int>> X, std::vector<std::vector<int>> Y);
void ComputeDependcies(Level &L);
void Prune(Level &L);
bool isHolds(std::vector<int> X, int B);
std::vector<int> ComputeRhsPlus(std::vector<int> &X);


void ComputePartition() {
	std::ifstream infile;
	infile.open("test_data.txt", std::ios::in);
	if (!infile.is_open())
		std::cout << "Failed to open the file." << std::endl;
	std::string tmp_str;

	int current_attr = 1;

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

	int value_num[15] = { 0 };
	for (int i = 0; i < ATTRIBUTE_NUMBER; i++)
		value_num[i] = 1;

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

	// 计算单属性划分
	for (int i = 0; i < ATTRIBUTE_NUMBER; i++) {
		for (int j = 0; j < value_num[i] - 1; j++) {
			std::vector<int> tmp_vec;
			(partition[i]).push_back(tmp_vec);
		}
		int tmp_size = data[i].size();
		for (int j = 0; j < tmp_size; j++) {
			partition[i][map[i][data[i][j]] - 1].push_back(j);
		}
		std::vector<int> tmp_vec;
		tmp_vec.push_back(i + 1);
		m_map[tmp_vec] = partition[i];
	}
	

	infile.close();
}

std::vector<std::vector<int>> XMultipleY(std::vector<std::vector<int>> X, std::vector<std::vector<int>> Y) {
	std::vector<std::vector<int>> result;
	std::unordered_map<int, int> T;
	std::vector<std::vector<int>> S;
	std::vector<int> tmp;
	S.push_back(tmp);

	for (int i = 1; i <= X.size(); i++) {
		for (int j = 0; j < X[i-1].size(); j++) {
			int t = X[i - 1][j];
			T[t] = i;
		}
		std::vector<int> tmp_vec;
		S.push_back(tmp_vec);
	}
	for (int i = 1; i <= Y.size(); i++) {
		for (int j = 0; j < Y[i - 1].size(); j++) {
			int t = Y[i - 1][j];
			if (T[t] != 0)
				S[T[t]].push_back(t);
		}
		for (int j = 0; j < Y[i - 1].size(); j++) {
			int t = Y[i - 1][j];
			if (S[T[t]].size() >= 2) {
				result.push_back(S[T[t]]);
			}
			S[T[t]].clear();
		}
	}
	for (int i = 1; i <= X.size(); i++) {
		for (int j = 0; j < X[i-1].size(); j++) {
			int t = X[i - 1][j];
			T[t] = 0;
		}
	}
	return result;
}

void ComputeDependcies(Level &L) {
	for (auto &X : L.set) {
		auto rhsPlus = ComputeRhsPlus(X);
		int length = X.size();
		for (int i = 0; i < length; ++i) {
			auto index = std::find(rhsPlus.begin(), rhsPlus.end(), X[i]);
			if (index != rhsPlus.end()) {
				auto xDeleteA = X;
				xDeleteA.erase(xDeleteA.begin() + i);
				if (isHolds(xDeleteA, X[i])) {
					for (auto &i : xDeleteA) {
						std::cout << i << " ";
					}
					std::cout << "->" << X[i] << std::endl;
					rhsPlus.erase(index);
					for (auto iter = rhsPlus.begin(); iter != rhsPlus.end(); ++iter) {
						if (std::find(X.begin(), X.end(), *iter) == X.end()) {
							rhsPlus.erase(iter);
						}
					}
				}
			}
		}
	}
}

void Prune(Level &L) {
	for (int i = 0; i < L.set.size(); ++i) {
		auto rhsPlus = ComputeRhsPlus(L.set[i]);
		if (rhsPlus.empty()) {
			L.set.erase(L.set.begin() + i);
		}
	}
}

bool isHolds(std::vector<int> X, int B) {
	if (X.size() == 0)
		return false;
	int a = m_map[X].size();
	std::vector<int> Y;
	Y.push_back(B);
	std::vector<std::vector<int>> tmp_vec = XMultipleY(m_map[X], m_map[Y]);
	int b = tmp_vec.size();
	X.push_back(B);
	sort(X.begin(), X.end());
	m_map[X] = tmp_vec;
	if (a == b)
		return true;
	else
		return false;
}

std::vector<int> ComputeRhsPlus(std::vector<int> &X) {
	std::vector<int> result;
	for (int i = 1; i <= ATTRIBUTE_NUMBER; i++) {
		// 如果i不在X中，不进行计算
		bool iInX = false;
		for (int j = 0; j < X.size(); j++) {
			if (i == X[j]) {
				iInX = true;
				break;
			}
		}
		if (!iInX)
			continue;
		//如果i在X中，进行计算
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
	while (L.set.size() != 0) {
		ComputeDependcies(L);
		Prune(L);
		L.GenerateNextLevel();
	}
	auto end_time = clock();
	std::cout << "cost time: " << (end_time - start_time) / 1000.0 << "s" << std::endl;

	system("pause");

	return 0;
}
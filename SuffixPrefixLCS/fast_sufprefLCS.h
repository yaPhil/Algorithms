#pragma once
#include<vector>
#include<string>

std::vector<std::vector<std::pair<size_t, size_t>>> makeGraph(const std::string &A, const std::string &B);

std::vector<std::vector<size_t>> buildMatrix(const std::string &A, const std::string &B);

std::vector<std::vector<size_t>> calculateSuffixPrefixLCS(const std::string &A, const std::string &B);
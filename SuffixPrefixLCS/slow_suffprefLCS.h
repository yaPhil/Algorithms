#pragma once
#include<string>
#include<algorithm>
#include<vector>

size_t LCS(const std::string& A, const std::string& B);

std::vector < std::vector<size_t>> slowSuffPrefLCS(const std::string& str1, const std::string& str2);

#include"slow_suffprefLCS.h"

size_t LCS(const std::string& A, const std::string& B)
{
	if (A.size() == 0 || B.size() == 0)
		return 0;
	std::vector<std::vector<size_t>> ans;
	ans.resize(A.size() + 1);
	for (size_t i = 0; i <= A.size(); ++i)
		ans[i].resize(B.size() + 1);

	for (int i = A.size() - 1; i >= 0; --i)
		for (int j = B.size() - 1; j >= 0; --j)
		{
			if (A[i] == B[j])
				ans[i][j] = 1 + ans[i + 1][j + 1];
			else
				ans[i][j] = std::max(ans[i + 1][j], ans[i][j + 1]);
		}
	return ans[0][0];
}

std::vector < std::vector<size_t>> slowSuffPrefLCS(const std::string& str1, const std::string& str2)
{
	std::vector<std::vector<size_t>> res;
	res.resize(str1.size());
	for (size_t i = 0; i < res.size(); ++i)
	{
		res[i].resize(str2.size());
		for (size_t j = 0; j < res[i].size(); ++j)
		{
			res[i][j] = LCS(str1.substr(i), str2.substr(0, j + 1));
		}
	}
	return res;
}
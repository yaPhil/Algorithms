#include"fast_sufprefLCS.h"

std::vector<std::vector<std::pair<size_t, size_t>>> makeGraph(const std::string &A, const std::string &B)
{
	std::vector<std::vector<std::pair<size_t, size_t>>> graph;
	graph.resize(B.size() + 1);
	for (auto i = 0; i <= B.size(); ++i)
		graph[i].resize(A.size() + 1);
	for (auto i = 1; i <= A.size(); ++i)
		graph[0][i].second = i;
	for (auto i = 1; i <= B.size(); ++i)
		graph[i][0].first = 0;
	for (auto i = 1; i <= B.size(); ++i)
	{
		for (auto j = 1; j <= A.size(); ++j)
		{
			if (graph[i][j - 1].first < graph[i - 1][j].second && B[i - 1] != A[j - 1])
			{
				graph[i][j].first = graph[i][j - 1].first;
				graph[i][j].second = graph[i - 1][j].second;
			}
			else
			{
				graph[i][j].first = graph[i - 1][j].second;
				graph[i][j].second = graph[i][j - 1].first;
			}
		}
	}
	return graph;
}

std::vector<std::vector<size_t>> buildMatrix(const std::string &A, const std::string &B)
{
	auto graph = makeGraph(A, B);
	std::vector<std::vector<size_t>> ans;
	ans.resize(B.size());
	for (auto i = 0; i < B.size(); ++i)
		ans[i].resize(A.size());
	for (auto i = 0; i < B.size(); ++i)
	{
		auto crit = graph[i + 1][A.size()].first;
		for (auto j = 0; j < A.size(); ++j)
		{
			if (crit == 0)
			{
				if (i != 0)
					ans[i][j] = ans[i - 1][j];
				else
					ans[i][j] = 0;
			}
			else
			{
				if (i != 0)
					if (j < crit)
						ans[i][j] = ans[i - 1][j] + 1;
					else
						ans[i][j] = ans[i - 1][j];
				else
					if (j < crit)
						ans[i][j] = 1;
					else
						ans[i][j] = 0;
			}
		}
	}
	return ans;
}

std::vector<std::vector<size_t>> calculateSuffixPrefixLCS(const std::string &A, const std::string &B)
{
	auto ans = buildMatrix(A, B);
	std::vector<std::vector<size_t>> res;
	res.resize(A.size());
	for (size_t i = 0; i < A.size(); ++i)
	{
		res[i].resize(B.size());
		for (size_t j = 0; j < B.size(); ++j)
			res[i][j] = ans[j][i];
	}
	return res;
}
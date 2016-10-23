#include<iostream>
#include<vector>
#include<algorithm>
#include<queue>
#include<set>
#include<iomanip>

class TEdge
{
public:
	int from;
	int to;
	int capacity;
	int flow;
	int cost;
	int reverseInd;
};

std::vector<std::vector<TEdge>> gr;
std::vector<int> distance;
std::vector<std::pair<int, int>> parent;
std::vector<int> minFlow;
std::vector<int> potencial;
std::vector<int> used;
int start;
int finish;
long long maxFlow;
long long minCost;

void Dejkstra(int n)
{
	distance[start] = 0;
	used.assign(n + 1, 0);
	while (2 * 2 == 4)
	{
		int pos = -1;
		int min = 2000000100;
		for (size_t i = start; i <= finish; ++i)
		{
			if (distance[i] < min && used[i] == 0)
			{
				min = distance[i];
				pos = i;
			}
		}
		if (pos == -1)
		{
			break;
		}
		used[pos] = 1;
		for (size_t i = 0; i < gr[pos].size(); ++i)
		{
			if (gr[pos][i].capacity > gr[pos][i].flow && used[gr[pos][i].to] == 0)
			{
				if (distance[pos] + gr[pos][i].cost + potencial[pos] - potencial[gr[pos][i].to] < distance[gr[pos][i].to])
				{
					distance[gr[pos][i].to] = distance[pos] + gr[pos][i].cost + potencial[pos] - potencial[gr[pos][i].to];
					parent[gr[pos][i].to] = std::make_pair(pos, i);
					minFlow[gr[pos][i].to] = std::min(minFlow[pos], gr[pos][i].capacity - gr[pos][i].flow);
				}
			}
		}
	}
	for (int i = start; i <= n; ++i)
	{
		if (used[i] == 1)
			potencial[i] += distance[i];
		else
			potencial[i] += distance[finish];
	}
}

void minCostMaxFLow(int n)
{
	potencial.assign(n + 1, 2000000100);
	potencial[start] = 0;
	for (size_t tmp = 0; tmp < n; ++tmp)
	{
		for (size_t i = start; i <= finish; ++i)
		{
			for (size_t j = 0; j < gr[i].size(); ++j)
			{
				if (gr[i][j].capacity > 0)
				{
					if (potencial[i] + gr[i][j].cost < potencial[gr[i][j].to])
					{
						potencial[gr[i][j].to] = potencial[i] + gr[i][j].cost;
					}
				}
			}
		}
	}
	//potencial.assign(n + 1, 0);
	while (2 * 2 == 4)
	{
		distance.assign(n + 1, 2000000100);
		parent.assign(n + 1, { -1, -1 });
		minFlow.assign(n + 1, 2000000100);
		Dejkstra(n);
		if (distance[finish] == 2000000100)
		{
			break;
		}
		maxFlow += minFlow[finish];
		int i = parent[finish].first;
		int j = parent[finish].second;
		int flow = minFlow[finish];
		while (i != -1)
		{
			gr[i][j].flow += flow;
			gr[gr[i][j].to][gr[i][j].reverseInd].flow -= flow;
			minCost += gr[i][j].cost * flow;
			j = parent[i].second;
			i = parent[i].first;
		}
		distance.clear();
		parent.clear();
		minFlow.clear();
	}
}


int main()
{
	int n;
	std::cin >> n;
	gr.resize(2 * n + 2);
	for (size_t i = 1; i <= n; ++i)
	{
		for (size_t j = 1; j <= n; ++j)
		{
			int tmp;
			std::cin >> tmp;
			gr[i].push_back({ (int)i, (int)(n + j), 1, 0, tmp, (int)gr[n + j].size() });
			gr[n + j].push_back({ (int)(n + j), (int)i, 0, 0, -tmp, (int)gr[i].size() - 1 });
		}
		gr[0].push_back({ 0, (int)i, 1, 0, 0, (int)gr[i].size() });
		gr[i].push_back({ (int)i, 0, 0, 0, 0, (int)gr[0].size() - 1 });
		gr[n + i].push_back({ (int)(n + i), 2 * n + 1, 1, 0, 0, (int)gr[2 * n + 1].size() });
		gr[2 * n + 1].push_back({ 2 * n + 1, (int)(n + i), 0, 0, 0, (int)gr[n + i].size() - 1 });
	}
	start = 0;
	finish = 2 * n + 1;
	minCostMaxFLow(2 * n + 2);
	std::cout << minCost << std::endl;
	for (size_t i = 1; i <= n; ++i)
	{
		for (size_t j = 0; j < gr[i].size(); ++j)
		{
			if (gr[i][j].capacity > 0 && gr[i][j].flow == gr[i][j].capacity)
			{
				std::cout << i << ' ' << gr[i][j].to - n << std::endl;
				break;
			}
		}
	}
	return 0;
}
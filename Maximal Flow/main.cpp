#include<iostream>
#include<vector>
#include<algorithm>
#include<queue>

class TEdge
{
public:
	int from;
	int to;
	int capacity;
	int flow;
	int reverseInd;
};

std::vector<std::vector<TEdge>> gr;
std::vector<int> skip;
std::vector<int> distance;
int maxFlow;
int start, finish;

int dinica(int v, int min)
{
	if (v == finish)
	{
		maxFlow += min;
		return min;
	}

	for (size_t i = skip[v]; i < gr[v].size(); ++i)
	{
		if ((gr[v][i].capacity - gr[v][i].flow > 0) && (distance[v] + 1 == distance[gr[v][i].to]))
		{
			int result = dinica(gr[v][i].to, std::min(min, gr[v][i].capacity - gr[v][i].flow));
			if (result > 0)
			{
				gr[v][i].flow += result;
				gr[gr[v][i].to][gr[v][i].reverseInd].flow -= result;
				return result;
			}
			else
			{
				++skip[v];
			}
		}
	}
	return 0;
}

int main()
{
	int n;
	
	std::vector<std::vector<int>> capacity;
	std::cin >> n;
	while (n != 0)
	{
		int m;
		capacity.resize(n + 1);
		gr.resize(n + 1);
		skip.assign(n + 1, 0);
		for (size_t i = 1; i <= n; ++i)
		{
			capacity[i].assign(n + 1, 0);
		}
		std::cin >> start >> finish >> m;
		for (size_t i = 0; (int)i < m; ++i)
		{
			int a, b, c;
			std::cin >> a >> b >> c;
			capacity[a][b] += c;
			capacity[b][a] += c;
		}
		for (size_t i = 1; (int)i <= n; ++i)
		{
			for (size_t j = 1; (int)j <= n; ++j)
			{
				if (capacity[i][j] > 0)
				{
					gr[i].push_back({ (int)i, (int)j, capacity[i][j], 0, (int)gr[j].size() });
					gr[j].push_back({ (int)j, (int)i, 0, 0, (int)gr[i].size() - 1 });
				}
			}
		}
		while (2 * 2 == 4)
		{
			std::queue<std::pair<int, int>> bfs;
			distance.assign(n + 1, 1000000000);
			skip.assign(n + 1, 0);
			bfs.push(std::make_pair(start, 0));
			distance[start] = 0;
			while (bfs.size() != 0)
			{
				std::pair<int, int> tmp = bfs.front();
				bfs.pop();
				for (size_t i = 0; i < gr[tmp.first].size(); ++i)
				{
					if ((distance[gr[tmp.first][i].to] > distance[tmp.first] + 1) && (gr[tmp.first][i].capacity - gr[tmp.first][i].flow > 0))
					{
						distance[gr[tmp.first][i].to] = distance[tmp.first] + 1;
						bfs.push(std::make_pair(gr[tmp.first][i].to, distance[tmp.first] + 1));
					}
				}
			}
			if (distance[finish] == 1000000000)
				break;
			while (dinica(start, 1000000000) > 0)
			{

			}
		}
		std::cout << maxFlow << std::endl;
		maxFlow = 0;
		distance.clear();
		skip.clear();
		gr.clear();
		capacity.clear();
		std::cin >> n;
	}
	return 0;
}
#include<iostream>
#include<vector>
#include<algorithm>
#include<queue>
#include<string>
#include<cmath>

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
std::vector<std::vector<int>> capacity;
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

void maximalFlow(int n)
{
	skip.assign(n + 1, 0);
	while (2 * 2 == 4)
	{
		std::queue<std::pair<int, int>> bfs;
		distance.assign(n + 1, 2000000000);
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
		if (distance[finish] == 2000000000)
			break;
		while (dinica(start, 2000000000) > 0)
		{

		}
	}
	distance.clear();
	skip.clear();
}

std::vector<int> comp;
std::vector<int> col;

void dfs(int v)
{
	col[v] = 1;
	comp[v] = 1;
	for (size_t i = 0; i < gr[v].size(); ++i)
	{
		if (gr[v][i].flow < gr[v][i].capacity && col[gr[v][i].to] == 0)
		{
			dfs(gr[v][i].to);
		}
	}
	col[v] = 2;
}

int main()
{
	std::string s, p;
	std::cin >> s >> p;
	std::vector<int> n, m;
	n.assign(s.size(), 0);
	m.assign(p.size(), 0);
	int cnt = 0;
	for (size_t i = 0; i < s.size(); ++i)
	{
		if (s[i] == '?')
		{
			++cnt;
			n[i] = cnt;
		}
	}
	for (size_t i = 0; i < p.size(); ++i)
	{
		if (p[i] == '?')
		{
			++cnt;
			m[i] = cnt;
		}
	}
	std::vector<std::pair<int, int>> cap;
	cap.assign(cnt + 2, { 0, 0 });
	gr.resize(cnt + 2);
	int ans = 0;
	for (size_t i = 0; (i + p.size() - 1) < s.size(); ++i)
	{
		for (size_t j = 0; j < p.size(); ++j)
		{
			if (p[j] == '?' && s[i + j] == '1')
			{
				cap[m[j]].second++;
			}
			if (p[j] == '?' && s[i + j] == '0')
			{
				cap[m[j]].first++;
			}
			if (s[i + j] == '?' && p[j] == '1')
			{
				cap[n[i + j]].second++;
			}
			if (s[i + j] == '?' && p[j] == '0')
			{
				cap[n[i + j]].first++;
			}
			if (p[j] == '?' && s[i + j] == '?')
			{
				gr[m[j]].push_back({ m[j], n[i + j], 1, 0, (int)gr[n[i + j]].size() });
				gr[n[i + j]].push_back({ n[i + j], m[j], 0, 0, (int)gr[m[j]].size() - 1 });

				gr[n[i + j]].push_back({ n[i + j], m[j], 1, 0, (int)gr[m[j]].size() });
				gr[m[j]].push_back({ m[j], n[i + j], 0, 0, (int)gr[n[i + j]].size() - 1 });
			}
			if ((p[j] == '1' && s[i + j] == '0') || (p[j] == '0' && s[i + j] == '1'))
			{
				++ans;
			}
		}
	}
	for (size_t i = 1; i <= cnt; ++i)
	{
		gr[0].push_back({ 0, (int)i, cap[i].second, 0, (int)gr[i].size() });
		gr[i].push_back({ (int)i, 0, 0, 0, (int)gr[0].size() - 1 });
		gr[i].push_back({ (int)i, cnt + 1, cap[i].first, 0, (int)gr[cnt + 1].size() });
		gr[cnt + 1].push_back({ cnt + 1, (int)i, 0, 0, (int)gr[i].size() - 1 });
	}
	start = 0;
	finish = cnt + 1;
	maximalFlow(cnt + 2);
	col.assign(cnt + 2, 0);
	comp.assign(cnt + 2, 0);
	dfs(0);
	for (size_t i = 1; i <= cnt; ++i)
	{
		if (comp[i] == 1)
		{
			for (size_t j = 0; j < s.size(); ++j)
			{
				if (n[j] == i)
				{
					s[j] = '1';
					break;
				}
			}
			for (size_t j = 0; j < p.size(); ++j)
			{
				if (m[j] == i)
				{
					p[j] = '1';
					break;
				}
			}
		}
		else
		{
			for (size_t j = 0; j < s.size(); ++j)
			{
				if (n[j] == i)
				{
					s[j] = '0';
					break;
				}
			}
			for (size_t j = 0; j < p.size(); ++j)
			{
				if (m[j] == i)
				{
					p[j] = '0';
					break;
				}
			}
		}
	}
	ans += maxFlow;
	std::cout << ans << std::endl << s << std::endl << p << std::endl;
	return 0;
}
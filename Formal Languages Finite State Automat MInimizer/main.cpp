#include<iostream>
#include<vector>
#include<set>
#include<queue>
#include<map>

using std::cout;
using std::endl;
using std::vector;
using std::cin;

int vertex;
int edges;
int numFinish;
int start;
vector<int> finish;
std::set<int> finishSet;
vector<vector<vector<int>>> notDeterminate;
vector<vector<vector<int>>> determinatedAutomat;
vector < vector < vector<int>>> minimizedAutomat;
vector<int> determinatedFinish;
std::set<int> determinatedFinishSet;
vector<int> minimizedFinish;

vector<int> color;

void dfs(int v, int from)
{
	color[v] = 1;
	if (finishSet.count(v))
	{
		if (finishSet.count(from) == 0)
		{
			finishSet.insert(from);
			finish.push_back(from);
		}
	}
	if (v != from)
	{
		for (int i = 0; i < notDeterminate[v][0].size(); ++i)
		{
			notDeterminate[from][0].push_back(notDeterminate[v][0][i]);
		}
		for (int i = 0; i < notDeterminate[v][1].size(); ++i)
		{
			notDeterminate[from][1].push_back(notDeterminate[v][1][i]);
		}
	}
	for (int i = 0; i < notDeterminate[v][2].size(); ++i)
	{
		if(color[notDeterminate[v][2][i]] == 0)
			dfs(notDeterminate[v][2][i], from);
	}
}

int main()
{
	cout << "ENTER NUMBER OF nodes" << endl;
	cin >> vertex;
	cout << "Enter start" << endl;
	cin >> start;
	cout << "Enter num finish vertexes" << endl;
	cin >> numFinish;
	for (int i = 0; i < numFinish; ++i)
	{
		int a;
		cin >> a;
		finish.push_back(a);
		finishSet.insert(a);
	}
	cout << "Enter number of edges and edges in format |from to letter|" << endl;
	cin >> edges;
	notDeterminate.resize(vertex + 1);
	for (int i = 1; i <= vertex; ++i)
	{
		notDeterminate[i].resize(3);
	}
	for (int i = 0; i < edges; ++i)
	{
		int from, to;
		char letter;
		cin >> from >> to >> letter;
		if (letter == 'a')
		{
			notDeterminate[from][0].push_back(to);
		}
		if(letter == 'b')
		{
			notDeterminate[from][1].push_back(to);
		}
		if (letter == 'e')
		{
			notDeterminate[from][2].push_back(to);
		}
	}

	cout << "I'm gonna delete epsilon" << endl;

	for (int i = 1; i <= vertex; ++i)
	{
		color.assign(vertex + 1, 0);
		dfs(i, i);
	}

	std::queue<std::set<int>> determinize;
	std::map<std::set<int>, int> biector;
	determinatedAutomat.resize(2);
	determinatedAutomat[1].resize(2);
	determinize.push({ start });
	biector.insert({ {start}, 1 });

	cout << "Determinating....." << endl;

	while (determinize.size() != 0)
	{
		std::set<int> current;
		current = determinize.front();
		determinize.pop();
		std::set<int>byA, byB;

		for (size_t i = 0; i < finish.size(); ++i)
		{
			if (current.count(finish[i]))
			{
				determinatedFinish.push_back(biector[current]);
				determinatedFinishSet.insert(biector[current]);
				break;
			}
		}

		for (auto it = current.begin(); it != current.end(); ++it)
		{
			int from = *it;
			for (size_t i = 0; i < notDeterminate[from][0].size(); ++i)
			{
				byA.insert(notDeterminate[from][0][i]);
			}
			for (size_t i = 0; i < notDeterminate[from][1].size(); ++i)
			{
				byB.insert(notDeterminate[from][1][i]);
			}
		}
		if (biector.count(byA))
		{
			determinatedAutomat[biector[current]][0].push_back(biector[byA]);
		}
		else
		{
			biector.insert({ byA, biector.size() + 1 });
			determinize.push(byA);
			determinatedAutomat.push_back({});
			determinatedAutomat[determinatedAutomat.size() - 1].resize(2);
			determinatedAutomat[biector[current]][0].push_back(biector[byA]);
		}
		if (biector.count(byB))
		{
			determinatedAutomat[biector[current]][1].push_back(biector[byB]);
		}
		else
		{
			biector.insert({ byB, biector.size() + 1 });
			determinize.push(byB);
			determinatedAutomat.push_back({});
			determinatedAutomat[determinatedAutomat.size() - 1].resize(2);
			determinatedAutomat[biector[current]][1].push_back(biector[byB]);
		}
	}
	cout << "Determinate!!" << endl;
	bool f = false;
	for (size_t i = 1; i < determinatedAutomat.size(); ++i)
	{
		cout << i << " byA ";
		if (determinatedAutomat[i][0].size() == 1)
		{
			cout << determinatedAutomat[i][0][0];
		}
		else
		{
			f = true;
		}
		cout << " byB ";
		if (determinatedAutomat[i][1].size() == 1)
		{
			cout << determinatedAutomat[i][1][0] << endl;
		}
		else
		{
			f = true;
		}
	}
	cout << "Finish vertexes: ";
	for (size_t i = 0; i < determinatedFinish.size(); ++i)
	{
		cout << determinatedFinish[i] << ' ';
	}
	cout << endl << "Popolnenie..." << "Popolnil" << endl;
	if (f)
	{
		int tmp = determinatedAutomat.size();
		determinatedAutomat.push_back({});
		determinatedAutomat[tmp].resize(2);
		determinatedAutomat[tmp][0].push_back(tmp);
		determinatedAutomat[tmp][1].push_back(tmp);
		for (size_t i = 1; i < determinatedAutomat.size(); ++i)
		{
			cout << i << " byA ";
			if (determinatedAutomat[i][0].size() == 1)
			{
				cout << determinatedAutomat[i][0][0];
			}
			else
			{
				determinatedAutomat[i][0].push_back(tmp);
				cout << tmp;
			}
			cout << " byB ";
			if (determinatedAutomat[i][1].size() == 1)
			{
				cout << determinatedAutomat[i][1][0] << endl;
			}
			else
			{
				determinatedAutomat[i][1].push_back(tmp);
				cout << tmp;
			}
		}
	}
	cout << "Now I'm going to MINIMIZE THIS" << endl;
	vector<int> classOfVertex;
	vector < vector<int>> classes;
	size_t numberOfVertex = determinatedAutomat.size() - 1;
	classes.resize(numberOfVertex + 1);
	classOfVertex.resize(numberOfVertex + 1);
	for (size_t i = 1; i <= numberOfVertex; ++i)
	{
		if (determinatedFinishSet.count(i))
		{
			classOfVertex[i] = 1;
		}
		else
		{
			classOfVertex[i] = 0;
		}
	}
	while (2 * 2 == 4)
	{
		vector<int> newClasses;
		newClasses.resize(numberOfVertex + 1);
		for (size_t i = 1; i <= numberOfVertex; ++i)
		{
			newClasses[i] = i;
		}
		for (size_t i = 1; i <= numberOfVertex; ++i)
		{
			for (size_t j = 1; j <= numberOfVertex; ++j)
			{
				if (i == j)
				{
					continue;
				}
				if (classOfVertex[i] == classOfVertex[j])
				{
					std::pair<int, int> fromI, fromJ;
					fromI.first = classOfVertex[determinatedAutomat[i][0][0]];
					fromI.second = classOfVertex[determinatedAutomat[i][1][0]];

					fromJ.first = classOfVertex[determinatedAutomat[j][0][0]];
					fromJ.second = classOfVertex[determinatedAutomat[j][1][0]];
					if (fromI.first == fromJ.first && fromI.second == fromJ.second)
					{
						newClasses[i] = std::min(newClasses[i], newClasses[j]);
						newClasses[j] = std::min(newClasses[i], newClasses[j]);
					}
				}
			}
		}
		bool changed = false;
		for (size_t i = 1; i <= numberOfVertex; ++i)
		{
			if (newClasses[i] != classOfVertex[i])
			{
				changed = true;
			}
			classOfVertex[i] = newClasses[i];
		}
		if (!changed)
			break;
	}
	vector<int> packer;
	packer.assign(numberOfVertex + 1, 0);
	int numberOfClasses = 0;
	for (size_t i = 1; i <= numberOfVertex; ++i)
	{
		if (packer[classOfVertex[i]] == 0)
		{
			packer[classOfVertex[i]] = numberOfClasses + 1;
			++numberOfClasses;
			classOfVertex[i] = packer[classOfVertex[i]];
		}
		else
		{
			classOfVertex[i] = packer[classOfVertex[i]];
		}
		classes[classOfVertex[i]].push_back(i);
	}
	minimizedAutomat.resize(numberOfClasses + 1);
	for (size_t i = 1; i <= numberOfClasses; ++i)
	{
		minimizedAutomat[i].resize(2);
	}
	for (size_t i = 1; i <= numberOfClasses; ++i)
	{
		minimizedAutomat[i][0].push_back(classOfVertex[determinatedAutomat[classes[i][0]][0][0]]);
		minimizedAutomat[i][1].push_back(classOfVertex[determinatedAutomat[classes[i][0]][1][0]]);
		if (determinatedFinishSet.count(classes[i][0]))
		{
			minimizedFinish.push_back(i);
		}
	}
	cout << "MINIMIZED!!11!!odin!odin" << endl;
	for (size_t i = 1; i <= numberOfClasses; ++i)
	{
		cout << i << " byA " << minimizedAutomat[i][0][0] << " byB " << minimizedAutomat[i][1][0] << endl;
	}
	cout << "Final vertexes are: ";
	for (size_t i = 0; i < minimizedFinish.size(); ++i)
	{
		cout << minimizedFinish[i] << ' ';
	}
	cout << endl;
	return 0;
}
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <assert.h>
#include <unordered_map>
#include <algorithm>
#include <stack>
#include <sstream>
#include <queue>
#include <memory>
#include <set>
#include <unordered_set>

using namespace std;

struct Tile
{
	int x, y;
};

bool operator<(const Tile& left, const Tile& right)
{
	return left.x * 10000 + left.y < right.x * 10000 + right.y;
}

void debugMap(vector<string>& map)
{
	for (int i = 0; i < map.size(); i++)
	{
		cout << map[i] << "\n";
	}
}

void explore(vector<string>& map, int startX, int startY, int steps = 64)
{
	int count = 0;
	stack<Tile> toExplore;
	toExplore.push({ startX, startY});
	for (int i = 0; i <= steps; i++)
	{
		stack<Tile> newToExplore;
		while (!toExplore.empty())
		{
			Tile curr = toExplore.top();
			toExplore.pop();

			if (curr.x < 0 || curr.x >= map[0].size())
				continue;
			if (curr.y < 0 || curr.y >= map.size())
				continue;
			if (map[curr.y][curr.x] == '#')
				continue;
			if (map[curr.y][curr.x] == 'O')
				continue;

			map[curr.y][curr.x] = 'O';
			newToExplore.push({ curr.x + 1, curr.y});
			newToExplore.push({ curr.x - 1, curr.y});
			newToExplore.push({ curr.x, curr.y + 1});
			newToExplore.push({ curr.x, curr.y - 1});
		}
		//debugMap(map);
		//cout << "\n";
		count = 0;
		for (int i = 0; i < map.size(); i++)
		{
			for (int j = 0; j < map[i].length(); j++)
			{
				if (map[j][i] == 'O')
				{
					count++;
					map[j][i] = '.';
				}
			}
		}
		toExplore = newToExplore;
	}
	cout << "Tile count: " << count << "\n";

	//int stepCounts[65];
	//memset(stepCounts, 0, sizeof(stepCounts));
	//stack<Tile> toExplore;
	//toExplore.push({ startX, startY, 0 });
	//while (!toExplore.empty())
	//{
	//	Tile curr = toExplore.top();
	//	toExplore.pop();

	//	if (curr.stepCount > 64)
	//		continue;
	//	if (curr.x < 0 || curr.x >= map[0].size())
	//		continue;
	//	if (curr.y < 0 || curr.y >= map.size())
	//		continue;

	//	cout << curr.stepCount << ": " << curr.x << "," << curr.y << "\n";
	//	stepCounts[curr.stepCount]++;

	//	toExplore.push({ curr.x + 1, curr.y, curr.stepCount + 1 });
	//	toExplore.push({ curr.x - 1, curr.y, curr.stepCount + 1 });
	//	toExplore.push({ curr.x, curr.y + 1, curr.stepCount + 1 });
	//	toExplore.push({ curr.x, curr.y - 1, curr.stepCount + 1 });
	//}

	//cout << "Tiles at step 64: " << stepCounts[64] << "\n";
}

int main()
{
	ifstream fin("input.txt");

	string currentIn;
	vector<string> map;

	while (std::getline(fin, currentIn))
	{
		map.push_back(currentIn);
	}
	fin.close();

	for (int i = 0; i < map.size(); i++)
	{
		for (int j = 0; j < map[i].length(); j++)
		{
			if (map[j][i] == 'S')
			{
				map[j][i] = '.';
				explore(map, i, j, 64);
			}
		}
	}
}
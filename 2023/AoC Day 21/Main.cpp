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
#include <map>

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
	set<string> test;
	for (int i = 0; i <= steps; i++)
	{
		stack<Tile> newToExplore;
		set<Tile> explored;
		set<Tile> toBeExploredSet;
		while (!toExplore.empty())
		{
			Tile curr = toExplore.top();
			toExplore.pop();

			//if (curr.x < 0 || curr.x >= map[0].size())
			//	continue;
			//if (curr.y < 0 || curr.y >= map.size())
			//	continue;
			int x = (curr.x + map[0].size()) % map[0].size();
			int y = (curr.y + map.size()) % map.size();
			if (map[y][x] == '#')
				continue;

			explored.insert({ curr.x,curr.y });

			vector<Tile> tilesToExplore{
				{ curr.x + 1, curr.y},
				{ curr.x - 1, curr.y},
				{ curr.x, curr.y + 1},
				{ curr.x, curr.y - 1},
			};

			for (Tile& t : tilesToExplore)
			{
				if (!toBeExploredSet.contains(t))
				{
					newToExplore.push(t);
					toBeExploredSet.insert(t);
				}
			}
		}

		string mapString = "";
		string mapString2 = "";
		string mapString3 = "";
		for (int i = 0; i < map.size(); i++)
		{
			string row = "";
			string row2 = "";
			string row3 = "";
			for (int j = 0; j < map[i].length(); j++)
			{
				//cout << (explored.contains({ j,i }) ? 'O' : map[i][j]);
				row += (explored.contains({ j,i }) ? 'O' : map[i][j]);
				row2 += (explored.contains({ j,(i - (int)map.size())}) ? 'O' : map[i][j]);
				row3 += (explored.contains({ j,(i - (int)map.size())*2 }) ? 'O' : map[i][j]);
			}
			mapString += row;
			mapString2 += row2;
			mapString2 += row3;
			//cout << "\n";
		}
		test.insert(mapString);
		test.insert(mapString2);
		test.insert(mapString3);
		//debugMap(map);
		//cout << "\n";
		count = explored.size();
		cout << "Step " << i << " tile count: " << count << "\n";
		toExplore = newToExplore;
	}
	cout << "Tile count: " << count << "\n";
}

void countPlots(std::map<Tile, int>& explored, int step)
{
	uint64_t count = 0;
	for (auto& item : explored)
	{
		int steps = item.second;
		if (steps % 2 == step % 2)
			count++;
	}
	cout << "Step " << step << " has count " << count << "\n";
}

void solvePart2(vector<string>& map, int startX, int startY, int steps = 64)
{
	std::map<Tile, int> explored;
	set<Tile> allExploredTilesSet;
	stack<Tile> toExplore;
	toExplore.push({ startX, startY });
	uint64_t countEvens = 0;
	uint64_t countOdds = 0;
	for (int i = 0; i <= steps; i++)
	{
		stack<Tile> newToExplore;
		set<Tile> toBeExploredSet;
		while (!toExplore.empty())
		{
			Tile curr = toExplore.top();
			toExplore.pop();
			
			// (a%b+b)%b;
			int width = map[0].size();
			int height = map.size();
			int x = (curr.x % width + width) % width;
			int y = (curr.y % height + height) % height;
			if (map[y][x] == '#')
				continue;
			if (explored.contains(curr))
				continue;
			explored[curr] = i;
			if (i % 2 == 1)
				countOdds++;
			else
				countEvens++;

			vector<Tile> tilesToExplore{
				{ curr.x + 1, curr.y},
				{ curr.x - 1, curr.y},
				{ curr.x, curr.y + 1},
				{ curr.x, curr.y - 1},
			};

			for (Tile& t : tilesToExplore)
			{
				if (toBeExploredSet.contains(t))
					continue;
				if (explored.contains(t))
					continue;
				newToExplore.push(t);
				toBeExploredSet.insert(t);
			}
		}
		toExplore = newToExplore;
		//countPlots(explored, i);
		if (i == steps || i % map.size() == 65)
		if (i % 2 == 1)
			cout << "Step " << i << " has count " << countOdds << "\n";
		else
			cout << "Step " << i << " has count " << countEvens << "\n";
	}
}

int main()
{
	// part 2 solution
	uint64_t count = 96829;
	uint64_t diffOfDiff = 30906;
	uint64_t diff = 61909;
	uint64_t target = 26501365;
	uint64_t stepInc = 131;
	for (uint64_t steps = 327; steps <= target; steps += stepInc)
	{
		if (steps == target)
			cout << "step " << steps << " has count " << count << "\n";
		diff += diffOfDiff;
		count += diff;
	}
	//---

	ifstream fin("input.txt");

	string currentIn;
	vector<string> map;

	while (std::getline(fin, currentIn))
	{
		map.push_back(currentIn);
	}
	fin.close();

	int countDots = 0;
	for (int i = 0; i < map.size(); i++)
	{
		for (int j = 0; j < map[i].length(); j++)
		{
			if (map[j][i] == 'S')
			{
				map[j][i] = '.';
				//explore(map, i, j, 5000);
				//solvePart2(map, i, j, 26501365);
			}
			if (map[j][i] == '.')
			{
				countDots++;
				//explore(map, i, j, 5000);
				//solvePart2(map, i, j, 26501365);
			}
		}
	}
	cout << countDots;
}
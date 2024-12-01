#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <assert.h>
#include <unordered_map>
#include <algorithm>
#include <stack>

using namespace std;

const vector<pair<int, int>> directions = { {1,0}, {-1,0}, {0,1}, {0,-1} };
vector<string> tileMap;
vector<string> tileMapExplored;
vector<vector<int>> tileMapCosts;

bool areDirectionsOpposite(pair<int, int> dir1, pair<int, int> dir2)
{
	int x1 = dir1.first;
	int y1 = dir1.second;
	int x2 = dir2.first;
	int y2 = dir2.second;
	if (x1 != 0 && x1 == -x2)
		return true;
	if (y1 != 0 && y1 == -y2)
		return true;
	return false;
}

bool canExplore(int x, int y, int xDir, int yDir)
{
	if (x < 0 || x >= tileMap[0].size())
		return false;
	if (y < 0 || y >= tileMap.size())
		return false;
	if (tileMap[y][x] == '#')
		return false;
	//if (tileMap[y][x] == '<' && xDir != -1)
	//	return false;
	//if (tileMap[y][x] == '>' && xDir != 1)
	//	return false;
	//if (tileMap[y][x] == '^' && yDir != -1)
	//	return false;
	//if (tileMap[y][x] == 'v' && yDir != 1)
	//	return false;

	if (tileMapExplored[y][x] == 'O')
		return false;

	return true;
}

int maxCost = -1;
void dfs(int x, int y, int cost)
{
	tileMapExplored[y][x] = 'O';

	if (y == tileMap.size() - 1 && tileMap[y][x] == '.')
	{
		maxCost = max(maxCost, cost);
		//cout << "Arrived with cost " << cost << "\n";
	}

	for (auto& dir : directions)
	{
		int newX = x + dir.first;
		int newY = y + dir.second;
		if (canExplore(newX, newY, dir.first, dir.second))
		{
			dfs(newX, newY, cost + 1);
		}
	}
	tileMapExplored[y][x] = tileMap[y][x];
}

// Part 2
bool canExplore2(int x, int y, int xDir, int yDir, int cost)
{
	if (x < 0 || x >= tileMap[0].size())
		return false;
	if (y < 0 || y >= tileMap.size())
		return false;
	if (tileMap[y][x] == '#')
		return false;

	//if (tileMapCosts[y][x] > cost) // we have a more expensive path, don't bother exploring
	//	return false;

	if (tileMapExplored[y][x] == 'O') // we explored already
		return false;

	return true;
}

int maxCost2 = -1;
void dfs2(int x, int y, int cost, int xDir = 0, int yDir = 0)
{
	//tileMapCosts[y][x] = max(tileMapCosts[y][x], cost);

	vector<pair<int, int>> validDirs;
	do
	{
		if (y == tileMap.size() - 1 && tileMap[y][x] == '.')
		{
			if (cost > maxCost2)
				cout << "Arrived with cost " << cost << "\n";
			maxCost2 = max(maxCost2, cost);
		}

		validDirs.clear();
		for (auto& dir : directions)
		{
			int newX = x + dir.first;
			int newY = y + dir.second;
			if (areDirectionsOpposite(dir, { xDir, yDir }))
				continue;
			if (canExplore2(newX, newY, dir.first, dir.second, cost + 1))
			{
				validDirs.push_back(dir);
			}
		}
		if (validDirs.size() == 1)
		{
			auto& dir = validDirs[0];
			xDir = dir.first;
			yDir = dir.second;
			x += dir.first;
			y += dir.second;
			cost++;
		}
	} while (validDirs.size() == 1);

	tileMapExplored[y][x] = 'O';
	cost++;

	for (auto& dir : validDirs)
	{
		int newX = x + dir.first;
		int newY = y + dir.second;
		
		dfs2(newX, newY, cost, dir.first, dir.second);
	}
	tileMapExplored[y][x] = tileMap[y][x];
}

int main()
{
	ifstream fin("input.txt");

	string currentIn;

	while (fin >> currentIn)
	{
		tileMap.push_back(currentIn);
		tileMapExplored.push_back(currentIn);
		tileMapCosts.push_back(vector<int>(currentIn.length()));
	}
	fin.close();

	for (int i = 0; i < tileMap[0].size(); i++)
	{
		if (tileMap[0][i] == '.')
		{
			// part 1
			//dfs(i, 0, 0);
			//cout << "Max cost is " << maxCost << "\n";
			//maxCost = 0;

			dfs2(i, 0, 0);
			cout << "Max cost is " << maxCost2 << "\n";
			break;
		}
	}
}
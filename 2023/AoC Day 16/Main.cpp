#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <cstring>
#include <assert.h>
#include <stack>

using namespace std;

struct Tile
{
	char symbol = '.';
	bool isEnergized = false;
	bool isExploredFromLeft = false;
	bool isExploredFromRight = false;
	bool isExploredFromTop = false;
	bool isExploredFromBottom = false;
};

struct Exploration
{
	int x, y, fromX, fromY;
};

vector<vector<Tile>> tileMap;

void refreshMap()
{
	for (int i = 0; i < tileMap.size(); i++)
	{
		for (int j = 0; j < tileMap[i].size(); j++)
		{
			tileMap[i][j].isEnergized = false;
			tileMap[i][j].isExploredFromBottom = false;
			tileMap[i][j].isExploredFromLeft = false;
			tileMap[i][j].isExploredFromRight = false;
			tileMap[i][j].isExploredFromTop = false;
		}
	}
}

void exploreMapNoRecursion(Exploration first)
{
	std::stack<Exploration> toExplore;
	toExplore.push(first);
	while (toExplore.size() > 0)
	{
		Exploration current = toExplore.top();
		toExplore.pop();
		int x = current.x;
		int y = current.y;
		int fromX = current.fromX;
		int fromY = current.fromY;
		if (x < 0 || x >= tileMap[0].size())
			continue;
		if (y < 0 || y >= tileMap.size())
			continue;

		int xDirection = x - fromX;
		int yDirection = y - fromY;

		if (xDirection < 0)
			if (tileMap[y][x].isExploredFromLeft)
				continue;
			else
				tileMap[y][x].isExploredFromLeft = true;
		if (xDirection > 0)
			if (tileMap[y][x].isExploredFromRight)
				continue;
			else
				tileMap[y][x].isExploredFromRight = true;
		if (yDirection > 0)
			if (tileMap[y][x].isExploredFromTop)
				continue;
			else
				tileMap[y][x].isExploredFromTop = true;
		if (yDirection < 0)
			if (tileMap[y][x].isExploredFromBottom)
				continue;
			else
				tileMap[y][x].isExploredFromBottom = true;

		//cout << x << ", " << y << "\n";
		tileMap[y][x].isEnergized = true;
		switch (tileMap[y][x].symbol)
		{
		case '.':
			toExplore.push({ x + xDirection, y + yDirection, x, y });
			break;
		case '\\':
			toExplore.push({ x + yDirection, y + xDirection, x, y });
			break;
		case '/':
			toExplore.push({ x - yDirection, y - xDirection, x, y });
			break;
		case '-':
			if (yDirection == 0)
			{
				toExplore.push({ x + xDirection, y + yDirection, x, y });
			}
			else
			{
				toExplore.push({ x + 1, y, x, y });
				toExplore.push({ x - 1, y, x, y });
			}
			break;
		case '|':
			if (xDirection == 0)
			{
				toExplore.push({ x + xDirection, y + yDirection, x, y });
			}
			else
			{
				toExplore.push({ x, y + 1, x, y });
				toExplore.push({ x, y - 1, x, y });
			}
			break;
		}
	}
}

void exploreMap(int x, int y, int fromX, int fromY)
{
	if (x < 0 || x >= tileMap[0].size())
		return;
	if (y < 0 || y >= tileMap.size())
		return;

	int xDirection = x - fromX;
	int yDirection = y - fromY;

	if (xDirection < 0)
		if (tileMap[y][x].isExploredFromLeft)
			return;
		else
			tileMap[y][x].isExploredFromLeft = true;
	if (xDirection > 0)
		if (tileMap[y][x].isExploredFromRight)
			return;
		else
			tileMap[y][x].isExploredFromRight = true;
	if (yDirection > 0)
		if (tileMap[y][x].isExploredFromTop)
			return;
		else
			tileMap[y][x].isExploredFromTop = true;
	if (yDirection < 0)
		if (tileMap[y][x].isExploredFromBottom)
			return;
		else
			tileMap[y][x].isExploredFromBottom = true;

	cout << x << ", " << y << "\n";
	tileMap[y][x].isEnergized = true;
	switch (tileMap[y][x].symbol)
	{
	case '.':
		exploreMap(x + xDirection, y + yDirection, x, y);
		break;
	case '\\':
		exploreMap(x + yDirection, y + xDirection, x, y);
		break;
	case '/':
		exploreMap(x - yDirection, y - xDirection, x, y);
		break;
	case '-':
		if (yDirection == 0)
		{
			exploreMap(x + xDirection, y + yDirection, x, y);
		}
		else
		{
			exploreMap(x + 1, y, x, y);
			exploreMap(x - 1, y, x, y);
		}
		break;
	case '|':
		if (xDirection == 0)
		{
			exploreMap(x + xDirection, y + yDirection, x, y);
		}
		else
		{
			exploreMap(x, y + 1, x, y);
			exploreMap(x, y - 1, x, y);
		}
		break;
	}
}

int countEnergized()
{
	int sum = 0;
	for (int i = 0; i < tileMap.size(); i++)
	{
		for (int j = 0; j < tileMap[i].size(); j++)
		{
			if (tileMap[i][j].isEnergized)
				sum++;
		}
	}
	return sum;
}

void printEnergized()
{
	for (int i = 0; i < tileMap.size(); i++)
	{
		for (int j = 0; j < tileMap[i].size(); j++)
		{
			if (tileMap[i][j].isEnergized)
				cout << "#";
			else
				cout << tileMap[i][j].symbol;
		}
		cout << "\n";
	}
}

int main()
{
	ifstream fin("input.txt");

	string currentIn;
	

	while (fin >> currentIn)
	{
		vector<Tile> currentRow;
		for (int i = 0; i < currentIn.length(); i++)
		{
			Tile t;
			t.symbol = currentIn[i];
			currentRow.push_back(t);
		}
		tileMap.push_back(currentRow);
	}
	fin.close();
	
	// part 1
	exploreMapNoRecursion({ 0, 0, -1, 0 });
	cout << "Energized tiles: " << countEnergized() << "\n\n";
	refreshMap();
	//exploreMap(0, 0, -1, 0);
	//printEnergized();

	// part 2
	vector<Exploration> explorations;
	int xMax = tileMap[0].size() - 1;
	int yMax = tileMap.size() - 1;
	for (int i = 0; i < tileMap.size(); i++)
	{
		explorations.push_back({ 0, i, -1, i });
		explorations.push_back({ xMax, i, xMax+1, i });
	}
	for (int i = 0; i < tileMap[0].size(); i++)
	{
		explorations.push_back({ i, 0, i, -1 });
		explorations.push_back({ i, yMax, i, yMax+1 });
	}
	int maxEnergized = -1;
	for (auto& e : explorations)
	{
		exploreMapNoRecursion(e);
		int energized = countEnergized();
		//cout << "Energized tiles from " << e.x << ", " << e.y << " : " << energized << "\n";
		//printEnergized();
		maxEnergized = max(maxEnergized, energized);
		refreshMap();
	}
	cout << "Max energized: " << maxEnergized << "\n";
}
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <algorithm>

using namespace std;

const int expandedTileSize = 1000000;

struct Galaxy
{
	int x, y;
};

struct UniverseTile
{
	char symbol = '.';
	int xSize = 1;
	int ySize = 1;
};

int main()
{
	ifstream fin("input.txt");

	string currentIn;
	vector<string> universeRaw;

	// read input
	while (fin >> currentIn)
	{
		universeRaw.push_back(currentIn);
	}
	fin.close();

	// Build universe with tiles
	vector<vector<UniverseTile>> universe;
	for (int i = 0; i < universeRaw.size(); i++)
	{
		vector<UniverseTile> row(universeRaw[i].length());
		for (int j = 0; j < universeRaw[i].length(); j++)
		{
			row[j].symbol = universeRaw[i][j];
		}
		universe.push_back(row);
	}

	// expand rows
	for (int i = 0; i < universe.size(); i++)
	{
		bool isEmpty = true;
		for (int j = 0; j < universe.size(); j++)
		{
			if (universe[i][j].symbol != '.')
			{
				isEmpty = false;
				break;
			}
		}
		if (isEmpty)
		{
			for (int j = 0; j < universe.size(); j++)
				universe[i][j].ySize = expandedTileSize;
		}
	}

	// expand columns
	for (int i = 0; i < universe[0].size(); i++)
	{
		bool isEmpty = true;
		for (int j = 0; j < universe.size(); j++)
		{
			if (universe[j][i].symbol != '.')
			{
				isEmpty = false;
				break;
			}
		}
		if (isEmpty)
		{
			for (int j = 0; j < universe.size(); j++)
				universe[j][i].xSize = expandedTileSize;
		}
	}

	// Identify galaxies
	vector<Galaxy> galaxies;
	for (int i = 0; i < universe.size(); i++)
	{
		for (int j = 0; j < universe[i].size(); j++)
		{
			if (universe[i][j].symbol == '#')
			{
				galaxies.push_back({j, i});
			}
		}
	}

	// Compute galaxy pairs
	int64_t sumLengths = 0;
	for (int i = 0; i < galaxies.size(); i++)
	{
		Galaxy& g1 = galaxies[i];
		for (int j = i + 1; j < galaxies.size(); j++)
		{
			Galaxy& g2 = galaxies[j];
			int xDir = g2.x > g1.x ? 1 : -1;
			int yDir = g2.y > g1.y ? 1 : -1;
			int x = g1.x, y = g1.y;
			int len = 0;
			while (x != g2.x)
			{
				x += xDir;
				len += universe[y][x].xSize;
			}

			while (y != g2.y)
			{
				y += yDir;
				len += universe[y][x].ySize;
			}
			sumLengths += len;
		}
	}
	cout << "Sum lengths: " << sumLengths << "\n";
}
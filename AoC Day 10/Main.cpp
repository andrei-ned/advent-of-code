#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <queue>
#include <stack>

#define N 150

using namespace std;


struct Tile
{
	char symbol = '.';
	int distanceToStart = -1;
	int x, y;
};

Tile pipeMap[N][N];

enum Direction
{
	None  = 0,
	North = 1 << 0,
	East  = 1 << 1,
	South = 1 << 2, 
	West  = 1 << 3,
};

bool isConnected(char pipeSymbol, Direction direction)
{
	switch (pipeSymbol)
	{
	case '|':
		return direction & (Direction::North | Direction::South);
	case '-':
		return direction & (Direction::East | Direction::West);
	case 'L':
		return direction & (Direction::North | Direction::East);
	case 'J':
		return direction & (Direction::North | Direction::West);
	case '7':
		return direction & (Direction::South | Direction::West);
	case 'F':
		return direction & (Direction::South | Direction::East);
	case 'S':
		return true;
	default:
		return false;
	}
}

void visitTile(queue<Tile*>& tilesToVisit, int x, int y, int dist, Direction direction)
{
	if (!isConnected(pipeMap[y][x].symbol, direction))
		return;
	Tile* tile = &pipeMap[y][x];
	if (tile->distanceToStart >= 0) // we visited before
		return;
	tile->distanceToStart = dist;
	tilesToVisit.push(tile);
}

int main()
{
	ifstream fin("input.txt");

	string currentIn;

	int Y = 0;
	while (fin >> currentIn)
	{
		for (int x = 0; x < currentIn.size(); x++)
		{
			Tile t;
			t.symbol = currentIn[x];
			pipeMap[Y+1][x+1] = t;
		}
		Y++;
	}
	fin.close();

	for (int y = 0; y < N; y++)
	{
		for (int x = 0; x < N; x++)
		{
			pipeMap[y][x].x = x;
			pipeMap[y][x].y = y;
		}
	}


	Tile* startTile;
	for (int y = 0; y < N; y++)
	{
		for (int x = 0; x < N; x++)
		{
			if (pipeMap[y][x].symbol == 'S')
			{
				startTile = &pipeMap[y][x];
				startTile->distanceToStart = 0;
			}
		}
	}

	queue<Tile*> tilesToVisit;
	tilesToVisit.push(startTile);
	while (tilesToVisit.size() > 0)
	{
		Tile& tile = *tilesToVisit.front();
		tilesToVisit.pop();
		//cout << tile.symbol << " at " << tile.x << ", " << tile.y << "\n";

		int distance = tile.distanceToStart + 1;
		if (isConnected(tile.symbol, Direction::East))
			visitTile(tilesToVisit, tile.x + 1, tile.y + 0, distance, Direction::West); // east
		if (isConnected(tile.symbol, Direction::West))
			visitTile(tilesToVisit, tile.x - 1, tile.y + 0, distance, Direction::East); // west
		if (isConnected(tile.symbol, Direction::South))
			visitTile(tilesToVisit, tile.x + 0, tile.y + 1, distance, Direction::North); // north
		if (isConnected(tile.symbol, Direction::North))
			visitTile(tilesToVisit, tile.x + 0, tile.y - 1, distance, Direction::South); // south
	}

	int maxDist = -1;
	for (int y = 0; y < N; y++)
	{
		for (int x = 0; x < N; x++)
		{
			maxDist = max(maxDist, pipeMap[y][x].distanceToStart);
		}
	}
	cout << "Max distance is " << maxDist << "\n";

	// Part 2
	// purge non-main loop
	for (int y = 0; y < N; y++)
	{
		for (int x = 0; x < N; x++)
		{
			if (pipeMap[y][x].distanceToStart < 0)
			{
				pipeMap[y][x].symbol = '.';
			}
		}
	}

	// build extended map
	char extendedMap[N * 2][N * 2];
	memset(extendedMap, '.', sizeof(extendedMap));
	for (int y = 0; y < N; y++)
	{
		for (int x = 0; x < N; x++)
		{
			extendedMap[y * 2][x * 2] = pipeMap[y][x].symbol;
		}
	}
	for (int y = 0; y < N * 2; y++)
	{
		for (int x = 0; x < N* 2; x++)
		{
			if ((x % 2 != 1) != (y % 2 != 1))
			{
				if (   isConnected(extendedMap[y][x - 1], Direction::East)
					&& isConnected(extendedMap[y][x + 1], Direction::West))
				{
					extendedMap[y][x] = '-';
				}

				if (   isConnected(extendedMap[y + 1][x], Direction::North)
					&& isConnected(extendedMap[y - 1][x], Direction::South))
				{
					extendedMap[y][x] = '|';
				}
			}
		}
	}

	// explore extended map
	stack<pair<int,int>> toExplore;
	toExplore.push({ 0,0 });
	while (toExplore.size() > 0)
	{
		auto current = toExplore.top();
		toExplore.pop();
		int x = current.second;
		int y = current.first;

		if (extendedMap[y][x] != '.')
			continue;

		//cout << "Filling at " << x << ", " << y << "\n";
		extendedMap[y][x] = ',';

		// Explore adjacent
		if (x > 0)
		{
			toExplore.push({ y, x - 1 });
		}
		if (x < N * 2 - 1)
		{
			toExplore.push({ y, x + 1 });
		}
		if (y > 0)
		{
			toExplore.push({ y - 1, x });

		}
		if (y < N * 2 - 1)
		{
			toExplore.push({ y + 1, x });
		}

	}

	// count area from extended map
	int innerCount = 0;
	for (int y = 0; y < N * 2; y++)
	{
		for (int x = 0; x < N * 2; x++)
		{
			if ((x % 2 == 0) && (y % 2 == 0))
			{
				if (extendedMap[y][x] == '.')
				{
					extendedMap[y][x] = 'I';
					innerCount++;
				}
			}
		}
	}
	cout << "Inner count: " << innerCount << "\n";

	// DEBUG
	for (int y = 0; y < 50; y++)
	{
		for (int x = 0; x < 50; x++)
		{
			cout << extendedMap[y][x];
		}
		cout << "\n";
	}
	cout << "\n";

	for (int y = 0; y < 50; y++)
	{
		for (int x = 0; x < 50; x++)
		{
			if ((x % 2 == 0) && (y % 2 == 0))
			{
				cout << extendedMap[y][x];
			}
		}
		if (y%2 == 0)
			cout << "\n";
	}
	cout << "\n";

	for (int y = 0; y < 30; y++)
	{
		for (int x = 0; x < 30; x++)
		{
			cout << pipeMap[y][x].symbol;
		}
		cout << "\n";
	}
	cout << "\n";

	for (int y = 0; y < 30; y++)
	{
		for (int x = 0; x < 30; x++)
		{
			if (pipeMap[y][x].distanceToStart < 0)
			{
				cout << '_';
			}
			else
			{
				if (pipeMap[y][x].distanceToStart > 9)
					cout << '9';
				else
					cout << pipeMap[y][x].distanceToStart;
			}
		}
		cout << "\n";
	}
	cout << "\n";
	// ---
}
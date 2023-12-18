#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <assert.h>
#include <unordered_map>
#include <algorithm>
#include <stack>

#define N 1000

using namespace std;

struct Tile
{
	char symbol = '.';
	string color = "(#000000)";
};

Tile tileMap[N][N];

void dig(int x, int y, string color)
{
	tileMap[y][x].symbol = '#';
	tileMap[y][x].color = color;
}

void printMap()
{
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			cout << tileMap[i][j].symbol;
		}
		cout << "\n";
	}
	cout << "\n";
}

void floodFill()
{
	stack<pair<int,int>> tileStack;
	tileStack.push({ 0,0 });
	
	while (!tileStack.empty())
	{
		auto t = tileStack.top();
		tileStack.pop();
		int x = t.first;
		int y = t.second;
		if (x < 0 || x >= N)
			continue;
		if (y < 0 || y >= N)
			continue;

		if (tileMap[y][x].symbol == '.')
		{
			tileMap[y][x].symbol = '_';
			tileStack.push({x + 1, y});
			tileStack.push({x - 1, y});
			tileStack.push({x, y + 1});
			tileStack.push({x, y - 1});
		}
	}
}

void countArea()
{
	int sum = 0;
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			if (tileMap[i][j].symbol != '_')
				sum++;
		}
	}
	cout << "Area is " << sum << "\n";
}

int main()
{
	ifstream fin("input.txt");

	string color;
	char direction;
	int len;

	int x = N/2;
	int y = N/2;
	dig(x, y, "(#000000)");
	while (fin >> direction >> len >> color)
	{
		int xDir = 0;
		int yDir = 0;
		switch (direction)
		{
		case 'U':
			yDir = -1;
			break;
		case 'D':
			yDir = 1;
			break;
		case 'L':
			xDir = -1;
			break;
		case 'R':
			xDir = 1;
			break;
		}
		while (len--)
		{
			x += xDir;
			y += yDir;
			dig(x, y, color);
		}
	}
	fin.close();

	floodFill();
	countArea();
	printMap();
}
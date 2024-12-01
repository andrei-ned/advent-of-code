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

};

Tile tileMap[N][N];

void dig(int x, int y)
{
	tileMap[y][x].symbol = '#';
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

void solvePart1()
{
	ifstream fin("input.txt");

	string color;
	char direction;
	int len;

	int x = N / 2;
	int y = N / 2;
	dig(x, y);
	while (fin >> direction >> len >> color)
	{
		int xDir = 0;
		int yDir = 0;

		//color.pop_back();
		//char instruction = color.back();
		//color.pop_back();
		////color.erase(0, 2);
		//color[0] = '0';
		//color[1] = 'x';
		//len = stoul(color, nullptr, 16);

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
			dig(x, y);
		}
	}
	fin.close();

	floodFill();
	countArea();
}

struct Instruction
{
	int64_t x, y;
	uint64_t len;
	char direction;
};

void insertIfMissing(vector<int64_t>& vec, int64_t toInsert)
{
	if (vec[0] > toInsert)
	{
		vec.insert(vec.begin(), toInsert);
		return;
	}
	if (vec[0] == toInsert)
		return;
	for (int i = 1; i < vec.size(); i++)
	{
		if (vec[i] == toInsert)
		{
			return;
		}
		if (toInsert > vec[i-1] && toInsert < vec[i])
		{
			vec.insert(vec.begin() + i, toInsert);
			return;
		}
	}
	vec.push_back(toInsert);
}

struct Wall
{
	int64_t from, to;
	int64_t other;
};

void solvePart2()
{
	ifstream fin("input.txt");

	string color;
	char direction;
	uint64_t len;

	int64_t x = 0;
	int64_t y = 0;
	
	vector<int64_t> xPositions{ 0 };
	vector<int64_t> yPositions{ 0 };
	vector<Wall> verticalWalls;
	vector<Wall> horizontalWalls;

	int64_t sumArea = 0;
	int64_t perimeter = 0;

	while (fin >> direction >> len >> color)
	{
		int xDir = 0;
		int yDir = 0;

		color.pop_back();
		char instruction = color.back();
		color.pop_back();
		color[0] = '0';
		color[1] = 'x';
		len = stoul(color, nullptr, 16);
		perimeter += len;

		switch (instruction)
		{
		case '3'://up
			yDir = -1;
			break;
		case '1'://down
			yDir = 1;
			break;
		case '2'://left
			xDir = -1;
			break;
		case '0'://right
			xDir = 1;
			break;
		}
		int64_t xPrev = x;
		int64_t yPrev = y;
		x += len * xDir;
		y += len * yDir;
		sumArea += (xPrev * y - yPrev * x);
		//Wall w;
		//if (xPrev - x != 0)
		//{
		//	w.from = min(xPrev, x);
		//	w.to   = max(xPrev, x);
		//	w.other = y;
		//	horizontalWalls.push_back(w);
		//}
		//else if (yPrev - y != 0)
		//{
		//	w.from = min(yPrev, y);
		//	w.to   = max(yPrev, y);
		//	w.other = x;
		//	verticalWalls.push_back(w);
		//}
		//insertIfMissing(xPositions, x);
		//insertIfMissing(yPositions, y);
		cout << "(" << x << "," << y << ")\n";

	}
	fin.close();

	perimeter /= 2;
	perimeter++;
	sumArea /= 2;
	sumArea += perimeter;
	cout << "Total area is " << sumArea << "\n";
	return;

	assert(x == 0);
	assert(y == 0);

	std::sort(verticalWalls.begin(), verticalWalls.end(), [](Wall& a, Wall& b) {return a.other < b.other;});

	
	for (int i = 1; i < yPositions.size(); i++)
	{
		int wallCount = 0;
		for (int j = 1; j < xPositions.size(); j++)
		{
			int64_t yLen = yPositions[i] - yPositions[i - 1];
			int64_t xLen = xPositions[j] - xPositions[j - 1];
			int64_t area = xLen * yLen;
			//cout << "Area at " << j << "," << i << " is " << area;
			int yPos = yPositions[i - 1];
			int wallMin = verticalWalls[j-1].from;
			int wallMax = verticalWalls[j-1].to;

			if (yPos >= wallMin && yPos < wallMax)
			{
				//cout << " (found wall " << j - 1 << ")";
				wallCount++;
			}
			if (wallCount % 2 == 1) // is in dig area
			{
				//cout << "!!!";
				sumArea += area;
			}
			//cout << "\n";
		}
	}

	cout << "Total area is " << sumArea << "\n";
}

int main()
{
	//solvePart1();
	solvePart2();
}

// 139896139742920 is too high
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <map>
#include <vector>
#include <set>

using namespace std;

bool isDigit(char in)
{
	return in >= '0' && in <= '9';
}

bool isSymbol(char in)
{
	bool isDot = in == '.';
	return !isDot && !isDigit(in);
}

struct EngineNumber
{
	int x, y;
	int value;
	bool isPartNumber = false;
};


int main()
{
	ifstream fin("input.txt");

	vector<string> engine;
	vector<EngineNumber> numbers;
	string currentIn;
	map<pair<int,int>, int> engineNumberPositions;

	while (fin >> currentIn)
	{
		engine.push_back(currentIn);
	}

	fin.close();

	bool isPrevDigit = false;
	//EngineNumber* current = nullptr;
	for (int y = 0; y < engine.size(); y++)
	{
		for (int x = 0; x < engine[0].length(); x++)
		{
			char current = engine[y][x];
			int currentDigit = current - '0';
			bool isCurrentDigit = isDigit(current);
			if (isCurrentDigit)
			{
				if (isPrevDigit)
				{
					numbers.back().value = numbers.back().value * 10 + currentDigit;
				}
				else
				{
					EngineNumber num;
					num.x = x;
					num.y = y;
					num.value = currentDigit;
					numbers.push_back(num);
				}

				for (int yy = y-1; yy <= y+1; yy++)
				{
					for (int xx = x-1; xx <= x+1; xx++)
					{
						if (!(yy > 0 && yy < engine.size()))
							continue;
						if (!(xx > 0 && xx < engine[0].length()))
							continue;
						if (isSymbol(engine[yy][xx]))
						{
							numbers.back().isPartNumber = true;
						}
					}
				}

				engineNumberPositions[{x, y}] = numbers.size()-1;
			}
			isPrevDigit = isCurrentDigit;
		}
	}

	// Part 2
	int gearSum = 0;
	for (int y = 0; y < engine.size(); y++)
	{
		for (int x = 0; x < engine[0].length(); x++)
		{
			if (engine[y][x] == '*')
			{
				set<int> adjacentParts;
				for (int yy = y - 1; yy <= y + 1; yy++)
				{
					for (int xx = x - 1; xx <= x + 1; xx++)
					{
						if (!(yy > 0 && yy < engine.size()))
							continue;
						if (!(xx > 0 && xx < engine[0].length()))
							continue;
						if (engineNumberPositions.contains({xx,yy}))
						{
							adjacentParts.insert(engineNumberPositions[{xx, yy}]);
						}
					}
				}
				if (adjacentParts.size() == 2)
				{
					// We have a gear!
					int gearRatio = 1;
					for (int idx : adjacentParts)
					{
						int no = numbers[idx].value;
						gearRatio *= no;
					}
					gearSum += gearRatio;
				}
			}
		}
	}

	int sum = 0;
	for (int i = 0; i < numbers.size(); i++)
	{
		cout << numbers[i].value << " " << numbers[i].isPartNumber << " y:" << numbers[i].y << "\n";
		if (numbers[i].isPartNumber)
		{
			sum += numbers[i].value;
		}
	}

	cout << "sum: " << sum << "\n";
	cout << "gear sum: " << gearSum << "\n";
}
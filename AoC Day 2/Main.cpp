#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>

// The Elf would first like to know which games would have been possible 
// if the bag contained only 12 red cubes, 13 green cubes, and 14 blue cubes?

using namespace std;

int main()
{
	ifstream fin("day2input.txt");

	unordered_map<string, int> possibleCubes =
	{
		{"red", 12},
		{"green", 13},
		{"blue", 14},
	};

	int sumIds = 0;
	int powerSum = 0;

	string currentIn;
	while (fin >> currentIn)
	{
		char terminator;
		int gameId;
		char colon;
		fin >> gameId >> colon;
		bool isGamePossible = true;

		unordered_map<string, int> minCubesThisGame =
		{
			{"red", 0},
			{"green", 0},
			{"blue", 0},
		};

		do
		{
			int cubeCount;
			string cubeColorWithTerminator;
			string cubeColor;
			fin >> cubeCount >> cubeColorWithTerminator;
			terminator = cubeColorWithTerminator.back();
			cubeColor = cubeColorWithTerminator;
			if (terminator == ',' || terminator == ';')
			{
				cubeColor.pop_back();
			}

			// Part 1
			int possibleCount = possibleCubes[cubeColor];
			if (cubeCount > possibleCount)
			{
				isGamePossible = false;
			}

			// Part 2
			int prevMin = minCubesThisGame[cubeColor];
			minCubesThisGame[cubeColor] = max(prevMin, cubeCount);
			if (!(terminator == ',' || terminator == ';')) // this game ends
			{
				int power = 1;
				for (const auto& item : minCubesThisGame)
				{
					auto color = item.first;
					auto count = item.second;
					power *= count;
				}
				powerSum += power;

				minCubesThisGame =
				{
					{"red", 0},
					{"green", 0},
					{"blue", 0},
				};
			}
		}
		while (terminator == ',' || terminator == ';');

		if (isGamePossible)
		{
			sumIds += gameId;
		}
	}

	cout << "sum Ids:" << sumIds << "\n";
	cout << "sum power:" << powerSum << "\n";

	fin.close();
}
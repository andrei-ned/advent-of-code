#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <map>
#include <vector>
#include <set>

using namespace std;

//const int winNoCount = 5;
//const int ownNoCount = 8;
const int winNoCount = 10;
const int ownNoCount = 25;

int main()
{
	ifstream fin("input.txt");

	string currentIn;
	int cardId;
	char colon;

	int pointsTotal = 0;
	int totalScratchcards = 0;

	int copiesCount[250];
	memset(copiesCount, 0, sizeof(copiesCount));

	while (fin >> currentIn)
	{
		fin >> cardId >> colon;
		copiesCount[cardId]++;
		bool isWinning[100];
		memset(isWinning, 0, sizeof(isWinning));
		for (int i = 0; i < winNoCount; i++)
		{
			int winningNo;
			fin >> winningNo;
			isWinning[winningNo] = true;
		}

		fin >> colon;

		// Part 1
		int points = 0;
		int winCount = 0;
		for (int i = 0; i < ownNoCount; i++)
		{
			int ownNo;
			fin >> ownNo;
			if (isWinning[ownNo])
			{
				winCount++;
				if (points == 0)
				{
					points = 1;
				}
				else
				{
					points *= 2;
				}
			}
		}
		cout << "Card " << cardId << " has " << points << " points.\n";
		pointsTotal += points;

		// Part 2
		cout << "There's " << copiesCount[cardId] << " copies of card " << cardId << "\n";
		for (int i = 0; i < winCount; i++)
		{
			copiesCount[cardId + i + 1] += copiesCount[cardId];
		}
		totalScratchcards += copiesCount[cardId];
	}

	cout << "Total points: " << pointsTotal << "\n";
	cout << "Total scratchcards: " << totalScratchcards << "\n";

	fin.close();
}
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

const string patterns[] = { "zero", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine"};

int getCalibration(string input)
{
	bool foundFirst = false;
	int first = 0, last = 0;

	int patternsIndex[10];
	memset(patternsIndex, 0, sizeof(patternsIndex));
	for (int i = 0; i < input.size(); i++)
	{
		char current = input[i];

		// current is last letter of spelled out digit
		int spelledDigit = -1;
		for (int i = 0; i < 10; i++)
		{
			if (current == patterns[i][patternsIndex[i]])
			{
				patternsIndex[i]++;
			}
			else
			{
				patternsIndex[i] = 0;
				if (current == patterns[i][patternsIndex[i]])
				{
					patternsIndex[i]++;
				}
			}

			if (patternsIndex[i] == patterns[i].size())
			{
				spelledDigit = i;
				patternsIndex[i] = 0;
			}
		}
		if (spelledDigit > -1)
		{
			if (!foundFirst)
			{
				foundFirst = true;
				first = spelledDigit;
			}
			last = spelledDigit;
		}

		// current is digit
		if (current >= '0' && current <= '9')
		{
			if (!foundFirst)
			{
				foundFirst = true;
				first = current - '0';
			}
			last = current - '0';
		}
	}
	int answer = first * 10 + last;
	//cout << input << ":" << answer << "\n";
	return answer;
}

int main()
{
	ifstream fin("day1input.txt");

	string currentIn;
	int sum = 0;
	while (fin >> currentIn)
	{
		sum += getCalibration(currentIn);
	}
	cout << sum;

	fin.close();
}
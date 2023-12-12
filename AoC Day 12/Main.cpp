#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <algorithm>

using namespace std;

int getBit(int target, int bitIdx)
{
	return (target >> bitIdx) & 1;
}

bool matchesGroups(string spring, vector<int> groups)
{
	int idx = 0;
	int count = 0;
	char last = '-';
	for (int i = 0; i < spring.length(); i++)
	{
		if (spring[i] == '#')
		{
			count++;
		}
		else
		{
			if (count == 0)
				continue;
			if (idx == groups.size())
				return false;
			if (groups[idx] != count)
				return false;
			idx++;
			count = 0;
		}
	}
	if (count > 0)
	{
		if (idx == groups.size())
			return false;
		if (groups[idx] != count)
			return false;
		idx++;
	}
	return idx == groups.size();
}

int countArrangements(string springs, vector<int>& groups)
{
	//cout << "Checking " << springs << "\n";

	int matches = 0;
	int countQuestions = 0;
	for (int i = 0; i < springs.length(); i++)
		if (springs[i] == '?')
			countQuestions++;
	for (int i = 0; i < 1 << countQuestions; i++)
	{
		string copySpring = springs;
		int qIdx = 0;
		for (int j = 0; j < copySpring.length(); j++)
		{
			if (copySpring[j] == '?')
			{
				copySpring[j] = getBit(i, qIdx) ? '#' : '.';
				qIdx++;
			}
		}
		if (matchesGroups(copySpring, groups))
		{
			//cout << "Found    " << copySpring << "\n";
			matches++;
		}
	}

	//cout << "\n";
	cout << "Total " << matches << " matches for " << springs << "\n";
	return matches;
}

int main()
{
	ifstream fin("input.txt");

	string currentIn;

	int sumArrangements = 0;
	while (fin >> currentIn)
	{
		string groupSizes;
		fin >> groupSizes;
		stringstream sin(groupSizes);
		vector<int> groups;
		int currSize;
		char comma;
		while (sin >> currSize)
		{
			sin >> comma;
			groups.push_back(currSize);
		}
		sumArrangements += countArrangements(currentIn, groups);
		//cout << currentIn << ": " << matchesGroups(currentIn, groups) << "\n";
	}
	fin.close();

	cout << "Sum: " << sumArrangements << "\n";
}
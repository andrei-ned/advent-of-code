#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <algorithm>

using namespace std;

unordered_map<string, uint64_t> cache;
uint64_t countCache(string springs, vector<int> groups)
{
	string key = springs;
	for (int i = 0; i < groups.size(); i++)
	{
		key += groups[i];
	}
	if (cache.contains(key))
	{
		return cache[key];
	}
	if (groups.size() == 0)
	{
		return springs.find('#') == string::npos ? 1 : 0; // if there is still # then we don't have a group for it
	}
	if (springs.length() == 0)
	{
		return 0; // there's still groups, but we ran out of characters
	}
	uint64_t count = 0;
	char first = springs[0];
	if (first == '.' || first == '?')
	{
		string copy = springs;
		copy.erase(0, 1);
		count += countCache(copy, groups); // recurse without first character
	}
	if (first == '#' || first == '?') // check if this can be a group
	{
		if (springs.length() < groups[0]) // not enough characters for this group
			return count;
		for (int i = 0; i < groups[0]; i++) // check group not to have '.'
			if (springs[i] == '.') // don't have room for a group of this length
				return count;
		if (springs[groups[0]] == '#') // element after group shouldn't be part of group
			return count;
		springs.erase(0, groups[0] + 1); // delete group + element after the group
		groups.erase(groups.begin());
		count += countCache(springs, groups); // recurse without first group and its characters
	}
	cache[key] = count;
	return count;
}

int main()
{
	ifstream fin("input.txt");

	string currentIn;

	uint64_t sumArrangements = 0;
	uint64_t sumUnfolded = 0;

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

		// part 2, unfold records
		string unfolded = currentIn;
		vector<int> unfoldedGroups;
		for (int i = 0; i < 4; i++)
			unfolded += "?" + currentIn;
		for (int i = 0; i < 5; i++)
		{
			for (int j = 0; j < groups.size(); j++)
				unfoldedGroups.push_back(groups[j]);
		}

		sumArrangements += countCache(currentIn, groups);
		sumUnfolded += countCache(unfolded, unfoldedGroups);
	}
	fin.close();

	cout << "Sum1: " << sumArrangements << "\n";
	cout << "Sum2: " << sumUnfolded << "\n";
}
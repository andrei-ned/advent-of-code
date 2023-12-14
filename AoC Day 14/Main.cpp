#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <map>

#define N 1000000000

using namespace std;

void printMap(vector<string>& map)
{
	for (int i = 0; i < map.size(); i++)
	{
		for (int j = 0; j < map[i].length(); j++)
		{
			cout << map[i][j];
		}
		cout << "\n";
	}
	cout << "\n";
}

void rollNorth(vector<string>& map)
{
	vector<int> emptySpaceIdx(map.size(), 0);
	for (int i = 0; i < map.size(); i++)
	{
		for (int j = 0; j < map[i].length(); j++)
		{
			if (map[i][j] == 'O')
			{
				swap(map[i][j], map[emptySpaceIdx[j]][j]);
				emptySpaceIdx[j]++;
			}
			if (map[i][j] != '.')
			{
				emptySpaceIdx[j] = i + 1;
			}
		}
	}
}

void rollSouth(vector<string>& map)
{
	vector<int> emptySpaceIdx(map.size(), map.size() - 1);
	for (int i = map.size() - 1; i >= 0; i--)
	{
		for (int j = 0; j < map[i].length(); j++)
		{
			if (map[i][j] == 'O')
			{
				swap(map[i][j], map[emptySpaceIdx[j]][j]);
				emptySpaceIdx[j]--;
			}
			if (map[i][j] != '.')
			{
				emptySpaceIdx[j] = i - 1;
			}
		}
	}
}

void rollWest(vector<string>& map)
{
	vector<int> emptySpaceIdx(map.size(), 0);
	for (int i = 0; i < map[0].length(); i++)
	{
		for (int j = 0; j < map.size(); j++)
		{
			if (map[i][j] == 'O')
			{
				swap(map[i][j], map[i][emptySpaceIdx[i]]);
				emptySpaceIdx[i]++;
			}
			if (map[i][j] != '.')
			{
				emptySpaceIdx[i] = j + 1;
			}
		}
	}
}

void rollEast(vector<string>& map)
{
	vector<int> emptySpaceIdx(map.size(), map.size() - 1);
	for (int i = 0; i < map[0].length(); i++)//y
	{
		for (int j = map.size() - 1; j >= 0; j--)//x
		{
			if (map[i][j] == 'O')
			{
				swap(map[i][j], map[i][emptySpaceIdx[i]]);
				emptySpaceIdx[i]--;
			}
			if (map[i][j] != '.')
			{
				emptySpaceIdx[i] = j - 1;
			}
		}
	}
}

int computeNorthLoad(vector<string>& map)
{
	int sum = 0;
	for (int i = 0; i < map.size(); i++)
	{
		int load = map.size() - i;
		for (int j = 0; j < map[i].length(); j++)
		{
			if (map[i][j] == 'O')
				sum += load;
		}
	}
	return sum;
}

void rollCycle(vector<string>& map)
{
	rollNorth(map);
	rollWest(map);
	rollSouth(map);
	rollEast(map);
}

int main()
{
	ifstream fin("input.txt");

	string currentIn;
	vector<string> map;

	while (getline(fin, currentIn))
	{
		map.push_back(currentIn);
	}
	fin.close();

	// part 1
	//printMap(map);
	//rollNorth(map);
	//printMap(map);

	// part 2
	vector<string> mapCopy(map);
	std::map<vector<string>,int> setOfMaps;
	int countCycles = 0, firstRepeat = 0;
	for (int i = 0; ; i++)
	{
		setOfMaps[map] = i;
		rollCycle(map);
		if (setOfMaps.contains(map))
		{
			cout << "Found map in set after " << i + 1 << " cycles.\n";
			cout << "Also occured at cycle " << setOfMaps[map] + 1 << "\n";
			countCycles = i - setOfMaps[map];
			firstRepeat = setOfMaps[map];
			break;
		}
	}

	map = mapCopy;
	for (int i = 0; i < firstRepeat; i++)
	{
		rollCycle(map);
	}
	int rollsInCycle = (N - firstRepeat) % (countCycles + 1);
	for (int i = 0; i < rollsInCycle; i++)
	{
		rollCycle(map);
	}

	cout << "Sum: " << computeNorthLoad(map) << "\n";
}
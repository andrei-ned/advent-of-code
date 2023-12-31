#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

vector<string> transposeMap(vector<string>& map)
{
	vector<string> transpose;
	for (int i = 0; i < map[0].length(); i++)
	{
		string line = "";
		for (int j = 0; j < map.size(); j++)
		{
			line += map[j][i];
		}
		transpose.push_back(line);
	}
	return transpose;
}

uint64_t solveMap(vector<string>& map)
{
	//int sum = 0;
	for (int i = 0; i < map.size() - 1; i++)
	{
		// Check if reflected between i and i+1
		//bool isMirrored = true;
		int differencesCount = 0;
		//cout << "Trying reflection at " << i+1 << ", " << i+2 << ": ";
		for (int j = i, k = i + 1; j >= 0 && k < map.size(); j--, k++)
		{
			//cout << "Comparing " << j << ", " << k << ": ";
			//if (map[j] != map[k])
			//{
			//	isMirrored = false;
			//	break;
			//}
			for (int q = 0; q < map[0].length(); q++)
			{
				if (map[j][q] != map[k][q])
				{
					differencesCount++;
				}
			}
		}
		//cout << differencesCount << "\n";
		if (differencesCount == 1)
		{
			return i + 1;
		}
		//cout << isMirrored << "\n";
		//if (isMirrored)
		//{
		//	//cout << (i + 1) << ", " << (i + 2) << "\n";
		//	sum += i + 1;
		//}
	}
	return 0;
}

uint64_t solve(vector<string>& map)
{
	auto transpose = transposeMap(map);
	return solveMap(map) * 100 + solveMap(transpose);
}

int main()
{
	ifstream fin("input.txt");

	string currentIn;
	vector<string> map;

	uint64_t sum = 0;
	while (getline(fin, currentIn))
	{
		if (!currentIn.empty())
			map.push_back(currentIn);
		if (currentIn.empty() || fin.eof())
		{
			sum += solve(map);
			map.clear();
			//cout << "\n\n";
		}
	}
	fin.close();

	cout << "Sum: " << sum << "\n";
}
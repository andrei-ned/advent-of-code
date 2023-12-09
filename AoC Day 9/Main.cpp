#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <algorithm>

using namespace std;

int solveFuture(vector<int>& sequence)
{
	vector<int> diff;
	for (int i = 1; i < sequence.size(); i++)
	{
		diff.push_back(sequence[i] - sequence[i - 1]);
	}
	bool isAllZeroes = true;
	for (int i = 0; i < diff.size(); i++)
	{
		if (diff[i] != 0)
			isAllZeroes = false;
	}
	if (isAllZeroes)
	{
		return sequence[sequence.size()-1];
	}
	else
	{
		return solveFuture(diff) + sequence[sequence.size() - 1];
	}
}

int solveHistory(vector<int>& sequence)
{
	vector<int> diff;
	for (int i = 1; i < sequence.size(); i++)
	{
		diff.push_back(sequence[i] - sequence[i - 1]);
	}
	bool isAllZeroes = true;
	for (int i = 0; i < diff.size(); i++)
	{
		if (diff[i] != 0)
			isAllZeroes = false;
	}
	if (isAllZeroes)
	{
		return sequence[0];
	}
	else
	{
		return sequence[0] - solveHistory(diff);
	}
}

int main()
{
	ifstream fin("input.txt");

	int currentIn;
	int sumFuture = 0;
	int sumHistory = 0;
	vector<int> oasis;

	while (fin >> currentIn)
	{
		oasis.push_back(currentIn);
		char peek = fin.peek();
		if (peek == '\n')
		{
			sumFuture += solveFuture(oasis);
			sumHistory += solveHistory(oasis);
			oasis.clear();
		}
	}

	fin.close();
	sumFuture += solveFuture(oasis);
	sumHistory += solveHistory(oasis);
	cout << "Future:  " << sumFuture << '\n';
	cout << "History: " << sumHistory << '\n';
}
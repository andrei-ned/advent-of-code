#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <map>
#include <vector>
#include <set>
#include <sstream>
#include <algorithm>
#include <assert.h>

using namespace std;

struct NumberRange
{
	NumberRange(int64_t start, int64_t length) : start(start), length(length)
	{

	}
	int64_t start, length;
};

void printVector(vector<NumberRange>& v)
{
	for (int i = 0; i < v.size(); i++)
	{
		cout << v[i].start << "," << v[i].length << " ";
	}
	cout << "\n";
}

void printVector(vector<int64_t>& v)
{
	for (int i = 0; i < 3; i++)
	{
		cout << v[i] << " ";
	}
	cout << "\n";
}

int main()
{
	ifstream fin("input.txt");

	string currentIn;

	vector<NumberRange> seeds;
	vector<NumberRange> seedsModified;

	fin >> currentIn;
	while (fin >> currentIn)
	{
		if (currentIn[0] >= '0' && currentIn[0] <= '9')
		{
			stringstream ss(currentIn);
			int64_t seed, range;
			ss >> seed;
			fin >> range;
			seeds.push_back({ seed, range });
		}
		else
		{
			break;
		}
	}

	printVector(seeds);

	cout << currentIn << "\n";
	fin >> currentIn;

	while (fin >> currentIn)
	{
		if (currentIn[0] >= '0' && currentIn[0] <= '9')
		{
			vector<NumberRange> newSeeds;

			int64_t destination, length, source;
			stringstream ss(currentIn);
			ss >> destination;
			fin >> source >> length;
			int64_t modifier = destination - source;

			int64_t min = source;
			int64_t max = source + length - 1;
			for (int i = 0; i < seeds.size(); i++)
			{
				int64_t start = seeds[i].start;
				int64_t len = seeds[i].length;
				int64_t end = start + len - 1;

				int64_t totalNewLen = 0;

				if (min >= start && min <= end && max >= start && max <= end) // modify some inner range
				{
					// start to min, unmodified
					int64_t dist = min - seeds[i].start;
					newSeeds.push_back({ start, dist });
					totalNewLen += dist;
					//assert(dist > 0);
					// min to max, modified
					seedsModified.push_back({ destination, length });
					totalNewLen += length;
					//assert(length > 0);
					// max to end, unmodified
					int64_t dist2 = max - seeds[i].start + 1;
					int64_t newStart = start + dist2;
					int64_t newLen = len - length - dist;
					newSeeds.push_back({ newStart, newLen });
					totalNewLen += newLen;
					//assert(newLen > 0);

					assert(totalNewLen == seeds[i].length);
				}
				else if (min >= start && min <= end) // modify from somewhere all the way to end
				{
					// start to min, unmodified
					int64_t newLen = min - start;
					newSeeds.push_back({ start, newLen });
					totalNewLen += newLen;
					//assert(newLen > 0);
					
					// min to end, modified
					seedsModified.push_back({ destination, len - newLen});
					totalNewLen += len - newLen;
					//assert(len - newLen > 0);
					
					assert(totalNewLen == seeds[i].length);
				}
				else if (max >= start && max <= end) // modify from start to somewhere
				{
					// start to max, modified
					int64_t dist = start - min;
					int64_t newStart = start + modifier;
					int64_t newLen = length - dist;
					seedsModified.push_back({ newStart, newLen });
					totalNewLen += newLen;
					//assert(newLen > 0);
					// max to end, unmodified
					int64_t dist2 = max - start + 1;
					int64_t newStart2 = start + dist2;
					int64_t newLen2 = len - newLen;
					newSeeds.push_back({ newStart2, newLen2 });
					totalNewLen += newLen2;
					//assert(newLen2 > 0);
					//if (newLen2 == 0)
					//{
					//	cout << "asdf";
					//}

					assert(totalNewLen == seeds[i].length);
				}
				else if (min <= start && max >= end) // modify all
				{
					// start to end, modified
					int64_t dist = seeds[i].start - min;
					//int64_t newStart = destination + dist;
					int64_t newStart = start + modifier;
					seedsModified.push_back({ newStart, seeds[i].length});
					totalNewLen += seeds[i].length;

					assert(totalNewLen == seeds[i].length);
				}
				else
				{
					// start to end, unmodified
					newSeeds.push_back(seeds[i]);
				}
			}

			seeds = newSeeds;
		}
		else
		{
			for (int i = 0; i < seeds.size(); i++)
			{
				if (seeds[i].length == 0)
				{
					continue;
				}
				seedsModified.push_back(seeds[i]);
			}
			seeds.swap(seedsModified);
			seedsModified.clear();
			printVector(seeds);

			cout << currentIn << "\n";
			fin >> currentIn;
		}
	}

	for (int i = 0; i < seeds.size(); i++)
	{
		if (seeds[i].length == 0)
		{
			continue;
		}
		seedsModified.push_back(seeds[i]);
	}
	seeds.swap(seedsModified);
	printVector(seeds);

	int64_t min = _I64_MAX;
	for (int i = 0; i < seeds.size(); i++)
	{
		if (seeds[i].start < min)
		{
			min = seeds[i].start;
		}
	}
	cout << "Lowest location number: " << min << "\n";

	fin.close();
}

// part 1
//int main()
//{
//	ifstream fin("input.txt");
//
//	string currentIn;
//
//	vector<int64_t> seeds;
//	vector<int64_t> seeds2;
//	fin >> currentIn;
//	while (fin >> currentIn)
//	{
//		if (currentIn[0] >= '0' && currentIn[0] <= '9')
//		{
//			//long seed = stol(currentIn);
//			stringstream ss(currentIn);
//			long seed;
//			ss >> seed;
//			seeds.push_back(seed);
//			seeds2.push_back(seed);
//		}
//		else
//		{
//			break;
//		}
//	}
//
//	cout << currentIn << "\n";
//	fin >> currentIn;
//
//	while (fin >> currentIn)
//	{
//		if (currentIn[0] >= '0' && currentIn[0] <= '9')
//		{
//			int64_t destination, length, source;
//			stringstream ss(currentIn);
//			ss >> destination;
//			fin >> source >> length;
//
//			int64_t min = source;
//			int64_t max = source + length;
//			for (int i = 0; i < seeds.size(); i++)
//			{
//				int64_t no = seeds[i];
//				if (no >= min && no < max)
//				{
//					int64_t dist = no - min;
//					int64_t newVal = destination + dist;
//					seeds2[i] = newVal;
//				}
//			}
//		}
//		else
//		{
//			seeds.swap(seeds2);
//			seeds2 = seeds;
//			printVector(seeds);
//
//			cout << currentIn << "\n";
//			fin >> currentIn;
//		}
//	}
//
//	seeds.swap(seeds2);
//	printVector(seeds);
//
//	sort(seeds.begin(), seeds.end());
//	cout << "Lowest location number: " << seeds[0] << "\n";
//
//	fin.close();
//}
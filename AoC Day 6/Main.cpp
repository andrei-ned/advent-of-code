#include <fstream>
#include <iostream>
#include <string>
#include <sstream>

using namespace std;


struct Record
{
	int time, distance;
};

//void solvePart1()
//{
//	ifstream fin("input.txt");
//
//	string currentIn;
//	vector<Record> records;
//
//	// Read input
//	fin >> currentIn;
//	while (fin >> currentIn)
//	{
//		if (currentIn[0] < '0' || currentIn[0] > '9')
//		{
//			break;
//		}
//		Record r;
//		r.time = stoi(currentIn);
//		records.push_back(r);
//	}
//
//	int i = 0;
//	while (fin >> currentIn)
//	{
//		records[i].distance = stoi(currentIn);
//		i++;
//	}
//
//	fin.close();
//
//
//	// Part 1
//	int winProduct = 1;
//	for (int j = 0; j < records.size(); j++)
//	{
//		int waysToWin = 0;
//		for (int buttonPress = 0; buttonPress < records[j].time; buttonPress++)
//		{
//			int speed = buttonPress;
//			int millisecs = records[j].time - buttonPress;
//			int distance = millisecs * speed;
//			if (distance > records[j].distance)
//			{
//				waysToWin++;
//			}
//		}
//		winProduct *= waysToWin;
//	}
//	cout << "Win product is " << winProduct << "\n";
//}

void solvePart2()
{
	ifstream fin("input.txt");
	string currentIn, fullstring = "";
	int64_t time = 0, distanceRecord = 0;


	// Read input
	fin >> currentIn;
	while (fin >> currentIn)
	{
		if (currentIn[0] < '0' || currentIn[0] > '9')
		{
			break;
		}
		fullstring += currentIn;
	}
	cout << fullstring << endl;
	stringstream ss(fullstring);
	ss >> time;

	fullstring = "";
	while (fin >> currentIn)
	{
		fullstring += currentIn;
	}
	cout << fullstring << endl;
	stringstream ss2(fullstring);
	ss2 >> distanceRecord;

	fin.close();

	// Solve part 2
	int64_t waysToWin = 0;
	for (int64_t buttonPress = 0; buttonPress < time; buttonPress++)
	{
		int64_t speed = buttonPress;
		int64_t millisecs = time - buttonPress;
		int64_t distance = millisecs * speed;
		if (distance > distanceRecord)
		{
			waysToWin++;
		}
	}
	cout << "Ways to win " << waysToWin << "\n";
}

int main()
{
	//solvePart1();
	solvePart2();
}
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <cstring>
#include <assert.h>

#define N 1000000000

using namespace std;

struct Lense
{
	int focalLength = -1;
	char label[20] = "";
};

void debugBoxContents(Lense boxes[][10])
{
	for (int i = 0; i < 256; i++)
	{
		if (boxes[i][0].focalLength >= 0)
		{
			bool isBoxEmpty = true;
			for (int j = 0; j < 10; j++)
			{

				if (boxes[i][j].focalLength >= 0)
					isBoxEmpty = false;
			}
			if (!isBoxEmpty)
			{
				cout << "Box " << i << ": ";
			}
			for (int j = 0; j < 10; j++)
			{
			
				if (boxes[i][j].focalLength >= 0)
					cout << "[" << boxes[i][j].label << " " << boxes[i][j].focalLength << "]" << " ";
			}
			cout << "\n";
		}
	}
	cout << "\n\n";
}

int HASH(string input)
{
	int count = 0;
	for (int i = 0; i < input.length(); i++)
	{
		count += input[i];
		count *= 17;
		count %= 256;
	}
	return count;
}

int main()
{
	ifstream fin("input.txt");

	char currentIn;
	vector<string> steps;
	string currentStep = "";

	while (fin >> currentIn)
	{
		if (currentIn == ',')
		{
			steps.push_back(currentStep);
			currentStep = "";
		}
		else
		{
			currentStep += currentIn;
		}
	}
	fin.close();
	steps.push_back(currentStep);

	// part 1
	int sum = 0;
	for (int i = 0; i < steps.size(); i++)
	{
		int currentHash = HASH(steps[i]);
		sum += currentHash;
		//cout << currentHash << ": " << steps[i] << "\n";
	}
	cout << "Sum hash: " << sum << '\n';

	// part 2
	Lense boxes[256][10];
	for (int stepIdx = 0; stepIdx < steps.size(); stepIdx++)
	{
		string label = steps[stepIdx];
		while (*(label.end()-1) < 'a' || *(label.end()-1) > 'z')
		{
			label.pop_back();
		}
		int boxId = HASH(label);
		char operation = steps[stepIdx][label.length()];
		//cout << boxId << ": " << label << " " << operation << "\n";
		if (operation == '-')
		{
			// remove lense with label from box
			for (int i = 0; i < 10; i++)
			{
				if (boxes[boxId][i].focalLength > 0 && boxes[boxId][i].label == label)
				{
					boxes[boxId][i].focalLength = -1;
					boxes[boxId][i].label[0] = 0;
					// shove lenses to the front
					int front = 0;
					for (int i = 0; i < 10; i++)
					{
						if (boxes[boxId][i].focalLength > 0)
						{
							if (front != i)
							{
								boxes[boxId][front] = boxes[boxId][i];
								boxes[boxId][i].focalLength = -1;
								boxes[boxId][i].label[0] = 0;
							}

							front++;
						}
					}
					break;
				}
			}
		}
		else if (operation == '=')
		{
			int focalLength = steps[stepIdx][steps[stepIdx].length() - 1] - '0';
			bool isReplaced = false;
			int firstEmptySlot = 0;
			
			// replace lense with same label
			for (int i = 0; i < 10; i++)
			{
				if (boxes[boxId][i].focalLength > 0)
					firstEmptySlot++;
				if (boxes[boxId][i].label == label)
				{
					boxes[boxId][i].focalLength = focalLength;
					isReplaced = true;
				}
			}
			// put in first empty slot if not replaced
			if (!isReplaced)
			{
				strcpy_s(boxes[boxId][firstEmptySlot].label, label.c_str());
				boxes[boxId][firstEmptySlot].label[label.length()] = '\0';
				boxes[boxId][firstEmptySlot].focalLength = focalLength;
			}

		}
		else
		{
			assert(false);
		}

		// debug
		//cout << "After " << steps[i] << "\n";
		//debugBoxContents(boxes);
	}

	debugBoxContents(boxes);

	int64_t totalFocusPower = 0;
	for (int i = 0; i < 256; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			Lense& lense = boxes[i][j];
			if (lense.focalLength >= 0)
			{
				int focusPower = (1 + i) * (j + 1) * lense.focalLength;
				totalFocusPower += focusPower;
				cout << lense.label << ": " << focusPower << "\n";
			}
		}
	}
	cout << "Total focus power: " << totalFocusPower << "\n";
}
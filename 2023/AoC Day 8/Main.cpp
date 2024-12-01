#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <algorithm>

using namespace std;

bool checkLastChar(char c, string& str)
{
	return c == str[2];
}

struct Node
{
	string left, right;
};

int main()
{
	ifstream fin("input.txt");

	string currentIn;
	string instructions;
	fin >> instructions;

	unordered_map<string, Node> nodes;
	vector<Node*> currentNodes;

	// read input
	while (fin >> currentIn)
	{
		Node currentNode;

		string left, right;
		char equal;
		fin >> equal >> left >> right;
		left.pop_back();
		left.erase(0, 1);
		right.pop_back();
		currentNode.left = left;
		currentNode.right = right;

		nodes[currentIn] = currentNode;
		if (checkLastChar('A', currentIn))
		{
			currentNodes.push_back(&nodes[currentIn]);
		}
	}

	//Node* currentNode = &nodes["AAA"];
	int instructionIdx = 0;
	int steps = 0;
	while (true)
	{
		steps++;

		char currentInstruction = instructions[instructionIdx];
		bool allAreZ = true;
		//for (auto& it : currentNodes)
		auto& it = currentNodes[5];
		{
			string nextStep =
				currentInstruction == 'L' ?
				it->left :
				it->right;
			it = &nodes[nextStep];
			//cout << nextStep << ", ";

			allAreZ = allAreZ && checkLastChar('Z', nextStep);

			if (allAreZ)
			{
				cout << nextStep << ": " << steps << ", " << instructionIdx << "\n";
				//break;
			}
		}
		//cout << "\n";
		//19631, 292
		//17287, 292
		//12599, 292
		//23147, 292
		//13771, 292
		//20803, 292
		// Ok, kinda cheated with the code here,
		// The answer was the lowest common denominator of these 6 numbers

		//if (allAreZ)
		//{
		//	cout << steps << ", " << instructionIdx << "\n";
		//	//break;
		//}
		//string nextStep =
		//	instructions[instructionIdx] == 'L' ?
		//	currentNode->left :
		//	currentNode->right;
		//currentNode = &nodes[nextStep];

		//cout << nextStep << "\n";

		//if (nextStep == "ZZZ")
		//{
		//	break;
		//}

		// Update index
		instructionIdx++;
		if (instructionIdx >= instructions.length())
		{
			instructionIdx = 0;
		}
	}

	cout << "Steps: " << steps << "\n";

	fin.close();
}
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <algorithm>

using namespace std;

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
	}

	Node* currentNode = &nodes["AAA"];
	int instructionIdx = 0;
	int steps = 0;
	while (true)
	{
		steps++;
		string nextStep =
			instructions[instructionIdx] == 'L' ?
			currentNode->left :
			currentNode->right;
		currentNode = &nodes[nextStep];

		//cout << nextStep << "\n";

		if (nextStep == "ZZZ")
		{
			break;
		}

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
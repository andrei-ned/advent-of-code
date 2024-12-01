#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <assert.h>
#include <unordered_map>
#include <algorithm>

using namespace std;

struct Node
{
	int x, y, xDir, yDir, dirCount;
};

uint64_t getCompareVal(const Node& node)
{

	int val = node.x;
	val = val << 8;
	val = val |= node.y;
	val = val << 8;
	val = val |= (node.xDir + 1);
	val = val << 3;
	val = val |= (node.yDir + 1);
	val = val << 3;
	val = val |= node.dirCount;
	return val;
}

bool operator==(const Node& left, const Node& right)
{
	return left.x == right.x
		&& left.y == right.y
		&& left.xDir == right.xDir
		&& left.yDir == right.yDir
		&& left.dirCount == right.dirCount;
}

template <>
struct std::hash<Node>
{
	std::size_t operator()(const Node& k) const
	{
		return getCompareVal(k);
	}
};

const vector<pair<int, int>> directions = { {1,0}, {-1,0}, {0,1}, {0,-1} };
vector<vector<int>> heatLossMap;
unordered_map<Node, int> nodeCosts;
vector<Node> toExplore;

bool cmpNode(Node& a, Node& b)
{
	return nodeCosts[a] > nodeCosts[b];
}

void tryAddToExplore(Node& current, int xDir, int yDir, int dirCount)
{
	Node newNode = current;
	newNode.x += xDir;
	newNode.y += yDir;
	newNode.xDir = xDir;
	newNode.yDir = yDir;
	newNode.dirCount = dirCount;
	int x = newNode.x;
	int y = newNode.y;
	if (x < 0 || x >= heatLossMap[0].size())
		return;
	if (y < 0 || y >= heatLossMap.size())
		return;
	int newCost = nodeCosts[current] + heatLossMap[y][x];

	if (nodeCosts.contains(newNode))
	{
		if (nodeCosts[newNode] <= newCost)
			return;
	}
	//cout << "adding " << newNode.x << "," << newNode.y << " with dir " << newNode.xDir << "," << newNode.yDir << " with dist " << newNode.dirCount << " and heat " << newCost << "\n";
	nodeCosts[newNode] = newCost;
	toExplore.push_back(newNode);
	push_heap(toExplore.begin(), toExplore.end(), cmpNode);
}

int prevCost = -1;
void traverseMap(int minDist, int maxDist)
{
	Node first = { 1,0,1,0,1 };
	toExplore.push_back(first);
	nodeCosts[first] = heatLossMap[0][1];
	Node second{ 0,1,0,1,1 };
	toExplore.push_back(second);
	nodeCosts[second] = heatLossMap[1][0];
	std::make_heap(toExplore.begin(), toExplore.end(), cmpNode);
	while (toExplore.size() > 0)
	{
		pop_heap(toExplore.begin(), toExplore.end(), cmpNode);
		Node current = toExplore.back();
		//cout << "exploring [" << current.x << "," << current.y << "] with dir " << current.xDir << "," << current.yDir << " with dist " << current.dirCount << " and heat " << nodeCosts[current] << "\n";
		toExplore.pop_back();
		if (nodeCosts[current] < prevCost)
		{
			assert(false);
		}
		prevCost = nodeCosts[current];
		if (current.x == heatLossMap[0].size() - 1 && current.y == heatLossMap.size() - 1 && current.dirCount >= minDist)
		{
			cout << "End heat is " << nodeCosts[current] << "\n";
			return;
		}
		if (current.dirCount < maxDist)
		{
			tryAddToExplore(current, current.xDir, current.yDir, current.dirCount+1);
		}
		if (current.dirCount >= minDist)
		{
			for (auto& dir : directions)
			{
				if (abs(current.xDir) == abs(dir.first) || abs(current.yDir) == abs(dir.second))
					continue;
				tryAddToExplore(current, dir.first, dir.second, 1);
			}
		}
	}
}

int main()
{
	ifstream fin("input.txt");

	string currentIn;

	while (fin >> currentIn)
	{
		vector<int> currentRow;
		for (int i = 0; i < currentIn.length(); i++)
		{
			int heatLoss = currentIn[i] - '0';
			currentRow.push_back(heatLoss);
		}
		heatLossMap.push_back(currentRow);
	}
	fin.close();

	// part 1
	//traverseMap(0, 3);

	// part 2
	traverseMap(4, 10);
}
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <assert.h>
#include <unordered_map>
#include <algorithm>
#include <set>
#include <sstream>
#include <stack>
#include <time.h>
#include <stdlib.h>
#include <map>
#include <queue>

using namespace std;

struct Node {
	vector<string> adjacent;
	map<string, int> distances;
};

void removeElemFromVector(vector<string>& vec, string& elem)
{
	vec.erase(find(vec.begin(), vec.end(), elem));
}

void debugConnection(const pair<string, string>& con)
{
	cout << "{" <<  con.first << "," << con.second << "} ";
}

int traverseGraph(unordered_map<string, Node>& nodes)
{
	set<string> visited;
	stack<string> toVisit;
	toVisit.push(nodes.begin()->first);
	while (!toVisit.empty())
	{
		string curr = toVisit.top();
		toVisit.pop();
		visited.insert(curr);
		for (auto& adjacent : nodes[curr].adjacent)
		{
			if (!visited.contains(adjacent))
			{
				toVisit.push(adjacent);
			}
		}
	}

	//cout << "Visited " << visited.size() << " nodes out of " << nodes.size() << "\n";
	return visited.size();
}

int traverseWithoutConnections(
	unordered_map<string, Node> nodes,
	pair<string, string> con1,
	pair<string, string> con2,
	pair<string, string> con3)
{
	removeElemFromVector(nodes[con1.first].adjacent,  con1.second);
	removeElemFromVector(nodes[con1.second].adjacent, con1.first);
	removeElemFromVector(nodes[con2.first].adjacent,  con2.second);
	removeElemFromVector(nodes[con2.second].adjacent, con2.first);
	removeElemFromVector(nodes[con3.first].adjacent,  con3.second);
	removeElemFromVector(nodes[con3.second].adjacent, con3.first);

	return traverseGraph(nodes);
}

void solvePart1(unordered_map<string, Node>& nodes)
{
	set<string> nodesInGroup2;
	nodesInGroup2.insert(nodes.begin()->first);

	while (true)
	{
		int totalNeighboursInGroup2 = 0;
		int maxNeighboursInGroup2 = -1;
		string target;
		for (auto& node : nodes)
		{
			if (nodesInGroup2.contains(node.first))
				continue;
			int neighboursInGroup2 = 0;
			for (auto& neighbour : node.second.adjacent)
			{
				if (nodesInGroup2.contains(neighbour))
				{
					neighboursInGroup2++;
					totalNeighboursInGroup2++;
				}
			}
			if (neighboursInGroup2 > maxNeighboursInGroup2)
			{
				maxNeighboursInGroup2 = neighboursInGroup2;
				target = node.first;
			}
		}

		if (totalNeighboursInGroup2 == 3)
		{
			int group2Count = nodesInGroup2.size();
			int group1Count = nodes.size() - group2Count;
			cout << "Group 1 has " << group1Count << "\n";
			cout << "Group 2 has " << group2Count << "\n";
			cout << "Answer is " << group1Count*group2Count << "\n";
			return;
		}

		nodesInGroup2.insert(target);
	}
}

int main()
{
	ifstream fin("input.txt");

	string currentIn;
	unordered_map<string, Node> nodes;
	vector<pair<string, string>> connections;

	while (std::getline(fin, currentIn))
	{
		stringstream ss(currentIn);
		string curr, con;
		ss >> curr;
		curr.pop_back();
		while (ss >> con)
		{
			nodes[curr].adjacent.push_back(con);
			nodes[con].adjacent.push_back(curr);
			connections.push_back({ curr, con });
		}
	}
	fin.close();
	
	solvePart1(nodes);

	//for (int i = 1; i < connections.size(); i++) 
	//{
	//	auto& a = connections[i];
	//	for (int j = i + 1; j < connections.size(); j++) 
	//	{
	//		auto& b = connections[j];
	//		for (int k = j + 1; k < connections.size(); k++)
	//		{
	//			auto& c = connections[k];
	//			int visitedCount = traverseWithoutConnections(nodes, a, b, c);
	//			if (visitedCount != nodes.size())
	//			{
	//				cout << "Found 3 connections that split the graph: ";
	//				debugConnection(a);
	//				debugConnection(b);
	//				debugConnection(c);
	//				int nonVisited = nodes.size() - visitedCount;
	//				cout << " with sizes " << visitedCount << ", " << nonVisited << "\n\n";
	//			}
	//		}
	//	}
	//}
}
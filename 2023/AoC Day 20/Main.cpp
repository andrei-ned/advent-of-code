#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <assert.h>
#include <unordered_map>
#include <algorithm>
#include <stack>
#include <sstream>
#include <queue>
#include <memory>

using namespace std;

struct Pulse
{
	bool isLow;
	string target;
	string from;
};

struct Module
{
	Module()
	{
		targets = {};
	}
	Module(vector<string> targets) : targets(targets)
	{}

	vector<Pulse> virtual receivePulse(Pulse& p)
	{
		return receivePulse(p.isLow, p.from);
	}
	vector<Pulse> virtual receivePulse(bool isLow, const string& fromModule) = 0;
	void virtual initInput(string inputModule) {}

	vector<string> targets;
};

struct NoOp : Module
{
	vector<Pulse> virtual receivePulse(bool isLow, const string& fromModule)
	{
		return {};
	}
};

struct FlipFlop : Module
{
	FlipFlop(vector<string> targets) : Module(targets)
	{

	}

	bool state = false;

	vector<Pulse> receivePulse(bool isLow, const string& fromModule)
	{
		vector<Pulse> pulses;
		if (isLow)
		{
			for (string& target : targets)
			{
				pulses.push_back({ state, target });
			}
			state = !state;
		}
		return pulses;
	}
};

struct Conjuction : Module
{
	Conjuction(vector<string> targets) : Module(targets)
	{
		allAreLow = true;
		allAreHigh = false;
	}

	unordered_map<string, bool> lastStates;
	bool allAreLow, allAreHigh;

	vector<Pulse> receivePulse(bool isLow, const string& fromModule)
	{
		vector<Pulse> pulses;
		lastStates[fromModule] = isLow;
		allAreHigh = true;
		allAreLow = true;
		for (auto& item : lastStates)
		{
			bool rememberedStateIsLow = item.second;
			if (rememberedStateIsLow)
				allAreHigh = false;
			else
				allAreLow = false;
		}
		for (string& target : targets)
		{
			pulses.push_back({ allAreHigh, target });
		}
		return pulses;
	}

	void debugStates()
	{
		for (auto item : lastStates)
		{
			cout << (item.second ? 0 : 1);
		}
		cout << "\n";
	}

	void initInput(string inputModule)
	{
		lastStates[inputModule] = true;
		allAreLow = true;
	}
};

struct Broadcast : Module
{
	Broadcast(vector<string> targets) : Module(targets)
	{

	}

	vector<Pulse> receivePulse(bool isLow, const string& fromModule)
	{
		vector<Pulse> pulses;
		for (string& target : targets)
		{
			pulses.push_back({ isLow, target });
		}
		return pulses;
	}
};

bool isConjuction(Module* m)
{
	return dynamic_cast<Conjuction*>(m) != nullptr;
}

void initConjuctions(unordered_map<string, unique_ptr<Module>>& modules)
{
	for (auto& item : modules)
	{
		const string& name = item.first;
		Module& module = *item.second;
		for (string& target : module.targets)
		{
			modules[target]->initInput(name);
		}
	}
}

void solvePart1(unordered_map<string, unique_ptr<Module>>& modules)
{
	initConjuctions(modules);

	uint64_t lowPulseCount = 0;
	uint64_t highPulseCount = 0;

	for (int i = 0; i < 1000; i++)
	{
		queue<Pulse> pulses;
		pulses.push({ true, "broadcaster", "button" });

		while (!pulses.empty())
		{
			Pulse curr = pulses.front();
			pulses.pop();
			if (curr.isLow)
				lowPulseCount++;
			else
				highPulseCount++;

			//cout << curr.from << (curr.isLow ? " -low-> " : " -high-> ") << curr.target << "\n";
			for (Pulse& p : modules[curr.target]->receivePulse(curr))
			{
				p.from = curr.target;
				pulses.push(p);
			}
		}
		//cout << "\n";
	}

	uint64_t mul = lowPulseCount * highPulseCount;
	cout << "Answer p1: " << mul << "\n\n";
}

void solvePart2(unordered_map<string, unique_ptr<Module>>& modules)
{
	initConjuctions(modules);

	for (auto& item : modules)
	{
		string label = item.first;
		Module& m = *item.second;
		for (string& target : m.targets)
		{
			if (isConjuction(&m))
				cout << "&";
			cout << label << " ";
			if (isConjuction(modules[target].get()))
				cout << "&";
			cout << target << "\n";
		}
	}

	vector<string> modulesSorted;
	queue<string> mq;
	for (string& target : modules["broadcaster"]->targets)
	{
		mq.push(target);
	}
	while (!mq.empty())
	{
		string label = mq.front();
		mq.pop();
		if (find(modulesSorted.begin(), modulesSorted.end(), label) != modulesSorted.end())
			continue;
		Module& m = *modules[label];
		modulesSorted.push_back(label);
		for (string& target : m.targets)
		{
			Module* toPush = modules[target].get();
			mq.push(target);
			cout << label << " -> " << target << "\n";
		}
	}

	unordered_map<string, int> conjCycleLowLen;
	unordered_map<string, int> conjCycleHighLen;

	uint64_t buttonPresses = 0;

	bool found = false;
	while (!found)
	{
		buttonPresses++;

		queue<Pulse> pulses;
		pulses.push({ true, "broadcaster", "button" });
		//pulses.push({ true, "vm", "test" });

		while (!pulses.empty())
		{
			Pulse curr = pulses.front();
			pulses.pop();
			//cout << curr.from << (curr.isLow ? " -low-> " : " -high-> ") << curr.target << "\n";
			if (curr.target == "rx" && curr.isLow)
			{
				found = true;
				break;
			}
			for (Pulse& p : modules[curr.target]->receivePulse(curr))
			{
				p.from = curr.target;
				pulses.push(p);

				string label = curr.target;
				Conjuction* conj = dynamic_cast<Conjuction*>(modules[label].get());
				if (conj != nullptr)
				{
					//cout << label << ": "; conj->debugStates();
					//if (conj->allAreLow && !conjCycleLowLen.contains(label))
					//{
					//	conjCycleLowLen[label] = buttonPresses;
					//	cout << "Conjuction " << label << " is all low after " << buttonPresses << " presses\n";
					//}
					if (conj->allAreHigh && !conjCycleHighLen.contains(label))
					{
						conjCycleHighLen[label] = buttonPresses;
						cout << "Conjuction " << label << " is all high after " << buttonPresses << " presses\n";
					}
				}
			}
		}
		//for (auto& item : modules)
		//{
		//	string label = item.first;
		//	Conjuction* conj = dynamic_cast<Conjuction*>(modules[label].get());
		//	if (conj != nullptr)
		//	{
		//		//cout << label << ": "; conj->debugStates();
		//		//if (conj->allAreLow && !conjCycleLowLen.contains(label))
		//		//{
		//		//	conjCycleLowLen[label] = buttonPresses;
		//		//	cout << "Conjuction " << label << " is all low after " << buttonPresses << " presses\n";
		//		//}
		//		if (conj->allAreHigh && !conjCycleHighLen.contains(label))
		//		{
		//			conjCycleHighLen[label] = buttonPresses;
		//			cout << "Conjuction " << label << " is all high after " << buttonPresses << " presses\n";
		//		}
		//	}
		//}
		//cout << "\n";
	}

	cout << "Answer p2: " << buttonPresses << "\n\n";
}

void part2Attempt2(unordered_map<string, unique_ptr<Module>>& modules)
{
	initConjuctions(modules);

	unordered_map<string, int> cycles;

	for (int i = 1; ; i++)
	{
		queue<Pulse> pulses;
		pulses.push({ true, "broadcaster", "button" });

		while (!pulses.empty())
		{
			Pulse curr = pulses.front();
			pulses.pop();

			//cout << curr.from << (curr.isLow ? " -low-> " : " -high-> ") << curr.target << "\n";
			Module& targetModule = *modules[curr.target];
			for (Pulse& p : targetModule.receivePulse(curr))
			{
				p.from = curr.target;
				pulses.push(p);

				if (isConjuction(&targetModule))
				{
					if (p.isLow && !cycles.contains(curr.target))
					{
						cycles[curr.target] = i;
						cout << curr.target << " : " << i << "\n";
					}
				}
			}
		}
		//cout << "\n";
	}

	//cout << "Answer p: " << mul << "\n\n";
}

int main()
{
	ifstream fin("input.txt");

	string currentIn;
	unordered_map<string, unique_ptr<Module>> modules;

	while (std::getline(fin, currentIn))
	{
		stringstream ss(currentIn);
		string moduleName, arrow, currTarget;
		vector<string> targets;
		ss >> moduleName >> arrow;
		while (ss >> currTarget)
		{
			if (currTarget.back() == ',')
				currTarget.pop_back();
			targets.push_back(currTarget);
		}
		if (moduleName == "broadcaster")
		{
			modules[moduleName] = make_unique<Broadcast>(targets);
		}
		else
		{
			char type = moduleName[0];
			moduleName.erase(0, 1);
			if (type == '%')
				modules[moduleName] = make_unique<FlipFlop>(targets);
			else
				modules[moduleName] = make_unique<Conjuction>(targets);
		}
	}
	fin.close();
	for (auto& item : modules)
	{
		for (string& target : item.second->targets)
		{
			if (!modules.contains(target))
			{
				modules[target] = make_unique<NoOp>();
			}
		}
	}

	//solvePart1(modules);
	//solvePart2(modules);
	part2Attempt2(modules);
}
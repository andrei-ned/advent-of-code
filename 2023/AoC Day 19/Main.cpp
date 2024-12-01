#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <assert.h>
#include <unordered_map>
#include <algorithm>
#include <stack>
#include <sstream>

using namespace std;

struct Rule
{
	int value = 0;
	char op = 0;
	char category = 0;
	string target = "";
};

struct Workflow
{
	vector<Rule> rules;
	string destination;
};

struct Part
{
	int x, m, a, s;
	int getSum()
	{
		return x + m + a + s;
	}
};

struct PartCondition
{
	int xMin = 1, xMax = 4000;
	int mMin = 1, mMax = 4000;
	int aMin = 1, aMax = 4000;
	int sMin = 1, sMax = 4000;

	bool isValid()
	{
		return xMin < xMax
			&& mMin < mMax
			&& aMin < aMax
			&& sMin < sMax;
	}

	uint64_t countSolutions()
	{
		if (!isValid())
			return 0;
		uint64_t x = xMax - xMin + 1;
		uint64_t m = mMax - mMin + 1;
		uint64_t a = aMax - aMin + 1;
		uint64_t s = sMax - sMin + 1;
		return x * m * a * s;
	}
};

PartCondition intersection(const PartCondition& c1, const PartCondition& c2)
{
	PartCondition intersection;
	intersection.xMax = min(c1.xMax, c2.xMax);
	intersection.xMin = max(c1.xMin, c2.xMin);
	intersection.xMax = min(c1.mMax, c2.mMax);
	intersection.xMin = max(c1.mMin, c2.mMin);
	intersection.xMax = min(c1.aMax, c2.aMax);
	intersection.xMin = max(c1.aMin, c2.aMin);
	intersection.xMax = min(c1.sMax, c2.sMax);
	intersection.xMin = max(c1.sMin, c2.sMin);
	return intersection;
}

bool checkOp(char op, int ruleValue, int partValue)
{
	if (op == '<')
	{
		return partValue < ruleValue;
	}
	else
	{
		return partValue > ruleValue;
	}
}

bool passesRule(const Rule& rule, const Part& part)
{
	switch (rule.category)
	{
	case 'x':
		return checkOp(rule.op, rule.value, part.x);
		break;
	case 'm':
		return checkOp(rule.op, rule.value, part.m);
		break;
	case 'a':
		return checkOp(rule.op, rule.value, part.a);
		break;
	case 's':
		return checkOp(rule.op, rule.value, part.s);
		break;
	}
}

void DoOpPassCondition(char op, int ruleVal, int& valMin, int& valMax)
{
	if (op == '<')
	{
		valMax = min(valMax, ruleVal - 1);
	}
	else // >
	{
		valMin = max(valMin, ruleVal + 1);
	}
}

void DoRulePassCondition(const Rule& rule, PartCondition& cond)
{
	switch (rule.category)
	{
	case 'x':
		DoOpPassCondition(rule.op, rule.value, cond.xMin, cond.xMax);
		break;
	case 'm':
		DoOpPassCondition(rule.op, rule.value, cond.mMin, cond.mMax);
		break;
	case 'a':
		DoOpPassCondition(rule.op, rule.value, cond.aMin, cond.aMax);
		break;
	case 's':
		DoOpPassCondition(rule.op, rule.value, cond.sMin, cond.sMax);
		break;
	}
}

void DoOpFailCondition(char op, int ruleVal, int& valMin, int& valMax)
{
	if (op == '<')
	{
		valMin = max(valMin, ruleVal);
	}
	else // >
	{
		valMax = min(valMax, ruleVal);
	}
}

void DoRuleFailCondition(const Rule& rule, PartCondition& cond)
{
	switch (rule.category)
	{
	case 'x':
		DoOpFailCondition(rule.op, rule.value, cond.xMin, cond.xMax);
		break;
	case 'm':
		DoOpFailCondition(rule.op, rule.value, cond.mMin, cond.mMax);
		break;
	case 'a':
		DoOpFailCondition(rule.op, rule.value, cond.aMin, cond.aMax);
		break;
	case 's':
		DoOpFailCondition(rule.op, rule.value, cond.sMin, cond.sMax);
		break;
	}
}

void debugWorkflows(unordered_map<string, Workflow>& workflows)
{
	for (auto& item : workflows)
	{
		cout << item.first << ":\n";
		Workflow& wf = item.second;
		for (int i = 0; i < wf.rules.size(); i++)
		{
			Rule& r = wf.rules[i];
			cout << r.category << r.op << r.value << ": " << r.target << "\n";
		}
		cout << wf.destination << "\n\n";
	}
}

void debugPartCond(const PartCondition& cond)
{
	cout << "x [" << cond.xMin << "," << cond.xMax << "] ";
	cout << "m [" << cond.mMin << "," << cond.mMax << "] ";
	cout << "a [" << cond.aMin << "," << cond.aMax << "] ";
	cout << "s [" << cond.sMin << "," << cond.sMax << "] ";
	cout << "\n";
}

void solvePart1(unordered_map<string, Workflow>& workflows, vector<Part>& parts)
{

	int sumAccepted = 0;
	for (Part& part : parts)
	{
		//cout << part.x << " ";
		string currWfName = "in";
		while (currWfName != "A" && currWfName != "R")
		{
			//cout << currWfName << " -> ";
			Workflow wf = workflows[currWfName];
			bool passedRule = false;
			for (int i = 0; i < wf.rules.size(); i++)
			{
				Rule& rule = wf.rules[i];
				if (passesRule(rule, part))
				{
					currWfName = rule.target;
					passedRule = true;
					break;
				}
			}
			if (!passedRule)
				currWfName = wf.destination;
		}
		//cout << currWfName << " \n";
		if (currWfName == "A")
		{
			sumAccepted += part.getSum();
		}
	}
	cout << "Sum accepted: " << sumAccepted << "\n";
}

void recurseConds(unordered_map<string, Workflow>& workflows, string currentWf, PartCondition cond, vector<PartCondition>& conds)
{
	if (!cond.isValid())
		return;
	if (currentWf == "A")
	{
		conds.push_back(cond);
		//debugPartCond(cond);
		return;
	}
	if (currentWf == "R")
	{
		return;
	}
	Workflow& wf = workflows[currentWf];
	for (int i = 0; i < wf.rules.size(); i++)
	{
		Rule& rule = wf.rules[i];
		PartCondition condIfPassingThisRule(cond);
		DoRulePassCondition(rule, condIfPassingThisRule);
		recurseConds(workflows, rule.target, condIfPassingThisRule, conds);
		DoRuleFailCondition(rule, cond);
	}
	recurseConds(workflows, wf.destination, cond, conds);
}

void solvePart2(unordered_map<string, Workflow>& workflows)
{
	vector<PartCondition> conds;
	recurseConds(workflows, "in", PartCondition(), conds);

	int64_t sum = 0;
	for (auto& c : conds)
	{
		sum += c.countSolutions();
	}

	cout << "Solution: " << sum << "\n";
}

int main()
{
	ifstream fin("input.txt");

	string currentIn;
	char direction;
	int len;

	unordered_map<string, Workflow> workflows;
	vector<Part> parts;

	while (std::getline(fin, currentIn))
	{
		if (currentIn.empty())
		{
			break;
		}
		Workflow wf;
		stringstream ss(currentIn);
		string workflowName = "";
		char c;
		while (ss.peek() != '{')
		{
			ss >> c;
			workflowName += c;
		}
		ss >> c; // '{'
		while (ss >> c)
		{
			if (ss.peek() != '>' && ss.peek() != '<')
			{
				wf.destination = c;
				string rest;
				ss >> rest;
				wf.destination += rest;
				wf.destination.pop_back();
				workflows.insert(pair(workflowName, wf));
				break;
			}
			Rule r;
			r.category = c;
			ss >> r.op >> r.value;
			ss >> c; // ':'
			while (ss.peek() >= 'a' && ss.peek() <= 'z' || ss.peek() == 'A' || ss.peek() == 'R')
			{
				ss >> c;
				r.target += c;
			}
			ss >> c; // ','
			wf.rules.push_back(r);
		}
	}
	//debugWorkflows(workflows);
	cout << "\n";
	while (std::getline(fin, currentIn))
	{
		char c;
		stringstream ss(currentIn);
		Part p;
		ss >> c >> c >> c; // "{x="
		ss >> p.x;
		ss >> c >> c >> c; // ",m="
		ss >> p.m;
		ss >> c >> c >> c; // ",a="
		ss >> p.a;
		ss >> c >> c >> c; // ",s="
		ss >> p.s;
		parts.push_back(p);
	}
	fin.close();

	solvePart1(workflows, parts);
	solvePart2(workflows);
}
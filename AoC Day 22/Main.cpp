#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <assert.h>
#include <unordered_map>
#include <algorithm>
#include <set>

using namespace std;

struct Brick;
bool bricksSortPred(const Brick& a, const Brick& b);
bool bricksSortPredPtr(const Brick* a, const Brick* b);

struct Pos
{
	int x, y, z;
};

struct Brick
{
	Pos p1, p2;
	vector<Brick*> bricksImSupporting;
	vector<Brick*> bricksImSupportedBy;
	bool isSafeToDisintegrate = true;

	int getMaxZ() const
	{
		return p2.z;
	}

	void fallAboveZ(int z)
	{
		int height = p2.z - p1.z;
		p1.z = z + 1;
		p2.z = p1.z + height;
	}

	int lenY() const
	{
		return p2.y - p1.y + 1;
	}

	int lenX() const
	{
		return p2.x - p1.x + 1;
	}

	void debugBrick() const
	{
		cout << "{{" << p1.x << "," << p1.y << "," << p1.z << "},{" << p2.x << "," << p2.y << "," << p2.z << "}}";
	}

	void disintegrateRecursively(set<const Brick*>& disintegratedSet) const
	{
		disintegratedSet.insert(this);
		vector<Brick*> toDisintegrate;
		//sort(toDisintegrate.begin(), toDisintegrate.end(), bricksSortPredPtr);
		for (int i = 0; i < bricksImSupporting.size(); i++)
		{
			Brick* brickCurr = bricksImSupporting[i];
			bool willFall = true;
			for (int j = 0; j < brickCurr->bricksImSupportedBy.size(); j++)
			{
				Brick* support = brickCurr->bricksImSupportedBy[j];
				if (!disintegratedSet.contains(support))
				{
					willFall = false; // there is a brick supporting this that didn't fall yet
				}
			}
			if (willFall)
			{
				toDisintegrate.push_back(brickCurr);
				disintegratedSet.insert(brickCurr);
			}
		}
		for (Brick* br : toDisintegrate)
		{
			br->disintegrateRecursively(disintegratedSet);
		}
	}
};

bool bricksSortPred(const Brick& a, const Brick& b)
{
	if (a.p1.z == b.p1.z)
	{
		if (a.p1.y == b.p1.y)
			return a.p1.x < b.p1.x;
		return a.p1.y < b.p1.y;
	}
	return a.p1.z < b.p1.z;
}
bool bricksSortPredPtr(const Brick* a, const Brick* b)
{
	const Brick& aRef = *a;
	const Brick& bRef = *b;
	return bricksSortPred(aRef, bRef);
}

bool doBricksIntersectXY(const Brick& a, const Brick& b, bool debug = false)
{
	bool isBp1x = b.p1.x >= a.p1.x && b.p1.x <= a.p2.x;
	bool isBp2x = b.p2.x >= a.p1.x && b.p2.x <= a.p2.x;
	bool isAp1x = a.p1.x >= b.p1.x && a.p1.x <= b.p2.x;
	bool isAp2x = a.p2.x >= b.p1.x && a.p2.x <= b.p2.x;
	bool isX = isBp1x || isBp2x || isAp1x || isAp2x;
	bool isBp1y = b.p1.y >= a.p1.y && b.p1.y <= a.p2.y;
	bool isBp2y = b.p2.y >= a.p1.y && b.p2.y <= a.p2.y;
	bool isAp1y = a.p1.y >= b.p1.y && a.p1.y <= b.p2.y;
	bool isAp2y = a.p2.y >= b.p1.y && a.p2.y <= b.p2.y;
	bool isY = isBp1y || isBp2y || isAp1y || isAp2y;
	bool intersects = isX && isY;
	if (debug)
	{
		a.debugBrick();
		cout << " and ";
		b.debugBrick();
		cout << (intersects ? " intersect" : " do not intersect") << "\n";
	}
	return intersects;
	return (abs((a.p1.x + a.lenX() / 2) - (b.p1.x + b.lenX() / 2)) * 2 < (a.lenX() + b.lenX())) &&
		   (abs((a.p1.y + a.lenY() / 2) - (b.p1.y + b.lenY() / 2)) * 2 < (a.lenY() + b.lenY()));
}

void solvePart1(vector<Brick>& bricks)
{
	// fall bricks
	sort(bricks.begin(), bricks.end(), bricksSortPred);
	for (int i = 0; i < bricks.size(); i++)
	{
		Brick& brickCurr = bricks[i];
		int landZ = 0;
		for (int j = 0; j < i; j++)
		{
			Brick& brickBelow = bricks[j];
			if (doBricksIntersectXY(brickCurr, brickBelow))
			{
				landZ = max(landZ, brickBelow.p2.z);
			}
		}
		cout << "Brick " << i << " fell at " << (landZ + 1) << " on top of ";
		for (int j = 0; j < i; j++)
		{
			Brick& brickBelow = bricks[j];

			if (doBricksIntersectXY(brickCurr, brickBelow))
			{
				if (landZ == brickBelow.p2.z)
				{
					cout << j << ", ";
					brickBelow.bricksImSupporting.push_back(&brickCurr);
					brickCurr.bricksImSupportedBy.push_back(&brickBelow);
				}
			}
		}
		cout << "\n";
		//cout << "Brick " << i << " fell at " << (landZ + 1) << " on top of " << brickCurr.bricksImSupportedBy.size() << " other bricks\n";
		brickCurr.fallAboveZ(landZ);
	}
	for (int i = 0; i < bricks.size(); i++)
	{
		Brick& brickCurr = bricks[i];
		int supportingCount = brickCurr.bricksImSupportedBy.size();
		if (supportingCount == 1)
		{
			brickCurr.bricksImSupportedBy[0]->isSafeToDisintegrate = false;
		}
	}
	uint64_t safeCount = 0;
	for (int i = 0; i < bricks.size(); i++)
	{
		Brick& brickCurr = bricks[i];
		if (brickCurr.isSafeToDisintegrate)
		{
			//cout << "Brick " << i << " is safe\n";
			safeCount++;
		}
		else
		{
			//cout << "Brick " << i << " is unsafe\n";
		}
	}
	cout << safeCount << " bricks can be safely disintegrated\n";
}

// part 1 should be run before to fall the bricks
void solvePart2(vector<Brick>& bricks)
{
	uint64_t totalFalls = 0;
	for (int i = 0; i < bricks.size(); i++)
	{
		//cout << "Brick " << i << " disintegrates ";
		Brick& brickCurr = bricks[i];
		set<const Brick*> disintegratedBricks;
		brickCurr.disintegrateRecursively(disintegratedBricks);
		int fallCount = disintegratedBricks.size() - 1; // -1 so we don't count self
		totalFalls += fallCount;
		//cout << disintegrateCount << " bricks: ";
		//for (const Brick* br : disintegratedBricks)
		//{
		//	br->debugBrick();
		//}
		//cout << "\n";
	}
	cout << "Total falls is " << totalFalls << "\n";
	// 1310 is too low
}

void debugIntersections()
{
	assert(doBricksIntersectXY({ {0,0,1}, {0,0,1} }, { {0,0,3}, {0,0,6} }) == true);
	assert(doBricksIntersectXY({ {1,0,1}, {1,0,1} }, { {0,0,3}, {0,0,6} }) == false);
	assert(doBricksIntersectXY({ {0,1,1}, {0,1,1} }, { {0,0,3}, {0,0,6} }) == false);
	assert(doBricksIntersectXY({ {0,0,1}, {9,0,1} }, { {0,1,3}, {9,2,6} }) == false);
	assert(doBricksIntersectXY({ {0,0,1}, {9,0,1} }, { {1,0,3}, {1,1,6} }) == true);


	assert(doBricksIntersectXY({ {0,0,1}, {0,0,1} }, { {0,0,0}, {0,0,2} }) == true);
	assert(doBricksIntersectXY({ {0,0,1}, {0,0,1} }, { {1,1,0}, {2,2,2} }) == false);
	assert(doBricksIntersectXY({ {0,0,1}, {0,0,1} }, { {0,0,0}, {2,2,2} }) == true);
	assert(doBricksIntersectXY({ {1,1,1}, {2,2,1} }, { {0,0,0}, {4,4,2} }) == true);
	assert(doBricksIntersectXY({ {1,1,1}, {2,2,1} }, { {0,0,6}, {1,1,6} }) == true);
}

int main()
{
	debugIntersections();

	ifstream fin("input.txt");

	int x1, y1, z1, x2, y2, z2;
	char s; // separator

	vector<Brick> bricks;

	while (fin >> x1 >> s >> y1 >> s >> z1 >> s >> x2 >> s >> y2 >> s >> z2)
	{
		assert(z1 <= z2);
		assert(y1 <= y2);
		assert(x1 <= x2);
		bricks.push_back({ {x1,y1,z1}, {x2,y2,z2} });
	}
	fin.close();

	solvePart1(bricks);
	solvePart2(bricks);
}
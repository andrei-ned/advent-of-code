#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <assert.h>
#include <unordered_map>
#include <algorithm>
#include <stack>

using namespace std;

template<typename T>
T sign(T x)
{
	if (x > 0) return 1;
	if (x < 0) return -1;
	return 0;
}

template<typename T>
struct Vec3 {
	T x, y, z;
	void debug()
	{
		cout << x << ", " << y << ", " << z;
	}
};

struct Hailstone {
	Vec3<int64_t> pos;
	Vec3<int> velocity;

	void debug()
	{
		pos.debug();
		cout << " @ ";
		velocity.debug();
		cout << "\n";
	}
};

bool intersectsXY(Hailstone a, Hailstone b, double minPos, double maxPos, bool debug = false)
{
	// Line equation:
	// ax + by - c = 0
	// y = mx + b
	// b = -mx1 + y1
	double slopeA = (double)a.velocity.y / a.velocity.x;
	double slopeB = (double)b.velocity.y / b.velocity.x;
	double cA = -slopeA * a.pos.x + a.pos.y;
	double cB = -slopeB * b.pos.x + b.pos.y;

	double interX = (cA - cB) / (slopeB - slopeA);
	double interY = (cA * slopeB - cB * slopeA) / (slopeB - slopeA);

	if (debug)
	{
		cout << "Hailstone A: ";
		a.debug();
		cout << "Hailstone B: ";
		b.debug();
		cout << "Intersection is at " << interX << ", " << interY;
	}

	bool intersectionPasses = true;

	if (interX < minPos || interX > maxPos || interY < minPos || interY > maxPos)
	{
		intersectionPasses = false;
		if (debug) cout << " (outside the test area)";
	}
	if (sign(interX - a.pos.x) != sign(a.velocity.x) || sign(interY - a.pos.y) != sign(a.velocity.y))
	{
		intersectionPasses = false;
		if (debug) cout << " (in the past for A)";
	}
	if (sign(interX - b.pos.x) != sign(b.velocity.x) || sign(interY - b.pos.y) != sign(b.velocity.y))
	{
		intersectionPasses = false;
		if (debug) cout << " (in the past for B)";
	}

	if (debug) cout << "\n\n";

	return intersectionPasses;
}

int main()
{
	ifstream fin("input.txt");
	
	Hailstone h;
	vector<Hailstone> hail;
	char c;

	while (fin >> h.pos.x >> c >> h.pos.y >> c >> h.pos.z >> c >> h.velocity.x >> c >> h.velocity.y >> c >> h.velocity.z)
	{
		hail.push_back(h);
	}
	fin.close();

	// part 1
	int64_t intersectionCount = 0;
	for (int i = 0; i < hail.size(); i++)
	{
		for (int j = i + 1; j < hail.size(); j++)
		{
			intersectionCount += intersectsXY(hail[i], hail[j], 200000000000000, 400000000000000) ? 1 : 0;
		}
	}
	cout << "\nTotal intersections: " << intersectionCount << "\n";

	// part 2
	string t = "pqv";
	// rock pos: a,b,c
	// t1 = p, t2 = q, t3 = v
	// rock vel: x,y,z

	// x = t1*vel.x, ...
	for (int i = 0; i < 3; i++)
	{
		Hailstone& p = hail[i];
		char ti = t[i];

		cout << p.pos.x << "+" << p.velocity.x << "*" << ti << "=a+x*" << ti << "\n";
		cout << p.pos.y << "+" << p.velocity.y << "*" << ti << "=b+y*" << ti << "\n";
		cout << p.pos.z << "+" << p.velocity.z << "*" << ti << "=c+z*" << ti << "\n";
	}
	// plug the output into equation solver for answer :)
	//https://quickmath.com/webMathematica3/quickmath/equations/solve/advanced.jsp
}
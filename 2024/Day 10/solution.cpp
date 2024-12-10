#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <set>

using namespace std;

vector<string> map;

void traverseTrail(int x, int y, char h, set<pair<int,int>>& peaks)
{
    if (x < 0 || x >= map[0].size())
        return;
    if (y < 0 || y >= map.size())
        return;
    if (map[y][x] != h)
        return;
    if (h == '9')
        peaks.insert(make_pair(x,y));
    
    traverseTrail(x+1,y, h+1, peaks);
    traverseTrail(x-1,y, h+1, peaks);
    traverseTrail(x,y+1, h+1, peaks);
    traverseTrail(x,y-1, h+1, peaks);
}

int traverseTrail(int x, int y)
{
    set<pair<int,int>> peaks;
    traverseTrail(x,y,'0',peaks);
    return peaks.size();
}

int countTrails(int x, int y, char h = '0')
{
    if (x < 0 || x >= map[0].size())
        return 0;
    if (y < 0 || y >= map.size())
        return 0;
    if (map[y][x] != h)
        return 0;
    if (h == '9')
        return 1;
    
    int sum = 0;
    sum += countTrails(x+1,y, h+1);
    sum += countTrails(x-1,y, h+1);
    sum += countTrails(x,y+1, h+1);
    sum += countTrails(x,y-1, h+1);
    return sum;
}

int main()
{
    ifstream fin("input.txt");
    string line;
    while (fin >> line)
        map.push_back(line);

    int sum1 = 0, sum2 = 0;
    for (int y = 0; y < map.size(); y++)
    {
        for (int x = 0; x < map[y].size(); x++)
        {
            if (map[y][x] == '0')
            {
                sum1 += traverseTrail(x, y);
                sum2 += countTrails(x, y);
            }
        }
    }
    cout << "Part one: " << sum1 << "\n";
    cout << "Part two: " << sum2 << "\n";
}
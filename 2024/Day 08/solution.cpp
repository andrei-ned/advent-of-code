#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
//#include <unordered_set>
#include <set>

using namespace std;

int main()
{
    ifstream fin("input.txt");
    vector<string> map;
    string line;
    while (fin >> line) {
        map.push_back(line);
    }

    unordered_map<char, vector<pair<int,int>>> antennas;
    for (int y = 0; y < map.size(); y++)
    {
        for (int x = 0; x < map[0].size(); x++)
        {
            if (map[y][x] != '.')
            {
                antennas[map[y][x]].push_back(make_pair(x,y));
            }
        }
    }

    set<pair<int,int>> antinodes;
    for (const auto&[key,positions] : antennas)
    {
        if (positions.size() < 2)
            continue;
        for (int i = 0; i < positions.size(); i++)
        {
            for (int j = 0; j < positions.size(); j++)
            {
                if (i == j) continue;
                int xDiff = positions[j].first - positions[i].first;
                int yDiff = positions[j].second - positions[i].second;
                int xAnti = positions[j].first + xDiff;
                int yAnti = positions[j].second + yDiff;
                if (xAnti >= 0 && xAnti < map[0].size() && yAnti >= 0 && yAnti < map.size())
                {
                    map[yAnti][xAnti] = '#';
                    antinodes.insert(make_pair(xAnti,yAnti));
                }
            }
        }
    }

    for (int i = 0; i < map.size(); i++)
        cout << map[i] << "\n";

    cout << "There are " << antinodes.size() << " unique antinodes\n";
}
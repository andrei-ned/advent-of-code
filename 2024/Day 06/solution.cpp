#include <fstream>
#include <iostream>
#include <vector>
#include <string>

using namespace std;

int dirsX[] = { 0, 1, 0, -1};
int dirsY[] = { -1, 0, 1, 0};
char dirChars[] = {'^', '>', 'v', '<'};

void traverseMap(vector<string> map, int currX, int currY, int& exploredCount, bool& isLoop)
{
    int stepsWithoutExploring = 0;
    isLoop = false;
    int dirIdx = 0;
    while (currX >= 0 && currX < map[0].size() && currY >= 0 && currY < map.size())
    {
        if (map[currY][currX] == '.')
        {
            exploredCount++;
            stepsWithoutExploring = 0;
        }
        else
        {
            stepsWithoutExploring++;
        }
        if (map[currY][currX] == dirChars[dirIdx] || stepsWithoutExploring > 1000)
        {
            isLoop = true;
            return;
        }
        map[currY][currX] = dirChars[dirIdx];
        int nextX = currX + dirsX[dirIdx];
        int nextY = currY + dirsY[dirIdx];

        if (!(nextX >= 0 && nextX < map[0].size() && nextY >= 0 && nextY < map.size()))
            return;

        if (map[nextY][nextX] == '#')
            dirIdx = (dirIdx + 1) % 4;
        else
        {
            currX = nextX;
            currY = nextY;
        }
    }
}

int main()
{
    ifstream fin("input.txt");
    vector<string> map;
    string line;
    while (fin >> line) {
        map.push_back(line);
    }

    // Find start position
    int startX, startY;
    for (int y = 0; y < map.size(); y++)
    {
        for (int x = 0; x < map[y].length(); x++)
        {
            if (map[y][x] == '^')
            {
                map[y][x] = '.';
                startX = x;
                startY = y;
            }
        }
    }

    int dirIdx = 0;
    int exploredCount = 0;
    bool isLoop;

    // Part 1
    traverseMap(map, startX, startY, exploredCount, isLoop);
    cout << "Explored positions: " << exploredCount << "\n";

    // Part 2
    int loopCount = 0;
    for (int y = 0; y < map.size(); y++)
    {
        for (int x = 0; x < map[y].length(); x++)
        {
            //cout << x << "," << y;
            if (y == startY && x == startX)
                continue;
            if (map[y][x] == '.')
            {
                map[y][x] = '#';
                traverseMap(map, startX, startY, exploredCount, isLoop);
                if (isLoop)
                {
                    //cout << "LOOP";
                    loopCount++;
                }
                map[y][x] = '.';
            }
            //cout << "\n";
        }
    }
    cout << "Ways of making a loop: " << loopCount << "\n";
}
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <cstdint>
#include <cassert>
#include <set>

using namespace std;

void printDisk(const vector<int>& disk)
{
    for (int i = 0; i < disk.size(); i++)
    {
        if (disk[i] == -1)
            cout << '.';
        else
            cout<<disk[i]%9;
    }
    cout << "\n";
}

void printCheckSum(string message, const vector<int>& disk)
{
    //printDisk(disk);
    uint64_t checksum = 0;
    for (int i = 0; i < disk.size(); i++)
    {
        if (disk[i] != -1)
        {
            checksum += disk[i] * i;
        }
    }
    cout << message << checksum << "\n";
}

void part1(vector<int> disk)
{
    int front = 0;
    int back = disk.size()-1;
    while (true)
    {
        while (disk[front] != -1)
            front++;
        while (disk[back] == -1)
            back--;
        if (front >= back)
            break;
        swap(disk[front], disk[back]);
    }
    printCheckSum("Checksum for part one is: ", disk);
}

void part2(vector<int> disk)
{
    set<int> movedIds;
    int back = disk.size()-1;
    while (back > 0)
    {
        int len = 0;
        int freeSpace = 0;

        while (disk[back] == -1)
            back--;
        while (disk[back-len] == disk[back])
            len++;
        if (movedIds.find(disk[back]) == movedIds.end())
        {
            for (int i = 0; i <= back - len; i++)
            {
                if (disk[i] == -1)
                    freeSpace++;
                else
                    freeSpace = 0;
                if (freeSpace == len)
                {
                    //cout << "Swapping file id " << disk[back] << " from " << (back-len+1) << " to " << (i-len+1) << " diff " << (back-i) << "\n";
                    movedIds.insert(disk[back]);
                    for (int j = 0; j < len; j++)
                    {
                        assert(disk[i - j] == -1);
                        assert(disk[back - j] != -1);
                        swap(disk[i - j], disk[back - j]);
                    }
                    break;
                }
            }
        }

        back -= len;
    }
    printCheckSum("Checksum for part two is: ", disk);
}

int main()
{
    ifstream fin("input.txt");
    string diskMap;
    fin >> diskMap;
    vector<int> disk;
    for (int i = 0; i < diskMap.size(); i++)
    {
        int fileId = (i % 2 == 0) ? (i / 2) : -1;
        for (int j = 0; j < diskMap[i] - '0'; j++)
        {
            disk.push_back(fileId);
        }
    }
    part1(disk);
    part2(disk);
}
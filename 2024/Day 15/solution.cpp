#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <windows.h>
#include <cstdint>

using namespace std;

void printWarehouse(const vector<string>& warehouse)
{
    COORD coord;
    coord.X = 0;
    coord.Y = 0;
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(hConsole, coord);
    
    for (int i = 0; i < warehouse.size(); i++)
    {
        cout << warehouse[i] << "\n";
    }
}

void findRobot(const vector<string>& warehouse, int& robot_x, int& robot_y)
{
    for (int y = 0; y < warehouse.size(); y++)
    {
        for (int x = 0; x < warehouse[y].size(); x++)
        {
            if (warehouse[y][x] == '@')
            {
                robot_x = x;
                robot_y = y;
                return;
            }
        }
    }
}

void step(vector<string>& warehouse, int& robot_x, int& robot_y, char direction)
{
    int x_dir = 0, y_dir = 0;
    int x = robot_x, y = robot_y;
    switch (direction)
    {
        case '>':
        x_dir = 1;
        break;
        case '<':
        x_dir = -1;
        break;
        case 'v':
        y_dir = 1;
        break;
        case '^':
        y_dir = -1;
        break;
    }

    int dist = 0;
    do
    {
        dist++;
        x += x_dir;
        y += y_dir;
    }
    while (warehouse[y][x] == 'O');
    if (warehouse[y][x] == '.')
    {
        robot_x += x_dir;
        robot_y += y_dir;
        for (int i = 0; i < dist; i++)
        {
            swap(warehouse[y][x],warehouse[y-y_dir][x-x_dir]);
            x -= x_dir;
            y -= y_dir;
        }
    }
}

uint64_t sumGpsCoords(const vector<string>& warehouse)
{
    uint64_t sum =0;
    for (int y = 0; y < warehouse.size(); y++)
    {
        for (int x = 0; x < warehouse[y].size(); x++)
        {
            if (warehouse[y][x] == 'O')
            {
                sum += y * 100 + x;
            }
        }
    }
    return sum;
}

int main()
{
    ifstream fin("input.txt");
    string line;
    vector<string> warehouse;
    while (fin >> line)
    {
        if (warehouse.size() != 0 && line.size() > warehouse[0].size())
        {
            break;
        }
        warehouse.push_back(line);
    }
    
    int x,y;
    findRobot(warehouse,x,y);
    do
    {
        printWarehouse(warehouse);
        for (int i = 0; i < line.size(); i++)
        {
            //Sleep(100);
            step(warehouse, x, y, line[i]);
            printWarehouse(warehouse);
            cout << line[i] << "                                  \n";
        }
    } while (fin >> line);

    cout << "Sum coordinates: " << sumGpsCoords(warehouse) << "    \n";
}
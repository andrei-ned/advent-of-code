#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <windows.h>
#include <cstdint>
#include <stack>
#include <queue>
#include <set>

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

void getDirection(int& x_dir, int& y_dir, char direction)
{
    x_dir = 0;
    y_dir = 0;

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
}

// Step func for part 1
void step1(vector<string>& warehouse, int& robot_x, int& robot_y, char direction)
{
    int x_dir, y_dir;
    getDirection(x_dir, y_dir, direction);
    int x = robot_x, y = robot_y;

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

// Step func for part 2
void step2(vector<string>& warehouse, int& robot_x, int& robot_y, char direction)
{
    int x_dir, y_dir;
    getDirection(x_dir, y_dir, direction);
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

    stack<pair<int,int>> cellsToMove;
    queue<pair<int,int>> cellsToCheck;
    set<pair<int,int>> cellsAlreadyChecked;
    cellsToMove.push(make_pair(x,y));
    cellsToCheck.push(make_pair(x+x_dir, y+y_dir));
    bool canMove = true;
    while (!cellsToCheck.empty() && canMove)
    {
        auto cell = cellsToCheck.front();
        cellsToCheck.pop();
        if (cellsAlreadyChecked.contains(cell))
            continue;
        cellsAlreadyChecked.insert(cell);
        switch (warehouse[cell.second][cell.first])
        {
            case '#':
            canMove = false;
            break;
            case '[':
            {
                auto rightHalf = make_pair(cell.first + 1, cell.second);
                cellsToMove.push(cell);
                cellsToMove.push(rightHalf);
                cellsAlreadyChecked.insert(rightHalf);
                cellsToCheck.push(make_pair(cell.first + x_dir, cell.second + y_dir));
                cellsToCheck.push(make_pair(rightHalf.first + x_dir, rightHalf.second + y_dir));
            }
            break;
            case ']':
            {
                auto leftHalf = make_pair(cell.first - 1, cell.second);
                cellsToMove.push(cell);
                cellsToMove.push(leftHalf);
                cellsAlreadyChecked.insert(leftHalf);
                cellsToCheck.push(make_pair(cell.first + x_dir, cell.second + y_dir));
                cellsToCheck.push(make_pair(leftHalf.first + x_dir, leftHalf.second + y_dir));
            }
            break;
        }
    }
    if (canMove)
    {
        robot_x += x_dir;
        robot_y += y_dir;
        while (!cellsToMove.empty())
        {
            auto cell = cellsToMove.top();
            cellsToMove.pop();
            swap(warehouse[cell.second][cell.first],warehouse[cell.second+y_dir][cell.first+x_dir]);
        }
    }
}

uint64_t sumGpsCoords(const vector<string>& warehouse, char box = 'O')
{
    uint64_t sum = 0;
    for (int y = 0; y < warehouse.size(); y++)
    {
        for (int x = 0; x < warehouse[y].size(); x++)
        {
            if (warehouse[y][x] == box)
            {
                sum += y * 100 + x;
            }
        }
    }
    return sum;
}

void doPart1(vector<string> warehouse, ifstream& fin, string& line)
{
    int x,y;
    findRobot(warehouse,x,y);
    do
    {
        printWarehouse(warehouse);
        for (int i = 0; i < line.size(); i++)
        {
            //Sleep(100);
            //cin.get();
            step1(warehouse, x, y, line[i]);
            printWarehouse(warehouse);
            cout << line[i] << "                                  \n";
        }
    } while (fin >> line);

    cout << "Sum coordinates (small): " << sumGpsCoords(warehouse) << "    \n";
}

void doPart2(vector<string> warehouse, ifstream& fin, string& line)
{
    // Make map bigger
    vector<string> warehouse_big;
    for (int y = 0; y < warehouse.size(); y++)
    {
        string line = "";
        for (int x = 0; x < warehouse[y].size(); x++)
        {
            switch (warehouse[y][x])
            {
                case '#':
                line.push_back('#');
                line.push_back('#');
                break;
                case 'O':
                line.push_back('[');
                line.push_back(']');
                break;
                case '.':
                line.push_back('.');
                line.push_back('.');
                break;
                case '@':
                line.push_back('@');
                line.push_back('.');
                break;
            }
        }
        warehouse_big.push_back(line);
    }

    printWarehouse(warehouse_big);
    int x,y;
    findRobot(warehouse_big,x,y);
    do
    {
        printWarehouse(warehouse_big);
        for (int i = 0; i < line.size(); i++)
        {
            //Sleep(100);
            //cin.get();
            step2(warehouse_big, x, y, line[i]);
            printWarehouse(warehouse_big);
            cout << line[i] << "                                  \n";
        }
    } while (fin >> line);

    cout << "Sum coordinates (big): " << sumGpsCoords(warehouse_big, '[') << "    \n";
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
    
    //doPart1(warehouse, fin, line);
    doPart2(warehouse, fin, line);
}
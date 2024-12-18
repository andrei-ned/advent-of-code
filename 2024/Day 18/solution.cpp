#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <set>
#include <queue>

using namespace std;

// #define GRID_SIZE 7
// #define BYTES_TO_FALL 12

#define GRID_SIZE 71
#define BYTES_TO_FALL 1024

int directions_x[] = {1,0,-1,0};
int directions_y[] = {0,1,0,-1};

struct exploration
{
    int x, y, score;
};

struct compare {
    bool operator()(exploration a, exploration b)
    {
        return a.score > b.score;
    }
};

void print_grid(char grid[][GRID_SIZE])
{
    for (int  y = 0; y < GRID_SIZE; y++)
    {
        for (int x = 0; x < GRID_SIZE; x++)
        {
            cout << grid[y][x];
        }
        cout << "\n";
    }
}

int main()
{
    ifstream fin("input.txt");
    vector<pair<int,int>> bytes_coords;
    int x, y;
    char comma;
    while (fin >> x >> comma >> y)
        bytes_coords.push_back(make_pair(x,y));

    char grid[GRID_SIZE][GRID_SIZE];
    int scores[GRID_SIZE][GRID_SIZE];
    fill_n(&grid[0][0], GRID_SIZE * GRID_SIZE, '.');

    for (int i = 0; i < BYTES_TO_FALL; i++)
    {
        //cout << bytes_coords[i].first << "," << bytes_coords[i].second << "\n";
        grid[bytes_coords[i].second][bytes_coords[i].first] = '#';
    }
    //print_grid(grid);

    for (int i = BYTES_TO_FALL; i < bytes_coords.size(); i++)
    {
        fill_n(&scores[0][0], GRID_SIZE * GRID_SIZE, INT_MAX);
        priority_queue<exploration, vector<exploration>, compare> pq;
        pq.push({0, 0, 0});
        bool is_solvable = false;
        while (!pq.empty())
        {
            exploration e = pq.top();
            pq.pop();
            if (e.x == GRID_SIZE-1 && e.y == GRID_SIZE-1)
            {
                if (i == BYTES_TO_FALL)
                    cout << "Minimum number of steps to reach exit after " << i << " bytes: " << e.score << "\n";
                is_solvable = true;
                break;
            }

            for (int i = 0; i < 4; i++)
            {
                int x_dir = directions_x[i];
                int y_dir = directions_y[i];
                int new_x = e.x + x_dir;
                int new_y = e.y + y_dir;
                
                if (new_x < 0 || new_x >= GRID_SIZE || new_y < 0 || new_y >= GRID_SIZE)
                    continue;
                if (grid[new_y][new_x] != '#' && e.score + 1 < scores[new_y][new_x]) // Avoid walls and squares with worse scores
                {
                    pq.push({new_x, new_y, e.score + 1});
                    scores[new_y][new_x] = e.score + 1;
                }
            }
        }
        if (!is_solvable)
        {
            //print_grid(grid);
            cout << "Exit is no longer reachable after byte at coordinate: " << bytes_coords[i-1].first << "," << bytes_coords[i-1].second << "\n";
            break;
        }
        grid[bytes_coords[i].second][bytes_coords[i].first] = '#';
    }

}
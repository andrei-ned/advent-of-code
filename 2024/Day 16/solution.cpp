#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <set>
#include <queue>

using namespace std;

vector<string> maze;
int visited[200][200];

void mark_visited(int x, int y, int dir_idx)
{
    int mask = 1 << dir_idx;
    visited[y][x] |= mask;
}

bool is_visited(int x, int y, int dir_idx)
{
    int mask = 1 << dir_idx;
    int at_pos = visited[y][x];
    return (at_pos & mask) != 0;
}

struct exploration
{
    int x, y, dir_idx, score;
};

struct compare {
    bool operator()(exploration a, exploration b)
    {
        return a.score > b.score;
    }
};

int directions_x[] = {1,0,-1,0};
int directions_y[] = {0,1,0,-1};
char directions_char[] = {'>','v','<','^'};
int turn_count[] = {0,1,2,1};
int min_score = INT_MAX;

void print_maze()
{
    for (int  y = 0; y < maze.size(); y++)
    {
        cout << maze[y] <<"\n";
    }
}

int explore_with_priority(int start_x, int start_y)
{
    priority_queue<exploration, vector<exploration>, compare> pq;
    pq.push({start_x, start_y, 0, 0});
    while (!pq.empty())
    {
        exploration e = pq.top();
        pq.pop();
        if (maze[e.y][e.x] == 'E')
        {
            return e.score;
        }
        if (is_visited(e.x,e.y,e.dir_idx))
            continue;
        mark_visited(e.x,e.y,e.dir_idx);
        if (maze[e.y][e.x] == '.')
            maze[e.y][e.x] = directions_char[e.dir_idx];

        for (int i = 0; i < 4; i++)
        {
            int new_dir_idx = (e.dir_idx + i) % 4;
            int x_dir = directions_x[new_dir_idx];
            int y_dir = directions_y[new_dir_idx];
            int new_x = e.x + x_dir;
            int new_y = e.y + y_dir;

            // Check bounds
            if (new_y < 0 || new_y >= maze.size())
                continue;
            if (new_x < 0 || new_x >= maze[0].size())
                continue;
            // Avoid walls and visited spaces
            if (maze[new_y][new_x] != '#' && !is_visited(new_x,new_y,new_dir_idx))
                pq.push({new_x, new_y, new_dir_idx, e.score + 1 + 1000 * turn_count[i]});
        }
    }
    return -1;
}

int main()
{
    ifstream fin("input.txt");
    string line;
    while (fin >> line)
        maze.push_back(line);

    int score = -1;
    for (int  y = 0; y < maze.size(); y++)
    {
        for (int x = 0; x < maze[y].size(); x++)
        {
            if (maze[y][x] == 'S')
            {
                score = explore_with_priority(x,y);
            }
        }
    }
    print_maze();
    cout << "Min score: " << score << "\n";
}
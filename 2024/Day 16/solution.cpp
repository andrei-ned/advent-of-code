#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <set>
#include <queue>

using namespace std;

vector<string> maze;
int scores[200][200][4]; // [y][x][dir_idx]
int directions_x[] = {1,0,-1,0};
int directions_y[] = {0,1,0,-1};
int turn_count[] = {0,1,2,1};

struct exploration
{
    int x, y, dir_idx, score;
    vector<pair<int,int>> path;
};

struct compare {
    bool operator()(exploration a, exploration b)
    {
        return a.score > b.score;
    }
};

void print_maze()
{
    for (int  y = 0; y < maze.size(); y++)
    {
        cout << maze[y] <<"\n";
    }
}

int best_score = INT_MAX;
set<pair<int,int>> squares_in_best_paths;
void explore_with_priority(int start_x, int start_y)
{
    priority_queue<exploration, vector<exploration>, compare> pq;
    pq.push({start_x, start_y, 0, 0});
    while (!pq.empty())
    {
        exploration e = pq.top();
        pq.pop();
        if (maze[e.y][e.x] == 'E' && e.score <= best_score)
        {
            best_score = e.score;
            for (int i = 0; i < e.path.size(); i++)
                squares_in_best_paths.insert(e.path[i]);
            squares_in_best_paths.insert(make_pair(e.x,e.y));
        }

        for (int i = 0; i < 4; i++)
        {
            if (i == 2) continue;
            int new_dir_idx = (e.dir_idx + i) % 4;
            int x_dir = directions_x[new_dir_idx];
            int y_dir = directions_y[new_dir_idx];
            int new_x = e.x + x_dir;
            int new_y = e.y + y_dir;

            int new_score = e.score + 1 + 1000 * turn_count[i];
            
            if (maze[new_y][new_x] != '#' && new_score <= scores[new_y][new_x][new_dir_idx]) // Avoid walls and squares with worse scores
            {
                vector<pair<int,int>> path {e.path};
                path.push_back(make_pair(e.x,e.y));
                pq.push({new_x, new_y, new_dir_idx, new_score, std::move(path)});
                scores[new_y][new_x][new_dir_idx] = new_score;
            }
        }
    }
}

int main()
{
    fill_n(&scores[0][0][0], 200 * 200 * 4, INT_MAX);

    ifstream fin("input.txt");
    string line;
    while (fin >> line)
        maze.push_back(line);

    for (int  y = 0; y < maze.size(); y++)
        for (int x = 0; x < maze[y].size(); x++)
            if (maze[y][x] == 'S')
                explore_with_priority(x,y);

    //print_maze();
    cout << "Min score: " << best_score << "\n";
    cout << "Unique squares in best paths: " << squares_in_best_paths.size() << "\n";
}
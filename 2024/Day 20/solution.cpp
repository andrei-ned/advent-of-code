#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <set>
#include <queue>
#include <map>

using namespace std;

#define GRID_SIZE 142
#define CHEAT_DURATION 20

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
    vector<string> racetrack;
    string line;
    while (fin >> line)
        racetrack.push_back(line);

    // --- Do Dijkstra without cheats, and get a grid of scores
    int scores[GRID_SIZE][GRID_SIZE];
    fill_n(&scores[0][0], GRID_SIZE * GRID_SIZE, INT_MAX);
    
    priority_queue<exploration, vector<exploration>, compare> pq;
    for (int y = 0; y < racetrack.size(); y++)
        for (int x = 0; x < racetrack[y].size(); x++)
            if (racetrack[y][x] == 'S')
                pq.push({x, y, 0});

    
    int score_no_cheats = INT_MAX;

    while (!pq.empty())
    {
        exploration e = pq.top();
        pq.pop();
        scores[e.y][e.x] = e.score;
        if (racetrack[e.y][e.x] == 'E')
        {
            score_no_cheats = e.score;
        }

        // Explore lambda
        auto explore = [&] (exploration new_e) {
            if (new_e.x < 0 || new_e.x >= racetrack[0].size() || new_e.y < 0 || new_e.y >= racetrack.size())
                return;
            if (racetrack[new_e.y][new_e.x] != '#' && e.score + 1 < scores[new_e.y][new_e.x]) // Avoid walls and squares with worse scores
            {
                pq.push(new_e);
                scores[new_e.y][new_e.x] = new_e.score;
            }
        };
        for (int i = 0; i < 4; i++)
        {
            int x_dir = directions_x[i];
            int y_dir = directions_y[i];

            explore({e.x + x_dir, e.y + y_dir, e.score + 1});
        }
    }

    // --- Figure out how much time can be saved with cheats, using the score grid and manhattan distance
    int* cheats = new int[score_no_cheats]; // cheats[saved_time] = no of cheats
    fill_n(cheats, score_no_cheats, 0);
    for (int y = 0; y < racetrack.size(); y++)
        for (int x = 0; x < racetrack[y].size(); x++)
        {
            if (racetrack[y][x] == '#' || scores[y][x] >= score_no_cheats)
                continue;
            for (int y_new = y - CHEAT_DURATION; y_new <= y + CHEAT_DURATION; y_new++)
            {
                for (int x_new = x - CHEAT_DURATION; x_new <= x + CHEAT_DURATION; x_new++)
                {
                    if (x_new < 0 || x_new >= racetrack[0].size() || y_new < 0 || y_new >= racetrack.size()) // out of bounds
                        continue;
                    if (racetrack[y_new][x_new] == '#') // end place is wall
                        continue;
                    int manhattan_dist = abs(x_new-x)+abs(y_new-y);
                    if (manhattan_dist > CHEAT_DURATION) // too far to cheat
                        continue;
                    int saved_time = scores[y_new][x_new] - scores[y][x] - manhattan_dist;
                    if (saved_time > 0)
                    {
                        cheats[saved_time]++;
                        //cout << "Cheat from " << x << "," << y << " to " << x_new << "," << y_new << " saving " << saved_time << " picoseconds.\n";
                    }
                }
            }
        }

    cout << "Time to race without cheating: " << score_no_cheats << "\n";
    int cheat_count_that_saves_100 = 0;
    for (int i = 100; i < score_no_cheats; i++)
    {
        cheat_count_that_saves_100 += cheats[i];
        if (cheats[i] > 0)
            cout << cheats[i] << " cheats that save " << i << " picoseconds.\n";
    }
    cout << cheat_count_that_saves_100 << " cheats that save at least 100 picoseconds\n";

    delete [] cheats;
}
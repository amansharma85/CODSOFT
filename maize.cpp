#include <iostream>
#include <vector>
#include <queue>
#include <unordered_set>
using namespace std;

const int ROWS = 4;
const int COLS = 5;
char maze[ROWS][COLS] = {
    {'S', '.', 'T', '.', '.'},
    {'.', '#', '.', '#', '.'},
    {'T', '.', '.', 'T', '.'},
    {'#', '.', '#', '.', 'G'}
};

double score = 0;
struct State {
    int x, y;
    double score;
    vector<pair<int, int>> path;
};

vector<pair<int, int>> directions = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

void displayMaze() {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            cout << maze[i][j] << " ";
        }
        cout << endl;
    }
    cout << "Score: " << score << endl;
    cout << "-----------------" << endl;
}

bool isValidMove(int x, int y) {
    return (x >= 0 && x < ROWS && y >= 0 && y < COLS && maze[x][y] != '#');
}

vector<pair<int, int>> bfsCollectAllTreasures(int startX, int startY) {
    queue<State> q;
    q.push({startX, startY, score, {}});
    unordered_set<string> visited;
    vector<pair<int, int>> bestPath;
    double maxScore = -1e9;
    
    vector<pair<int, int>> treasures;
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (maze[i][j] == 'T') {
                treasures.push_back({i, j});
            }
        }
    }
    
    while (!q.empty()) {
        State current = q.front(); q.pop();
        int x = current.x, y = current.y;
        string key = to_string(x) + "," + to_string(y);
        if (visited.count(key)) continue;
        visited.insert(key);
        
        current.path.push_back({x, y});
        double currentScore = current.score - 0.2;
        
        if (maze[x][y] == 'T') {
            currentScore += 5;
            maze[x][y] = '.';
        }
        
        bool allTreasuresCollected = true;
        for (auto [tx, ty] : treasures) {
            if (maze[tx][ty] == 'T') {
                allTreasuresCollected = false;
                break;
            }
        }
        
        if (allTreasuresCollected && maze[x][y] == 'G' && currentScore > maxScore) {
            bestPath = current.path;
            maxScore = currentScore;
        }
        
        for (auto d : directions) {
            int newX = x + d.first, newY = y + d.second;
            if (isValidMove(newX, newY)) {
                q.push({newX, newY, currentScore, current.path});
            }
        }
    }
    
    if (maxScore > -1e9) {
        score = maxScore;
    }
    return bestPath;
}

void playGame() {
    cout << "Initial Maze State:" << endl;
    displayMaze();
    
    cout << "Enter your first move (U/D/L/R): ";
    char move;
    cin >> move;
    int startX = 0, startY = 0;
    
    if (move == 'U') startX--;
    else if (move == 'D') startX++;
    else if (move == 'L') startY--;
    else if (move == 'R') startY++;
    
    if (!isValidMove(startX, startY)) {
        cout << "Invalid move! Game Over." << endl;
        return;
    }
    
    score -= 0.2;
    if (maze[startX][startY] == 'T') {
        score += 5;
        maze[startX][startY] = '.';
    }
    
    maze[startX][startY] = 'P';
    displayMaze();
    
    vector<pair<int, int>> optimalPath = bfsCollectAllTreasures(startX, startY);
    
    for (auto [x, y] : optimalPath) {
        maze[x][y] = 'P';
        displayMaze();
    }
    
    if (score > -1e9) {
        cout << "Final Score: " << score << "\n";
    } else {
        cout << "No valid path found!\n";
    }
    
    cout << "Game Completed!\n";
}

int main() {
    cout << "Running BFS Treasure Collector...\n";
    playGame();
    return 0;
}

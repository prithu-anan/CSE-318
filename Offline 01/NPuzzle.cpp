#include <iostream>
#include <iomanip>
#include <vector>
#include <queue>
#include <stack>
#include <map>
#include <cmath>
#include <memory>
#include <chrono>

using namespace std;
using namespace std::chrono;
typedef vector<vector<string>> Grid;

class NPuzzle {
    int k;
    Grid initial, goal;

    enum Direction {
        NOOP = 0,
        UP = 1,   
        LEFT = 2,
        RIGHT = 3,
        DOWN = 4,
        INVALID = 5
    };

    enum Heuristic {
        HAMMING = 0,
        MANHATTAN = 1
    };

    struct Node {
        Grid grid;
        shared_ptr<Node> prev; 
        int parent_move, g, f;       

        Node(Grid& board, int pm = NOOP, shared_ptr<Node> parent = nullptr, int actual = 0, int total = 0) {            
            grid = board;
            prev = parent;
            parent_move = pm;
            g = actual;
            f = total;
        }
    };

    struct Compare {
        bool operator()(const shared_ptr<Node>& n1, const shared_ptr<Node>& n2) {
            return n1->f > n2->f;
        }
    };

    int conquer(vector<int>& flat, int left, int mid, int right) {
        vector<int> temp;
        int i = left, j = mid + 1;
        int count = 0;

        while(i <= mid && j <= right) {
            if(flat[i] <= flat[j])
                temp.push_back(flat[i++]);
            else {
                temp.push_back(flat[j++]);
                count += mid + 1 - i;
            }
        }

        while(i <= mid)
            temp.push_back(flat[i++]);

        while(j <= right)
            temp.push_back(flat[j++]);

        for(i = 0; i < temp.size(); i++)
            flat[left + i] = temp[i];

        return count;
    }

    int divide(vector<int>& flat, int left, int right) {
        if(left >= right) return 0;
        int mid = (left + right) / 2;
        int count = divide(flat, left, mid) + divide(flat, mid + 1, right) + conquer(flat, left, mid, right);
        return count;
    }

    int inversionCount() {
        vector<int> flat;

        for(auto row : initial) {
            for(auto box : row) {
                if(box != "*") {
                    flat.push_back(stoi(box));
                }
            }
        }
        
        int count = divide(flat, 0, flat.size() - 1);
        return count;
    }

    int findBlankRow() {
        for(int row = 0; row < k; row++)
            for(auto box : initial[row])
                if(box == "*")
                    return k - row - 1;
        return -1;
    }

    bool isSolvable() {
        int inversion = inversionCount();
        bool isInversionEven = (inversion % 2 == 0);
        
        if(k % 2 != 0) 
            return isInversionEven;
        else {
            int blankRow = findBlankRow();
            bool isBlankRowEven = (blankRow % 2 != 0);
            return isBlankRowEven ^ isInversionEven;
        }
    }

    string flattenGrid(Grid grid) {
        string flat = "";
        for(auto row : grid)
            for(auto box : row)
                flat += box + "#";
        return flat;
    }

    void printGrid(Grid& grid) {
        for(auto row : grid) {           
            for(auto box : row)
                cout << setw(3) << box << " ";
            cout << endl;
        }
        cout << endl << endl;
    }

    void printSolution(shared_ptr<Node> node) {
        stack<shared_ptr<Node>> moves;
        shared_ptr<Node> itr = node;
        int count = 0;
        while(itr != nullptr) {
            moves.push(itr);
            itr = itr->prev;
            count++;
        }
        cout << "Number of Moves: " << count - 1 << endl << endl;
        while(!moves.empty()) {
            printGrid(moves.top()->grid);
            moves.pop();
        }
    }

    pair<int, int> findBlank(Grid& grid) {
        for(int row = 0; row < k; row++)
            for(int col = 0; col < k; col++)
                if(grid[row][col] == "*")
                    return {row, col};
        return {-1, -1};
    }

    Grid swap(Grid& oldGrid, int a, int b, int c, int d) {
        Grid newGrid = oldGrid;
        string temp = newGrid[a][b];
        newGrid[a][b] = newGrid[c][d];
        newGrid[c][d] = temp;
        return newGrid;
    }

    Grid move(Grid& oldGrid, int dir, int row, int col) {
        switch(dir) {
            case UP:
                if (row > 0) return swap(oldGrid, row, col, row - 1, col);
                break;
            case DOWN:
                if (row < k - 1) return swap(oldGrid, row, col, row + 1, col);
                break;
            case LEFT:
                if (col > 0) return swap(oldGrid, row, col, row, col - 1);
                break;
            case RIGHT:
                if (col < k - 1) return swap(oldGrid, row, col, row, col + 1);
                break;
        }
        return oldGrid;
    }

    int calcHammingDistance(Grid grid) {
        int count = 0;
        for(int i = 0; i < k; i++) {
            for(int j = 0; j < k; j++) {
                if(grid[i][j] == "*") continue;
                if(stoi(grid[i][j]) != (i * k) + j + 1) count++;
            }
        }           
        return count;
    }

    int calcManhattanDistance(Grid grid) {
        int count = 0;
        for(int i = 0; i < k; i++) {
            for(int j = 0; j < k; j++) {
                if(grid[i][j] == "*") continue;
                int value = stoi(grid[i][j]) - 1;
                int targetRow = value / k;
                int targetCol = value % k;
                count += abs(i - targetRow) + abs(j - targetCol);
            }
        }           
        return count;
    }

    void AStarSearch(int distance = MANHATTAN) {
        auto start = high_resolution_clock::now();
        priority_queue<shared_ptr<Node>, vector<shared_ptr<Node>>, Compare> open;
        map<string, int> close;
        int explored_nodes = 0, expanded_nodes = 0;
        string goal_flat = flattenGrid(goal);

        open.push(make_shared<Node>(initial));
        expanded_nodes++;
        while(!open.empty()) {
            auto node = open.top();
            explored_nodes++;
            open.pop();
            string flat = flattenGrid(node->grid);
            close[flat] = node->f;

            if(flat == goal_flat) {
                auto end = high_resolution_clock::now();
                cout << "Using " << ((distance == HAMMING) ? "Hamming" : "Manhattan") << " Distance:" << endl; 
                auto duration = duration_cast<milliseconds>(end - start);
                cout << "Search Duration: " << duration.count() << " ms" << endl;
                cout << "Expanded Nodes: " << expanded_nodes << endl;
                cout << "Explored Nodes: " << explored_nodes << endl;                
                printSolution(node);
                return;
            }
            
            for(int dir = 1; dir <= 4; dir++) {
                if(dir + node->parent_move != INVALID) {
                    pair<int, int> position = findBlank(node->grid);
                    Grid grid = move(node->grid, dir, position.first, position.second);                    
                    int newG = node->g + 1;
                    int newF = newG; 
                    newF += (distance == HAMMING) ? calcHammingDistance(grid) : calcManhattanDistance(grid);

                    expanded_nodes++;
                    flat = flattenGrid(grid);

                    if(close.find(flat) == close.end() || close[flat] > newF) {
                        close[flat] = newF;
                        open.push(make_shared<Node>(grid, dir, node, newG, newF));
                    }
                }
            }
        }
    }


    public:
    NPuzzle(int k) {
        this->k = k;
        goal.resize(k);
        for(int i = 0; i < k; i++)
            for(int j = 0; j < k; j++)
                goal[i].push_back(to_string((i * k) + j + 1));
        goal[k - 1][k - 1] = "*";
    }

    void initGrid() {
        string input;
        initial.resize(k);
        for(int i = 0; i < k; i++) {
            for(int j = 0; j < k; j++) {
                cin >> input;
                initial[i].push_back(input);
            }
        }
    }

    void solve() {
        if(!isSolvable()) {
            cout << "Puzzle not solvable" << endl;
            return;
        }        
        AStarSearch(MANHATTAN);
        AStarSearch(HAMMING);
    }
};

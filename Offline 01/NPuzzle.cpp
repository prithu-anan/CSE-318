#include <iostream>
#include <vector>
#include <queue>
using namespace std;

typedef vector<string> Array;
typedef vector<vector<string>> Grid;

class NPuzzle {
    int k;
    Grid initial, goal;

    struct Node {
        Grid curr;
        Node* prev;
        int g;

        Node(Grid& c, Node* p = nullptr, int src = 0) {            
            curr = c;
            prev = p;
            g = src;
        }
    };

    struct Compare {
        bool operator()(Node n1, Node n2) {
            return n1.g > n2.g;
        }
    };

    int inversionCount() {
        int count = 0, offset = 0;
        for(int i = 0; i < k; i++) {
            for(int j = 0; j < k; j++) {
                if(initial[i][j] == "*") {
                    offset = 1;
                    continue;
                }
                count += max(0, stoi(initial[i][j]) - ((i * k) + j + 1) + offset);
            }
        }
        cout << count << endl;           
        return count;
    }

    int findBlank() {
        for(int row = 0; row < k; row++)
            for(auto box : initial[row])
                if(box == "*")
                    return k - row - 1;
    }

    bool isSolvable() {
        int inversion = inversionCount();
        bool isInversionEven = (inversion % 2 == 0);
        if(k % 2 != 0) return isInversionEven;
        else {
            int blankRow = findBlank();
            bool isBlankRowEven = (blankRow % 2 != 0);
            return isBlankRowEven ^ isInversionEven;
        }
    }

    int calcHammingDistance() {
        int count = 0;
        for(int i = 0; i < k; i++) {
            for(int j = 0; j < k; j++) {
                if(initial[i][j] == "*") continue;
                if(stoi(initial[i][j]) != (i * k) + j + 1) count++;
            }
        }           
        return count;
    }

    int calcManhattanDistance() {
        int count = 0;
        for(int i = 0; i < k; i++) {
            for(int j = 0; j < k; j++) {
                if(initial[i][j] == "*") continue;
                int y = 0;
                while((y + 1) * k <= stoi(initial[i][j]))
                    y++;
                int x = stoi(initial[i][j]) - 1 - (y * k);
                count += abs(i - y) + abs(j - x);
            }
        }           
        return count;
    }

    public:
    NPuzzle(int k) {
        this->k = k;
        goal.resize(k);
        for(int i = 0; i < k; i++)
            for(int j = 0; j < k; j++)
                goal[i].push_back(to_string((i * k) + j + 1));
        goal[k - 1][k - 1] = "*";

        // cout << "Printing Goal Grid: " << endl;
        // for(auto row : goal) {
        //     for(auto box : row)
        //         cout << box << " ";
        //     cout << endl;
        // }
    }

    void initGrid() {
        string input;
        // cout << "Enter Initial Grid in Row Major Order:" << endl;
        initial.resize(k);
        for(int i = 0; i < k; i++) {
            for(int j = 0; j < k; j++) {
                cin >> input;
                initial[i].push_back(input);
            }
        }
    }

    void AStarSearch() {
        priority_queue<Node, vector<Node>, Compare> open, close;
    }

    void solve() {
        if(!isSolvable()) {
            cout << "Puzzle not solvable";
            return;
        }
        AStarSearch();
    }

    void printGrid() {
        cout << "Printing Initial Grid: " << endl;
        for(auto row : initial) {
            for(auto box : row)
                cout << box << " ";
            cout << endl;
        }
    }
};
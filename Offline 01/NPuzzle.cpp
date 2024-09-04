#include <iostream>
#include <vector>
#include <map>
using namespace std;

typedef vector<string> array;
typedef vector<vector<string>> grid;

class NPuzzle {
    int k;
    grid initial, goal;

    public:
    NPuzzle(int k) {
        this->k = k;
        goal.resize(k);
        for(int i = 0; i < k; i++)
            for(int j = 0; j < k; j++)
                goal[i].push_back(to_string((i * k) + j + 1));
        goal[k - 1][k - 1] = "*";

        cout << "Printing Goal Grid: " << endl;
        for(auto row : goal) {
            for(auto box : row)
                cout << box << " ";
            cout << endl;
        }
    }

    void initGrid() {
        string input;
        cout << "Enter Initial Grid in Row Major Order:" << endl;
        initial.resize(k);
        for(int i = 0; i < k; i++) {
            for(int j = 0; j < k; j++) {
                cin >> input;
                initial[i].push_back(input);
            }
        }
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
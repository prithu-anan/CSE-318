#include <iostream>
#include "NPuzzle.cpp"
using namespace std;

int main() {
    NPuzzle npuzzle(4);
    npuzzle.initGrid();
    npuzzle.printGrid();
    return 0;
}
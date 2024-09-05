#include <iostream>
#include <fstream>
#include "NPuzzle.cpp"
using namespace std;

int main() {
    ifstream input_file("input.txt");
    streambuf* cin_buf = cin.rdbuf();
    cin.rdbuf(input_file.rdbuf());

    ofstream output_file("output.txt");
    streambuf* cout_buf = cout.rdbuf();
    cout.rdbuf(output_file.rdbuf());

    int k;
    cin >> k;

    NPuzzle npuzzle(k);
    npuzzle.initGrid();
    npuzzle.solve();

    cin.rdbuf(cin_buf);
    input_file.close();
    cout.rdbuf(cout_buf);
    output_file.close();
    return 0;
}
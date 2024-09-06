#include <iostream>
#include <fstream>
#include "2005045_NPuzzle.cpp"
using namespace std;

int main() {
    ifstream input_file("input.txt");
    streambuf* cin_buf = cin.rdbuf();
    cin.rdbuf(input_file.rdbuf());

    ofstream manhattan_output_file("manhattan_output.txt");
    ofstream hamming_output_file("hamming_output.txt");
    streambuf* cout_buf = cout.rdbuf();

    int k;
    cin >> k;
    NPuzzle npuzzle(k);
    npuzzle.initGrid();

    cout.rdbuf(manhattan_output_file.rdbuf());
    npuzzle.solve(MANHATTAN);
    manhattan_output_file.close();

    cout.rdbuf(hamming_output_file.rdbuf());
    npuzzle.solve(HAMMING);
    hamming_output_file.close();

    cout.rdbuf(cout_buf);
    cin.rdbuf(cin_buf);
    input_file.close();

    return 0;
}

/*
4
1 3 4 10
6 5 7 2
9 12 8 11
14 13 15 *
*/
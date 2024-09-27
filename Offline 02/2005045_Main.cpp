#include <iostream>
#include <fstream>
#include "2005045_Mancala.cpp"
using namespace std;

int main() {
    // ofstream out("output.txt");
    // streambuf *cout_buf = cout.rdbuf();
    // cout.rdbuf(out.rdbuf());

    Mancala mancala;
    mancala.play();

    // cout.rdbuf(cout_buf);
    // out.close();
    return 0;
}
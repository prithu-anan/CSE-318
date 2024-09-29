#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include "2005045_Mancala.cpp"
using namespace std;

int main() {
    Mancala mancala;
    vector<int> results = {0, 0, 0};
    bool isFile = true;
    bool showBoard = true;
    streambuf *cout_buf = cout.rdbuf();

    if(isFile) {
        ofstream out("output.txt");
        ofstream report("report.txt");

        cout.rdbuf(report.rdbuf());

        for(int i = 0; i < 75; i++)
            cout << "-";
        cout << endl;

        cout << "|" << setw(7) << "|" << setw(17) << "Player 1" << setw(11) << "|" << setw(17) << "Player 2" << setw(10) << "|" << setw(12) << "|" << endl;

        for(int i = 0; i < 75; i++)
            cout << "-";
        cout << endl;

        cout << "|" << setw(5) << "Game" << setw(2) << "|" << setw(4) << "W1" << setw(5) << "W2" << setw(5) << "W3" << setw(5) << "W4" << setw(7) << "Score" << setw(2) << "|" << setw(6) << "Score" << setw(4) << "W1" << setw(5) << "W2" << setw(5) << "W3" << setw(5) << "W4" << setw(2) << "|" <<  setw(10) << "Winner" << setw(2) << "|" << endl;

        for(int i = 0; i < 75; i++)
            cout << "-";
        cout << endl;

        // Play 100 games
        for(int game = 1; game <= 100; game++) {
            cout.rdbuf(out.rdbuf());

            mancala.initBoard(H1, H3);
            cout << "Game " << game << ": ";
            int result = mancala.play(isFile);

            results[result - PLAYER1_WINS]++;

            cout.rdbuf(report.rdbuf());
            mancala.printReport(game, result);
        }

        for(int i = 0; i < 75; i++)
            cout << "-";
        cout << endl << endl;

        cout << "Player 1 wins: " << results[0] << endl;
        cout << "Player 2 wins: " << results[1] << endl;
        cout << "Draws: " << results[2] << endl;

        if(results[1] != 0)
            cout << "Ratio: " << (double)results[0] / (double)results[1] << endl;
        else
            cout << "Ratio: Infinity" << endl;


        cout.rdbuf(cout_buf);

        out.close();
        report.close();
    }

    else {
        mancala.initBoard(H3, H4);
        mancala.play(isFile, showBoard);
    }

    return 0;
}

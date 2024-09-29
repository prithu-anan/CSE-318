#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include "2005045_Mancala.cpp"
using namespace std;

/*******************************/
#define MATCHES 100

#define PLAYER1_HEURISTIC H3
#define PLAYER2_HEURISTIC H4

#define IS_PLAYER1_HUMAN true
#define IS_PLAYER2_HUMAN !true

#define IS_EXPERIMENT !true
#define IS_FILE true
#define SHOW_BOARD !true
/*******************************/

int main() {
    Mancala mancala;
    vector<int> results = {0, 0, 0};
    streambuf *cout_buf = cout.rdbuf();

    if(IS_FILE) {
        ofstream out("output.txt");
        ofstream report("report.txt");

        if(IS_EXPERIMENT) {
            ofstream experiment("experiment.txt");

            cout.rdbuf(experiment.rdbuf());

            for(int i = 0; i < 89; i++)
                cout << "-";
            cout << endl;

            cout    << "| " << "Player1_Heuristic " 
                    << "| " << "Player2_Heuristic " 
                    << "| " << "Player1_Wins " 
                    << "| " << "Player2_Wins " 
                    << "| " << "Draws " 
                    << "| " << " Ratio  " 
                    << "|" << endl;

            for(int i = 0; i < 89; i++)
                cout << "-";
            cout << endl;

            cout.rdbuf(report.rdbuf());

            for(int h1 = H1; h1 <= H4; h1++) {
                for(int h2 = H1; h2 <= H4; h2++) {

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

                    for(int game = 1; game <= MATCHES; game++) {
                        cout.rdbuf(out.rdbuf());

                        mancala.initBoard(h1, h2);
                        cout << "Game " << game << ": ";
                        int result = mancala.play(IS_FILE, SHOW_BOARD);

                        results[result - PLAYER1_WINS]++;

                        cout.rdbuf(report.rdbuf());
                        mancala.printReport(game, result);
                    }

                    for(int i = 0; i < 75; i++)
                        cout << "-";
                    cout << endl << endl;

                    cout << "H" << h1 << " vs H" << h2 << endl;
                    cout << "Player 1 wins: " << results[0] << endl;
                    cout << "Player 2 wins: " << results[1] << endl;
                    cout << "Draws: " << results[2] << endl;

                    if(results[1] != 0)
                        cout << "Ratio: " << (double)results[0] / (double)results[1] << endl << endl;
                    else
                        cout << "Ratio: Infinity" << endl << endl;

                    cout.rdbuf(experiment.rdbuf());

                    cout    << "|" << setw(10) << "H" << setw(9) << left << h1 
                            << "|" << setw(10) << right << "H" << setw(9) << left << h2 
                            << "|" << setw(6) << "" << setfill('0') << setw(2) << right << results[0] << setfill(' ') << setw(7) 
                            << "|" << setw(6) << "" << setfill('0') << setw(2) << right << results[1] << setfill(' ') << setw(7) 
                            << "|" << setw(3) << "" << setfill('0') << setw(2) << right << results[2] << setfill(' ') << setw(3) 
                            << "|" << setw(9);

                    if(results[1] != 0)
                        cout << setprecision(2) << (double)results[0] / (double)results[1] << "|" << endl;
                    else
                        cout << "Infinity" << "|" << endl;

                    cout.rdbuf(report.rdbuf());
                    results = {0, 0, 0};
                }

                cout.rdbuf(experiment.rdbuf());

                for(int i = 0; i < 89; i++)
                    cout << "-";
                cout << endl;

                cout.rdbuf(report.rdbuf());
            }
        }

        else {
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

            for(int game = 1; game <= MATCHES; game++) {
                cout.rdbuf(out.rdbuf());

                mancala.initBoard(PLAYER1_HEURISTIC, PLAYER1_HEURISTIC);
                cout << "Game " << game << ": ";
                int result = mancala.play(IS_FILE, SHOW_BOARD);

                results[result - PLAYER1_WINS]++;

                cout.rdbuf(report.rdbuf());
                mancala.printReport(game, result);
            }

            for(int i = 0; i < 75; i++)
                cout << "-";
            cout << endl << endl;

            cout << "H" << PLAYER1_HEURISTIC << " vs H" << PLAYER2_HEURISTIC << endl;
            cout << "Player 1 wins: " << results[0] << endl;
            cout << "Player 2 wins: " << results[1] << endl;
            cout << "Draws: " << results[2] << endl;

            if(results[1] != 0)
                cout << "Ratio: " << (double)results[0] / (double)results[1] << endl << endl;
            else
                cout << "Ratio: Infinity" << endl << endl;
        }

        cout.rdbuf(cout_buf);

        out.close();
        report.close();
    }

    else {
        mancala.initBoard(PLAYER1_HEURISTIC, PLAYER2_HEURISTIC);
        mancala.play(IS_FILE, SHOW_BOARD, IS_PLAYER1_HUMAN, IS_PLAYER2_HUMAN);
    }

    return 0;
}

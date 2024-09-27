#include <iostream>
#include <iomanip>
#include <vector>
using namespace std;

enum players {
    PLAYER1 = 1,
    PLAYER2 = 2
};

enum states {
    INVALID_MOVE = -1,
    CONTINUE = 0,
    MOVE_AGAIN = 1,
    PLAYER1_WINS = 2,
    PLAYER2_WINS = 3,
    DRAW = 4
};

class Mancala {
    struct Board {
        vector<int> pits1;
        vector<int> pits2;
        int mancala1;
        int mancala2;

        Board() {
            pits1 = vector<int>(7, 4);
            pits2 = vector<int>(7, 4);
            mancala1 = 0;
            mancala2 = 0;
        }
    };

    Board board;

    bool areAllPitsEmpty(int player) {
        vector<int>& pits = player == PLAYER1 ? board.pits1 : board.pits2;
        for(int i = 1; i <= 6; i++) {
            if(pits[i] != 0)
                return false;
        }
        return true;
    }

    int calculateWinner() {
        for(int i = 1; i <= 6; i++) {
            board.mancala1 += board.pits1[i];
            board.mancala2 += board.pits2[i];
            board.pits1[i] = 0;
            board.pits2[i] = 0;
        }

        if(board.mancala1 > board.mancala2)
            return PLAYER1_WINS;
        else if(board.mancala1 < board.mancala2)
            return PLAYER2_WINS;
        else
            return DRAW;
    }

    int move(int player, int pit) {
        vector<int>& myPits = ((player == PLAYER1) ? board.pits1 : board.pits2);
        vector<int>& opponentPits = ((player == PLAYER2) ? board.pits1 : board.pits2);

        int& myMancala = ((player == PLAYER1) ? board.mancala1 : board.mancala2);
        int& opponentMancala = ((player == PLAYER2) ? board.mancala1 : board.mancala2);

        if(myPits[pit] == 0 || pit < 1 || pit > 6) 
            return INVALID_MOVE;

        int count = myPits[pit];
        myPits[pit] = 0;

        for(int i = 1; i <= count; i++) {
            if((pit + i) % 14 <= 6) {
                myPits[pit + i]++;
                
                if(i == count && myPits[pit + i] == 1 && opponentPits[7 - (pit + i)] > 0) {
                    myMancala += myPits[pit + i] + opponentPits[7 - (pit + i)];
                    myPits[pit + i] = 0;
                    opponentPits[7 - (pit + i)] = 0;
                }
            }

            else if((pit + i) % 14 == 7) {
                myMancala++;

                if(i == count) {
                    myPits[pit + i] = 0;
                    return MOVE_AGAIN;
                }
            }

            else if((pit + i) % 14 <= 13 && (pit + i) % 14 >= 8)
                opponentPits[pit + i - 7]++;
            
            else 
                opponentMancala++;
        }

        return CONTINUE;
    }

    /*
            |   4   4   4   4   4   4   |
        0   |                           |   0
            |   4   4   4   4   4   4   |
    */

   void printBoard() {
        cout << setw(8) << "";
        for(int i = 6; i >= 1; i--)
            cout << setw(4) << i;
        cout << endl;

        cout << setw(8) << "";
        for(int i = 27; i >= 1; i--)
            cout << "-";
        cout << endl;

        cout << setw(8) << "|";
        cout << "\033[31m";
        for(int i = 1; i <= 6; i++)
            cout << setw(4) << board.pits2[7 - i];
        cout << "\033[0m";
        cout << setw(4) << "|" << endl;

        cout << "\033[31m" << setw(4) << board.mancala2 << "\033[0m" <<setw(4) << "|";
        for(int i = 1; i <= 6; i++)
            cout << setw(4) << "";
        cout << setw(4) << "|" << "\033[34m" << setw(4) << board.mancala1 << "\033[0m" << endl;

        cout << setw(8) << "|";
        cout << "\033[34m";
        for(int i = 1; i <= 6; i++)
            cout << setw(4) << board.pits1[i];
        cout << "\033[0m";
        cout << setw(4) << "|" << endl;

        cout << setw(8) << "";
        for(int i = 1; i <= 27; i++)
            cout << "-";
        cout << endl;

        cout << setw(8) << "";
        for(int i = 1; i <= 6; i++)
            cout << setw(4) << i;
        cout << endl;
    }

    public:
    void play() {
        int currentPlayer = PLAYER1;
        int currentState = CONTINUE;
        int pit;

        while(currentState == CONTINUE) {
            printBoard();

            if(areAllPitsEmpty(PLAYER1) || areAllPitsEmpty(PLAYER2)) {
                currentState = calculateWinner();
                break;
            }

            cout << "Player " << currentPlayer << "'s turn:\n";
            cin >> pit;

            currentState = move(currentPlayer, pit);

            if(currentState == INVALID_MOVE) {
                cout << "Invalid move. Try again." << endl;
                currentState = CONTINUE;
                continue;
            }

            if(currentState == MOVE_AGAIN) {
                cout << "Player " << currentPlayer << " gets another turn." << endl;
                currentState = CONTINUE;
                continue;
            }

            if(currentPlayer == PLAYER1)
                currentPlayer = PLAYER2;
            else
                currentPlayer = PLAYER1;
        }

        if(currentState == PLAYER1_WINS)
            cout << "Player 1 wins!" << endl;
        else if(currentState == PLAYER2_WINS)
            cout << "Player 2 wins!" << endl;
        else
            cout << "It's a draw!" << endl;
        printBoard();
    }  
};

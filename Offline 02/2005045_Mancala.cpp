#include <iostream>
#include <iomanip>
#include <vector>
#include <limits>
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

        Board(const Board& other) {
            pits1 = other.pits1;
            pits2 = other.pits2;
            mancala1 = other.mancala1;
            mancala2 = other.mancala2;
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

    // int move(int player, int pit) {
    //     vector<int>& myPits = ((player == PLAYER1) ? board.pits1 : board.pits2);
    //     vector<int>& opponentPits = ((player == PLAYER2) ? board.pits1 : board.pits2);

    //     int& myMancala = ((player == PLAYER1) ? board.mancala1 : board.mancala2);
    //     int& opponentMancala = ((player == PLAYER2) ? board.mancala1 : board.mancala2);

    //     if(myPits[pit] == 0 || pit < 1 || pit > 6) 
    //         return INVALID_MOVE;

    //     int count = myPits[pit];
    //     myPits[pit] = 0;

    //     for(int i = 1; i <= count; i++) {
    //         if((pit + i) % 14 <= 6) {
    //             myPits[pit + i]++;
                
    //             if(i == count && myPits[pit + i] == 1 && opponentPits[7 - (pit + i)] > 0) {
    //                 myMancala += myPits[pit + i] + opponentPits[7 - (pit + i)];
    //                 myPits[pit + i] = 0;
    //                 opponentPits[7 - (pit + i)] = 0;
    //             }
    //         }

    //         else if((pit + i) % 14 == 7) {
    //             myMancala++;

    //             if(i == count) {
    //                 myPits[pit + i] = 0;
    //                 return MOVE_AGAIN;
    //             }
    //         }

    //         else if((pit + i) % 14 <= 13 && (pit + i) % 14 >= 8)
    //             opponentPits[pit + i - 7]++;
            
    //         else 
    //             opponentMancala++;
    //     }

    //     return CONTINUE;
    // }

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

    int evaluate(const Board& board) {
        return board.mancala2 - board.mancala1;
    }

    int move(int player, int pit, Board& boardState) {
        vector<int>& myPits = (player == PLAYER1) ? boardState.pits1 : boardState.pits2;
        vector<int>& opponentPits = (player == PLAYER2) ? boardState.pits1 : boardState.pits2;

        int& myMancala = (player == PLAYER1) ? boardState.mancala1 : boardState.mancala2;
        int& opponentMancala = (player == PLAYER2) ? boardState.mancala1 : boardState.mancala2;

        if (myPits[pit] == 0 || pit < 1 || pit > 6) 
            return INVALID_MOVE;

        int count = myPits[pit];
        myPits[pit] = 0;

        for (int i = 1; i <= count; i++) {
            if ((pit + i) % 14 <= 6) {
                myPits[pit + i]++;

                if (i == count && myPits[pit + i] == 1 && opponentPits[7 - (pit + i)] > 0) {
                    myMancala += myPits[pit + i] + opponentPits[7 - (pit + i)];
                    myPits[pit + i] = 0;
                    opponentPits[7 - (pit + i)] = 0;
                }
            } 
            
            else if ((pit + i) % 14 == 7) {
                myMancala++;

                if (i == count)
                    return MOVE_AGAIN;
            } 
            
            else if ((pit + i) % 14 <= 13 && (pit + i) % 14 >= 8)
                opponentPits[pit + i - 7]++;
        }

        return CONTINUE;
    }

    int minimax(Mancala& game, Board state, int depth, bool maximizingPlayer, int alpha, int beta) {
        if (depth == 0 || game.areAllPitsEmpty(PLAYER1) || game.areAllPitsEmpty(PLAYER2))
            return evaluate(state);

        if (maximizingPlayer) {
            int maxEval = numeric_limits<int>::min();

            for (int pit = 1; pit <= 6; pit++) {
                Board newState = state;
                int result = game.move(PLAYER2, pit, newState);

                if (result == INVALID_MOVE) continue;

                int eval;

                if (result == MOVE_AGAIN)
                    eval = minimax(game, newState, depth - 1, true, alpha, beta);
                else
                    eval = minimax(game, newState, depth - 1, false, alpha, beta);

                maxEval = max(maxEval, eval);
                alpha = max(alpha, eval);

                if (beta <= alpha)
                    break;
            }

            return maxEval;
        }
        
        else {
            int minEval = numeric_limits<int>::max();

            for (int pit = 1; pit <= 6; pit++) {
                Board newState = state;
                int result = game.move(PLAYER1, pit, newState);

                if (result == INVALID_MOVE) continue;

                int eval;

                if (result == MOVE_AGAIN) 
                    eval = minimax(game, newState, depth - 1, false, alpha, beta);
                else
                    eval = minimax(game, newState, depth - 1, true, alpha, beta);

                minEval = min(minEval, eval);
                beta = min(beta, eval);

                if (beta <= alpha)
                    break;
            }

            return minEval;
        }
    }


    int getBestMove(Mancala& game, Board state, int depth) {
        int bestMove = -1;
        int bestValue = numeric_limits<int>::min();

        bool foundValidMove = false;

        for (int pit = 1; pit <= 6; pit++) {
            Board newState = state;
            int result = game.move(PLAYER2, pit, newState);

            if (result == INVALID_MOVE) continue;

            foundValidMove = true;

            int moveValue;

            if (result == MOVE_AGAIN) 
                moveValue = minimax(game, newState, depth - 1, true, numeric_limits<int>::min(), numeric_limits<int>::max());
            else 
                moveValue = minimax(game, newState, depth - 1, false, numeric_limits<int>::min(), numeric_limits<int>::max());

            if (moveValue > bestValue) {
                bestValue = moveValue;
                bestMove = pit;
            }
        }

        if (!foundValidMove) {
            for (int pit = 1; pit <= 6; pit++) {
                if (state.pits1[pit] > 0) {
                    bestMove = pit;
                    break;
                }
            }
        }

        return bestMove;
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

            if (currentPlayer == PLAYER1) {
                cout << "Player " << currentPlayer << "'s turn:\n";
                cin >> pit;
            } 
            else {
                cout << "Player 2's turn:\n";
                pit = getBestMove(*this, board, 5);
                cout << "Player 2 chooses pit: " << pit << endl;
            }

            currentState = move(currentPlayer, pit, board);

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

#include <iostream>
#include <iomanip>
#include <vector>
#include <limits>
#include <random>
using namespace std;

/*******************************/
#define MIN_W1 3
#define MIN_W2 1
#define MIN_W3 2
#define MIN_W4 3

#define MAX_W1 5
#define MAX_W2 2
#define MAX_W3 4
#define MAX_W4 5

#define MIN_BEGINING_DEPTH 4
#define MIN_MIDDLE_DEPTH 6
#define MIN_END_DEPTH 8

#define MAX_BEGINING_DEPTH 6
#define MAX_MIDDLE_DEPTH 8
#define MAX_END_DEPTH 10
/*******************************/

enum players {
    PLAYER1 = 0,
    PLAYER2 = 1
};

enum states {
    INVALID_MOVE = -1,
    CONTINUE = 0,
    MOVE_AGAIN = 1,
    PLAYER1_WINS = 2,
    PLAYER2_WINS = 3,
    DRAW = 4
};

enum heuristics {
    H1 = 1,
    H2 = 2,
    H3 = 3,
    H4 = 4
};

struct Board {
    vector<int> pits1, pits2;
    int mancala1, mancala2;
    int additionalMove1, additionalMove2;
    int capturedStones1, capturedStones2;
    int moves;

    Board() {
        pits1 = vector<int>(7, 4);
        pits2 = vector<int>(7, 4);
        mancala1 = 0;
        mancala2 = 0;
        additionalMove1 = 0;
        additionalMove2 = 0;
        capturedStones1 = 0;
        capturedStones2 = 0;
        moves = 0;
    }

    Board(const Board& other) {
        pits1 = other.pits1;
        pits2 = other.pits2;
        mancala1 = other.mancala1;
        mancala2 = other.mancala2;
        additionalMove1 = other.additionalMove1;
        additionalMove2 = other.additionalMove2;
        capturedStones1 = other.capturedStones1;
        capturedStones2 = other.capturedStones2;
        moves = other.moves;
    }

    int countStones(int player) {
        vector<int>& pits = player == PLAYER1 ? pits1 : pits2;
        int count = 0;

        for(int i = 1; i <= 6; i++)
            count += pits[i];
        
        return count;
    }
};

class Mancala {
    Board board;
    vector<int> weigths1, weigths2;
    mt19937 gen;

    int getRandomNumber(int min, int max);
    void initRandomWeights(vector<int>& weights, int numWeights);
    int calculateWinner();
    void printBoard(bool isFile);
    int evaluate(Board state, int weight);
    int move(int player, int pit, Board& state);
    int minimax(Board state, int depth, bool maximizingPlayer, int player, int alpha, int beta, int weight);
    int getBestMove(Board state, int depth, int player, bool maximizingPlayer);

    public:
    Mancala();
    void initBoard(int h1, int h2);
    int play(bool isFile, bool showBoard, bool isPlayer1Human, bool isPlayer2Human);
    void printReport(int game, int result); 
};

int Mancala::getRandomNumber(int min, int max) {
    uniform_int_distribution<> dis(min, max);
    return dis(gen);
}

void Mancala::initRandomWeights(vector<int>& weights, int numWeights) {
    for (int i = 0; i < numWeights; ++i) {
        if(i == 0)
            weights.push_back(getRandomNumber(MIN_W1, MAX_W1));
        else if(i == 1)
            weights.push_back(getRandomNumber(MIN_W2, MAX_W2));
        else if(i == 2)
            weights.push_back(getRandomNumber(MIN_W3, MAX_W3));
        else if(i == 3)
            weights.push_back(getRandomNumber(MIN_W4, MAX_W4));
    }
        

    for (int i = numWeights; i < 4; i++)
        weights.push_back(0);
}

int Mancala::calculateWinner() {
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

void Mancala::printBoard(bool isFile) {
    cout << endl;
    cout << setw(8) << "";
    for(int i = 6; i >= 1; i--)
        cout << setw(4) << i;
    cout << endl;

    cout << setw(8) << "";
    for(int i = 27; i >= 1; i--)
        cout << "-";
    cout << endl;

    if(isFile) {
        cout << setw(8) << "|";
        for(int i = 1; i <= 6; i++)
            cout << setw(4) << board.pits2[7 - i];
        cout << setw(4) << "|" << endl;

        cout << setw(4) << board.mancala2 <<setw(4) << "|";
        for(int i = 1; i <= 6; i++)
            cout << setw(4) << "";
        cout << setw(4) << "|" << setw(4) << board.mancala1 << endl;

        cout << setw(8) << "|";
        for(int i = 1; i <= 6; i++)
            cout << setw(4) << board.pits1[i];
        cout << setw(4) << "|" << endl;
    }

    else {
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
    }

    cout << setw(8) << "";
    for(int i = 1; i <= 27; i++)
        cout << "-";
    cout << endl;

    cout << setw(8) << "";
    for(int i = 1; i <= 6; i++)
        cout << setw(4) << i;
    cout << endl << endl;
}

int Mancala::evaluate(Board state, int weight) {
    vector<int>& w = weight == PLAYER1 ? weigths1 : weigths2;
    
    int h1 = w[0] * (state.mancala1 - state.mancala2);
    int h2 = w[1] * (state.countStones(PLAYER1) - state.countStones(PLAYER2));
    int h3 = w[2] * (state.additionalMove1 - state.additionalMove2);
    int h4 = w[3] * (state.capturedStones1 - state.capturedStones2);

    return h1 + h2 + h3 + h4;
}

int Mancala::move(int player, int pit, Board& state) {
    vector<int>& myPits = (player == PLAYER1) ? state.pits1 : state.pits2;
    vector<int>& opponentPits = (player == PLAYER2) ? state.pits1 : state.pits2;

    int& myMancala = (player == PLAYER1) ? state.mancala1 : state.mancala2;
    int& opponentMancala = (player == PLAYER2) ? state.mancala1 : state.mancala2;

    int& additionalMove = (player == PLAYER1) ? state.additionalMove1 : state.additionalMove2;
    int& capturedStones = (player == PLAYER1) ? state.capturedStones1 : state.capturedStones2;

    if (myPits[pit] == 0 || pit < 1 || pit > 6) 
        return INVALID_MOVE;

    int count = myPits[pit];
    myPits[pit] = 0;
    state.moves++;

    for (int i = 1; i <= count; i++) {
        if ((pit + i) % 14 <= 6 && (pit + i) % 14 >= 1) {
            myPits[(pit + i) % 14]++;

            if (i == count && myPits[(pit + i) % 14] == 1 && opponentPits[7 - ((pit + i) % 14)] > 0) {
                myMancala += myPits[(pit + i) % 14] + opponentPits[7 - ((pit + i) % 14)];
                myPits[(pit + i) % 14] = 0;
                capturedStones += opponentPits[7 - ((pit + i) % 14)];
                opponentPits[7 - ((pit + i) % 14)] = 0;
            }
        } 
        
        else if ((pit + i) % 14 == 7) {
            myMancala++;

            if (i == count) {
                additionalMove++;
                return MOVE_AGAIN;
            }
        } 
        
        else if ((pit + i) % 14 <= 13 && (pit + i) % 14 >= 8)
            opponentPits[((pit + i) % 14) - 7]++;

        else if((pit + i) % 14 == 0)
            opponentMancala++;
    }

    return CONTINUE;
}

int Mancala::minimax(Board state, int depth, bool maximizingPlayer, int player, int alpha, int beta, int weight) {
    if (depth == 0 || state.countStones(PLAYER1) == 0 || state.countStones(PLAYER2) == 0)
        return evaluate(state, weight);

    if (maximizingPlayer) {
        int maxEval = numeric_limits<int>::min();

        for (int pit = 1; pit <= 6; pit++) {
            int result = move(player, pit, state);
            if (result == INVALID_MOVE) continue;
            int eval;

            if (result == MOVE_AGAIN)
                eval = minimax(state, depth - 1, maximizingPlayer, player, alpha, beta, weight);
            else
                eval = minimax(state, depth - 1, !maximizingPlayer, !player, alpha, beta, weight);

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
            int result = move(player, pit, state);
            if (result == INVALID_MOVE) continue;
            int eval;

            if (result == MOVE_AGAIN) 
                eval = minimax(state, depth - 1, !maximizingPlayer, player, alpha, beta, weight);
            else
                eval = minimax(state, depth - 1, maximizingPlayer, !player, alpha, beta, weight);

            minEval = min(minEval, eval);
            beta = min(beta, eval);

            if (beta <= alpha)
                break;
        }

        return minEval;
    }
}


int Mancala::getBestMove(Board state, int depth, int player, bool maximizingPlayer) {
    int bestMove = -1;
    int bestValue = maximizingPlayer ? numeric_limits<int>::min() : numeric_limits<int>::max();

    bool foundValidMove = false;

    for (int pit = 1; pit <= 6; pit++) {
        Board newState = state;
        int result = move(player, pit, newState);

        if (result == INVALID_MOVE) continue;

        foundValidMove = true;

        int moveValue;

        if (result == MOVE_AGAIN) 
            moveValue = minimax(newState, depth - 1, maximizingPlayer, player, numeric_limits<int>::min(), numeric_limits<int>::max(), player);
        else 
            moveValue = minimax(newState, depth - 1, !maximizingPlayer, !player, numeric_limits<int>::min(), numeric_limits<int>::max(), player);

        if (maximizingPlayer) {
            if (moveValue > bestValue) {
                bestValue = moveValue;
                bestMove = pit;
            }
        } 
        else {
            if (moveValue < bestValue) {
                bestValue = moveValue;
                bestMove = pit;
            }
        }
    }

    if (!foundValidMove || bestMove == -1) {
        vector<int>& pits = player == PLAYER1 ? state.pits1 : state.pits2;

        for (int pit = 1; pit <= 6; pit++) {
            if (pits[pit] > 0) {
                bestMove = pit;
                break;
            }
        }
    }

    return bestMove;
}

Mancala::Mancala() {
        srand(time(0));
        random_device rd;
        gen.seed(rd());
    }

    void Mancala::initBoard(int h1, int h2) {
        board = Board();
        weigths1.clear();
        weigths2.clear();
        
        switch(h1) {
            case H1:
                weigths1 = {1, 0, 0, 0};
                break;
            case H2:
                initRandomWeights(weigths1, 2);
                break;
            case H3:
                initRandomWeights(weigths1, 3);
                break;
            case H4:
                initRandomWeights(weigths1, 4);
                break;
        }

        switch(h2) {
            case H1:
                weigths2 = {1, 0, 0, 0};
                break;
            case H2:
                initRandomWeights(weigths2, 2);
                break;
            case H3:
                initRandomWeights(weigths2, 3);
                break;
            case H4:
                initRandomWeights(weigths2, 4);
                break;
        }
    
    }

int Mancala::play(bool isFile = false, bool showBoard = false, bool isPlayer1Human = false, bool isPlayer2Human = false) {
    int currentPlayer = PLAYER1;
    int currentState = CONTINUE;
    int pit;

    if(showBoard)
        cout << "Starting game..." << endl;

    while(currentState == CONTINUE) {
        if(showBoard)
            printBoard(isFile);

        if(board.countStones(PLAYER1) == 0 || board.countStones(PLAYER2) == 0) {
            currentState = calculateWinner();
            break;
        }

        int depth;

        if (board.moves <= 15)
            depth = getRandomNumber(MIN_BEGINING_DEPTH, MAX_BEGINING_DEPTH);
        else if(board.moves <= 30)
            depth = getRandomNumber(MIN_MIDDLE_DEPTH, MAX_MIDDLE_DEPTH);
        else
            depth = getRandomNumber(MIN_END_DEPTH, MAX_END_DEPTH);


        if (currentPlayer == PLAYER1) {
            if(showBoard)
                cout << "Player 1's turn: ";

            if(isPlayer1Human)
                cin >> pit;
            else
                pit = getBestMove(board, depth, PLAYER1, true);
                
            if(showBoard)
                cout << pit << endl;
        }
        else {
            if(showBoard)
                cout << "Player 2's turn: ";

            if(isPlayer2Human)
                cin >> pit;
            else
                pit = getBestMove(board, depth, PLAYER2, false);

            if(showBoard)
                cout << pit << endl;
        }

        currentState = move(currentPlayer, pit, board);

        if(currentState == INVALID_MOVE) {
            if(showBoard)
                cout << "Invalid move. Try again." << endl;
            currentState = CONTINUE;
            continue;
        }

        if(currentState == MOVE_AGAIN) {
            if(showBoard)
                cout << "Player " << currentPlayer + 1 << " gets another turn." << endl;
            currentState = CONTINUE;
            continue;
        }

        currentPlayer = !currentPlayer;
    }


    if(currentState == PLAYER1_WINS) {
        cout << "Player 1 wins!" << endl;
        printBoard(isFile);
        return PLAYER1_WINS;
    }
    else if(currentState == PLAYER2_WINS) {
        cout << "Player 2 wins!" << endl;
        printBoard(isFile);
        return PLAYER2_WINS;
    }
    else {
        cout << "It's a draw!" << endl;
        printBoard(isFile);
        return DRAW;
    } 
}  

void Mancala::printReport(int game, int result) {
    cout << "|" << setw(5) << game << setw(2) << "|" << setw(4) << weigths1[0] << setw(5) << weigths1[1] << setw(5) << weigths1[2] << setw(5) << weigths1[3] << setw(7) << board.mancala1 << setw(3) << "| " << left << setw(5) << board.mancala2 << right << setw(4) << weigths2[0] << setw(5) << weigths2[1] << setw(5) << weigths2[2] << setw(5) << weigths2[3] << setw(2) << "|" << setw(10);

    if(result == PLAYER1_WINS)
        cout << "Player 1";
    else if(result == PLAYER2_WINS)
        cout << "Player 2";
    else
        cout << "Draw";

    cout << setw(2) << "|" << endl;;
}
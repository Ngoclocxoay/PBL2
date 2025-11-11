#pragma once
#include "Vector.h"
#include "constant.hpp"   
#include "Move.h"          

class Board;              

class ChessAI {
public:
    int depth = 5;
    int Evaluate(const Board& board, Side pov) const;
    void GenerateLegalMoves(const Board& board, Side side, Vector<Move>& out) const;
    int Search(Board& board, int d, int alpha, int beta, Side sideToMove, Side pov);
    bool FindBestMove(Board& board, Side sideToMove, Move& outBest);

    //Helper
    bool isInCheck(Colors, const Board*) const;
};

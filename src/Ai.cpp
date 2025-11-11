#include "Ai.h"
#include "Board.h"       
#include "All_piece.h"
#include "constant.hpp"
#include <algorithm>     
#include <climits>     

static inline Colors SideToColor(Side s) {
    return (s == Side::White) ? Colors::White : Colors::Black;
}

// Giá trị nước đi
int ChessAI::Evaluate(const Board& board, Side pov) const {
    static const int val[6] = { 100, 300, 300, 500, 900, 100000 }; // P,N,B,R,Q,K
    int score = 0;
    for (int x = 0; x < 8; ++x)
        for (int y = 0; y < 8; ++y) {
            const Piece* p = board.GetPiece(x,y);
            if (!p) continue;
            int v = 0;
            switch (p->getType()) {
                case PieceType::PAWN:   v = val[0]; break;
                case PieceType::KNIGHT: v = val[1]; break;
                case PieceType::BISHOP: v = val[2]; break;
                case PieceType::ROOK:   v = val[3]; break;
                case PieceType::QUEEN:  v = val[4]; break;
                case PieceType::KING:   v = val[5]; break;
                default: v = 0; break; 
            }
            score += (p->getColor() == Colors::White) ? v : -v;
        }

    return (pov == Side::White) ? score : -score;
}

// Đệ quy nước đi
void ChessAI::GenerateLegalMoves(const Board& board, Side side, Vector<Move>& out) const {
    out.clear();
    Colors me = SideToColor(side);

    for (int fx = 0; fx < 8; ++fx)
        for (int fy = 0; fy < 8; ++fy) {
            const Piece* p = board.GetPiece(fx, fy);
            if (!p || p->getColor() != me) continue;

            for (int tx = 0; tx < 8; ++tx)
                for (int ty = 0; ty < 8; ++ty) {
                    // di chuyển
                    if (!p->isValidMove(fx, fy, tx, ty, &board)) continue;

                    Move mv{fx, fy, tx, ty};
                    const Piece* dst = board.GetPiece(tx, ty);
                    if (dst && dst->getColor() == me) continue; // tự ăn đồng đội = break
                    Board& b = const_cast<Board&>(board);
                    if (!b.MakeMoveEngine(mv)) continue;
                    bool ischeck = isInCheck(me, &b);
                    b.UndoMoveEngine(mv);
                    //clean
                    mv.captured     = nullptr;
                    mv.promotedPawn = nullptr;
                    mv.wasPromote   = false;
                    mv.wasCastling  = false;
                    mv.rookfromX    = -1;
                    mv.rooktoX      = -1;
                    if (!ischeck) out.push_back(mv);
                }
        }
}

// ====== MINIMAX + ALPHA-BETA ======
int ChessAI::Search(Board& board, int d, int alpha, int beta, Side sideToMove, Side pov) {
    if (d == 0) return Evaluate(board, pov);

    Vector<Move> moves;
    GenerateLegalMoves(board, sideToMove, moves);
    if (moves.empty()) {
        return 0;
    }
    std::sort(moves.begin(), moves.end(), [&](const Move& a, const Move& b){
        Piece* ca = board.GetPiece(a.toX, a.toY);
        Piece* cb = board.GetPiece(b.toX, b.toY);
        return (cb != nullptr) < (ca != nullptr);
    });

    if (sideToMove == pov) 
    {
        int best = INT_MIN/2;
        for (auto mv : moves) {
            if (!board.MakeMoveEngine(mv)) continue;
            int sc = Search(board, d-1, alpha, beta, (sideToMove==Side::White?Side::Black:Side::White), pov); //MINIMIZE
            board.UndoMoveEngine(mv);
            if (sc > best) best = sc;
            if (best > alpha) alpha = best;
            if (alpha >= beta) break; 
        }
        return best;
    }
    else 
    {
        int best = INT_MAX/2;
        for (auto mv : moves) 
        {
            if (!board.MakeMoveEngine(mv)) continue;
            int sc = Search(board, d-1, alpha, beta, (sideToMove==Side::White?Side::Black:Side::White), pov); //MAXIMIZE
            board.UndoMoveEngine(mv);
            if (sc < best) best = sc;
            if (best < beta) beta = best;
            if (alpha >= beta) break; 
        }
        return best;
    }
}

bool ChessAI::FindBestMove(Board& board, Side sideToMove, Move& outBest) {
    Vector<Move> moves;
    GenerateLegalMoves(board, sideToMove, moves);
    if (moves.empty()) return false;

    int bestScore = INT_MIN/2;
    Move best = moves[0];

    for (auto mv : moves) {
        if (!board.MakeMoveEngine(mv)) continue;
        int sc = Search(board, depth-1, INT_MIN/2, INT_MAX/2,
                        (sideToMove==Side::White?Side::Black:Side::White),
                        sideToMove);
        board.UndoMoveEngine(mv);

        if (sc > bestScore) { bestScore = sc; best = mv; }
    }
    outBest = best;
    return true;
}

// =====HELPER======
bool ChessAI::isInCheck(Colors side, const Board* board) const
{
    int kingX = -1;
    int kingY = -1;
    for (int x = 0; x < 8; x++)
    for (int y = 0; y < 8; y++)
    {
        const Piece* target = board->GetPiece(x, y);
        if (target)
        {
            if (target->getType() == PieceType::KING && target->getColor() == side)
            {
                kingX = x;
                kingY = y;
                goto foundKing;
            }
        }
    }
    foundKing:
    if (kingX == -1 && kingY == -1) return false;

    for (int fx = 0; fx < 8; fx++)
    for (int fy = 0; fy < 8; fy++)
    {
        const Piece* atker = board->GetPiece(fx, fy);
        if (atker)
        {
            if (atker->getColor() == side) continue;
            if (atker->isValidMove(fx, fy, kingX, kingY, board)) return true;
        }
    }
    return false;
}


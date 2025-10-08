#include "Queen.h"
#include "Board.h"
#include <cstdlib>  // std::abs
#include <cassert>

static inline bool inside(int x, int y) { return (0 <= x && x < 8) && (0 <= y && y < 8); }

Queen::Queen(Colors c)
    : Piece(PieceType::QUEEN, c)
{}

bool Queen::isStraight(int fromX, int fromY, int toX, int toY) const noexcept {
    // đi theo hàng hoặc cột, và không đứng yên
    return (fromX == toX && fromY != toY) || (fromY == toY && fromX != toX);
}

bool Queen::isDiagonal(int fromX, int fromY, int toX, int toY) const noexcept {
    int dx = std::abs(toX - fromX);
    int dy = std::abs(toY - fromY);
    return (dx == dy) && dx != 0;
}

bool Queen::pathClear(int fromX, int fromY, int toX, int toY, const Board* board) const {
    assert(board);
    int stepX = (toX > fromX) ? 1 : (toX < fromX ? -1 : 0);
    int stepY = (toY > fromY) ? 1 : (toY < fromY ? -1 : 0);

    int x = fromX + stepX;
    int y = fromY + stepY;
    while (x != toX || y != toY) {
        if (!board->isInBounds(x, y)) return false;
        if (board->GetPiece(x, y) != nullptr) return false;
        x += stepX; 
        y += stepY;
    }
    return true;
}

bool Queen::isValidMove(int fromX, int fromY, int toX, int toY, const Board* board) const {
    if (!board) return false;
    if (!inside(fromX, fromY) || !inside(toX, toY)) return false;

    // Không ăn quân cùng màu
    if (Piece* target = board->GetPiece(toX, toY)) {
        if (target->getColor() == this->getColor()) return false;
    }

    // Hậu = Rook (thẳng) + Bishop (chéo)
    bool straight = isStraight(fromX, fromY, toX, toY);
    bool diag = isDiagonal(fromX, fromY, toX, toY);
    if (!straight && !diag) return false;

    if (!pathClear(fromX, fromY, toX, toY, board)) return false;

    return true;
}

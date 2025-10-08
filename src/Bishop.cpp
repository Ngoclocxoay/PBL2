#include "Bishop.h"
#include "Board.h"
#include <cstdlib>  // std::abs
#include <cassert>

static inline bool inside(int x, int y) { return (0 <= x && x < 8) && (0 <= y && y < 8); }

Bishop::Bishop(Colors c)
    : Piece(PieceType::BISHOP, c)
{}

bool Bishop::isDiagonal(int fromX, int fromY, int toX, int toY) const noexcept {
    int dx = std::abs(toX - fromX);
    int dy = std::abs(toY - fromY);
    return (dx == dy) && dx != 0; // đi chéo và không đứng yên
}

bool Bishop::pathClear(int fromX, int fromY, int toX, int toY, const Board* board) const {
    assert(board);
    int stepX = (toX > fromX) ? 1 : -1;
    int stepY = (toY > fromY) ? 1 : -1;

    int x = fromX + stepX;
    int y = fromY + stepY;
    while (x != toX || y != toY) {
        if (!board->isInBounds(x, y)) return false;
        if (board->GetPiece(x, y) != nullptr) return false;
        x += stepX; y += stepY;
    }
    return true;
}

bool Bishop::isValidMove(int fromX, int fromY, int toX, int toY, const Board* board) const {
    if (!board) return false;
    if (!inside(fromX, fromY) || !inside(toX, toY)) return false;

    // Không ăn quân cùng màu
    if (Piece* target = board->GetPiece(toX, toY)) {
        if (target->getColor() == this->getColor()) return false;
    }

    if (!isDiagonal(fromX, fromY, toX, toY)) return false;
    if (!pathClear(fromX, fromY, toX, toY, board)) return false;

    return true;
}

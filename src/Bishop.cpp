#include "Bishop.h"
#include "Board.h"
//#include <cstdlib>  // std::abs
#include <cassert>

static inline bool inside(int x, int y) { return (0 <= x && x < 8) && (0 <= y && y < 8); }
static inline int ABS(int a)
{
    if (a < 0) return (-1)*a;
    return a;
}

Bishop::Bishop(Colors c)
    : Piece(PieceType::BISHOP, c)
{}

bool Bishop::isDiagonal(int fromX, int fromY, int toX, int toY) const noexcept {
    int dx = ABS(toX - fromX);
    int dy = ABS(toY - fromY);
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

bool Bishop::ignoreKing(int fromX, int fromY, int toX, int toY, const Board* board) const
{
    assert(board);
    int stepX = (toX > fromX) ? 1 : -1;
    int stepY = (toY > fromY) ? 1 : -1;

    int x = fromX + stepX;
    int y = fromY + stepY;
    while (x != toX || y != toY) {
        if (!board->isInBounds(x, y)) return false;
        const Piece* target = board->GetPiece(x, y);
        if (target)
        {
            if (ABS(x - toX) == 1 || ABS(y - toY) == 1)
            {
                if (target->getType() == PieceType::KING && target->getColor() != this->getColor()) break;
            }
            return false;
        }
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

bool Bishop::checkKing(int fromX, int fromY, int toX, int toY, const Board* board) const
{
    if (!board) return false;
    if (!inside(fromX, fromY) || !inside(toX, toY)) return false;

    // Không ăn quân cùng màu
    if (Piece* target = board->GetPiece(toX, toY)) {
        if (target->getColor() == this->getColor()) return false;
    }

    if (!isDiagonal(fromX, fromY, toX, toY)) return false;
    if (!ignoreKing(fromX, fromY, toX, toY, board)) return false;

    return true;
}
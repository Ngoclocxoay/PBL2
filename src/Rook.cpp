#include "Rook.h"
#include "Board.h"
#include <cassert>

static inline bool inside(int x, int y) { return (0 <= x && x < 8) && (0 <= y && y < 8); }

Rook::Rook(Colors c)
    : Piece(PieceType::ROOK, c)
{}

bool Rook::isStraightLine(int fromX, int fromY, int toX, int toY) const noexcept {
    // Đi theo hàng hoặc theo cột (không đứng yên)
    return (fromX == toX && fromY != toY) || (fromY == toY && fromX != toX);
}

bool Rook::pathClear(int fromX, int fromY, int toX, int toY, const Board* board) const {
    assert(board);

    int stepX = (toX > fromX) ? 1 : (toX < fromX ? -1 : 0);
    int stepY = (toY > fromY) ? 1 : (toY < fromY ? -1 : 0);

    int x = fromX + stepX;
    int y = fromY + stepY;

    // dừng trước ô đích
    while (x != toX || y != toY) {
        if (!board->isInBounds(x, y)) return false;
        if (board->GetPiece(x, y) != nullptr) return false; // có quân cản
        x += stepX;
        y += stepY;
    }
    return true;
}

bool Rook::isValidMove(int fromX, int fromY, int toX, int toY, const Board* board) const
{
    if (!board) return false;
    if (!inside(fromX, fromY) || !inside(toX, toY)) return false;

    // Không ăn quân cùng màu
    if (Piece* target = board->GetPiece(toX, toY)) {
        if (target->getColor() == this->getColor()) return false;
    }

    // Phải đi thẳng theo hàng/cột và đường không bị cản
    if (!isStraightLine(fromX, fromY, toX, toY)) return false;
    if (!pathClear(fromX, fromY, toX, toY, board)) return false;


    return true;
}

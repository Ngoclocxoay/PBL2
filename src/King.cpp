#include "King.h"
#include "Board.h"

static inline bool inside(int x, int y) { return (0 <= x && x < 8) && (0 <= y && y < 8);}
static inline int abs(int a) {
    if (a < 0) return (-1)*a;
    return a;
}

King::King(Colors c) 
    : Piece(PieceType::KING, c)
{

}

bool King::isValidMove(int fromX, int fromY, int toX, int toY, const Board* board) const
{
    if (fromX == toX && fromY == toY) return false;
    if (!board || !inside(fromX, fromY) || !inside(toX, toY)) return false;
    Piece* target = board->GetPiece(toX, toY);
    if (target && target->getColor() == this->getColor()) return false;

    int dX = abs(toX - fromX);
    int dY = abs(toY - fromY);

    return (dX <= 1 && dY <= 1);
}
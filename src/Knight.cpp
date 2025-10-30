#include "Knight.h"
#include "Board.h"
#include "library.h"

static inline bool inside(int x, int y) { return (0 <= x && x < 8) && (0 <= y && y < 8);}


Knight::Knight(Colors c)
    : Piece(PieceType::KNIGHT, c)
{

}

bool Knight::isValidMove(int fromX, int fromY, int toX, int toY, const Board* board) const
{
    if (!board || !inside(fromX, fromY) || !inside(toX, toY)) return false;
    if (fromX == toX && fromY == toY) return false;

    const Piece* target = board->GetPiece(toX, toY);
    if (target && target->getColor() == this->getColor()) return false;
    
    int dX = abs(toX - fromX);
    int dY = abs(toY - fromY);

    return ((dX == 2 && dY == 1) || (dX == 1 && dY == 2));    
}
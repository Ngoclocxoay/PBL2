#include "King.h"
#include "Board.h"

static inline bool inside(int x, int y) { return (0 <= x && x < 8) && (0 <= y && y < 8);}

King::King(Colors c)
    : Piece(PieceType::KING, c)
{

}

bool King::isValidMove(int fromX, int fromY, int toX, int toY, const Board* board) const
{
    if (!inside(fromX, fromY) || !inside(toX, toY)) return false;
    Piece* target = board->GetPiece(toX, toY);
    if (target && target->getColor() == this->getColor()) return false;
}
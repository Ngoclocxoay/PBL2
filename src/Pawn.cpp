#include "Pawn.h"
#include "Board.h"

static inline bool inside(int x, int y ) {return (0 <= x && x < 8) && (0 <= y && y < 8);}

Pawn::Pawn(Colors c)
    : Piece(PieceType::PAWN, c)
{

}

bool Pawn::isValidMove(int fromX, int fromY, int toX, int toY, const Board *board) const
{
    if (!board || !inside(fromX, fromY) || !inside(toX, toY)) return false;
    if (fromX == toX && fromY == toY) return false;

    //Chặn ăn quân cùng màu
    const Piece* target = board->GetPiece(toX, toY);
    if (target && target->getColor() == this->getColor()) return false;
    
    int dir = (getColor() == Colors::White) ? -1 : 1; //trang di len, den di xuong
    int start = (getColor() == Colors::White) ? 6 : 1;

    int dx = toX - fromX;
    int dy = toY - fromY;

    //đi thẳng 1 ô
    if (dx == 0 && dy == dir)
    {
        if (!target) return true;
        return false;
    }

    //đi thẳng 2 ô từ ô xuất phát
    if (dx == 0 && fromY == start && dy == 2*dir)
    {
        const Piece* mid = board->GetPiece(fromX, fromY + dir);
        if (!mid && !target) return true;
        return false;
    }

    //ăn chéo
    if ((dx == 1 || dx == -1) && dy == dir)
    {
        if (target && target->getColor() != this->getColor()) return true;
        return false;
    }
    return false;
 }

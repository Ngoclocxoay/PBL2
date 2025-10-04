#include "Knight.h"
#include "Board.h"

static inline bool inside(int x, int y) { return (0 <= x && x < 8) && (0 <= y && y < 8);}
static inline int abs(int a) {
    if (a < 0) return (-1)*a;
    return a;
}

Knight::Knight(Colors c)
    : Piece(PieceType::KNIGHT, c)
{

}

bool Knight::isValidMove(int fromX, int fromY, int toX, int toY, const Board* board) const
{
    if (!board || !inside(fromX, fromY) || !inside(toX, toY)) return false;
    if (fromX == toX || fromY == toY) return false;

    const Piece* target = board->GetPiece(toX, toY);
    if (target && target->getColor() == this->getColor()) return false;
    
    int dirXB[] = {1, 2, 2, 1, -1, -2, -2, -1};
    int dirXW[] = {-1, -2, -2, -1, 1, 2, 2, 1};
    int dirX[8]; 
    int dirYB[] = {2, 1, -1, -2, -2, -1, 1, 2};
    int dirYW[] = {-2, -1, 1, 2, 2, 1, -1, -2};
    int dirY[8]; 
    if (getColor() == Colors::White) 
    {
        for (int i = 0; i < 8; i++)
            dirX[i] = dirXW[i];

    }
    else
    {
        for (int i = 0; i < 8; i++)
            dirX[i] = dirXB[i];
    }

    int index = 0;
    while ((toX == fromX + dirX[index]) && (fromY + dirY[index] == toY))
    {
        index++;
        if (index >= 8) break;
    }

    if (abs(toX - fromX) == 2)
    {
        if (toX > fromX)
        {
            Piece* target1 = board->GetPiece(fromX + 1, fromY);
            Piece* target2 = board->GetPiece(fromX + 2, fromY);
            if (!target1 && !target2) return true;
            return false;
        }
        else
        {
            Piece* target1 = board->GetPiece(fromX - 1, fromY);
            Piece* target2 = board->GetPiece(fromX - 2, fromY);
            if (!target1 && !target2) return true;
            return false;
        }
    }
    else if (abs(toY - fromY) == 2)
    {
        if (toY > fromY)
        {
            Piece* target1 = board->GetPiece(fromX, fromY + 1);
            Piece* target2 = board->GetPiece(fromX, fromY + 2);
            if (!target1 && !target2) return true;
            return false;
        }
        else
        {
            Piece* target1 = board->GetPiece(fromX, fromY - 1);
            Piece* target2 = board->GetPiece(fromX, fromY - 2);
            if (!target1 && !target2) return true;
            return false;
        }
    }
}
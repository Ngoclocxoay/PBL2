#include "Move.h"

Move::Move() 
{
    fromX = -1; fromY = -1;
    toX   = -1; toY   = -1;
    rookfromX = rooktoX = -1;
    captured     = nullptr;
    promotedPawn = nullptr;
}

Move::Move(int fx, int fy, int tx, int ty, Piece* cap)
    : fromX(fx), fromY(fy), toX(tx), toY(ty), captured(cap)
{
    rookfromX = rooktoX = -1;
}

Move::Move(const Move& mv)
    : fromX(mv.fromX), fromY(mv.fromY), toX(mv.toX), toY(mv.toY), captured(mv.captured)
{
    promotedPawn = mv.promotedPawn;
    moverColor   = mv.moverColor;
    moverType    = mv.moverType;
    promoteTo    = mv.promoteTo;
    wasCastling  = mv.wasCastling;
    wasPromote   = mv.wasPromote;
    preHasMoved  = mv.preHasMoved;
    rookfromX    = -1;
    rooktoX      = -1;  
}

void Move::operator=(const Move& mv)
{
    if (this == &mv) return;
    fromX = mv.fromX; fromY = mv.fromY;
    toX   = mv.toX;   toY   = mv.toY;
    captured     = mv.captured;
    promotedPawn = mv.promotedPawn;  
    moverColor   = mv.moverColor;
    moverType    = mv.moverType;
    promoteTo    = mv.promoteTo;
    wasPromote   = mv.wasPromote;
    preHasMoved  = mv.preHasMoved;
    wasCastling  = mv.wasCastling;
    rookfromX    = mv.rookfromX;
    rooktoX      = mv.rooktoX;  
}
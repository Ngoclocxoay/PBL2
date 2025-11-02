#include "Move.h"

Move::Move() 
{
    fromX = -1; fromY = -1;
    toX   = -1; toY   = -1;
    captured = nullptr;
}

Move::Move(int fx, int fy, int tx, int ty, Piece* cap)
    : fromX(fx), fromY(fy), toX(tx), toY(ty), captured(cap)
{
}

Move::Move(const Move& mv)
    : fromX(mv.fromX), fromY(mv.fromY), toX(mv.toX), toY(mv.toY), captured(mv.captured)
{
    moverColor  = mv.moverColor;
    moverType   = mv.moverType;
    promoteTo   = mv.promoteTo;
    wasCastling = mv.wasCastling;
    wasPromote  = mv.wasPromote;
    preHasMoved = mv.preHasMoved;
}

void Move::operator=(const Move& mv)
{
    if (this == &mv) return;
    fromX = mv.fromX; fromY = mv.fromY;
    toX   = mv.toX;   toY   = mv.toY;
    captured      = mv.captured;
    promotedPawn  = mv.promotedPawn;   // nếu anh dùng cho ENGINE
    moverColor    = mv.moverColor;
    moverType     = mv.moverType;
    promoteTo     = mv.promoteTo;
    wasPromote    = mv.wasPromote;
    preHasMoved   = mv.preHasMoved;
    wasCastling   = mv.wasCastling;
}
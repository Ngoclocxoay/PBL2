#pragma once
#include "TextureCache.h"
class Piece;  

struct Move {
    int    fromX, fromY;
    int    toX, toY;
    Piece* captured     = nullptr;  
    Piece* promotedPawn  = nullptr;

    Colors moverColor   = Colors::None;
    PieceType moverType = PieceType::NONE;

    PieceType promoteTo = PieceType::QUEEN;
    bool wasPromote     = false;
    bool preHasMoved    = false;
    bool wasCastling    = false;

    Move();
    Move(int, int, int, int, Piece* = nullptr);    
    Move(const Move&);

    void operator=(const Move&);
};

struct MoveHint { int x, y; bool canCapture;};
#pragma once
#include "TextureCache.h"
class Piece;  

struct Move {
    int fromX, fromY;
    int toX, toY;
    Piece* captured;  
    Piece* promotion;  

    Colors moverColor = Colors::None;
    PieceType moverType = PieceType::NONE;
    PieceType promoteTo = PieceType::QUEEN;
    bool wasPromote = false;
    bool preHasMoved = false;
};

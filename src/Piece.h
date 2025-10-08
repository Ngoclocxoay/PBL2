#pragma once
#include <raylib.h>
#include "constant.hpp"
#include "TextureCache.h"

 class Board; //Forward declaration
//Base class for all piece
class Piece
{
    protected:
        PieceType type;
        Colors color;
        bool hasmoved; // For castling and en passant captureint frromX, int fromY, int toX, int toY, const Board *boardB
    public:
        Piece(PieceType, Colors);
        virtual ~Piece();
        
        virtual bool isValidMove(int, int, int, int, const Board*) const = 0;
        virtual Piece* clone() const = 0;

        //Get method
        PieceType getType() const {return type; }
        Colors getColor() const {return color; }
        bool getMoved() const {return hasmoved; }
        void setHasMoved(bool moved) {hasmoved = moved; }

        void Draw_texture(int, int, const TextureCache&) const; 

};    

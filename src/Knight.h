#pragma once
#include "Piece.h"

class Knight : public Piece
{
    public:
        Knight(Colors);
        
        virtual Piece* clone() const override { return new Knight(*this);}

        virtual bool isValidMove(int, int, int, int, const Board*) const override;
};
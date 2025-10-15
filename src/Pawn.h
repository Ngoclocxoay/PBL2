#pragma once
#include "Piece.h"

class Pawn : public Piece 
{
    public:
        Pawn(Colors);
        virtual Piece* clone() const override { return new Pawn(*this);}

        virtual bool isValidMove(int, int, int, int, const Board*) const override;

        bool killPiece(int, int, int, int, const Board*) const;
};
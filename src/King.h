#pragma once
#include "Piece.h"

class King : public Piece
{
    public:
        King(Colors);

        virtual Piece* clone() const override { return new King(*this);}
        virtual bool isValidMove(int, int, int, int, const Board*) const override;
        
        bool BanMove(int, int, Colors, const Board*) const;
        bool pathClear(int, int, int ,int, const Board*) const;
        bool Castling(int, int, int, int, const Board*) const;
        bool Check(int, int, const Board*) const;
    };
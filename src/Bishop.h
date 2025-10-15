#pragma once
#include "Piece.h"

class Bishop : public Piece
{
public:
    explicit Bishop(Colors);
    virtual ~Bishop() = default;

    virtual Piece* clone() const override { return new Bishop(*this); }
    virtual bool isValidMove(int fromX, int fromY, int toX, int toY, const Board*) const override;
    bool checkKing(int, int, int, int, const Board*) const;

private:
    bool isDiagonal(int fromX, int fromY, int toX, int toY) const noexcept;
    bool pathClear(int fromX, int fromY, int toX, int toY, const Board*) const;
    bool ignoreKing(int, int, int, int, const Board*) const;
};

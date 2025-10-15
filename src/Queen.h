#pragma once
#include "Piece.h"

class Queen : public Piece
{
public:
    explicit Queen(Colors);
    virtual ~Queen() = default;

    virtual Piece* clone() const override { return new Queen(*this); }
    virtual bool isValidMove(int fromX, int fromY, int toX, int toY, const Board*) const override;

    bool checkKing(int, int, int, int,const Board*) const;

private:
    bool isStraight(int fromX, int fromY, int toX, int toY) const noexcept;
    bool isDiagonal(int fromX, int fromY, int toX, int toY) const noexcept;
    bool pathClear(int fromX, int fromY, int toX, int toY, const Board*) const;
    bool ignoreKing(int, int, int, int, const Board*) const;
};

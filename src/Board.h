#pragma once
#include "Piece.h"
#include "TextureCache.h"
#include "Move.h"
#include "Stack.h"
#include "Vector.h"


class Board
{
    private:
        Piece* board[8][8];
        Stack<Move> moveHistory;
    public:
        Board();
        ~Board();

        void Initialize(); //Initial Board

        void DrawBoardBase(int, int) const;
        void DrawHighLight(int, int, int, int,const Vector<MoveHint>&) const;
        void DrawPiece(int, int, const TextureCache&) const;
        void DrawHighLight(int, int, int, int) const;
        void DrawHighLight(int, int, int, int, int, int) const;

        //Piece access method
        Piece* GetPiece(int, int) const;
        bool MakeMove(const Move&);
        void UndoMove();

        //Gia lap nuoc di
        bool MakeMoveEngine(Move&);
        void UndoMoveEngine(const Move&);

        //Valid check
        bool isInBounds(int, int) const;
        bool checkEmpty(int, int) const;

        void ClearHistory();

};


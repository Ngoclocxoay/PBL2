#pragma once
#include "Piece.h"
#include "TextureCache.h"

class Board
{
    private:
        Piece* board[8][8];
    public:
        Board();
        ~Board();

        void Initialize(); //Initial Board

        void DrawBoardBase(int, int) const;
        void DrawHighlight(int, int, int, int) const;
        void DrawPiece(int, int, const TextureCache&) const;

        //Piece access method
        Piece* GetPiece(int, int) const;
       // void SetPiece( int, int,int, int);
        void MovePiece(int, int, int, int);

        //Valid check
        bool isInBounds(int, int) const;
        bool checkEmpty(int, int) const;
};
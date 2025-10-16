#pragma once
#include "Piece.h"
#include "TextureCache.h"
#include <vector>

struct MoveHint { int x, y; bool canCapture;};

struct StateMove
{
    int fromX, fromY;
    int toX, toY;
    Piece* moved;
    Piece* capture;
};

class Board
{
    private:
        Piece* board[8][8];
    public:
        Board();
        ~Board();

        void Initialize(); //Initial Board

        void DrawBoardBase(int, int) const;
        void DrawHighlight(int, int, int, int, const std::vector<MoveHint>&) const;
        void DrawPiece(int, int, const TextureCache&) const;
        void DrawHighLight(int, int, int, int) const;

        //Piece access method
        Piece* GetPiece(int, int) const;
       // void SetPiece( int, int,int, int);
        bool MovePiece(int, int, int, int);

        //Gia lap nuoc di
        bool RawMoveNoSideEffect(int, int, int, int, StateMove*);
        void RawUndoNoSideEffect(const StateMove&);

        //Valid check
        bool isInBounds(int, int) const;
        bool checkEmpty(int, int) const;

};


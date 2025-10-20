#pragma once
#include "Piece.h"
#include "TextureCache.h"
#include "Move.h"
#include <stack>  //TODO : tự build lại sau
#include <vector> //TODO : sau tự build lại

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
        std::stack<Move> moveHistory;
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
        bool MakeMove(const Move&);
        void UndoMove();

        //Gia lap nuoc di
        bool RawMoveNoSideEffect(int, int, int, int, StateMove*);
        void RawUndoNoSideEffect(const StateMove&);

        //Valid check
        bool isInBounds(int, int) const;
        bool checkEmpty(int, int) const;

        

};


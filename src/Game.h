#pragma once
#include "Board.h"
#include "TextureCache.h"

class Game
{
    private:
        Board BOARD;
        TextureCache cache;
        int screenWidth;
        int screenHeight;
        int originX;
        int originY;
        bool gameOver;
        Turn currentTurn;
        Turn winnerTurn;
        int get_horizontal, get_vertical;
        int selX, selY;
        int kingX, kingY;
    public:
        Game(int, int);
        ~Game();

        void Init();

        void ResetGame();

        void Render();

        void Draw_frame() const;

        void Run();

        void HandleInput();

        void findKing();

        bool hasAnyLegalMove() const;

        bool inCheck() const;
        
        bool checkMate () const;
};
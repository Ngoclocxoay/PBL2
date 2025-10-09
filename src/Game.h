#pragma once
#include "Board.h"
#include "TextureCache.h"

enum class Turn { White, Black};

class Game
{
    private:
        Board BOARD;
        TextureCache cache;
        int screenWidth;
        int screenHeight;
        int originX;
        int originY;
        Turn currentTurn;
        int get_horizontal, get_vertical;
        int selX, selY;
    public:
        Game(int, int);
        ~Game();

        void Init();

        void Render();

        void Draw_frame() const;

        void Run();

        void HandleInput();
};
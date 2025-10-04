#include "Game.h"
#include <raylib.h>
#include "constant.hpp"

// const int Size = 640;

Game::Game(int width, int height)
    : screenWidth(width), screenHeight(height), currentTurn(Turn::White), get_horizontal(-1), get_vertical(-1) 
{
    originX = (width - size)/2;
    originY = (height - size)/2;
}

Game::~Game()
{
    cache.Shutdown();
    CloseWindow();
}

void Game::Init()
{
    InitWindow(screenWidth, screenHeight, "Chess Game");
    SetTargetFPS(60);
    cache.Init_texture();
    BOARD.Initialize();
}

void Game::Render()
{
    HandleInput();
    Draw_frame();
}


void Game::Draw_frame() const
{
    BeginDrawing();
    ClearBackground(RAYWHITE);

    
    BOARD.DrawBoardBase(originX, originY);
    BOARD.DrawHighlight(originX, originY, this->get_horizontal, this->get_vertical);
    for (int toX = 0; toX < 8; toX++)
    {
        for (int toY = 0; toY < 8; toY++)
        {   
            int check = BOARD.GetPiece(get_horizontal, get_vertical)->isValidMove(get_horizontal, get_vertical, toX, toY, &BOARD);
            if (check )
            {
                int xi = originX + toX*cell_size;
                int yi = originY + toY*cell_size;
                BOARD.DrawHighlight(originX, originY, get_horizontal, get_vertical);
            }
        }
    }
    BOARD.DrawPiece(originX, originY, cache);
    
    /* const char* turnText = (currentTurn == Turn::White) ? "White's turn" : "Black's turn";
    DrawText(turnText, 10, 10, 20, (Color){0, 0, 0, 255}); */

    EndDrawing();
}

void Game::HandleInput()
{
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        Vector2 mousePos = GetMousePosition();
        int xVal = (mousePos.x - originX)/cell_size;
        int yVal = (mousePos.y - originY)/cell_size;
        //TODO:
            //xu li click chon quan co
        if ((0 <= xVal && xVal < 8) && (0 <= yVal && yVal < 8))
        {
            if ( this->get_horizontal == xVal && this->get_vertical == yVal)
            {
                //Click same cell
                this->get_horizontal = -1;
                this->get_vertical = -1;
            }
            else
            {
                this->get_horizontal = xVal;
                this->get_vertical = yVal;
            }
        }
        
    }
}

void Game::Run()
{
    Init();

    while(!WindowShouldClose())
    {
        Render();
    }
}

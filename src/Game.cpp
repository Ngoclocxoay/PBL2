#include "Game.h"
#include <raylib.h>
#include "constant.hpp"
#include <vector>
using namespace std;


vector<MoveHint> moveHints;

Game::Game(int width, int height)
    : screenWidth(width), screenHeight(height), currentTurn(Turn::White), get_horizontal(-1), get_vertical(-1), selX(-1), selY(-1), kingX(-1), kingY(-1)
{
    originX = (width - Size)/2;
    originY = (height - Size)/2;
    this->gameOver = false;
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

void Game::ResetGame()
{
    BOARD.Initialize();  // reset lại bàn cờ
    currentTurn = Turn::White;
    get_horizontal = get_vertical = -1;
    selX = selY = -1;
    kingX = -1; kingY = -1;
    moveHints.clear();
    gameOver = false;
    winnerTurn = Turn::White;
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
    BOARD.DrawHighlight(originX, originY, this->get_horizontal, this->get_vertical, moveHints);

    Colors turnColor = (currentTurn == Turn::White) ? Colors::White : Colors::Black;
    int xVal, yVal;
    if (isInCheck(turnColor, xVal, yVal))
    {
        
        BOARD.DrawHighLight(originX, originY, xVal, yVal);
    }
    BOARD.DrawPiece(originX, originY, cache);

    //Highlight VUa khi bi chieu
    
    
    const char* turnText = (currentTurn == Turn::White) ? "White's turn" : "Black's turn";
    DrawText(turnText, 10, 10, 20, (Color){0, 0, 0, 255});

    if (gameOver)
    {
        const char* wintext = (winnerTurn == Turn::White) ? "CheckMate - White Win" : "CheckMate - Black Win";

        DrawRectangle(0, 0, screenWidth, screenHeight, Color{0, 0, 0, 120});

        int fontsize = 40;
        int textwidth = MeasureText(wintext, fontsize);
        DrawText(wintext, (screenWidth - textwidth)/2, screenHeight/2 - fontsize, fontsize, Color{255, 255, 255, 255});

        // thêm hướng dẫn restart
        const char* restartText = "Press [R] to start a new game";
        int restartWidth = MeasureText(restartText, 20);
        DrawText(restartText, (screenWidth - restartWidth)/2, screenHeight/2 + 40, 20, WHITE);
    }

    EndDrawing();
}

void Game::HandleInput()
{
    if (gameOver) return;

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        Vector2 mousePos = GetMousePosition();
        int xVal = (mousePos.x - originX)/cell_size;
        int yVal = (mousePos.y - originY)/cell_size;
        if ((0 > xVal || xVal >= 8) || (0 > yVal || yVal >= 8)) return;
        
        //Đã có ô đang chọn -> thử di chuyển / đổi chọn
        if (this->get_horizontal != -1 && this->get_vertical != -1)
        {
            if ( this->get_horizontal == xVal && this->get_vertical == yVal)
            {
                //Click same cell -> cancel
                this->get_horizontal = -1;
                this->get_vertical = -1;
                moveHints.clear();
                return;
            }

           
            //Check nước đi hợp lệ 
            bool legal = false;
            for (const auto& h : moveHints)
            {
                if (h.x == xVal && h.y == yVal) { legal = true; break;}
            }

            if (legal)
            {
                if (BOARD.MovePiece(this->get_horizontal, this->get_vertical, xVal, yVal))
                {
                    //clear state after move piece
                    this->get_horizontal = -1;
                    this->get_vertical = -1;
                    moveHints.clear();

                    currentTurn = (currentTurn == Turn::White) ? Turn::Black : Turn::White;
                    Colors current = (currentTurn == Turn::White) ? Colors::White : Colors::Black;

                    bool flag = checkMate(current);
                    if (flag)
                    {
                        this->gameOver = true;
                        this->winnerTurn = (currentTurn == Turn::White) ? Turn::Black : Turn::White;
                    }
                }
                return;
            }
            else
            {
                //click chọn ô k hợp lê -> nếu ô có quân hợp lệ theo lượt thì chọn lại nêu không tthif huỷ chọn
                const Piece* p = BOARD.GetPiece(xVal, yVal);
                if (p)
                {
                    Colors turnColor = (currentTurn == Turn::White) ? Colors::White : Colors::Black;
                    if (turnColor != p->getColor()) return;
                    //chon lai quan moi va build hint moi
                    this->get_horizontal = xVal;
                    this->get_vertical   = yVal;
                    moveHints.clear();
                    for (int toX = 0; toX < 8; toX++)
                    {
                        for (int toY = 0; toY < 8; toY++)
                        {
                            if (p->isValidMove(xVal, yVal, toX, toY, &BOARD))
                            {   
                                StateMove st;
                                if (!BOARD.RawMoveNoSideEffect(xVal, yVal, toX, toY, &st)) continue;

                                bool ok = isInCheck(turnColor);

                                BOARD.RawUndoNoSideEffect(st);
                                
                                if (!ok)
                                {
                                    const Piece* target = BOARD.GetPiece(toX, toY);
                                    bool cap = (target && target->getColor() != p->getColor());
                                    moveHints.push_back({toX, toY, cap});
                                }
                            }
                        }
                    }
                }
                else
                {
                    this->get_horizontal = -1;
                    this->get_vertical   = -1;
                    moveHints.clear();
                }
                return;
            }
            
        }

        //chua co o dang chon -> chon quan & build Hint
        moveHints.clear();
        const Piece* selected = BOARD.GetPiece(xVal, yVal);
        if (selected)
        {
            //check lượt đi
            Colors turnColor = (currentTurn == Turn::White) ? Colors::White : Colors::Black;
            if (turnColor != selected->getColor()) return;

            this->get_horizontal = xVal;
            this->get_vertical = yVal;

            for (int toX = 0; toX < 8; toX++)
            {
                for (int toY = 0; toY < 8; toY++)
                {
                    if (selected->isValidMove(xVal, yVal, toX, toY, &BOARD))
                    {
                        StateMove st;

                        if (!BOARD.RawMoveNoSideEffect(xVal, yVal, toX, toY, &st)) continue;

                        bool ok = isInCheck(turnColor);

                        BOARD.RawUndoNoSideEffect(st);

                        if (!ok)
                        {
                            const Piece* target = BOARD.GetPiece(toX, toY);
                            bool canCapture;
                            if (target && target->getColor() != selected->getColor()) canCapture = true;
                            else canCapture = false;
                            moveHints.push_back({toX, toY, canCapture});
                        }
                    }       
                } 
            }
        }
        else
        {
            this->get_horizontal = -1;
            this->get_vertical   = -1;
        }        
    }
}

void Game::Run()
{
    Init();

    while(!WindowShouldClose())
    {
        Render();

        if (gameOver && IsKeyPressed(KEY_R))
        {
            ResetGame();
        }
    }
}



bool Game::isInCheck(Colors side) const
{   
    int kX = -1;
    int kY = -1;

    for (int x = 0; x < 8; x++)
        for (int y = 0; y < 8; y++)
        {
            const Piece* tmp = BOARD.GetPiece(x, y);
            if (tmp && tmp->getType() == PieceType::KING && tmp->getColor() == side)
            {
                kX = x;
                kY = y;
                goto foundKing;
            }
        }
    foundKing:
    if (kX == -1 && kY == -1) return false;

    for (int fromX = 0; fromX < 8; fromX++)
        for (int fromY = 0; fromY < 8; fromY++)
        {
            const Piece* start = BOARD.GetPiece(fromX, fromY);
            if (start && start->getColor() != side)
            {
                bool check = start->isValidMove(fromX, fromY, kX, kY, &BOARD);
                if (check) 
                {
                    return true;
                }
            }
        }
    return false;
}

bool Game::isInCheck(Colors side, int& outX, int& outY) const
{
    int kX = -1;
    int kY = -1;

    for (int x = 0; x < 8; x++)
        for (int y = 0; y < 8; y++)
        {
            const Piece* tmp = BOARD.GetPiece(x, y);
            if (tmp && tmp->getType() == PieceType::KING && tmp->getColor() == side)
            {
                kX = x;
                kY = y;
                goto foundKing;
            }
        }
    foundKing:
    if (kX == -1 && kY == -1) return false;
    outX = kX;
    outY = kY;

    for (int fromX = 0; fromX < 8; fromX++)
        for (int fromY = 0; fromY < 8; fromY++)
        {
            const Piece* start = BOARD.GetPiece(fromX, fromY);
            if (start && start->getColor() != side)
            {
                bool check = start->isValidMove(fromX, fromY, kX, kY, &BOARD);
                if (check) 
                {
                    return true;
                }
            }
        }
    return false;
}

bool Game::hasAnyLegalMove(Colors side) 
{
    

    for (int fromX = 0; fromX < 8; fromX++)
        for (int fromY = 0; fromY < 8; fromY++)
        {
            const Piece* temp = BOARD.GetPiece(fromX, fromY);
            if (temp && temp->getColor() == side)
            {
                for (int toX = 0; toX < 8; toX++)
                    for (int toY = 0; toY < 8; toY++)
                    {
                        if (temp->isValidMove(fromX, fromY, toX, toY, &BOARD))
                        {
                            StateMove st;

                            if (!BOARD.RawMoveNoSideEffect(fromX, fromY, toX, toY, &st)) continue;

                            bool ok = isInCheck(side);

                            BOARD.RawUndoNoSideEffect(st);

                            if (!ok) return true;
                        }
                    }
            }
        }
    return false;
}

bool Game::checkMate(Colors side) 
{
    if (isInCheck(side) && hasAnyLegalMove(side) == false) return true;
    return false;
}
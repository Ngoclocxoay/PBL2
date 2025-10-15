#include "Game.h"
#include <raylib.h>
#include "constant.hpp"
#include <vector>
using namespace std;


vector<MoveHint> moveHints;

Game::Game(int width, int height)
    : screenWidth(width), screenHeight(height), currentTurn(Turn::White), get_horizontal(-1), get_vertical(-1), selX(-1), selY(-1)
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
    kingX = kingY = -1;
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
    if (inCheck())
    {
        BOARD.DrawHighLight(originX, originY, kingX, kingY);
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
                    
                    //Tìm vị trí của vua mỗi lần đi xog lượt
                    findKing();

                    bool flag = checkMate();
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
                                const Piece* target = BOARD.GetPiece(toX, toY);
                                bool cap = (target && target->getColor() != p->getColor());
                                moveHints.push_back({toX, toY, cap});
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
                        const Piece* target = BOARD.GetPiece(toX, toY);
                        bool canCapture;
                        if (target && target->getColor() != selected->getColor()) canCapture = true;
                        else canCapture = false;
                        moveHints.push_back({toX, toY, canCapture});
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

void Game::findKing()
{
    kingX = -1;
    kingY = -1;
    Colors current = (currentTurn == Turn::White) ? Colors::White : Colors::Black;
    
    for (int x = 0; x < 8; x++)
        for (int y = 0; y < 8; y++)
        {
            const Piece* target = BOARD.GetPiece(x, y);       
            if (target && target->getType() == PieceType::KING && target->getColor() == current) 
            {
                kingX = x;
                kingY = y;
                return;
            }
        }
}

bool Game::inCheck() const 
{   
    Colors current = (currentTurn == Turn::White) ? Colors::White : Colors::Black;

    for (int fromX = 0; fromX < 8; fromX++)
        for (int fromY = 0; fromY < 8; fromY++)
        {
            const Piece* start = BOARD.GetPiece(fromX, fromY);
            if (start && start->getColor() != current)
            {
                bool check = start->isValidMove(fromX, fromY, kingX, kingY, &BOARD);
                if (check) 
                {
                    return true;
                }
            }
        }
    return false;
}

bool Game::hasAnyLegalMove() const
{
    const Piece* king = BOARD.GetPiece(kingX, kingY);
    
    int dirX[] = {0, 1, 1, 1, 0, -1, -1, -1};
    int dirY[] = {1, 1, 0, -1, -1, -1, 0, 1};

    for (int i = 0; i < 8; i++)
    {
        if (king->isValidMove(kingX, kingY, kingX + dirX[i], kingY + dirY[i], &BOARD)) return true;
    }
    return false;
}

bool Game::checkMate() const
{
    if (inCheck() && hasAnyLegalMove() == false) return true;
    return false;
}
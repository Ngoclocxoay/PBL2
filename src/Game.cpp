#include "Game.h"
#include <raylib.h>
#include "constant.hpp"
#include <cmath> //TODO : kiem tra neu tu build ddc thi lam
#include <algorithm> //TODO : tu build lai
#include <string>
#include "Vector.h"


using namespace std;

static inline Side TurnToSide(Turn t)
{
    return (t == Turn::White) ? Side::White : Side::Black;
}

Vector<MoveHint> moveHints;

Game::Game(int width, int height)
    : screenWidth(width), screenHeight(height), currentTurn(Turn::White), get_horizontal(-1), get_vertical(-1), selX(-1), selY(-1), kingX(-1), kingY(-1)
{
    if (Size % 8 != 0) {
        TraceLog(LOG_WARNING, "Size (%d) not divisible by 8 -> pixels may look soft. Set Size to multiple of 8 in constant.hpp.", Size);
    }
    originX = (width - Size)/2;
    originY = (height - Size)/2;
    this->gameOver = false;

    ai.depth = 5; // 2 - 4 cho mượt
    state = GameState::Splash;
}

Game::~Game()
{
    //Unload Background
    if (bgSplash.id) UnloadTexture(bgSplash);
    if (bgMenu.id)   UnloadTexture(bgMenu);

    //Unload Font
    if (fontRegular.texture.id) UnloadFont(fontRegular);
    if (fontBold.texture.id)    UnloadFont(fontBold);

    cache.Shutdown();
    CloseWindow();
}

void Game::Init()
{

    SetConfigFlags(FLAG_WINDOW_HIGHDPI);
    InitWindow(screenWidth, screenHeight, "Chess Game");
    SetWindowState(FLAG_VSYNC_HINT);
    SetTargetFPS(60);

    SetTextureFilter(GetFontDefault().texture, TEXTURE_FILTER_POINT);

    cache.Init_texture();
    BOARD.Initialize();

    // ==== Load custom fonts ==== 
    fontRegular = LoadFont("asset/fonts/Inter_28pt-Regular.ttf");
    fontBold    = LoadFont("asset/fonts/Inter_18pt-Bold.ttf");

    if (fontRegular.texture.id == 0 || fontBold.texture.id == 0) {
        TraceLog(LOG_WARNING, "Failed to load Inter fonts! Using default font instead.");
    } else {
        SetTextureFilter(fontRegular.texture, TEXTURE_FILTER_BILINEAR);
        SetTextureFilter(fontBold.texture,    TEXTURE_FILTER_BILINEAR);
    }

    // Load backgrounds (đúng thư mục 'asset')
    bgSplash = LoadTexture("asset/background/chessboard_background.jpg");
    bgMenu   = LoadTexture("asset/background/chessgame_menu.jpg");
}

void Game::ResetGame()
{
    BOARD.Initialize();  // reset lại bàn cờ
    BOARD.ClearHistory();
    currentTurn = Turn::White;
    get_horizontal = get_vertical = -1;
    fX = fY = -1;
    tX = tY = -1;
    selX = selY = -1;
    kingX = -1; kingY = -1;
    moveHints.clear();
    gameOver = false;
    winnerTurn = Turn::White;
    plyCount = 0; //Quan trong de Undo an toan
    
    whiteTimeLeft = matchTimeSeconds;
    blackTimeLeft = matchTimeSeconds;
    timersEnabled = true;
    playerIsWhite = true;
}

void Game::UpdateTimers(float dt)
{
    if (!timersEnabled || state != GameState::Playing || gameOver) return;

    if (mode == GameMode::PvC)
    {
        if (playerIsWhite && currentTurn == Turn::White)
        {
            whiteTimeLeft -= dt;
            if (whiteTimeLeft <= 0.0f) { whiteTimeLeft = 0; gameOver = true; winnerTurn = Turn::Black; }
        }
        else if (!playerIsWhite && currentTurn == Turn::Black)
        {
            blackTimeLeft -= dt;
            if (blackTimeLeft <= 0.0f) { blackTimeLeft = 0; gameOver = true; winnerTurn = Turn::White; }
        }
    }
    else // PvP
    {
        if (currentTurn == Turn::White)
        {
            whiteTimeLeft -= dt;
            if (whiteTimeLeft <= 0.0f) { whiteTimeLeft = 0; gameOver = true; winnerTurn = Turn::Black; }
        }
        else
        {
            blackTimeLeft -= dt;
            if (blackTimeLeft <= 0.0f) { blackTimeLeft = 0; gameOver = true; winnerTurn = Turn::White; }
        }
    }
}

static std::string formatTime(float sec)
{
    if (sec < 0) sec = 0;
    int m = (int)(sec / 60);
    int s = (int)fmod(sec, 60);
    char buf[16];
    sprintf(buf, "%02d:%02d", m, s);
    return buf;
}

void Game::DrawTimers() const
{
    int cx = originX + Size / 2;
    int topY = originY - 60;
    int bottomY = originY + Size + 10;
    int w = 260, h = 48;
    Color bg = {0, 0, 0, 150};

    const Font& bigFont = (fontBold.texture.id) ? fontBold : GetFontDefault();
    const Font& smallFont = (fontRegular.texture.id) ? fontRegular : GetFontDefault();

    float bigSize = 22.0f;   
    float smallSize = 16.0f; 

    auto drawCenteredText = [&](const Font& f, const std::string& text, float centerX, float y, float size) {
        Vector2 txtSz = MeasureTextEx(f, text.c_str(), size, 1.0f);
        Vector2 pos = { centerX - txtSz.x/2.0f, y + (h - txtSz.y)/2.0f };
        DrawTextEx(f, text.c_str(), pos, size, 1.0f, WHITE);
    };

    if (mode == GameMode::PvP)
    {
        Rectangle topRect = { (float)(cx - w/2), (float)topY, (float)w, (float)h };
        Rectangle botRect = { (float)(cx - w/2), (float)bottomY, (float)w, (float)h };

        DrawRectangleRec(topRect, bg);
        DrawRectangleRec(botRect, bg);

        std::string blackText = "Black: " + formatTime(blackTimeLeft);
        std::string whiteText = "White: " + formatTime(whiteTimeLeft);

        drawCenteredText(bigFont, blackText, (float)cx, (float)topY, bigSize);
        drawCenteredText(bigFont, whiteText, (float)cx, (float)bottomY, bigSize);
    }
    else // PvC: chỉ vẽ đồng hồ người chơi ở dưới
    {
        Rectangle botRect = { (float)(cx - w/2), (float)bottomY, (float)w, (float)h };
        DrawRectangleRec(botRect, bg);

        float t = playerIsWhite ? whiteTimeLeft : blackTimeLeft;
        std::string playerText = "Player: " + formatTime(t);

        drawCenteredText(bigFont, playerText, (float)cx, (float)bottomY, bigSize);
    }
}

void Game::Render()
{   
    BeginDrawing();
    ClearBackground(RAYWHITE);
    UpdateTimers(GetFrameTime());

    // === Phím nhanh: Reset & Undo khi đang chơi ===
    if (state == GameState::Playing) 
    {
        if (bgMenu.id) {
            DrawTexturePro(bgMenu,
                {0,0,(float)bgMenu.width,(float)bgMenu.height},
                {0,0,(float)screenWidth,(float)screenHeight},
                {0,0}, 0.0f, WHITE);
        }
        
        float btnW = 44.0f;
        float btnH = 44.0f;
        float gap  = 10.0f;

        //Căn cột nút: sát bên trái bàn cờ
        float columX = originX - (btnW + gap);
        if (columX < 10.0f) columX = 10.0f; // nếu cửa sổ nhỏ quá thì neo vào lề

        // Căn giữa theo chiều dọc quanh bàn cờ
        float totalH = btnH * 3 + gap * 2;
        float startY = originY + (Size - totalH) * 0.5f;


        Rectangle restartBtn = {columX, startY + 0*(btnH + gap), btnW, btnH};
        Rectangle undoBtn    = {columX, startY + 1*(btnH + gap), btnW, btnH};
        Rectangle backBtn    = {columX, startY + 2*(btnH + gap), btnW, btnH};

        
        int iconSize = 28;
        bool uiClick = false;

        if (Button(restartBtn, "R", iconSize))
        {
            uiClick = true;
            ResetGame();
        }
        if (Button(undoBtn, "U", iconSize))
        {
            uiClick = true;
            UndoPlayerLast();
        }
        if (Button(backBtn, "B", iconSize))
        {
            uiClick = true;
            state = GameState::ModeSelect;
            ResetGame();
        }
        if (!uiClick)
        {
            HandleInput();
        }
        Draw_frame();
        EndDrawing();
        
        if (!gameOver && aiEnabled && currentTurn == Turn::Black)  
        {
            DoAIMove();
        }
        return;
    }

    if (state == GameState::Splash) 
    {
        if (bgSplash.id) {
            //Phong to ch
            DrawTexturePro(bgSplash,
                {0,0,(float)bgSplash.width,(float)bgSplash.height},
                {0,0,(float)screenWidth,(float)screenHeight},
                {0,0}, 0.0f, WHITE);
        }
        Rectangle btn = { (float)screenWidth*0.5f - 120, (float)screenHeight*0.75f - 30, 240, 60 };
        if (Button(btn, "Play Game", 36)) {
            state = GameState::ModeSelect;
        }
        EndDrawing();
        return;
    }

    // ====== TimeSelect (NEW) ======
    if (state == GameState::TimeSelect)
    {
        if (bgMenu.id) {
            DrawTexturePro(bgMenu,
                {0,0,(float)bgMenu.width,(float)bgMenu.height},
                {0,0,(float)screenWidth,(float)screenHeight},
                {0,0}, 0.0f, WHITE);
        }

        const char* title = "Select Mode";
        if (fontBold.texture.id)
            DrawTextEx(fontBold, title, {(float)screenWidth*0.5f - MeasureText(title, 28)/2.0f, (float)screenHeight*0.25f}, 28.0f, 1.0f, BLACK);
        else
            DrawText(title, screenWidth/2 - MeasureText(title, 28)/2, screenHeight/4, 28, BLACK);

        Rectangle ba_x = { (float)screenWidth*0.5f - 150, (float)screenHeight*0.45f - 30, 300, 60 };
        Rectangle ba_y  = { (float)screenWidth*0.5f - 150, (float)screenHeight*0.6f  - 30, 300, 60 };

        if (Button(ba_x, "Standard", 26))
        {
            matchTimeSeconds = 20.0f * 60.0f;
            ResetGame();
            state = GameState::Playing;
        }
        if (Button(ba_y, "Blitz", 26))
        {
            matchTimeSeconds = 10.0f * 60.0f;
            ResetGame();
            state = GameState::Playing;
        }

        // Draw a small hint to go back
        Rectangle backRect = {10, 10, 120, 40};
        if (Button(backRect, "Back", 20))
        {
            state = GameState::ModeSelect;
        }

        EndDrawing();
        return;
    }

    if (state == GameState::ModeSelect) 
    {
        if (bgMenu.id) {
            DrawTexturePro(bgMenu,
                {0,0,(float)bgMenu.width,(float)bgMenu.height},
                {0,0,(float)screenWidth,(float)screenHeight},
                {0,0}, 0.0f, WHITE);
        }

        Rectangle btn1 = { (float)screenWidth*0.5f - 260, (float)screenHeight*0.75f - 30, 220, 60 };
        Rectangle btn2 = { (float)screenWidth*0.5f +  40, (float)screenHeight*0.75f - 30, 220, 60 };

        // Instead of directly starting the game, go to TimeSelect so player can choose 20'/10'
        if (Button(btn1, "PvsP", 32)) StartGame(GameMode::PvP);
        if (Button(btn2, "PvsC", 32)) StartGame(GameMode::PvC);

        EndDrawing();
        return;
    }

    // ====== Playing ======
    HandleInput();
    Draw_frame();
    EndDrawing();

    if (!gameOver && aiEnabled && currentTurn == Turn::Black) 
    {
        DoAIMove();
    }
}

void Game::Run()
{
    Init();

    while(!WindowShouldClose())
    {
        Render();

        if (state == GameState::Playing && gameOver && IsKeyPressed(KEY_R))
        {
            ResetGame();
        }
    }
}

void Game::Draw_frame() const
{
    ClearBackground(RAYWHITE);
    BOARD.DrawBoardBase(originX, originY);
    BOARD.DrawHighLight(originX, originY, this->get_horizontal, this->get_vertical, moveHints);

    Colors turnColor = (currentTurn == Turn::White) ? Colors::White : Colors::Black;
    if (this->fX != -1 && this->fY != -1 && this->tX != -1 && this->tY != -1)
    {
        const Piece* fP = BOARD.GetPiece(fX, fY);
        const Piece* tP = BOARD.GetPiece(tX, tY);
        if ((fP && tP) && fP->getColor() == tP->getColor())
            BOARD.DrawHighLight(originX, originY, fX, fY, tX, tY);
        else if (!fP && tP)
            BOARD.DrawHighLight(originX, originY, fX, fY, tX, tY);
    }
    int xVal, yVal;
    if (isInCheck(turnColor, xVal, yVal))
    {
        
        BOARD.DrawHighLight(originX, originY, xVal, yVal);
    }
    BOARD.DrawPiece(originX, originY, cache);

   
    if (gameOver)
    {
        const char* wintext = (winnerTurn == Turn::White) ? "CheckMate - White Win" : "CheckMate - Black Win";
        DrawRectangle(0, 0, screenWidth, screenHeight, Color{0, 0, 0, 120});

        int fontsize = 40;
        
        if (fontBold.texture.id) {
            Vector2 sz = MeasureTextEx(fontBold, wintext, (float)fontsize, 1.0f);
            Vector2 pos = { (screenWidth - sz.x)/2.0f, (screenHeight/2.0f - fontsize) };
            DrawTextEx(fontBold, wintext, pos, (float)fontsize, 1.0f, WHITE);
        } else {
            int textwidth = MeasureText(wintext, fontsize);
            DrawText(wintext, (screenWidth - textwidth)/2, screenHeight/2 - fontsize, fontsize, WHITE);
        }

        const char* restartText = "Press [R] to start a new game";
        if (fontRegular.texture.id) {
            Vector2 rsz = MeasureTextEx(fontRegular, restartText, 20.0f, 1.0f);
            Vector2 rpos = { (screenWidth - rsz.x)/2.0f, (screenHeight/2.0f + 40.0f) };
            DrawTextEx(fontRegular, restartText, rpos, 20.0f, 1.0f, WHITE);
        } else {
            int restartWidth = MeasureText(restartText, 20);
            DrawText(restartText, (screenWidth - restartWidth)/2, screenHeight/2 + 40, 20, WHITE);
        }
    }
    DrawTimers();
}


/* ====== Helpers ====== */

void Game::HandleInput()
{   
    if (state != GameState::Playing) return;
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
            //Thực hiện di chuyển
            if (legal)
            {   
                Move mv{this->get_horizontal, this->get_vertical, xVal, yVal};
                this->tX = xVal;
                this->tY = yVal;
                if (BOARD.MakeMove(mv))
                {   
                    plyCount++;

                    //clear state after move piece
                    this->fX = this->get_horizontal;
                    this->fY = this->get_vertical;
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
                                Move st{xVal, yVal, toX, toY};
                                if (!BOARD.MakeMoveEngine(st)) continue;

                                bool ok = isInCheck(turnColor);

                                BOARD.UndoMoveEngine(st); /// Chú ý thay lại 
                                
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
            this->get_vertical   = yVal;

            for (int toX = 0; toX < 8; toX++)
            {
                for (int toY = 0; toY < 8; toY++)
                {
                    if (selected->isValidMove(xVal, yVal, toX, toY, &BOARD))
                    {
                        Move st{xVal, yVal, toX, toY};

                        if (!BOARD.MakeMoveEngine(st)) continue;

                        bool ok = isInCheck(turnColor);

                        BOARD.UndoMoveEngine(st);

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
            const Piece* attacker = BOARD.GetPiece(fromX, fromY);
            if (attacker && attacker->getColor() != side)
            {
                bool check = attacker->isValidMove(fromX, fromY, kX, kY, &BOARD);
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
                            Move st{fromX, fromY, toX, toY};

                            if (!BOARD.MakeMoveEngine(st)) continue;

                            bool ok = isInCheck(side);

                            BOARD.UndoMoveEngine(st);

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

bool Game::Button(const Rectangle& r, const char* text, int fontSize) const
{
    Vector2 mPos = GetMousePosition();
    bool hover = CheckCollisionPointRec(mPos, r);

    Color fill   = hover ? Color{255,255,255,180} : Color{255,255,255,140};
    Color border = BLACK;

    DrawRectangleRounded(r, 0.2f, 8, fill);
    DrawRectangleRoundedLines(r, 0.2f, 8, border);

    // dùng fontBold + MeasureTextEx để căn giữa chuẩn
    Vector2 tsize = (fontBold.texture.id)
        ? MeasureTextEx(fontBold, text, (float)fontSize, 1.0f)
        : Vector2{(float)MeasureText(text, fontSize), (float)fontSize};

    Vector2 pos = { r.x + (r.width  - tsize.x)/2.0f,
                    r.y + (r.height - tsize.y)/2.0f };

    if (fontBold.texture.id)
        DrawTextEx(fontBold, text, pos, (float)fontSize, 1.0f, BLACK);
    else
        DrawText(text, (int)pos.x, (int)pos.y, fontSize, BLACK);

    return hover && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
}

void Game::StartGame(GameMode m)
{
    mode = m;
    aiEnabled = (mode == GameMode::PvC); // PvP tắt AI, PvC bật AI
    state = GameState::TimeSelect;
}

void Game::DoAIMove()
{
    Side sideToMove = TurnToSide(currentTurn);

    Move best;
    if (ai.FindBestMove(BOARD, sideToMove, best)) {
        this->fX = best.fromX;
        this->fY = best.fromY;
        this->tX = best.toX;
        this->tY = best.toY;
        if (BOARD.MakeMove(best)) {  
            plyCount++;              
            get_horizontal = get_vertical = -1;
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
    } else {
        this->gameOver = true;
        this->winnerTurn = (currentTurn == Turn::White) ? Turn::Black : Turn::White;
    }
}

void Game::UndoPlayerLast()
{
    if (state != GameState::Playing) return;
    if (plyCount <= 0) return;

    gameOver = false;
    get_horizontal = get_vertical = -1;
    fX = fY = tX = tY = -1;
    moveHints.clear();

    if (mode == GameMode::PvC) 
    {
        if (currentTurn == Turn::Black) 
        {
            BOARD.UndoMove();
            plyCount--;
            currentTurn = Turn::White;
        } 
        else 
        {
            if (plyCount >= 1) 
            {
                BOARD.UndoMove(); // undo AI
                plyCount--;
                currentTurn = Turn::Black;
            }
            if (plyCount >= 1) 
            {
                BOARD.UndoMove(); // undo Người
                plyCount--;
                currentTurn = Turn::White;
            }
        }
    } 
    else 
    {
        BOARD.UndoMove();
        plyCount--;
        currentTurn = (currentTurn == Turn::White) ? Turn::Black : Turn::White;
    }
}

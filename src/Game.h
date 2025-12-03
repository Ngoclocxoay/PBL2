#pragma once
#include "Board.h"
#include "TextureCache.h"
#include "Ai.h"
#include "constant.hpp"
#include <raylib.h>


enum class GameState {Splash, ModeSelect, TimeSelect, Playing};
enum class GameMode {PvP, PvC};

class Game
{
    private:
        // match time setting (seconds). Default 15 minutes.
        float matchTimeSeconds = 15.0f * 60.0f;

        float whiteTimeLeft = 15.0f * 60.0f; 
        float blackTimeLeft = 15.0f * 60.0f; 
        bool playerIsWhite = true;
        bool timersEnabled = true;
        
        //Core Game
        int plyCount = 0;
        Board BOARD;
        TextureCache cache;

        Font fontRegular;
        Font fontBold;
        int screenWidth;
        int screenHeight;
        int originX;
        int originY;

        //Game state
        bool gameOver;
        Turn currentTurn;
        Turn winnerTurn;
        int get_horizontal, get_vertical;
        int selX, selY;
        int kingX, kingY;
        int fX, fY, tX, tY;


        // ==========Menu / Background =========
        GameState state = GameState::Splash;
        GameMode mode = GameMode::PvP;
        Texture2D bgSplash{};
        Texture2D bgMenu{};

        ChessAI ai;
        bool aiEnabled = true;

        
        // ====== Helpers ======
        void Draw_frame() const;
        void HandleInput();
        bool hasAnyLegalMove(Colors);
        bool isInCheck(Colors) const;
        bool isInCheck(Colors, int&, int&) const;
        bool checkMate (Colors);
        void DoAIMove();
        bool Button(const Rectangle&, const char*, int = 28) const;
        void StartGame(GameMode);      // now transitions to TimeSelect
        void UndoPlayerLast();
    public:
        Game(int, int);
        ~Game();

        void Init();

        void ResetGame();

        void Render();
        
        void Run();

        // time
        void UpdateTimers(float dt);
        void DrawTimers() const;
};

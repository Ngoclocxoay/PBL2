#include "Board.h"
#include "library.h"
#include <raylib.h>
#include "constant.hpp"
#include "All_piece.h"
#include "Game.h"



// const int square_size = 80;
// const int count = 8;
// const int Size = 640;

Board::Board()
{
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8;  j++)
        {
            board[i][j] = nullptr;
        }
    }
}   

Board::~Board()
{
    for (int i = 0; i< 8; i++)
        for (int j = 0; j < 8; j++)
        {
            delete board[i][j];
           // board[i][j] = nullptr;
        }
}



void Board::Initialize()
{
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
        {
            if (board[i][j]) delete board[i][j];
            board[i][j] = nullptr;
        }

    //Back rank - Black
    board[0][0] = new Rook(Colors::Black);
    board[1][0] = new Knight(Colors::Black);
    board[2][0] = new Bishop(Colors::Black);
    board[3][0] = new Queen(Colors::Black);
    board[4][0] = new King(Colors::Black);
    board[5][0] = new Bishop(Colors::Black);
    board[6][0] = new Knight(Colors::Black);
    board[7][0] = new Rook(Colors::Black);

    //Pawns - Black
    for (int i = 0; i < 8; i++)
        board [i][1] = new Pawn(Colors::Black);

    //Back rank - White
    board[0][7] = new Rook(Colors::White);
    board[1][7] = new Knight(Colors::White);
    board[2][7] = new Bishop(Colors::White);
    board[3][7] = new Queen(Colors::White);
    board[4][7] = new King(Colors::White);
    board[5][7] = new Bishop(Colors::White);
    board[6][7] = new Knight(Colors::White);
    board[7][7] = new Rook(Colors::White);

    //Pawns - White
    for (int i = 0; i < 8; i++)
        board [i][6] = new Pawn(Colors::White);
}

Piece* Board::GetPiece(int x, int y) const
{
    if (!isInBounds(x, y)) return nullptr;
    return board[x][y];
}

void Board::MovePiece(int fromX, int fromY, int toX, int toY)
{
    //Double check
    if(!isInBounds(toX, toY) || !isInBounds(fromX, fromY)) return;
    Piece* mover = board[fromX][fromY];
    if (!mover) return;
    if (fromX == toX && fromY == toY) return;

    if (board[toX][toY] && board[toX][toY]->getColor() == mover->getColor()) return;

    if (board[toX][toY]) { delete board[toX][toY]; }

    board[toX][toY] = mover;
    board[fromX][fromY] = nullptr;
    board[toX][toY]->setHasMoved(true);
    /* if (!board[toX][toY] || !board[fromX][fromY]) return;

    swap(board[toX][toY], board[fromX][fromY]);
    board[toX][toY]->setHasMoved(true); */
}

bool Board::isInBounds(int x, int y) const
{
    return (x >= 0 && x < 8) && (y >= 0 && y < 8);
}

bool Board::checkEmpty(int x, int y) const
{
    return !board[x][y];
}

/* DRAW */

void Board::DrawBoardBase(int truc_ngang, int truc_doc) const
{
    Color LightWood = Color{240, 217, 181, 255};
    Color DarkWood = Color{181, 136, 99, 255};

    for (int row = 0; row < count; row++)
    {
        for (int collum = 0; collum < count; collum++)
        {
            Color currentColor = ((row + collum) % 2 == 0) ? LightWood : DarkWood;
            int xi, yi;
            xi = truc_ngang + row*cell_size;
            yi = truc_doc + collum*cell_size;
            DrawRectangle(xi, yi, cell_size, cell_size, currentColor); //vẽ ô đặc
        }
    }
    DrawRectangleLines(truc_ngang, truc_doc, Size, Size, BLACK); //Vẽ viền mỏng
}

void Board::DrawHighlight(int originX, int originY, int getX, int getY, const std::vector<MoveHint>& moveHints) const
{
    for (int row = 0; row < count; row++)
    {
        for (int collum = 0; collum < count; collum++)
        {   
            int xi = originX + row*cell_size;
            int yi = originY + collum*cell_size;
            if (row == getX && collum == getY)
            {
                DrawRectangleLinesEx( {(float)xi, (float)yi, (float)cell_size, (float)cell_size}, 4, RED); //Highlight 1 ô với độ dày tự thiết lập
            }
        }
    }
    for (const auto& hint : moveHints)
    {
        int xi = originX + hint.x*cell_size;
        int yi = originY + hint.y*cell_size;
        if (hint.canCapture)
            DrawRectangleLinesEx( {(float)xi, (float)yi, (float)cell_size, (float)cell_size}, 4, RED);
        else
            DrawRectangleLinesEx( {(float)xi, (float)yi, (float)cell_size, (float)cell_size}, 4, GREEN);
    }
}

void Board::DrawPiece(int truc_ngang, int truc_doc, const TextureCache& cache) const
{
    for (int y = 0; y < 8; y++)
        for (int x = 0; x < 8; x++)
        {
            const Piece* target = GetPiece(x, y);
            if (!target) continue;
            //const Piece* p = GetPiece(x, y);
            int posX = truc_ngang + x*cell_size;
            int posY = truc_doc + y*cell_size;
            target->Draw_texture(posX, posY, cache);
        }
}

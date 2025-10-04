#include "Board.h"
#include "library.h"
#include <raylib.h>
#include "constant.hpp"
#include "All_piece.h"


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
            delete board[i][j];
}

/* void Board::Initialize()
{
    // 1) Clear toàn bộ bàn về ô trống
    for (int y = 0; y < 8; ++y) {
        for (int x = 0; x < 8; ++x) {
            *(*(board + x) + y) = Piece(PieceType::NONE, Colors::None);
        }
    }

    // 2) Helper xếp hàng
    auto placeBackRank = [&](int y, Colors color) {
        PieceType order[8] = {
            PieceType::ROOK, PieceType::KNIGHT, PieceType::BISHOP,
            PieceType::QUEEN, PieceType::KING,
            PieceType::BISHOP, PieceType::KNIGHT, PieceType::ROOK
        };
        for (int x = 0; x < 8; ++x) {
            *(*(board + x) + y) = Piece(order[x], color);
        }
    };

    auto placePawns = [&](int y, Colors color) {
        for (int x = 0; x < 8; ++x) {
            *(*(board + x) + y) = Piece(PieceType::PAWN, color);
        }
    };

    // 3) Đen ở trên, Trắng ở dưới
    placeBackRank(0, Colors::Black);
    placePawns(1,     Colors::Black);

    placePawns(6,     Colors::White);
    placeBackRank(7, Colors::White);
}

 */

void Board::Initialize()
{
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
            board[i][j] = nullptr;

    //Back rank - Black
    board[0][0] = new ROOK(Colors::Black);
    //
    //
    //
    //
    //
    //
    //

    //Pawns - Black
    for (int i = 0; i < 8; i++)
        board [i][1] = new Pawn(Colors::Black);

    //Back rank - White
    board[0][7] = new ROOK(Colors::White);
    //
    //
    //
    //
    //
    //
    //

    //Pawns - White
    for (int i = 0; i < 8; i++)
        board [i][6] = new Pawn(Colors::White);
}

Piece* Board::GetPiece(int x, int y) const
{
    if (!isInBounds(x, y)) return nullptr;
    return board[x][y];
}

void Board::MovePiece(int toX, int toY, int fromX, int fromY)
{
    //Double check
    if(!isInBounds(toX, toY) || !isInBounds(fromX, fromY)) return;
    if(board[toX][toY]->getType() == PieceType::NONE) return; 

    swap(board[toX][toY], board[fromX][fromY]);
    board[toX][toY]->setHasMoved(true);
}

bool Board::isInBounds(int x, int y) const
{
    return (x >= 0 && x < 8) && (y >= 0 && y < 8);
}

bool Board::checkEmpty(int x, int y) const
{
    return board[x][y]->getType() == PieceType::NONE;
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
    DrawRectangleLines(truc_ngang, truc_doc, size, size, BLACK); //Vẽ viền mỏng
}

void Board::DrawHighlight(int truc_ngang, int truc_doc, int getX, int getY) const
{
    for (int row = 0; row < count; row++)
    {
        for (int collum = 0; collum < count; collum++)
        {   
            int xi = truc_ngang + row*cell_size;
            int yi = truc_doc + collum*cell_size;
            if (row == getX && collum == getY)
            {
                DrawRectangleLinesEx( {(float)xi, (float)yi, (float)cell_size, (float)cell_size}, 4, RED); //Highlight 1 ô với độ dày tự thiết lập
            }
        }
    }
}

void Board::DrawPiece(int truc_ngang, int truc_doc, const TextureCache& cache) const
{
    /* TODO:
        liên kết với vẽ quân cờ bên lớp Piece */
        for (int y = 0; y < 8; y++)
            for (int x = 0; x < 8; x++)
            {
                const Piece* p = GetPiece(x, y);
                int posX = truc_ngang + x*cell_size;
                int posY = truc_doc + y*cell_size;
                p->Draw_texture(posX, posY, cache);
            }
}

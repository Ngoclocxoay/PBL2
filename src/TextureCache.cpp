#include "TextureCache.h"
#include <iostream>
using namespace std;

TextureCache::TextureCache()
    : piece_mng{}, none{}
{
}

void TextureCache::Init_texture()
{
    //BLACK
    this->piece_mng[0][0] = LoadTexture("asset/Piece/black_king.png");
    this->piece_mng[0][1] = LoadTexture("asset/Piece/black_queen.png");
    this->piece_mng[0][2] = LoadTexture("asset/Piece/black_bishop.png");
    this->piece_mng[0][3] = LoadTexture("asset/Piece/black_knight.png");
    this->piece_mng[0][4] = LoadTexture("asset/Piece/black_rook.png");
    this->piece_mng[0][5] = LoadTexture("asset/Piece/black_pawn.png");

    //WHITE
    this->piece_mng[1][0] = LoadTexture("asset/Piece/white_king.png");
    this->piece_mng[1][1] = LoadTexture("asset/Piece/white_queen.png");
    this->piece_mng[1][2] = LoadTexture("asset/Piece/white_bishop.png");
    this->piece_mng[1][3] = LoadTexture("asset/Piece/white_knight.png");
    this->piece_mng[1][4] = LoadTexture("asset/Piece/white_rook.png");
    this->piece_mng[1][5] = LoadTexture("asset/Piece/white_pawn.png");
}

void TextureCache::Shutdown()
{
    for (int i = 0; i < 2; i++)
        for (int j = 0; j < 6; j++)
            UnloadTexture(this->piece_mng[i][j]);
}

const Texture2D& TextureCache::GetPiece(PieceType type, Colors color) const
{
    if (this->piece_mng[(int)color][(int)type].id == 0)
    {
        cout << "Texture (" << (int)color << ", " << (int)type << ") is not found!" << endl;
        return none;
    }
    return piece_mng[int(color)][int(type)];
}
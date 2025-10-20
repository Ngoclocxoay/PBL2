#include "Piece.h"
#include "Board.h"
#include <iostream>
using namespace std;

Piece::Piece(PieceType t, Colors c)
    : type(t), color(c), hasmoved(false)
{
}


Piece::~Piece()
{
}


void Piece::Draw_texture(int x, int y, const TextureCache &cache) const
{
    const Texture2D& tex = cache.GetPiece(type, color);
    if (tex.id == 0) return ;
    float scale = (float)cell_size / tex.width; //scale theo chieu ngang o
    DrawTextureEx(tex, {(float)x, (float)y}, 0.0f, scale, WHITE);

}

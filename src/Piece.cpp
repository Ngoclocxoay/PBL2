#include "Piece.h"
#include "Board.h"
#include <iostream>
using namespace std;

Piece::Piece(PieceType t, Colors c)
    : type(t), color(c), hasmoved(false)
{
}

Piece::Piece(const Piece &p)
    : type(p.type), color(p.color), hasmoved(p.hasmoved)
{
}

Piece::~Piece()
{
}

bool Piece::isValidMove(int fromX, int fromY, int toX, int toY, const Board *board) const
{
    /* if (!board) return false;
    Piece *targetPiece = board->GetPiece(toX, toY);
    if ( targetPiece->getColor() == this->color) return false;
    return false; */
    return true;
}

void Piece::Draw_texture(int x, int y, const TextureCache &cache) const
{
    /* CODE */
    if (type == PieceType::NONE) return;
    const Texture2D& tex = cache.GetPiece(type, color);
    if (tex.id == 0) return ;
    float scale = (float)cell_size / tex.width; //scale theo chieu ngang o
    DrawTextureEx(tex, {(float)x, (float)y}, 0.0f, scale, WHITE);

}

#pragma once
#include <raylib.h>

enum class PieceType { KING, QUEEN, BISHOP, KNIGHT, ROOK, PAWN, NONE };
enum class Colors { Black, White, None};
enum class Turn { White, Black};

struct TextureCache{
    Texture2D piece_mng[2][6];
    Texture2D none;

    //Construction
    TextureCache();
    void Init_texture();

    void Shutdown();

    const Texture2D& GetPiece(PieceType type, Colors color) const;
};


#include "TextureCache.h"
#include <iostream>
using namespace std;

// Load + set filter BILINEAR (mượt như code 2)
static Texture2D LoadPieceBL(const char* path) {
    Image img = LoadImage(path);
    if (!img.data) {
        TraceLog(LOG_ERROR, "❌ LoadImage failed: %s (cwd=%s)", path, GetWorkingDirectory());
        return Texture2D{};
    }
    Texture2D tex = LoadTextureFromImage(img);
    UnloadImage(img);

    // Mượt khi scale
    SetTextureFilter(tex, TEXTURE_FILTER_BILINEAR);
    return tex;
}

TextureCache::TextureCache()
    : piece_mng{}, none{}
{
}

TextureCache::~TextureCache()
{
}

void TextureCache::Init_texture()
{
    //BLACK
    this->piece_mng[0][0] = LoadPieceBL("asset/Piece/black_king.png");
    this->piece_mng[0][1] = LoadPieceBL("asset/Piece/black_queen.png");
    this->piece_mng[0][2] = LoadPieceBL("asset/Piece/black_bishop.png");
    this->piece_mng[0][3] = LoadPieceBL("asset/Piece/black_knight.png");
    this->piece_mng[0][4] = LoadPieceBL("asset/Piece/black_rook.png");
    this->piece_mng[0][5] = LoadPieceBL("asset/Piece/black_pawn.png");

    //WHITE
    this->piece_mng[1][0] = LoadPieceBL("asset/Piece/white_king.png");
    this->piece_mng[1][1] = LoadPieceBL("asset/Piece/white_queen.png");
    this->piece_mng[1][2] = LoadPieceBL("asset/Piece/white_bishop.png");
    this->piece_mng[1][3] = LoadPieceBL("asset/Piece/white_knight.png");
    this->piece_mng[1][4] = LoadPieceBL("asset/Piece/white_rook.png");
    this->piece_mng[1][5] = LoadPieceBL("asset/Piece/white_pawn.png");

    // fallback none (1x1)
    if (none.id == 0) {
        Image im = GenImageColor(1, 1, BLANK);
        none = LoadTextureFromImage(im);
        UnloadImage(im);
        SetTextureFilter(none, TEXTURE_FILTER_BILINEAR);
    }
}

void TextureCache::Shutdown()
{
     for (int i = 0; i < 2; i++)
        for (int j = 0; j < 6; j++)
            if (piece_mng[i][j].id) UnloadTexture(piece_mng[i][j]);
    if (none.id) UnloadTexture(none);
}

const Texture2D& TextureCache::GetPiece(PieceType type, Colors color) const
{
    
    const Texture2D& t = piece_mng[(int)color][(int)type];
    if (t.id == 0) {
        std::cout << "Texture (" << (int)color << ", " << (int)type << ") is not found!\n";
        return none;
    }
    return t;
}
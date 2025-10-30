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

bool Board::isInBounds(int x, int y) const
{
    return (x >= 0 && x < 8) && (y >= 0 && y < 8);
}

bool Board::checkEmpty(int x, int y) const
{
    return !board[x][y];
}


bool Board::MakeMove(const Move& m) {
    if (!isInBounds(m.fromX, m.fromY) || !isInBounds(m.toX, m.toY)) return false;

    Piece* mover = board[m.fromX][m.fromY];
    if (!mover) return false;
    if (m.fromX == m.toX && m.fromY == m.toY) return false;
    if (board[m.toX][m.toY] && board[m.toX][m.toY]->getColor() == mover->getColor()) return false;
    

    //Backup
    Move rec        = m; //FromX, fromY, toX, toY
    rec.moverColor  = mover->getColor();
    rec.moverType   = mover->getType();
    rec.wasPromote  = false;
    rec.wasCastling = false;
    rec.captured    = board[m.toX][m.toY];   
    rec.preHasMoved = mover->getHasMoved();

    //Promote
    if (mover->getType() == PieceType::PAWN && (m.toY == 0 || m.toY == 7))
    {   
        
        board[m.toX][m.toY]     = mover;
        board[m.fromX][m.fromY] = nullptr;
        board[m.toX][m.toY] = new Queen(rec.moverColor);
        rec.wasPromote      = true;
        moveHistory.push(rec);
        return true;
    }
    //Castling
    else if (mover->getType() == PieceType::KING)
    {
        int dX = abs(m.fromX - m.toX);
        int dY = abs(m.fromY - m.toY);

        if (dY == 0 && dX == 2)
        {
            const Piece* rook = (rec.toX > rec.fromX) ? board[7][rec.toY] : board[0][rec.toY];
            if (!rook) return false;
            if (rook->getType() != PieceType::ROOK) return false;
            board[m.toX][m.toY] = mover;
            if (rec.fromX < rec.toX)
            {
                //King side
                board[7][rec.toY]->setHasMoved(true);
                board[5][rec.toY] = board[7][rec.toY];
                board[7][rec.toY] = nullptr;
            }
            else 
            {
                //Queen side
                board[0][rec.toY]->setHasMoved(true);
                board[3][rec.toY] = board[0][rec.toY];
                board[0][rec.toY] = nullptr;
            }
            board[m.fromX][m.fromY] = nullptr;
            mover->setHasMoved(true);
            rec.wasCastling     = true;
        }
        else
        {
            board[m.toX][m.toY] = mover;
            board[m.fromX][m.fromY] = nullptr;
            mover->setHasMoved(true);
        }      
    }
    else
    {
        board[m.toX][m.toY] = mover;
        board[m.fromX][m.fromY] = nullptr;
        mover->setHasMoved(true);
        
    }
    
    
    
    moveHistory.push(rec);
    return true;

}

void Board::UndoMove() 
{
    if (moveHistory.empty()) return;
    Move last = moveHistory.top();
    moveHistory.pop();

    if (last.wasPromote)
    {
        delete board[last.toX][last.toY];
        board[last.toX][last.toY]     = last.captured;
        board[last.fromX][last.fromY] = new Pawn(last.moverColor);
        board[last.fromX][last.fromY]->setHasMoved(last.preHasMoved);

    }
    else if (last.wasCastling)
    {
        board[last.fromX][last.fromY] = board[last.toX][last.toY];
        board[last.fromX][last.fromY]->setHasMoved(false);
        board[last.toX][last.toY]     = nullptr;
        if (last.toX > last.fromX)
        {
            board[7][last.toY] = board[5][last.toY];
            board[5][last.toY] = nullptr;
            board[7][last.toY]->setHasMoved(false);
        }
        else
        {
            board[0][last.toY] = board[3][last.toY];
            board[3][last.toY] = nullptr;
            board[0][last.toY]->setHasMoved(false);
        }
    }
    else
    {
        Piece* mover                  = board[last.toX][last.toY];
        if (mover) mover->setHasMoved(last.preHasMoved);
        board[last.fromX][last.fromY] = mover;
        board[last.toX][last.toY]     = last.captured;
    }
}


bool Board::MakeMoveEngine(Move& state)
{
    state.wasCastling = false;
    state.wasPromote  = false;

    Piece* mover         = board[state.fromX][state.fromY];
    const Piece* capture = board[state.toX][state.toY]; 
    if (!mover) return false;
    if (state.fromX == state.toX && state.fromY == state.toY) return false;
    if (capture && capture->getColor() == mover->getColor()) return false;


    // save backup
    state.captured   = board[state.toX][state.toY];    
    

    //Promote
    if (mover->getType() == PieceType::PAWN && (state.toY == 0 || state.toY == 7))
    {   
        board[state.toX][state.toY] = mover;
        board[state.fromX][state.fromY] = nullptr;
        mover->setType(PieceType::QUEEN);
        state.wasPromote = true;
        return true;
    }
    //Castling
    else if (mover->getType() == PieceType::KING)
    {
        int dX = abs(state.fromX - state.toX);
        int dY = abs(state.fromY - state.toY);

        if (dY == 0 && dX == 2)
        {
            const Piece* rook = (state.toX > state.fromX) ? board[7][state.toY] : board[0][state.toY];
            if (!rook) return false;
            if (rook->getType() != PieceType::ROOK) return false;
            board[state.toX][state.toY]     = mover;
            board[state.fromX][state.fromY] = nullptr;
            state.wasCastling = true;
            if (state.toX > state.fromX) 
            {
                //king side
                board[5][state.toY] = board[7][state.toY];
                board[7][state.toY] = nullptr;
            }
            else 
            {
                //queen side
                board[3][state.toY] = board[0][state.toY];
                board[0][state.toY] = nullptr;
            }
        }
        else
        {
            //Vua di chuyen binh thuong
            board[state.toX][state.toY]     = mover;
            board[state.fromX][state.fromY] = nullptr;
        }
    }

    else
    {
        board[state.toX][state.toY] = mover;
        board[state.fromX][state.fromY] = nullptr;
    }
    
    return true;
}

void Board::UndoMoveEngine(const Move& state)
{
    if (state.wasPromote)
    {
        board[state.fromX][state.fromY] = board[state.toX][state.toY];
        board[state.fromX][state.fromY]->setType(PieceType::PAWN);
        board[state.toX][state.toY]     = state.captured;
    }
    else if (state.wasCastling)
    {   
        if (state.toX > state.fromX)
        {
            board[state.fromX][state.fromY] = board[state.toX][state.toY];
            board[state.toX][state.toY]     = nullptr;
            board[7][state.toY]             = board[5][state.toY];
            board[5][state.toY]             = nullptr;
        }
        else
        {
            board[state.fromX][state.fromY] = board[state.toX][state.toY];
            board[state.toX][state.toY]     = nullptr;
            board[0][state.toY]             = board[3][state.toY];
            board[3][state.toY]             = nullptr;
        }
    }
    else
    {
        board[state.fromX][state.fromY] = board[state.toX][state.toY];
        board[state.toX][state.toY]     = state.captured;
    }
}




/* DRAW */

void Board::DrawBoardBase(int truc_ngang, int truc_doc) const
{
    Color LightWood = Color{240, 217, 181, 255};
    Color DarkWood  = Color{181, 136, 99, 255};

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
    if (getX == -1 && getY == -1) return;
    int xi = originX + getX * cell_size;
    int yi = originY + getY * cell_size;
    DrawRectangleLinesEx( {(float)xi, (float)yi, (float)cell_size, (float)cell_size}, 4, BLUE); //Highlight 1 ô với độ dày tự thiết lập

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

void Board::DrawHighLight(int originX, int originY, int getX, int getY) const
{
    int xi = originX + getX * cell_size;
    int yi = originY + getY * cell_size;
    DrawRectangleLinesEx({(float)xi, (float)yi, (float)cell_size, (float)cell_size}, 4, RED);
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



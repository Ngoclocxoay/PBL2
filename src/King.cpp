#include "King.h"
#include "Board.h"
#include "All_piece.h"
#include "library.h"
static inline bool inside(int x, int y) { return (0 <= x && x < 8) && (0 <= y && y < 8);}


King::King(Colors c) 
    : Piece(PieceType::KING, c)
{

}

bool King::isValidMove(int fromX, int fromY, int toX, int toY, const Board* board) const
{
    if (fromX == toX && fromY == toY) return false;
    if (!board || !inside(fromX, fromY) || !inside(toX, toY)) return false;
    const Piece* target = board->GetPiece(toX, toY);
    if (target && target->getColor() == this->getColor()) return false;

    int dX = abs(toX - fromX);
    int dY = abs(toY - fromY);

    //Castling
    if (!this->getHasMoved() && dY == 0 && dX == 2)
    {
        if (Castling(fromX, fromY, toX, toY, board)) return true;   
    }

    if (dX <= 1 && dY <= 1) return BanMove(toX, toY, this->getColor(), board);
    return false;
}

bool King::pathClear(int fromX, int fromY, int toX, int toY, const Board* board) const
{
    if (fromX < toX)
    {
        for (int x = fromX + 1 ; x <= toX; x++)
        {
            const Piece* tmp = board->GetPiece(x, toY);
            if (tmp) return false; //Check duong vua di co trong khong
            if (!BanMove(x, fromY, this->getColor(), board)) return false; //check duong vua di co bi chieu khong
        }
    }
    else
    {
        for (int x = fromX - 1; x >= toX; x--)
        {
            const Piece* tmp = board->GetPiece(x, fromY);
            if (tmp) return false;
            if (!BanMove(x, fromY, this->getColor(), board)) return false;
        }
    }
    return true;
}


//Kiem tra vi tri cua vua co bi chieu hay khong
bool King::Check(int fromX, int fromY, const Board* board) const
{
    for (int x = 0; x < 8; x++)
        for (int y = 0; y < 8; y++)
        {
            const Piece* attacker = board->GetPiece(x, y);
            if (!attacker || attacker->getType() == PieceType::KING) continue;
            if (attacker->getColor() != this->getColor())
            {
                if (attacker->isValidMove(x, y, fromX, fromY, board)) return true;
            }
        }
    return false;
}

bool King::Castling(int fromX, int fromY, int toX, int toY, const Board* board) const
{
    if (this->getHasMoved()) return false;

    if (Check(fromX, fromY, board)) return false;
    if (!pathClear(fromX, fromY, toX, toY, board)) return false;
    //Check cell b(1) in queenside
    if (toX < fromX)
    {
        if (board->GetPiece(1, toY) != nullptr) return false;
    }
    
    const Piece* target;
    if (fromX < toX) target = board->GetPiece(7, toY);
    else target = board->GetPiece(0, toY);

    if (target)
    {
        if (target->getType() == PieceType::ROOK && target->getColor() == this->getColor())
        {
            if (!target->getHasMoved()) return true;
        }
    }
    return false;
}

bool King::BanMove(int toX, int toY, Colors currentKing, const Board* board) const
{
    for (int fromX = 0; fromX < 8; fromX++)
        for (int fromY = 0; fromY < 8; fromY++)
        {
            const Piece* attacker = board->GetPiece(fromX, fromY);
            if (attacker && attacker->getType() != PieceType::KING && attacker->getColor() != currentKing) 
            {
                if (attacker->getType() == PieceType::PAWN)
                {
                    const Pawn* atker = dynamic_cast<const Pawn*>(attacker);
                    if (atker->killPiece(fromX, fromY, toX, toY, board)) return false;
                }
                else if (attacker->getType() == PieceType::ROOK)
                {
                    const Rook* atker = dynamic_cast<const Rook*>(attacker);
                    if (atker->checkKing(fromX, fromY, toX, toY, board)) return false;
                }
                else if (attacker->getType() == PieceType::QUEEN)
                {
                    const Queen* atker = dynamic_cast<const Queen*>(attacker);
                    if (atker->checkKing(fromX, fromY, toX, toY, board)) return false;
                }
                else if (attacker->getType() == PieceType::BISHOP)
                {
                    const Bishop* atker = dynamic_cast<const Bishop*>(attacker);
                    if (atker->checkKing(fromX, fromY, toX, toY, board)) return false;    
                }
                else
                {
                    if (attacker->isValidMove(fromX, fromY, toX, toY, board)) return false;
                }
            }
        }
    return true;
}
#include "King.h"
#include "Board.h"
#include "All_piece.h"

static inline bool inside(int x, int y) { return (0 <= x && x < 8) && (0 <= y && y < 8);}
static inline int abs(int a) {
    if (a < 0) return (-1)*a;
    return a;
}

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

    if (dX <= 1 && dY <= 1) return BanMove(toX, toY, this->getColor(), board);
    return false;
}

bool King::BanMove(int toX, int toY, Colors currentKing, const Board* board) const
{
    for (int fromX = 0; fromX < 8; fromX++)
        for (int fromY = 0; fromY < 8; fromY++)
        {
            const Piece* start = board->GetPiece(fromX, fromY);
            if (start && start->getType() != PieceType::KING && start->getColor() != currentKing) 
            {
                if (start->getType() == PieceType::PAWN)
                {
                    const Pawn* temp = dynamic_cast<const Pawn*>(start);
                    if (temp->killPiece(fromX, fromY, toX, toY, board)) return false;
                }
                else if (start->getType() == PieceType::ROOK)
                {
                    const Rook* temp = dynamic_cast<const Rook*>(start);
                    if (temp->checkKing(fromX, fromY, toX, toY, board)) return false;
                }
                else if (start->getType() == PieceType::QUEEN)
                {
                    const Queen* temp = dynamic_cast<const Queen*>(start);
                    if (temp->checkKing(fromX, fromY, toX, toY, board)) return false;
                }
                else if (start->getType() == PieceType::BISHOP)
                {
                    const Bishop* temp = dynamic_cast<const Bishop*>(start);
                    if (temp->checkKing(fromX, fromY, toX, toY, board)) return false;    
                }
                else
                {
                    if (start->isValidMove(fromX, fromY, toX, toY, board)) return false;
                }
            }
        }
    return true;
}
#pragma once
#include "Piece.h"

class Rook : public Piece
{
public:
    explicit Rook(Colors);
    virtual ~Rook() = default;

    // Clone động đúng kiểu Rook
    virtual Piece* clone() const override { return new Rook(*this); }

    // Kiểm tra nước đi hợp lệ của Xe
    virtual bool isValidMove(int fromX, int fromY, int toX, int toY, const Board*) const override;

    // --- Trạng thái phục vụ nhập thành ---
    bool hasMoved() const noexcept { return moved_; }
    void setMoved(bool moved) noexcept { moved_ = moved; }
    void markMoved() noexcept { moved_ = true; }

private:
    // Đi thẳng theo hàng hoặc cột?
    bool isStraightLine(int fromX, int fromY, int toX, int toY) const noexcept;

    // Đường đi có thông suốt không (không tính ô đích)?
    bool pathClear(int fromX, int fromY, int toX, int toY, const Board*) const;

private:
    bool moved_ = false; // mặc định: chưa di chuyển
};

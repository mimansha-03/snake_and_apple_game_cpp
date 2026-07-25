#pragma once
#include "Common.h"
#include <deque>
#include <vector>

class Snake {
public:
    explicit Snake(Position start);

    void setDirection(Direction d);
    [[nodiscard]] Direction getDirection() const { return direction_; }

    // Advances the snake one cell in its current direction. Snapshots the
    // pre-move body into prevBody() first, so the renderer can smoothly
    // interpolate between the two instead of snapping cell-to-cell.
    // If `grow` is true the tail is not popped (snake gets longer).
    void move(bool grow);

    // --- Concept satisfaction (see Common.h) -----------------------------------
    [[nodiscard]] Position getPosition() const { return body_.front(); } // head
    [[nodiscard]] const std::vector<Position>& occupiedCells() const { return cellsCache_; }
    // ---------------------------------------------------------------------------

    [[nodiscard]] bool hasSelfCollision() const;
    [[nodiscard]] size_t length() const { return body_.size(); }

    // Full body / previous-frame body, head-first; used by the renderer to
    // draw each segment and to interpolate motion between logic ticks.
    [[nodiscard]] const std::deque<Position>& body() const { return body_; }
    [[nodiscard]] const std::deque<Position>& prevBody() const { return prevBody_; }

private:
    std::deque<Position> body_;
    std::deque<Position> prevBody_;
    std::vector<Position> cellsCache_;
    Direction direction_{Direction::Right};
    Direction pendingDirection_{Direction::Right};

    void syncCache();
};

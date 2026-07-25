#include "Snake.h"
#include <algorithm>

Snake::Snake(Position start) {
    // Start with a body of length 3, extending to the left of `start`.
    body_.push_back(start);
    body_.push_back(Position{start.x - 1, start.y});
    body_.push_back(Position{start.x - 2, start.y});
    prevBody_ = body_;
    syncCache();
}

void Snake::setDirection(Direction d) {
    if (d == Direction::None) return;

    // Prevent the snake from reversing directly into itself.
    const bool opposite =
        (d == Direction::Up    && direction_ == Direction::Down)  ||
        (d == Direction::Down  && direction_ == Direction::Up)    ||
        (d == Direction::Left  && direction_ == Direction::Right) ||
        (d == Direction::Right && direction_ == Direction::Left);

    if (!opposite) {
        pendingDirection_ = d;
    }
}

void Snake::move(bool grow) {
    prevBody_ = body_;

    direction_ = pendingDirection_;
    Position head = body_.front();

    switch (direction_) {
        case Direction::Up:    head.y -= 1; break;
        case Direction::Down:  head.y += 1; break;
        case Direction::Left:  head.x -= 1; break;
        case Direction::Right: head.x += 1; break;
        default: break;
    }

    body_.push_front(head);
    if (!grow) {
        body_.pop_back();
    }
    syncCache();
}

bool Snake::hasSelfCollision() const {
    const Position& head = body_.front();
    // Skip the head itself when checking against the rest of the body.
    return std::any_of(body_.begin() + 1, body_.end(), [&](const Position& p) {
        return p == head;
    });
}

void Snake::syncCache() {
    cellsCache_.assign(body_.begin(), body_.end());
}

#pragma once
// -----------------------------------------------------------------------------
// Common.h
// Shared primitives + the C++20 "concepts" used to constrain generic code.
// -----------------------------------------------------------------------------
#include <concepts>
#include <vector>
#include <type_traits>

struct Position {
    int x{0};
    int y{0};
    bool operator==(const Position&) const = default;
};

enum class Direction { Up, Down, Left, Right, None };

// ---------------------------------------------------------------------------
// C++20 Concepts
// ---------------------------------------------------------------------------
// A type is Positionable if it can report a single grid Position.
// Both Snake (head) and Apple satisfy this.
template<typename T>
concept Positionable = requires(const T& t) {
    { t.getPosition() } -> std::same_as<Position>;
};

// A type is MultiCell if it can report *every* cell it occupies.
// Only Snake satisfies this (its whole body), used by Game to keep the
// apple from spawning on top of the snake without Apple ever needing to
// know what a Snake is.
template<typename T>
concept MultiCell = requires(const T& t) {
    { t.occupiedCells() } -> std::convertible_to<const std::vector<Position>&>;
};

// Generic collision test, usable for ANY two Positionable game objects
// (e.g. snake head vs apple) without writing type-specific overloads.
template<Positionable A, Positionable B>
[[nodiscard]] inline bool collides(const A& a, const B& b) {
    return a.getPosition() == b.getPosition();
}

// A minimal Positionable adapter for a bare grid coordinate that isn't a
// full game object - e.g. a hypothetical "next head" position - so it can
// be passed anywhere a Positionable is expected, including collides<>().
struct PositionProbe {
    Position pos;
    [[nodiscard]] Position getPosition() const { return pos; }
};

// Generic "is position occupied by this multi-cell entity" check.
template<MultiCell T>
[[nodiscard]] inline bool occupies(const T& entity, Position p) {
    const auto& cells = entity.occupiedCells();
    for (const auto& c : cells) {
        if (c == p) return true;
    }
    return false;
}

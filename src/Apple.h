#pragma once
#include "Common.h"
#include "raylib.h"
#include <vector>

class Apple {
public:
    Apple();

    // Places the apple at a random free cell strictly inside the playfield
    // border ([1, width-2] x [1, height-2]), avoiding every cell in `forbidden`.
    void respawn(int width, int height, const std::vector<Position>& forbidden);

    // --- Concept satisfaction (see Common.h) -----------------------------------
    [[nodiscard]] Position getPosition() const { return position_; }
    // ---------------------------------------------------------------------------

    // A touch of visual variety: each apple gets a very slightly different
    // shade of red, and remembers when it spawned so the renderer can play
    // a brief pop-in animation.
    [[nodiscard]] Color getColor() const { return color_; }
    [[nodiscard]] double getSpawnTime() const { return spawnTime_; }

private:
    Position position_{};
    Color color_{ 214, 40, 40, 255 };
    double spawnTime_{0.0};
};

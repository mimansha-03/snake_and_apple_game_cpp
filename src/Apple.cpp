#include "Apple.h"
#include <random>
#include <algorithm>

namespace {
    std::mt19937& rng() {
        static std::mt19937 engine(std::random_device{}());
        return engine;
    }
}

Apple::Apple() {
    spawnTime_ = GetTime();
}

void Apple::respawn(int width, int height, const std::vector<Position>& forbidden) {
    // Cells 0 and (width-1)/(height-1) are the wall border - never spawn there.
    std::uniform_int_distribution<int> distX(1, width - 2);
    std::uniform_int_distribution<int> distY(1, height - 2);

    Position candidate{};
    bool clashes = true;
    int attempts = 0;
    constexpr int kMaxAttempts = 100;

    while (clashes && attempts < kMaxAttempts) {
        candidate = Position{distX(rng()), distY(rng())};
        clashes = std::any_of(forbidden.begin(), forbidden.end(),
                               [&](const Position& p) { return p == candidate; });
        ++attempts;
    }

    position_ = candidate;
    spawnTime_ = GetTime();

    // Small random variation in shade so apples don't look perfectly identical.
    std::uniform_int_distribution<int> shadeJitter(-14, 14);
    int r = std::clamp(214 + shadeJitter(rng()), 185, 235);
    color_ = Color{ static_cast<unsigned char>(r), 40, 40, 255 };
}

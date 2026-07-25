#pragma once
#include "Common.h"
#include "Snake.h"
#include "Apple.h"
#include <string>

// Renders the game with raylib: a checkerboard playfield, rounded/segmented
// snake with directional eyes, a glossy animated apple, and dedicated
// title/pause/game-over screens. All drawing logic lives here so game logic
// (Game/Snake/Apple) never touches raylib directly.
class Renderer {
public:
    static constexpr int kCellSize  = 32;
    static constexpr int kMargin    = 20;
    static constexpr int kHudHeight = 64;

    static int windowWidth(int gridCols)  { return gridCols * kCellSize + kMargin * 2; }
    static int windowHeight(int gridRows) { return gridRows * kCellSize + kMargin * 2 + kHudHeight; }

    Renderer(int gridWidth, int gridHeight);

    // alpha in [0,1]: how far between the previous tick and the current tick
    // we are, used to smoothly interpolate the snake's on-screen position
    // instead of snapping it cell-to-cell.
    void drawFrame(const Snake& snake, const Apple& apple,
                   int score, int highScore, bool paused, double alpha) const;

    void drawGameOver(int score, int highScore, bool newHighScore) const;

private:
    int gridWidth_;
    int gridHeight_;

    [[nodiscard]] Vector2 cellTopLeft(Position p) const;
    [[nodiscard]] Vector2 cellCenter(Position p) const;
    [[nodiscard]] Vector2 lerpPos(Position from, Position to, double alpha) const;

    void drawBoard() const;
    void drawHud(int score, int highScore, bool paused) const;
    void drawSnake(const Snake& snake, double alpha) const;
    void drawApple(const Apple& apple) const;
};

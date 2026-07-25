#include "Renderer.h"
#include "raylib.h"
#include "raymath.h"
#include <cmath>
#include <cstdio>

namespace {
    constexpr Color kBgColor      = Color{18, 20, 28, 255};
    constexpr Color kTileLight = Color{32, 90, 55, 255};
    constexpr Color kTileDark  = Color{24, 70, 40, 255};
    constexpr Color kBoardBorder  = Color{0, 255, 255, 255};
    constexpr Color kHudBg        = Color{14, 16, 22, 255};
    constexpr Color kTextPrimary  = Color{240, 240, 245, 255};
    constexpr Color kTextMuted    = Color{150, 155, 165, 255};
    constexpr Color kScoreColor   = Color{255, 209, 102, 255};
    constexpr Color kHighColor    = Color{201, 145, 255, 255};
    constexpr Color kSnakeBody    = Color{80, 255, 120, 255};
    constexpr Color kSnakeBodyAlt = Color{60, 220, 100, 255};
    constexpr Color kSnakeHead    = Color{170, 255, 190, 255};
    constexpr Color kSnakeOutline = Color{25, 90, 60, 255};
    constexpr Color kOverlayDim   = Color{10, 10, 14, 190};
}

Renderer::Renderer(int gridWidth, int gridHeight)
    : gridWidth_(gridWidth), gridHeight_(gridHeight) {}

Vector2 Renderer::cellTopLeft(Position p) const {
    return Vector2{
        static_cast<float>(kMargin + p.x * kCellSize),
        static_cast<float>(kMargin + kHudHeight + p.y * kCellSize)
    };
}

Vector2 Renderer::cellCenter(Position p) const {
    Vector2 topLeft = cellTopLeft(p);
    return Vector2{topLeft.x + kCellSize * 0.5f, topLeft.y + kCellSize * 0.5f};
}

Vector2 Renderer::lerpPos(Position from, Position to, double alpha) const {
    Vector2 a = cellTopLeft(from);
    Vector2 b = cellTopLeft(to);
    float t = static_cast<float>(alpha);
    return Vector2{a.x + (b.x - a.x) * t, a.y + (b.y - a.y) * t};
}

void Renderer::drawBoard() const {
    // Checkerboard playfield, reminiscent of classic mobile snake games.
    for (int y = 0; y < gridHeight_; ++y) {
        for (int x = 0; x < gridWidth_; ++x) {
            Vector2 tl = cellTopLeft(Position{x, y});
            Color c = ((x + y) % 2 == 0) ? kTileLight : kTileDark;
            DrawRectangle(static_cast<int>(tl.x), static_cast<int>(tl.y), kCellSize, kCellSize, c);
        }
    }

    // Border frame around the whole playfield.
    Rectangle border{
        static_cast<float>(kMargin - 3),
        static_cast<float>(kMargin + kHudHeight - 3),
        static_cast<float>(gridWidth_ * kCellSize + 6),
        static_cast<float>(gridHeight_ * kCellSize + 6)
    };
    DrawRectangleRoundedLinesEx(border, 0.05f, 8, 5.0f, kBoardBorder);
}

void Renderer::drawHud(int score, int highScore, bool paused) const {
    int windowW = windowWidth(gridWidth_);
    DrawRectangle(0, 0, windowW, kHudHeight, kHudBg);
    DrawLine(0, kHudHeight, windowW, kHudHeight, kBoardBorder);

    char scoreBuf[32];
    std::snprintf(scoreBuf, sizeof(scoreBuf), "Score  %d", score);
    DrawText(scoreBuf, kMargin, kHudHeight / 2 - 12, 24, kScoreColor);

    char highBuf[40];
    std::snprintf(highBuf, sizeof(highBuf), "Best  %d", highScore);
    int highWidth = MeasureText(highBuf, 22);
    DrawText(highBuf, windowW - kMargin - highWidth, kHudHeight / 2 - 11, 22, kHighColor);

    if (paused) {
        const char* label = "PAUSED";
        int w = MeasureText(label, 22);
        DrawText(label, windowW / 2 - w / 2, kHudHeight / 2 - 11, 22, kTextPrimary);
    }
}

void Renderer::drawSnake(const Snake& snake, double alpha) const {
    const auto& body = snake.body();
    const auto& prev  = snake.prevBody();

    // Draw from tail to head so the head always ends up drawn on top.
    for (size_t i = body.size(); i-- > 0;) {
        Position oldPos = (i < prev.size()) ? prev[i] : body[i];
        Vector2 topLeft = lerpPos(oldPos, body[i], alpha);
        Rectangle rect{topLeft.x + 2, topLeft.y + 2,
                        static_cast<float>(kCellSize - 4), static_cast<float>(kCellSize - 4)};

        bool isHead = (i == 0);
        Color fill = isHead ? kSnakeHead : ((i % 2 == 0) ? kSnakeBody : kSnakeBodyAlt);

        DrawRectangleRounded(rect, isHead ? 0.6f : 0.45f, 8, fill);
        DrawRectangleRoundedLinesEx(rect, isHead ? 0.6f : 0.45f, 8, 3.5f, kSnakeOutline);

        if (isHead) {
            // Eyes + a subtly flicking tongue, oriented with travel direction.
            Vector2 center{rect.x + rect.width / 2, rect.y + rect.height / 2};
            float eyeOffset = kCellSize * 0.18f;
            float forwardOffset = kCellSize * 0.12f;
            Vector2 forward{0, 0}, side{0, 0};

            switch (snake.getDirection()) {
                case Direction::Up:    forward = {0, -1}; side = {1, 0}; break;
                case Direction::Down:  forward = {0,  1}; side = {1, 0}; break;
                case Direction::Left:  forward = {-1, 0}; side = {0, 1}; break;
                case Direction::Right: forward = { 1, 0}; side = {0, 1}; break;
                default: break;
            }

            Vector2 eyeBase{center.x + forward.x * forwardOffset, center.y + forward.y * forwardOffset};
            Vector2 eye1{eyeBase.x + side.x * eyeOffset, eyeBase.y + side.y * eyeOffset};
            Vector2 eye2{eyeBase.x - side.x * eyeOffset, eyeBase.y - side.y * eyeOffset};

            DrawCircleV(eye1, 4.0f, WHITE);
            DrawCircleV(eye2, 4.0f, WHITE);
            DrawCircleV(Vector2{eye1.x + forward.x * 1.5f, eye1.y + forward.y * 1.5f}, 2.0f, BLACK);
            DrawCircleV(Vector2{eye2.x + forward.x * 1.5f, eye2.y + forward.y * 1.5f}, 2.0f, BLACK);

            // Tongue flicks in and out on a slow cycle.
            if (std::fmod(GetTime(), 1.2) < 0.35) {
                Vector2 tongueStart{center.x + forward.x * (kCellSize * 0.42f), center.y + forward.y * (kCellSize * 0.42f)};
                Vector2 tongueEnd{center.x + forward.x * (kCellSize * 0.65f), center.y + forward.y * (kCellSize * 0.65f)};
                DrawLineEx(tongueStart, tongueEnd, 2.5f, Color{220, 40, 50, 255});
            }
        }
    }
}

void Renderer::drawApple(const Apple& apple) const {
    Vector2 center = cellCenter(apple.getPosition());

    // Brief pop-in / settle animation when the apple spawns.
    double age = GetTime() - apple.getSpawnTime();
    float scale = 1.0f;
    if (age >= 0.0 && age < 0.25) {
        float t = static_cast<float>(age / 0.25);
        scale = 0.4f + 0.8f * (1.0f - (1.0f - t) * (1.0f - t)); // ease-out overshoot-ish
    } else {
        // Very subtle idle pulse so it doesn't look static.
        scale = 1.0f + 0.04f * static_cast<float>(std::sin(GetTime() * 3.0));
    }

    float radius = (kCellSize * 0.42f) * scale;

    // Stem
    DrawLineEx(Vector2{center.x, center.y - radius},
               Vector2{center.x + 3, center.y - radius - 6},
               2.5f, Color{101, 67, 33, 255});
    // Leaf
    DrawEllipse(static_cast<int>(center.x + 7), static_cast<int>(center.y - radius - 4), 6, 3, Color{76, 175, 80, 255});

    // Body
    DrawCircleV(center, radius, apple.getColor());
    DrawCircleLines(static_cast<int>(center.x), static_cast<int>(center.y), radius, Color{150, 30, 30, 255});
    // Shine highlight
    DrawCircleV(Vector2{center.x - radius * 0.35f, center.y - radius * 0.35f}, radius * 0.22f, Color{255, 255, 255, 110});
}

void Renderer::drawFrame(const Snake& snake, const Apple& apple,
                          int score, int highScore, bool paused, double alpha) const {
    ClearBackground(kBgColor);
    drawBoard();
    drawApple(apple);
    drawSnake(snake, paused ? 1.0 : alpha);
    drawHud(score, highScore, paused);

    if (paused) {
        int w = windowWidth(gridWidth_);
        int h = windowHeight(gridHeight_);
        DrawRectangle(0, 0, w, h, kOverlayDim);
        const char* label = "PAUSED";
        int fontSize = 42;
        int tw = MeasureText(label, fontSize);
        DrawText(label, w / 2 - tw / 2, h / 2 - fontSize / 2, fontSize, kTextPrimary);
        const char* hint = "press P to resume";
        int hw = MeasureText(hint, 18);
        DrawText(hint, w / 2 - hw / 2, h / 2 + fontSize / 2 + 10, 18, kTextMuted);
    }
}

void Renderer::drawGameOver(int score, int highScore, bool newHighScore) const {
    int w = windowWidth(gridWidth_);
    int h = windowHeight(gridHeight_);

    DrawRectangle(0, 0, w, h, kOverlayDim);

    const char* title = "GAME OVER";
    int titleSize = 44;
    int tw = MeasureText(title, titleSize);
    DrawText(title, w / 2 - tw / 2, static_cast<int>(h * 0.28f), titleSize, Color{231, 76, 60, 255});

    char scoreBuf[40];
    std::snprintf(scoreBuf, sizeof(scoreBuf), "Final score: %d", score);
    int scw = MeasureText(scoreBuf, 24);
    DrawText(scoreBuf, w / 2 - scw / 2, static_cast<int>(h * 0.28f) + 70, 24, kTextPrimary);

    if (newHighScore) {
        const char* label = "NEW HIGH SCORE!";
        int lw = MeasureText(label, 22);
        DrawText(label, w / 2 - lw / 2, static_cast<int>(h * 0.28f) + 105, 22, kScoreColor);
    } else {
        char highBuf[40];
        std::snprintf(highBuf, sizeof(highBuf), "Best: %d", highScore);
        int hw = MeasureText(highBuf, 22);
        DrawText(highBuf, w / 2 - hw / 2, static_cast<int>(h * 0.28f) + 105, 22, kHighColor);
    }

    const char* hint = "press any move key to play again  *  Q to quit";
    int hw2 = MeasureText(hint, 18);
    DrawText(hint, w / 2 - hw2 / 2, static_cast<int>(h * 0.28f) + 150, 18, kTextMuted);
}

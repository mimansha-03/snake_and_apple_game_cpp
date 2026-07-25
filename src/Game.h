#pragma once
#include "Snake.h"
#include "Apple.h"
#include "Renderer.h"
#include "Menu.h"

enum class GameState{ MainMenu, DifficultyMenu, Playing, Paused, GameOver };
enum class Difficulty{ Easy, Medium, Hard };

class Game {
public:
    Game(int gridWidth, int gridHeight);
    void run();

private:
    int gridWidth_;
    int gridHeight_;
    Menu menu_;
    Difficulty difficulty_{Difficulty::Medium};
    Snake snake_;
    Apple apple_;
    Renderer renderer_;
    GameState state_{GameState::MainMenu};

    int score_{0};
    int highScore_{0};
    double tickInterval_{0.22};
    double accumulator_{0.0};
    bool quitRequested_{false};

    static constexpr double EASY_SPEED   = 0.30;
    static constexpr double MEDIUM_SPEED = 0.22;
    static constexpr double HARD_SPEED   = 0.14;
    static constexpr const char* kHighScoreFile = "highscore.txt";

    void handleInput();
    void update();
    void resetGame();
    [[nodiscard]] bool isWallCollision(const Position& p) const;

    void loadHighScore();
    void saveHighScoreIfBeaten();
    void applyDifficulty();
};

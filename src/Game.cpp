#include "Game.h"
#include "raylib.h"
#include <fstream>
#include <algorithm>
#include <optional>

Game::Game(int gridWidth, int gridHeight)
    : gridWidth_(gridWidth),
      gridHeight_(gridHeight),
      snake_(Position{gridWidth / 2, gridHeight / 2}),
      renderer_(gridWidth, gridHeight) {
    loadHighScore();
}

void Game::loadHighScore() {
    std::ifstream in(kHighScoreFile);
    if (in) in >> highScore_;
}

void Game::saveHighScoreIfBeaten() {
    if (score_ > highScore_) {
        highScore_ = score_;
        std::ofstream out(kHighScoreFile, std::ios::trunc);
        if (out) out << highScore_;
    }
}

bool Game::isWallCollision(const Position& p) const {
    return p.x <= 0 || p.x >= gridWidth_ - 1 || p.y <= 0 || p.y >= gridHeight_ - 1;
}

void Game::resetGame() {
    snake_ = Snake(Position{gridWidth_ / 2, gridHeight_ / 2});
    apple_.respawn(gridWidth_, gridHeight_, snake_.occupiedCells());
    score_ = 0;
    applyDifficulty();
    accumulator_ = 0.0;
    state_ = GameState::Playing;
}

namespace {
    // Returns the direction for the first movement key currently pressed, if any.
    std::optional<Direction> pollDirectionKey() {
        if (IsKeyPressed(KEY_W) || IsKeyPressed(KEY_UP))    return Direction::Up;
        if (IsKeyPressed(KEY_S) || IsKeyPressed(KEY_DOWN))  return Direction::Down;
        if (IsKeyPressed(KEY_A) || IsKeyPressed(KEY_LEFT))  return Direction::Left;
        if (IsKeyPressed(KEY_D) || IsKeyPressed(KEY_RIGHT)) return Direction::Right;
        return std::nullopt;
    }
}

void Game::handleInput() {
    bool quitKey = IsKeyPressed(KEY_Q) || IsKeyPressed(KEY_ESCAPE);
    bool pauseKey = IsKeyPressed(KEY_P);
    auto dir = pollDirectionKey();

    switch (state_) {
        case GameState::MainMenu:
        {
            menu_.update();

            switch(menu_.consumeResult())
            {
                case MenuResult::Start:
                    resetGame();
                    break;

                case MenuResult::Difficulty:
                    menu_.setDifficultyMode(true);
                    state_ = GameState::DifficultyMenu;
                    break;

                case MenuResult::Quit:
                    quitRequested_ = true;
                    break;

                default:
                    break;
            }

            break;
        }

        case GameState::Playing:
            if (quitKey) {
                quitRequested_ = true;
            } else if (pauseKey) {
                state_ = GameState::Paused;
            } else if (dir) {
                snake_.setDirection(*dir);
            }
            break;
                
        case GameState::DifficultyMenu:
        {
            menu_.update();

            switch(menu_.consumeResult())
            {
                case MenuResult::DifficultyEasy:

                    difficulty_ = Difficulty::Easy;

                    menu_.setDifficultyMode(false);

                    state_ = GameState::MainMenu;

                    break;

                case MenuResult::DifficultyMedium:

                    difficulty_ = Difficulty::Medium;

                    menu_.setDifficultyMode(false);

                    state_ = GameState::MainMenu;

                    break;

                case MenuResult::DifficultyHard:

                    difficulty_ = Difficulty::Hard;

                    menu_.setDifficultyMode(false);

                    state_ = GameState::MainMenu;

                    break;

                case MenuResult::Back:

                    menu_.setDifficultyMode(false);

                    state_ = GameState::MainMenu;

                    break;

                default:

                    break;
            }

            if(IsKeyPressed(KEY_ESCAPE))
            {
                menu_.setDifficultyMode(false);

                state_ = GameState::MainMenu;
            }

            break;
        }  
        
        case GameState::Paused:
            if (quitKey) {
                quitRequested_ = true;
            } else if (pauseKey) {
                state_ = GameState::Playing;
            }
            break;

        case GameState::GameOver:
            if (quitKey) {
                quitRequested_ = true;
            } else if (dir) {
                resetGame();
            }
            break;
    }
}

void Game::update() {
    Position nextHead = snake_.getPosition();
    switch (snake_.getDirection()) {
        case Direction::Up:    nextHead.y -= 1; break;
        case Direction::Down:  nextHead.y += 1; break;
        case Direction::Left:  nextHead.x -= 1; break;
        case Direction::Right: nextHead.x += 1; break;
        default: break;
    }

    if (isWallCollision(nextHead)) {
        state_ = GameState::GameOver;
        saveHighScoreIfBeaten();
        return;
    }

    const bool willEatApple = (nextHead == apple_.getPosition());
    snake_.move(willEatApple);

    if (snake_.hasSelfCollision()) {
        state_ = GameState::GameOver;
        saveHighScoreIfBeaten();
        return;
    }

    if (willEatApple) {
        score_ += 10;
        apple_.respawn(gridWidth_, gridHeight_, snake_.occupiedCells());
        double minSpeed = 0.0;
        switch(difficulty_)
        {
            case Difficulty::Easy:
                minSpeed = 0.18;
                break;

            case Difficulty::Medium:
                minSpeed = 0.13;
                break;

            case Difficulty::Hard:
                minSpeed = 0.09;
                break;
        }

        tickInterval_ = std::max(
            minSpeed,
            tickInterval_ - 0.0025
        );
    }
}

void Game::applyDifficulty()
    {
        switch(difficulty_)
        {
            case Difficulty::Easy:
                tickInterval_ = EASY_SPEED;
                break;

            case Difficulty::Medium:
                tickInterval_ = MEDIUM_SPEED;
                break;

            case Difficulty::Hard:
                tickInterval_ = HARD_SPEED;
                break;
        }
    }

void Game::run() {
    while (!WindowShouldClose() && !quitRequested_) {
        handleInput();

        double alpha = 1.0;
        if (state_ == GameState::Playing) {
            accumulator_ += GetFrameTime();
            while (accumulator_ >= tickInterval_) {
                update();
                accumulator_ -= tickInterval_;
                if (state_ != GameState::Playing) {
                    accumulator_ = 0.0;
                    break;
                }
            }
            alpha = tickInterval_ > 0.0 ? (accumulator_ / tickInterval_) : 1.0;
        }

        BeginDrawing();
        Color bg = {12, 15, 24, 255};
        switch (state_) {
            case GameState::MainMenu:
                ClearBackground(bg);
                menu_.draw();
                break;
            case GameState::Playing:
                renderer_.drawFrame(snake_, apple_, score_, highScore_, false, alpha);
                break;
            case GameState::Paused:
                renderer_.drawFrame(snake_, apple_, score_, highScore_, true, alpha);
                break;
            case GameState::GameOver: {
                bool isNewHigh = (score_ == highScore_ && score_ > 0);
                renderer_.drawFrame(snake_, apple_, score_, highScore_, false, 1.0);
                renderer_.drawGameOver(score_, highScore_, isNewHigh);
                break;
            }
            case GameState::DifficultyMenu:
            {
                ClearBackground(Color{12,15,24,255});

                menu_.draw();

                break;
            }
            default:
                break;
        }
        EndDrawing();
    }
}

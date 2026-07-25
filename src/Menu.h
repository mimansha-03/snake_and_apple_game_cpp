#pragma once

#include <string>
#include <vector>

enum class MenuResult
{
    None,
    Start,
    Difficulty,
    Quit,

    DifficultyEasy,
    DifficultyMedium,
    DifficultyHard,

    Back
};

class Menu
{
public:
    Menu();

    void update();
    void draw() const;

    // New
    void setDifficultyMode(bool enable);

    MenuResult consumeResult();

private:
    std::vector<std::string> items_;

    int selected_;

    MenuResult result_;

    bool difficultyMode_;

    mutable float animationTime_;
};
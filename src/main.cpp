#include "raylib.h"
#include "Game.h"
#include "Renderer.h"

int main() {
    constexpr int kGridWidth  = 24;
    constexpr int kGridHeight = 16;

    InitWindow(Renderer::windowWidth(kGridWidth), Renderer::windowHeight(kGridHeight), "Snake & Apple");
    SetTargetFPS(60);
    SetExitKey(KEY_NULL); // we handle Q / Escape ourselves so they don't instantly close the window

    Game game(kGridWidth, kGridHeight);
    game.run();

    CloseWindow();
    return 0;
}

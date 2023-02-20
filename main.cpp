#include "defines.h"
#include "raylib.h"
#include "third_party/raygui.h"
#include "settings.h"
#include "vector"
#include "tile.h"
#include "game.h"
#include <iostream>
#include <string>

#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif

using namespace ::minis;

void UpdateDrawFrame(Game *game);

int main(void)
{
    GameSettings settings = GetSettings(DifficultyLevel::BEGINNER_1);
    Vector2 win_size = GetWindowSize(&settings);

    InitWindow(win_size.x, win_size.y, "Minisweeper");
    Image window_icon = LoadImage("assets/mine_31x31.png");
    SetWindowIcon(window_icon);
    InitAudioDevice();

    Game *game = new Game(settings);

#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(UpdateDrawFrame, 60, 1);
#else
    SetTargetFPS(60);

    // Main game loop
    while (!WindowShouldClose())
    {
        UpdateDrawFrame(game);
    }
#endif

    game->~Game();

    CloseWindow();

    return 0;
}

void UpdateDrawFrame(Game *game)
{
    game->Update();

    BeginDrawing();

    ClearBackground(RAYWHITE);

    game->Draw();
    EndDrawing();
}
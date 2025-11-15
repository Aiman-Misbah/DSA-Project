#include <iostream>
#include <raylib.h>
#include "Game.h"
#include "Manager.h"
#include <cstdlib>
#include <ctime>
#include "Colours.h"

double lastUpdateTime = 0;

bool EventTriggered(double interval) {
    double currentTime = GetTime();
    if (currentTime - lastUpdateTime >= interval) {
        lastUpdateTime = currentTime;
        return true;
    }
    return false;
}

int main() {
    srand(time(0));
    InitWindow(1000, 750, "Tetris Game");
    SetTargetFPS(60);

    Color darkBlue = { 44,44,127,255 };
    Font font = LoadFontEx("Font/monogram.ttf", 64, 0, 0);

    Game game;
    Manager Manager;
    bool gamePaused = false;

    while (WindowShouldClose() == false) {
        if (game.musicOn && !gamePaused) {
            UpdateMusicStream(game.music);
        }

        Manager.Update(game.musicOn, gamePaused);

        if (!gamePaused) {
            game.HandleInput();
            if (EventTriggered(0.2)) {
                game.MoveDown();
            }
        }

        BeginDrawing();
        ClearBackground(darkBlue);

        // Always draw the game (even when paused)
        Manager.Draw(game.musicOn, gamePaused, game.score, font);
        game.Draw();


        if (game.GameOver) {
            DrawTextEx(font, "GAME OVER", { 320,450 }, 38, 2, WHITE);
        }

        if (gamePaused) {

            DrawRectangle(11, 11, 15 * 35, 20 * 35, { 0, 0, 0, 100 }); // Semi-transparent black overlay

            // Draw "PAUSED" text centered over the board
            int boardCenterX = 11 + (15 * 35) / 2;
            int boardCenterY = 11 + (20 * 35) / 2;
            Vector2 textSize = MeasureTextEx(font, "PAUSED", 38, 2);
            DrawTextEx(font, "PAUSED",
                { boardCenterX - textSize.x / 2, boardCenterY - textSize.y / 2 },
                38, 2, WHITE);
        }

        EndDrawing();
    }

    CloseWindow();
}

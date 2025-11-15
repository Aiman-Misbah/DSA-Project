#include <iostream>
#include <raylib.h>
#include "Game.h"
#include <cstdlib>
#include <ctime>
#include "Colours.h"
#include <string>
using namespace std;

double lastUpdateTime = 0;

bool EventTriggered(double interval) {
    double currentTime = GetTime();
    if (currentTime - lastUpdateTime >= interval) {
        lastUpdateTime = currentTime;
        return true;
    }
    return false;
}

// ADD: Check if point is inside rectangle
bool IsMouseInRectangle(Rectangle rect) {
    Vector2 mousePos = GetMousePosition();
    return (mousePos.x >= rect.x && mousePos.x <= rect.x + rect.width &&
        mousePos.y >= rect.y && mousePos.y <= rect.y + rect.height);
}

int main() {
    srand(time(0));
    InitWindow(600, 620, "Tetris Game");
    SetTargetFPS(60);
    Color darkBlue = { 44,44,127,255 };
    Font font = LoadFontEx("Font/monogram.ttf", 64, 0, 0);

    Game game;

    // ADD: Music button position and size
    Rectangle musicButton = { 320, 400, 170, 50 };
    Color buttonColor = LightBlue;

    while (WindowShouldClose() == false) {
        // UPDATE: Only update music if it's on
        if (game.musicOn) {
            UpdateMusicStream(game.music);
        }

        game.HandleInput();

        // ADD: Music button click handling
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            if (IsMouseInRectangle(musicButton)) {
                game.ToggleMusic();
            }
        }

        // ADD: Button hover effect
        if (IsMouseInRectangle(musicButton)) {
            buttonColor = BLUE;  // Highlight on hover
        }
        else {
            buttonColor = LightBlue;
        }

        if (EventTriggered(0.2)) {
            game.MoveDown();
        }

        BeginDrawing();
        ClearBackground(darkBlue);

        // Draw game elements
        DrawTextEx(font, "Score", { 365,15 }, 38, 2, WHITE);
        DrawTextEx(font, "Next", { 370,175 }, 38, 2, WHITE);

        DrawRectangleRounded({ 320, 55, 170,60 }, 0.3, 6, LightBlue);

        char scoreText[10]{};
        snprintf(scoreText, sizeof(scoreText), "%d", game.score);
        Vector2 textSize = MeasureTextEx(font, scoreText, 38, 2);

        DrawTextEx(font, scoreText, { 320 + (170 - textSize.x) / 2,65 }, 38, 2, WHITE);

        DrawRectangleRounded({ 320, 215, 170,160 }, 0.3, 6, LightBlue);

        // ADD: Music toggle button
        DrawRectangleRounded(musicButton, 0.3, 6, buttonColor);
        DrawTextEx(font, game.musicOn ? "MUSIC: ON" : "MUSIC: OFF",
            { musicButton.x + 20, musicButton.y + 10 }, 24, 2, WHITE);

        if (game.GameOver) {
            DrawTextEx(font, "GAME OVER", { 320,450 }, 38, 2, WHITE);
        }

        game.Draw();
        EndDrawing();
    }

    CloseWindow();
}

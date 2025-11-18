#include <iostream>
#include <raylib.h>
#include "Game.h"
#include "Manager.h"
#include "WelcomeScreen.h"  // Include the new WelcomeScreen
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
    WelcomeScreen welcomeScreen;  // Create welcome screen instance
    bool gamePaused = false;
    bool wasPaused = false;
    bool gameStarted = false;
    float ghostAnimationProgress = 1.0f; // Start at 1.0 (fully ON)
    bool wasGhostEnabled = true;

    while (WindowShouldClose() == false) {
        if (!gameStarted) {
            // Welcome screen logic
            bool startGame = false;
            welcomeScreen.Update(startGame);
            if (startGame) {
                gameStarted = true;
                game.StartCountdown(); // Start the initial countdown
            }
        }
        else {
            // Main game logic (same as before)
            bool previouslyPaused = gamePaused;
            if (game.showGhost != wasGhostEnabled) {
                ghostAnimationProgress = 0.0f;
                wasGhostEnabled = game.showGhost;
            }

            if (ghostAnimationProgress < 1.0f) {
                ghostAnimationProgress += GetFrameTime() * 5.0f; // Adjust speed here
                if (ghostAnimationProgress > 1.0f) ghostAnimationProgress = 1.0f;
            }
            Manager.Update(game.musicOn, gamePaused, game.IsCountingDown(), game.showGhost, ghostAnimationProgress);

            bool justResumed = previouslyPaused && !gamePaused;
            if (justResumed) {
                game.StartCountdown();
            }

            game.UpdateCountdown();

            if (game.musicOn && !gamePaused && !game.IsCountingDown()) {
                UpdateMusicStream(game.music);
            }

            // In main.cpp, inside the while loop (after uiManager.Update):
            if (!gamePaused && !game.IsCountingDown()) {
                game.HandleInput();

                // NEW: Handle hard drop animation at faster speed
                if (game.isDropping) {
                    static double lastHardDropTime = 0;  // Track time for hard drop
                    double currentTime = GetTime();
                    if (currentTime - lastHardDropTime >= 0.02) {  // Faster than normal (0.05s vs 0.2s)
                        game.UpdateHardDrop();
                        lastHardDropTime = currentTime;
                    }
                }
                else {
                    // Normal move down (only if not hard dropping)
                    if (EventTriggered(0.2)) {
                        game.MoveDown();
                    }
                }
            }

            wasPaused = gamePaused;
        }

        BeginDrawing();
        ClearBackground(darkBlue);

        if (!gameStarted) {
            // Draw welcome screen
            welcomeScreen.Draw();
        }
        else {
            // Draw game
            Manager.Draw(game.musicOn, gamePaused, game.score, font, game.showGhost, ghostAnimationProgress);
            game.Draw();

            if (game.GameOver) {
                DrawTextEx(font, "GAME OVER", { 320,450 }, 38, 2, WHITE);

                // Optionally add a "Back to Menu" button here later
            }

            if (gamePaused) {
                DrawRectangle(11, 11, 15 * 35, 20 * 35, { 0, 0, 0, 100 });
                int boardCenterX = 11 + (15 * 35) / 2;
                int boardCenterY = 11 + (20 * 35) / 2;
                Vector2 textSize = MeasureTextEx(font, "PAUSED", 38, 2);
                DrawTextEx(font, "PAUSED",
                    { boardCenterX - textSize.x / 2, boardCenterY - textSize.y / 2 },
                    38, 2, WHITE);
            }

            if (game.IsCountingDown()) {
                DrawRectangle(11, 11, 15 * 35, 20 * 35, { 0, 0, 0, 100 });
                char countdownText[2];
                snprintf(countdownText, sizeof(countdownText), "%d", game.GetCountdownNumber());
                int boardCenterX = 11 + (15 * 35) / 2;
                int boardCenterY = 11 + (20 * 35) / 2;
                Vector2 textSize = MeasureTextEx(font, countdownText, 120, 4);
                DrawTextEx(font, countdownText,
                    { boardCenterX - textSize.x / 2, boardCenterY - textSize.y / 2 },
                    120, 4, WHITE);
            }
        }

        EndDrawing();
    }

    CloseWindow();
}

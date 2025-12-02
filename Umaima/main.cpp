#include <iostream>
#include <raylib.h>
#include "Game.h"
#include "Manager.h"
#include "WelcomeScreen.h"
#include <cstdlib>
#include <ctime>
#include "Colours.h"
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

int main() {
    srand(time(0));
    InitWindow(1200, 800, "Tetris Game");
    SetTargetFPS(60);

    Color darkBlue = { 44,44,127,255 };
    Font font = LoadFontEx("Font/monogram.ttf", 64, 0, 0);

    Game game;
    Manager Manager;
    WelcomeScreen welcomeScreen;
    bool gamePaused = false;
    bool wasPaused = false;
    bool gameStarted = false;
    bool showInstructions = false;  // NEW: Track instructions screen
    float ghostAnimationProgress = 1.0f;
    bool wasGhostEnabled = true;

    bool restartRequested = false;
    bool returnToMenuRequested = false;

    while (WindowShouldClose() == false) {
        if (!gameStarted) {
            if (showInstructions) {
                // NEW: Instructions screen logic
                Vector2 mouse = GetMousePosition();
                Rectangle backBounds = { 400, 600, 200, 60 };

                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) &&
                    CheckCollisionPointRec(mouse, backBounds)) {
                    showInstructions = false;
                }
            }
            else {
                // Welcome screen logic
                bool startGame = false;
                welcomeScreen.Update(startGame, showInstructions);  // UPDATED: Pass showInstructions
                if (startGame) {
                    gameStarted = true;
                    game.StartCountdown();
                }
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
                ghostAnimationProgress += GetFrameTime() * 5.0f;
                if (ghostAnimationProgress > 1.0f) ghostAnimationProgress = 1.0f;
            }
            Manager.Update(game.musicOn, gamePaused, game.IsCountingDown(), game.showGhost, ghostAnimationProgress, game.GameOver, restartRequested, returnToMenuRequested);

            if (restartRequested) {
                game.Reset();
                game.StartCountdown();
                restartRequested = false;
                // Reset any other states if needed
            }

            if (returnToMenuRequested) {
                gameStarted = false;
                game.Reset();
                returnToMenuRequested = false;
                // Reset welcome screen if needed
            }

            bool justResumed = previouslyPaused && !gamePaused;
            if (justResumed) {
                game.StartCountdown();
            }

            game.UpdateCountdown();

            // Time tracking control - FIXED VERSION
            // SIMPLIFIED Time tracking control
            if (gamePaused || game.IsCountingDown() || game.GameOver) {
                game.StopTimeTracking(); // Stop timer in these states
            }
            else {
                game.StartTimeTracking(); // Start timer only when actually playing
            }

            if (game.musicOn && !gamePaused && !game.IsCountingDown()) {
                UpdateMusicStream(game.music);
            }

            if (!gamePaused && !game.IsCountingDown()) {
                game.UpdateMessages(GetFrameTime());
                game.HandleInput();

                if (game.isDropping) {
                    static double lastHardDropTime = 0;
                    double currentTime = GetTime();
                    if (currentTime - lastHardDropTime >= 0.02) {
                        game.UpdateHardDrop();
                        lastHardDropTime = currentTime;
                    }
                }
                else {
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
            if (showInstructions) {
                // NEW: Draw instructions screen
                welcomeScreen.DrawInstructions();
            }
            else {
                // Draw welcome screen
                welcomeScreen.Draw();
            }
        }
        else {
            // Draw game (same as before)
            game.Draw();

            Manager.Draw(game.musicOn, gamePaused, game.score, font, game.showGhost, ghostAnimationProgress, game.GetPlayTime(), game.GetTotalLinesCleared(), game.GameOver);

            game.DrawMessages();
            

            if (gamePaused) {
                DrawRectangle(290, 50, 15 * 35, 20 * 35, DarkOverlay);
                int boardCenterX = 290 + (15 * 35) / 2;
                int boardCenterY = 50 + (20 * 35) / 2;
                Vector2 textSize = MeasureTextEx(font, "PAUSED", 38, 2);
                DrawTextEx(font, "PAUSED",
                    { boardCenterX - textSize.x / 2, boardCenterY - textSize.y / 2 },
                    38, 2, WHITE);
            }

            if (game.IsCountingDown()) {
                DrawRectangle(290, 50, 15 * 35, 20 * 35, DarkOverlay);
                char countdownText[2];
                snprintf(countdownText, sizeof(countdownText), "%d", game.GetCountdownNumber());
                int boardCenterX = 290 + (15 * 35) / 2;
                int boardCenterY = 50 + (20 * 35) / 2;
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

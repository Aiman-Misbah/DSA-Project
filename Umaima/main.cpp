#include <iostream>
#include <raylib.h>
#include "Game.h"
#include "Manager.h"
#include "WelcomeScreen.h"
#include <cstdlib>
#include <ctime>
#include "Colours.h"
using namespace std;

double lastUpdateTime = 0;  //when the gravity was last applied

bool EventTriggered (double interval) { //checking if enough time has passed to apply gravity again
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

    Font font = LoadFontEx("Font/monogram.ttf", 64, 0, 0);

    Game game;
    Manager Manager;
    WelcomeScreen welcomeScreen;
    bool gamePaused = false;
    bool gameStarted = false;
    bool showInstructions = false;  

    float ghostAnimationProgress = 1.0f;
    bool wasGhostEnabled = true;

    bool restartRequested = false;
    bool returnToMenuRequested = false;

    while (WindowShouldClose() == false) {
        if (!gameStarted) {
            if (showInstructions) {
                welcomeScreen.UpdateInstructions(showInstructions);
                }
            
            else {
                bool startGame = false;
                welcomeScreen.Update(startGame, showInstructions); 
                if (startGame) {
                    gameStarted = true;
                    game.StartCountdown();
                }
            }
        }
        else {
            bool previouslyPaused = gamePaused; //state previous pause state
            if (game.showGhost != wasGhostEnabled) {    //if player clicked on the toggle update its state and animation bhi dikhaani hai
                ghostAnimationProgress = 0.0f;
                wasGhostEnabled = game.showGhost;
            }

            if (ghostAnimationProgress < 1.0f) {
                ghostAnimationProgress += GetFrameTime() * 5.0f;
                if (ghostAnimationProgress > 1.0f) ghostAnimationProgress = 1.0f;
            }
            //updating the ui accordingly
            Manager.Update(game.musicOn, gamePaused, game.IsCountingDown(), game.showGhost, ghostAnimationProgress, game.GameOver, restartRequested, returnToMenuRequested);

            if (restartRequested) {
                game.Reset();
                game.StartCountdown();
                restartRequested = false;
            }

            if (returnToMenuRequested) {
                gameStarted = false;    //game nhi khelna humein
                game.Reset();
                returnToMenuRequested = false;
            }

            //previously pause tha and ab resum krdia hai so start countdown
            bool justResumed = previouslyPaused && !gamePaused;
            if (justResumed) {
                game.StartCountdown();
            }

            game.UpdateCountdown();

            if (gamePaused || game.IsCountingDown() || game.GameOver) {
                game.StopTimeTracking(); // Stop timer in these states
            }
            else {
                game.StartTimeTracking(); // Start timer only when actually playing
            }

            //can only change the music toggle when playing
            if (game.musicOn && !gamePaused && !game.IsCountingDown()) {
                UpdateMusicStream(game.music);
            }

            //these features only work when actually playing
            if (!gamePaused && !game.IsCountingDown()) {
                game.UpdateMessages(GetFrameTime());
                game.HandleInput();

                if (game.isDropping) {
                    static double lastHardDropTime = 0;
                    double currentTime = GetTime();
                    //drops every 0.02 seconds
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

        }

        BeginDrawing();
        ClearBackground(DarkBlue);

        if (!gameStarted) {
            if (showInstructions) {
                //instruction screen
                welcomeScreen.DrawInstructions();
            }
            else {
                //drawing the welcome screen
                welcomeScreen.Draw();
            }
        }
        else {
            //actual game ko draw kro 
            game.Draw();

            //ui drawing
            Manager.Draw(game.musicOn, gamePaused, game.score, font, game.showGhost,
                ghostAnimationProgress, game.GetPlayTime(), game.GetTotalLinesCleared(),
                game.GameOver, game.GetLeaderboard(), game, game.GetPieceQueue());

            //if messages are to be drawn draw them 
            game.DrawMessages();
            

            if (gamePaused) {
                DrawRectangle(290, 50, 15 * 35, 20 * 35, DarkOverlay);  //overlay on the board
                int boardCenterX = 290 + (15 * 35) / 2;
                int boardCenterY = 50 + (20 * 35) / 2;
                //display PAUSED when paused
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

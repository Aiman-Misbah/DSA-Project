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


int main() {
    srand(time(0));
    InitWindow(600, 620, "Tetris Game");
    SetTargetFPS(60);
    Color darkBlue = {44,44,127,255};   //raylib's struct color {red, green, blue, alpha(transparency)}
    Font font = LoadFontEx("Font/monogram.ttf", 64, 0, 0);
    
    Game game;
   
    while (WindowShouldClose() == false) {
        UpdateMusicStream(game.music);
        game.HandleInput();

        if(EventTriggered(0.2)) {
            game.MoveDown();
        }
        BeginDrawing();
        ClearBackground(darkBlue);
        DrawTextEx(font, "Score", { 365,15 }, 38, 2, WHITE);
        DrawTextEx(font, "Next", { 370,175 }, 38, 2, WHITE);

        DrawRectangleRounded({320, 55, 170,60}, 0.3,6, LightBlue );
  
        char scoreText[10]{};
        snprintf(scoreText, sizeof(scoreText), "%d", game.score);
        Vector2 textSize = MeasureTextEx( font, scoreText, 38,2 );
        
        DrawTextEx(font, scoreText, {320 + (170-textSize.x)/2,65}, 38, 2, WHITE);

        DrawRectangleRounded({320, 215, 170,160}, 0.3,6, LightBlue );
        if (game.GameOver) {
            DrawTextEx(font, "GAME OVER", { 320,450 }, 38, 2, WHITE);
        }
        game.Draw();
        EndDrawing();
    }

    CloseWindow();
}

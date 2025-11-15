#include "Manager.h"
#include <iostream>

Manager::Manager() : buttonScale(0.2f), musicX(0), musicY(0), pauseX(0), pauseY(0),
isHoveringMusic(false), isHoveringPause(false), myBlue (59, 85, 162, 255) {
    LoadTextures();
}

Manager::~Manager() {
    UnloadTextures();
}

void Manager::LoadTextures() {
    musicOnTex = LoadTexture("Images/music-on.png");
    musicOffTex = LoadTexture("Images/music-off.png");
    pauseTex = LoadTexture("Images/pause.png");
    playTex = LoadTexture("Images/play.png");

    int scaledWidth = musicOnTex.width * buttonScale;
    musicX = 720 + (170 - scaledWidth) / 2;
    musicY = 395;

    pauseX = 720 + (170 - scaledWidth) / 2;
    pauseY = 465;
}

void Manager::Update(bool& musicOn, bool& gamePaused) {
    Vector2 mouse = GetMousePosition();

    int iconW = musicOnTex.width * buttonScale;
    int iconH = musicOnTex.height * buttonScale;

    Rectangle musicBounds = { (float)musicX, (float)musicY, (float)iconW, (float)iconH };
    isHoveringMusic = CheckCollisionPointRec(mouse, musicBounds);

    Rectangle pauseBounds = { (float)pauseX, (float)pauseY, (float)iconW, (float)iconH };
    isHoveringPause = CheckCollisionPointRec(mouse, pauseBounds);

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        if (CheckCollisionPointRec(mouse, musicBounds)) {
            musicOn = !musicOn;
        }
        if (CheckCollisionPointRec(mouse, pauseBounds)) {
            gamePaused = !gamePaused;
        }
    }
}

void Manager::Draw(bool musicOn, bool gamePaused, int score, Font& font) {
    DrawTextEx(font, "Score", { 765,15 }, 38, 2, WHITE);
    DrawTextEx(font, "Next", { 770,175 }, 38, 2, WHITE);

    DrawRectangleRounded({ 720, 55, 170,60 }, 0.3f, 6, myBlue);

    char scoreText[10]{};
    snprintf(scoreText, sizeof(scoreText), "%d", score);
    Vector2 textSize = MeasureTextEx(font, scoreText, 38, 2);
    DrawTextEx(font, scoreText, { 720 + (170 - textSize.x) / 2,65 }, 38, 2, WHITE);

    DrawRectangleRounded({ 720, 215, 170,160 }, 0.3f, 6, myBlue);

    float musicHoverScale = isHoveringMusic ? buttonScale * 1.2f : buttonScale;
    Color musicHoverColor = isHoveringMusic ? LIGHTGRAY : WHITE;

    if (musicOn) {
        DrawTextureEx(musicOnTex, { (float)musicX, (float)musicY }, 0.0f, musicHoverScale, musicHoverColor);
    }
    else {
        DrawTextureEx(musicOffTex, { (float)musicX, (float)musicY }, 0.0f, musicHoverScale, musicHoverColor);
    }

    float pauseHoverScale = isHoveringPause ? buttonScale * 1.2f : buttonScale;
    Color pauseHoverColor = isHoveringPause ? LIGHTGRAY : WHITE;

    if (gamePaused) {
        DrawTextureEx(playTex, { (float)pauseX, (float)pauseY }, 0.0f, pauseHoverScale, pauseHoverColor);
    }
    else {
        DrawTextureEx(pauseTex, { (float)pauseX, (float)pauseY }, 0.0f, pauseHoverScale, pauseHoverColor);
    }
}

void Manager::UnloadTextures() {
    UnloadTexture(musicOnTex);
    UnloadTexture(musicOffTex);
    UnloadTexture(pauseTex);
    UnloadTexture(playTex);
}

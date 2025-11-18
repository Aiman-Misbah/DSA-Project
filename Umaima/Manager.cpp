#include "Manager.h"
#include <iostream>
using namespace std;

Manager::Manager() : buttonScale(0.2f), musicX(0), musicY(0), pauseX(0), pauseY(0),
isHoveringMusic(false), isHoveringPause(false), isHoveringGhost(false),
ghostToggleX(0.0f), ghostToggleY(0.0f), myBlue(59, 85, 162, 255), musicAnimationProgress(0.0f), isMusicAnimating(false) {
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

    ghostToggleX = 720.0f + 10.0f;
    ghostToggleY = 535.0f;
}

void Manager::Update(bool& musicOn, bool& gamePaused, bool isCountingDown, bool& ghostEnabled, float& ghostAnimationProgress) {
    Vector2 mouse = GetMousePosition();

    int iconW = musicOnTex.width * buttonScale;
    int iconH = musicOnTex.height * buttonScale;

    Rectangle musicBounds = { (float)musicX, (float)musicY, (float)iconW, (float)iconH };
    isHoveringMusic = CheckCollisionPointRec(mouse, musicBounds);

    Rectangle pauseBounds = { (float)pauseX, (float)pauseY, (float)iconW, (float)iconH };
    isHoveringPause = CheckCollisionPointRec(mouse, pauseBounds);

    float toggleWidth = 120.0f;
    float toggleHeight = 35.0f;
    Rectangle ghostToggleBounds = { ghostToggleX, ghostToggleY, toggleWidth, toggleHeight };
    isHoveringGhost = CheckCollisionPointRec(mouse, ghostToggleBounds);

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        if (CheckCollisionPointRec(mouse, musicBounds)) {
            musicOn = !musicOn;
        }
        if (CheckCollisionPointRec(mouse, pauseBounds) && !isCountingDown) {
            gamePaused = !gamePaused;
        }
        if (CheckCollisionPointRec(mouse, ghostToggleBounds) && !isCountingDown) {
            ghostEnabled = !ghostEnabled;
            ghostAnimationProgress = 0.0f; // Start animation
        }
    }
}

void Manager::Draw(bool musicOn, bool gamePaused, int score, Font& font, bool ghostEnabled, float ghostAnimationProgress) {
    DrawTextEx(font, "Score", { 765,15 }, 38, 2, WHITE);
    DrawTextEx(font, "Next", { 770,175 }, 38, 2, WHITE);

    DrawRectangleRounded({ 720, 55, 170,60 }, 0.3f, 6, myBlue);

    char scoreText[10]{};
    snprintf(scoreText, sizeof(scoreText), "%d", score);
    Vector2 textSize = MeasureTextEx(font, scoreText, 38, 2);
    DrawTextEx(font, scoreText, { 720 + (170 - textSize.x) / 2,65 }, 38, 2, WHITE);

    DrawRectangleRounded({ 720, 215, 170,160 }, 0.3f, 6, myBlue);

    // Music button
    float musicHoverScale = isHoveringMusic ? buttonScale * 1.2f : buttonScale;
    Color musicHoverColor = isHoveringMusic ? LIGHTGRAY : WHITE;

    if (musicOn) {
        DrawTextureEx(musicOnTex, { (float)musicX, (float)musicY }, 0.0f, musicHoverScale, musicHoverColor);
    }
    else {
        DrawTextureEx(musicOffTex, { (float)musicX, (float)musicY }, 0.0f, musicHoverScale, musicHoverColor);
    }

    // Pause button
    float pauseHoverScale = isHoveringPause ? buttonScale * 1.2f : buttonScale;
    Color pauseHoverColor = isHoveringPause ? LIGHTGRAY : WHITE;

    if (gamePaused) {
        DrawTextureEx(playTex, { (float)pauseX, (float)pauseY }, 0.0f, pauseHoverScale, pauseHoverColor);
    }
    else {
        DrawTextureEx(pauseTex, { (float)pauseX, (float)pauseY }, 0.0f, pauseHoverScale, pauseHoverColor);
    }

    // Ghost toggle
    const char* fullText = ghostEnabled ? "Ghost ON" : "Ghost OFF";
    Color toggleBg = ghostEnabled ? GREEN : RED;

    float toggleWidth = 120.0f;
    float toggleHeight = 35.0f;
    DrawRectangleRounded({ ghostToggleX, ghostToggleY, toggleWidth, toggleHeight }, 0.9f, 8, toggleBg);

    // Calculate circle position with animation
    float startX = ghostToggleX + 20.0f;
    float endX = ghostToggleX + toggleWidth - 20.0f;

    float progress = ghostAnimationProgress;
    if (progress > 1.0f) progress = 1.0f;

    float circleX;
    if (ghostEnabled) {
        circleX = startX + (endX - startX) * progress;
    }
    else {
        circleX = endX - (endX - startX) * progress;
    }


    Color textColor = BLACK;
    Vector2 TextSize = MeasureTextEx(font, fullText, 16, 1);

    float textX;
    if (ghostEnabled) {
        textX = ghostToggleX + 10.0f;
    }
    else {
        textX = ghostToggleX + 40.0f;
    }

    float textY = ghostToggleY + (toggleHeight - TextSize.y) / 2;
    DrawTextEx(font, fullText, { textX, textY }, 16, 1, textColor);

    Color circleColor = isHoveringGhost ? Color{ 250, 250, 250, 255 } : WHITE;
    DrawCircle(circleX, ghostToggleY + toggleHeight / 2, 14.0f, circleColor);
}

void Manager::UnloadTextures() {
    UnloadTexture(musicOnTex);
    UnloadTexture(musicOffTex);
    UnloadTexture(pauseTex);
    UnloadTexture(playTex);
}

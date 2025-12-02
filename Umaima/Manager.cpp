#include "Manager.h"
#include "Colours.h"
#include <iostream>
using namespace std;

Manager::Manager() : buttonScale(0.2f), musicX(0), musicY(0), pauseX(0), pauseY(0),
isHoveringMusic(false), isHoveringPause(false), isHoveringGhost(false),
ghostToggleX(0.0f), ghostToggleY(0.0f), myBlue(59, 85, 162, 255), musicAnimationProgress(0.0f), isMusicAnimating(false), isHoveringReplay(false), isHoveringMenu(false) {
    LoadTextures();
    replayButton = { 0,0,200,60 };
    menuButton = { 0,0,200,60 };
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

    // NEW POSITIONS for controls (in the right panel)
    musicX = 900;  // X position in right panel
    musicY = 560;  // Y position for music button

    pauseX = 1025;  // X position in right panel  
    pauseY = 560;  // Y position for pause button

    ghostToggleX = 950.0f;  // X position in right panel
    ghostToggleY = 650.0f;  // Y position for ghost toggle
}
void Manager::Update(bool& musicOn, bool& gamePaused, bool isCountingDown, bool& ghostEnabled,
    float& ghostAnimationProgress, bool gameOver, bool& restartRequested, bool& returnToMenuRequested) {
    Vector2 mouse = GetMousePosition();

    // Only update game over screen if game is over
    if (gameOver) {
        UpdateGameOverScreen(mouse, gameOver, restartRequested, returnToMenuRequested);
        UpdateParticles(GetFrameTime());
    }

    // Original update logic for when game is not over
    int iconW = musicOnTex.width * buttonScale;
    int iconH = musicOnTex.height * buttonScale;

    // Music button bounds
    Rectangle musicBounds = { (float)musicX, (float)musicY, (float)iconW, (float)iconH };
    isHoveringMusic = CheckCollisionPointRec(mouse, musicBounds);

    // Pause button bounds
    Rectangle pauseBounds = { (float)pauseX, (float)pauseY, (float)iconW, (float)iconH };
    isHoveringPause = CheckCollisionPointRec(mouse, pauseBounds);

    // Ghost toggle bounds
    float toggleWidth = 100.0f;
    float toggleHeight = 30.0f;
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
            ghostAnimationProgress = 0.0f;
        }
    }
}

void Manager::Draw(bool musicOn, bool gamePaused, int score, Font& font, bool ghostEnabled, float ghostAnimationProgress, double time, int lines, bool gameOver) {


    // ============ LEFT PANEL - GAME STATS ============
    DrawRectangleRounded({ 50, 280, 200, 180 }, 0.3f, 6, PanelBlue);
    DrawText("GAME STATS", 65, 250, 20, WHITE);

    // Score in Game Stats box
    DrawTextEx(font, "Score:", { 70, 310 }, 28, 1, WHITE);
    char scoreText[20];
    snprintf(scoreText, sizeof(scoreText), "%d", score);
    DrawTextEx(font, scoreText, { 160, 310 }, 28, 1, WHITE);

    // Time in Game Stats box
    DrawTextEx(font, "Time:", { 70, 350 }, 28, 1, WHITE);
    char timeText[20];
    int minutes = (int)time / 60;
    int seconds = (int)time % 60;
    snprintf(timeText, sizeof(timeText), "%02d:%02d", minutes, seconds);
    DrawTextEx(font, timeText, { 150, 350 }, 28, 1, WHITE);

    // Lines in Game Stats box
    DrawTextEx(font, "Lines:", { 70, 390 }, 28, 1, WHITE);
    char linesText[20];
    snprintf(linesText, sizeof(linesText), "%d", lines);
    DrawTextEx(font, linesText, { 160, 390 }, 28, 1, WHITE);

    // ============ RIGHT PANEL - GAME CONTROLS ============
    // Draw controls background
    DrawRectangleRounded({ 850, 500, 300, 200 }, 0.3f, 6, PanelBlue);
    DrawText("GAME CONTROLS", 880, 475, 20, WHITE);

    // Music button
    Color musicHoverColor = isHoveringMusic ? LIGHTGRAY : WHITE;

    DrawText("Music:", 910, 520, 18, WHITE);
    if (musicOn) {
        DrawTextureEx(musicOnTex, { (float)musicX, (float)musicY }, 0.0f, buttonScale, musicHoverColor);
    }
    else {
        DrawTextureEx(musicOffTex, { (float)musicX, (float)musicY }, 0.0f, buttonScale, musicHoverColor);
    }

    // Pause button
    Color pauseHoverColor = isHoveringPause ? LIGHTGRAY : WHITE;

    DrawText("Pause:", 1025, 520, 18, WHITE);
    if (gamePaused) {
        DrawTextureEx(playTex, { (float)pauseX, (float)pauseY }, 0.0f, buttonScale, pauseHoverColor);
    }
    else {
        DrawTextureEx(pauseTex, { (float)pauseX, (float)pauseY }, 0.0f, buttonScale, pauseHoverColor);
    }

    // Ghost toggle
    const char* fullText = ghostEnabled ? "Ghost ON" : "Ghost OFF";
    Color toggleBg = ghostEnabled ? GREEN : RED;

    float toggleWidth = 100.0f;
    float toggleHeight = 30.0f;
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
    Vector2 TextSize = MeasureTextEx(font, fullText, 14, 1);

    float textX;
    if (ghostEnabled) {
        textX = ghostToggleX + 10.0f;
    }
    else {
        textX = ghostToggleX + 30.0f;
    }

    float textY = ghostToggleY + (toggleHeight - TextSize.y) / 2;
    DrawTextEx(font, fullText, { textX, textY }, 14, 1, textColor);

    Color circleColor = isHoveringGhost ? Color{ 250, 250, 250, 255 } : WHITE;
    DrawCircle(circleX, ghostToggleY + toggleHeight / 2, 12.0f, circleColor);

    if (gameOver) {
        bool dummyRestart = false;  // These will be set by Update
        bool dummyReturnToMenu = false;
        DrawGameOverScreen(font, score, gameOver, dummyRestart, dummyReturnToMenu);
        return;
    }
}

void Manager::UnloadTextures() {
    UnloadTexture(musicOnTex);
    UnloadTexture(musicOffTex);
    UnloadTexture(pauseTex);
    UnloadTexture(playTex);
}

void Manager::CreateParticles() {
    particles.clear();

    // Create explosion of particles
    for (int i = 0; i < 100; i++) {
        GameOverParticle p;
        p.position = { 600.0f, 300.0f }; // Center of screen
        float angle = GetRandomValue(0, 360) * DEG2RAD;
        float speed = GetRandomValue(2, 8);
        p.velocity = { cosf(angle) * speed, sinf(angle) * speed };

        // Random colors (gold, red, orange, yellow)
        int colorChoice = GetRandomValue(0, 3);
        switch (colorChoice) {
        case 0: p.color = GOLD; break;
        case 1: p.color = RED; break;
        case 2: p.color = ORANGE; break;
        case 3: p.color = YELLOW; break;
        }

        p.size = GetRandomValue(3, 8);
        p.life = GetRandomValue(50, 100) / 100.0f; // 0.5 to 1.0
        particles.push_back(p);
    }
}

void Manager::UpdateParticles(float deltaTime) {
    for (auto& particle : particles) {
        particle.position.x += particle.velocity.x * deltaTime * 60.0f;
        particle.position.y += particle.velocity.y * deltaTime * 60.0f;
        particle.life -= deltaTime * 0.3f;

        // Apply gravity
        particle.velocity.y += 0.2f * deltaTime * 60.0f;

        // Fade out
        particle.color.a = (unsigned char)(particle.life * 255);
    }

    // Remove dead particles
    particles.erase(
        std::remove_if(particles.begin(), particles.end(),
            [](const GameOverParticle& p) { return p.life <= 0; }),
        particles.end()
    );

    // Add new particles occasionally
    if (particles.size() < 50 && GetRandomValue(0, 100) < 20) {
        GameOverParticle p;
        p.position = { (float)GetRandomValue(200, 1000), (float)GetRandomValue(100, 700) };
        float angle = GetRandomValue(0, 360) * DEG2RAD;
        float speed = GetRandomValue(1, 3);
        p.velocity = { cosf(angle) * speed, sinf(angle) * speed };
        p.color = Color{ 255, 215, 0, 200 }; // Gold
        p.size = GetRandomValue(2, 5);
        p.life = GetRandomValue(30, 80) / 100.0f;
        particles.push_back(p);
    }
}

void Manager::DrawParticles() {
    for (const auto& particle : particles) {
        DrawCircle(particle.position.x, particle.position.y, particle.size, particle.color);
        // Add glow effect
        DrawCircleLines(particle.position.x, particle.position.y, particle.size + 1,
            Color{ particle.color.r, particle.color.g, particle.color.b, 100 });
    }
}

void Manager::UpdateGameOverScreen(Vector2 mousePos, bool& gameOver, bool& restartRequested, bool& returnToMenuRequested) {
    // Create particles once when game over starts
    static bool particlesCreated = false;
    if (!particlesCreated) {
        CreateParticles();
        particlesCreated = true;
    }

    // Update button hover states
    isHoveringReplay = CheckCollisionPointRec(mousePos, replayButton);
    isHoveringMenu = CheckCollisionPointRec(mousePos, menuButton);

    // Handle button clicks
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        if (isHoveringReplay) {
            restartRequested = true;
            particlesCreated = false; // Reset for next game over
        }
        if (isHoveringMenu) {
            returnToMenuRequested = true;
            particlesCreated = false; // Reset for next game over
        }
    }
}

void Manager::DrawGameOverScreen(Font& font, int score, bool& gameOver, bool& restartRequested, bool& returnToMenuRequested) {
    // Draw semi-transparent overlay over entire screen
    DrawRectangle(0, 0, 1200, 800, Color{ 0, 0, 0, 180 });

    // Draw particles
    DrawParticles();

    // Calculate center position
    int centerX = 1200 / 2;
    int centerY = 800 / 2;

    // ============ MAIN PANEL ============
    int panelWidth = 600;
    int panelHeight = 500;
    int panelX = centerX - panelWidth / 2;
    int panelY = centerY - panelHeight / 2;

    // Draw gradient background
    for (int i = 0; i < panelHeight; i++) {
        float progress = (float)i / panelHeight;
        Color gradientColor = Color{
            (unsigned char)(30 + progress * 30),
            (unsigned char)(30 + progress * 60),
            (unsigned char)(100 + progress * 50),
            240
        };
        DrawRectangle(panelX, panelY + i, panelWidth, 1, gradientColor);
    }

    // Draw panel border with glow
    for (int i = 0; i < 4; i++) {
        DrawRectangleLinesEx(
            Rectangle{ (float)panelX - i, (float)panelY - i,
                      (float)panelWidth + i * 2, (float)panelHeight + i * 2 },
            2.0f, Color{ 255, 215, 0, (unsigned char)(100 - i * 25) } // Gold glow
        );
    }

    // Draw decorative corner accents
    int cornerSize = 20;
    DrawRectangle(panelX, panelY, cornerSize, 4, GOLD);
    DrawRectangle(panelX, panelY, 4, cornerSize, GOLD);
    DrawRectangle(panelX + panelWidth - cornerSize, panelY, cornerSize, 4, GOLD);
    DrawRectangle(panelX + panelWidth - 4, panelY, 4, cornerSize, GOLD);
    DrawRectangle(panelX, panelY + panelHeight - 4, cornerSize, 4, GOLD);
    DrawRectangle(panelX, panelY + panelHeight - cornerSize, 4, cornerSize, GOLD);
    DrawRectangle(panelX + panelWidth - cornerSize, panelY + panelHeight - 4, cornerSize, 4, GOLD);
    DrawRectangle(panelX + panelWidth - 4, panelY + panelHeight - cornerSize, 4, cornerSize, GOLD);

    // ============ GAME OVER TITLE ============
    const char* gameOverText = "GAME OVER";
    Vector2 titleSize = MeasureTextEx(font, gameOverText, 72, 3);

    // Title shadow
    DrawTextEx(font, gameOverText,
        { centerX - titleSize.x / 2 + 4, (float)panelY + 30 + 4 },
        72, 3, Color{ 0, 0, 0, 150 });

    // Fixed: Draw the entire title with gradient
    for (int i = 0; i < 9; i++) { // "GAME OVER" has 9 characters including space
        float progress = (float)i / 9.0f;
        Color charColor = Color{
            (unsigned char)(255 * (1.0f - progress)),
            (unsigned char)(100 * progress),
            (unsigned char)(50 * progress),
            255
        };

        // Calculate position for each character
        float charWidth = titleSize.x / 9.0f;
        float charX = centerX - titleSize.x / 2 + (i * charWidth);

        // Draw each character individually
        char singleChar[2] = { gameOverText[i], '\0' };
        DrawTextEx(font, singleChar,
            { charX, (float)panelY + 30 },
            72, 3, charColor);
    }

    // ============ SCORE DISPLAY ============
    const char* scoreLabel = "FINAL SCORE";
    Vector2 labelSize = MeasureTextEx(font, scoreLabel, 36, 2);
    DrawTextEx(font, scoreLabel,
        { centerX - labelSize.x / 2, (float)panelY + 140 },
        36, 2, Color{ 200, 200, 255, 255 });

    // Score value with glowing effect
    char scoreText[50];
    snprintf(scoreText, sizeof(scoreText), "%d", score);
    Vector2 scoreTextSize = MeasureTextEx(font, scoreText, 64, 3);

    // Glow effect
    float pulse = sin(GetTime() * 3) * 0.2f + 0.8f;
    for (int i = 0; i < 8; i++) {
        DrawTextEx(font, scoreText,
            { centerX - scoreTextSize.x / 2, (float)panelY + 180 },
            64, 3, Color{ 255, 215, 0, (unsigned char)(50 * pulse) }); // Gold glow
    }

    // Main score text
    DrawTextEx(font, scoreText,
        { centerX - scoreTextSize.x / 2, (float)panelY + 180 },
        64, 3, Color{ 255, 255, 100, 255 }); // Bright gold

    // ============ BUTTONS ============
    int buttonY = panelY + 300;
    int buttonSpacing = 50;

    // REPLAY BUTTON
    replayButton = { (float)centerX - 200, (float)buttonY, 150, 50 };
    Color replayColor = isHoveringReplay ? Color{ 100, 200, 100, 255 } : Color{ 50, 150, 50, 255 };

    // Button background with hover effect
    DrawRectangleRounded(replayButton, 0.3f, 8, replayColor);
    DrawRectangleRoundedLines(replayButton, 0.3f, 8, Color{ 255, 255, 255, 200 });

    // Button glow on hover
    if (isHoveringReplay) {
        for (int i = 1; i <= 3; i++) {
            DrawRectangleRoundedLines(
                Rectangle{ replayButton.x - i, replayButton.y - i,
                          replayButton.width + i * 2, replayButton.height + i * 2 },
                0.3f, 8, Color{ 100, 255, 100, (unsigned char)(100 - i * 30) }
            );
        }
    }

    // Button text
    const char* replayText = "REPLAY";
    Vector2 replayTextSize = MeasureTextEx(font, replayText, 32, 2);
    DrawTextEx(font, replayText,
        { replayButton.x + replayButton.width / 2 - replayTextSize.x / 2,
          replayButton.y + replayButton.height / 2 - replayTextSize.y / 2 },
        28, 2, WHITE);

    // MENU BUTTON
    menuButton = { (float)centerX + 50, (float)buttonY, 150, 50 };
    Color menuColor = isHoveringMenu ? Color{ 200, 100, 100, 255 } : Color{ 150, 50, 50, 255 };

    DrawRectangleRounded(menuButton, 0.3f, 8, menuColor);
    DrawRectangleRoundedLines(menuButton, 0.3f, 8, Color{ 255, 255, 255, 200 });

    // Button glow on hover
    if (isHoveringMenu) {
        for (int i = 1; i <= 3; i++) {
            DrawRectangleRoundedLines(
                Rectangle{ menuButton.x - i, menuButton.y - i,
                          menuButton.width + i * 2, menuButton.height + i * 2 },
                0.3f, 8, Color{ 255, 100, 100, (unsigned char)(100 - i * 30) }
            );
        }
    }

    // Button text
    const char* menuText = "MAIN MENU";
    Vector2 menuTextSize = MeasureTextEx(font, menuText, 32, 2);
    DrawTextEx(font, menuText,
        { menuButton.x +5+ menuButton.width / 2 - menuTextSize.x / 2,
          menuButton.y + menuButton.height / 2 - menuTextSize.y / 2 },
        28, 2, WHITE);

    // ============ DECORATIVE ELEMENTS ============
    // Sparkle effects
    float sparkleTime = GetTime();
    for (int i = 0; i < 5; i++) {
        float x = panelX + 20 + (i * 120);
        float y = panelY + 120 + sin(sparkleTime * 2 + i) * 10;
        float size = 3 + sin(sparkleTime * 3 + i) * 2;
        DrawCircle(x, y, size, Color{ 255, 255, 200, 200 });
        DrawCircle(x, y, size + 2, Color{ 255, 255, 200, 100 });
    }

    // Bottom decorative line
    DrawRectangle(panelX + 50, panelY + panelHeight - 40, panelWidth - 100, 2, GOLD);

    // Game over quote
    const char* quote = "Great effort! Ready for another round?";
    Vector2 quoteSize = MeasureTextEx(GetFontDefault(), quote, 20, 1);
    DrawTextEx(font, quote,
        { centerX -50 - quoteSize.x / 2, (float)panelY + panelHeight - 35 },
        25, 1, Color{ 200, 200, 255, 200 });
}

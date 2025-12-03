#include "Manager.h"
#include "Colours.h"
#include <iostream>
using namespace std;

Manager::Manager() : buttonScale(0.2f), musicX(0), musicY(0), pauseX(0), pauseY(0),
isHoveringMusic(false), isHoveringPause(false), isHoveringGhost(false),
ghostToggleX(0.0f), ghostToggleY(0.0f), musicAnimationProgress(0.0f), isMusicAnimating(false), isHoveringReplay(false), isHoveringMenu(false) {
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

void Manager::Draw(bool musicOn, bool gamePaused, int score, Font& font, bool ghostEnabled,
    float ghostAnimationProgress, double time, int lines, bool gameOver,
    Leaderboard& leaderboard, const Game& game, const PieceQueue& pieceQueue) {

    // ============ LEFT PANEL ============
    DrawHoldPanel(game);
    DrawControlsPanel();

    // ============ RIGHT PANEL ============
    DrawNextPiecesPanel(pieceQueue);

    // ============ GAME STATS PANEL ============
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

    // ============ GAME CONTROLS PANEL ============
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

    Color circleColor = isHoveringGhost ? GhostHover : WHITE;
    DrawCircle(circleX, ghostToggleY + toggleHeight / 2, 12.0f, circleColor);

    if (gameOver) {
        bool dummyRestart = false;
        bool dummyReturnToMenu = false;
        DrawGameOverScreen(font, score, gameOver, dummyRestart, dummyReturnToMenu, leaderboard);
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
        remove_if(particles.begin(), particles.end(),
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
        p.color = ParticleGold; // Gold
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

void Manager::DrawGameOverScreen(Font& font, int score, bool& gameOver, bool& restartRequested, bool& returnToMenuRequested, Leaderboard& leaderboard) {
    // Draw semi-transparent overlay over entire screen
    DrawRectangle(0, 0, 1200, 800, GameOverShadow);

    // Calculate center position
    int centerX = 1200 / 2;
    int centerY = 800 / 2;

    // ============ MAIN PANEL ============
    int panelWidth = 800;
    int panelHeight = 600;  // Original size
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
        72, 3, GameOverShadow);

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

    // ============ SIDE-BY-SIDE SCORES ============
    int scoresY = panelY + 140;
    int spacing = panelWidth * 0.35; // Distance between the two scores

    // FINAL SCORE (LEFT SIDE) - PROPERLY CENTERED
    const char* finalScoreLabel = "FINAL SCORE";
    Vector2 finalLabelSize = MeasureTextEx(font, finalScoreLabel, 36, 2);
    int finalScoreX = centerX - spacing / 2 - finalLabelSize.x / 2;

    DrawTextEx(font, finalScoreLabel,
        { (float)finalScoreX, (float)scoresY },
        36, 2, ScoreColour);

    // Final score value
    char finalScoreText[50];
    snprintf(finalScoreText, sizeof(finalScoreText), "%d", score);
    Vector2 finalScoreSize = MeasureTextEx(font, finalScoreText, 48, 3);

    // Center the score value under its label
    int finalScoreValueX = centerX - spacing / 2 - finalScoreSize.x / 2;

    // Main final score text - SINGLE DRAW CALL
    DrawTextEx(font, finalScoreText,
        { (float)finalScoreValueX, (float)scoresY + 35 },
        48, 3, ScoreColour);

    // BEST SCORE (RIGHT SIDE) - PROPERLY CENTERED
    int bestScore = leaderboard.GetHighestScore();
    const char* bestScoreLabel = "BEST SCORE";
    Vector2 bestLabelSize = MeasureTextEx(font, bestScoreLabel, 36, 2);
    int bestScoreX = centerX + spacing / 2 - bestLabelSize.x / 2;

    DrawTextEx(font, bestScoreLabel,
        { (float)bestScoreX, (float)scoresY },
        36, 2, ScoreColour);

    // Best score value
    char bestScoreText[50];
    snprintf(bestScoreText, sizeof(bestScoreText), "%d", bestScore);
    Vector2 bestScoreSize = MeasureTextEx(font, bestScoreText, 48, 3);

    // Center the score value under its label
    int bestScoreValueX = centerX + spacing / 2 - bestScoreSize.x / 2;

    // Main best score text - SINGLE DRAW CALL
    DrawTextEx(font, bestScoreText,
        { (float)bestScoreValueX, (float)scoresY + 35 },
        48, 3, ScoreColour);

    // ============ LEADERBOARD ============
    const char* leaderTitle = "TOP 5 SCORES";
    Vector2 leaderTitleSize = MeasureTextEx(font, leaderTitle, 28, 2);
    DrawTextEx(font, leaderTitle,
        { centerX - leaderTitleSize.x / 2, (float)panelY + 250 },
        28, 2, ScoreColour);

    // Get top 5 scores
    vector<int> topScores = leaderboard.GetTopScores(5);

    // Display each score
    for (int i = 0; i < 5; i++) {
        float yPos = panelY + 290 + (i * 35);

        // Rank number
        char rank[10];
        snprintf(rank, sizeof(rank), "%d.", i + 1);
        DrawTextEx(font, rank,
            { (float)centerX - 100, yPos },
            24, 1, WHITE);

        // Score or "--"
        char scoreDisplayText[20];
        if (i < topScores.size()) {
            snprintf(scoreDisplayText, sizeof(scoreDisplayText), "%d", topScores[i]);

            // Highlight if it's the current score
            
            Color scoreColor = WHITE;
            if (topScores[i] == score) {
                scoreColor = CurrentScoreHighlight; // Green for current
            }

            DrawTextEx(font, scoreDisplayText,
                { (float)centerX - 30, yPos },
                24, 1, scoreColor);
        }
        else {
            // Show "--" for empty slots
            DrawTextEx(font, "--",
                { (float)centerX - 30, yPos },
                24, 1, EmptyScoreSlot);
        }
    }

    // ============ BUTTONS ============
    // Position buttons at the bottom of the panel
    int buttonY = panelY + 520;

    // REPLAY BUTTON
    replayButton = { (float)centerX - 200, (float)buttonY, 150, 50 };
    Color replayColor = isHoveringReplay ? ReplayButtonHover : GREEN;

    // Button background with hover effect
    DrawRectangleRounded(replayButton, 0.3f, 8, replayColor);
    DrawRectangleRoundedLines(replayButton, 0.3f, 8, ReplayButtonGlow);

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
    Vector2 replayTextSize = MeasureTextEx(font, replayText, 28, 2);
    DrawTextEx(font, replayText,
        { replayButton.x + replayButton.width / 2 - replayTextSize.x / 2,
          replayButton.y + replayButton.height / 2 - replayTextSize.y / 2 },
        28, 2, WHITE);

    // MENU BUTTON
    menuButton = { (float)centerX + 50, (float)buttonY, 150, 50 };
    Color menuColor = isHoveringMenu ? MenuButtonHover :RED;

    DrawRectangleRounded(menuButton, 0.3f, 8, menuColor);
    DrawRectangleRoundedLines(menuButton, 0.3f, 8, MenuButtonGlow);

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
    Vector2 menuTextSize = MeasureTextEx(font, menuText, 28, 2);
    DrawTextEx(font, menuText,
        { menuButton.x + menuButton.width / 2 - menuTextSize.x / 2,
          menuButton.y + menuButton.height / 2 - menuTextSize.y / 2 },
        28, 2, WHITE);


    // Decorative separator line above leaderboard
    DrawRectangle(panelX + 50, panelY + 235, panelWidth - 100, 2, GOLD);

    DrawRectangle(panelX + 50, panelY + 475, panelWidth - 100, 2, GOLD);

    // Game over quote - Position above buttons
    const char* quote = "Great effort! Ready for another round?";
    Vector2 quoteSize = MeasureTextEx(font, quote, 20, 1);
    DrawTextEx(font, quote,
        { centerX - quoteSize.x / 2, (float)buttonY - 40 },
        20, 1, GameOverQuote);

    // Draw particles
    DrawParticles();
}


void Manager::SetGameReferences(Game* game, PieceQueue* queue) {
    gameRef = game;
    pieceQueueRef = queue;
}

void Manager::DrawHoldPanel(const Game& game) {
    // Hold Area (Top-left) - X: 50, Y: 100
    DrawRectangleRounded({ 50, 100, 200, 140 }, 0.3f, 6, PanelBlue);
    DrawText("HOLD", 110, 70, 24, WHITE);

    // For HOLD piece (simpler version):
    if (game.IsHolding()) {
        Piece tempHold = game.GetHoldPiece();
        tempHold.rowOffset = 0;
        tempHold.colOffset = 0;

        int holdCellSize = 25;
        vector<Position> tiles = tempHold.GetCellPositions();

        // Find min/max for centering
        int minRow = 100, maxRow = -100;
        int minCol = 100, maxCol = -100;
        for (const Position& item : tiles) {
            if (item.ROW < minRow) minRow = item.ROW;
            if (item.ROW > maxRow) maxRow = item.ROW;
            if (item.COL < minCol) minCol = item.COL;
            if (item.COL > maxCol) maxCol = item.COL;
        }

        int pieceWidth = maxCol - minCol + 1;
        int pieceHeight = maxRow - minRow + 1;

        // Center in hold box (200x140 at 50,100)
        int drawX = 150 - (pieceWidth * holdCellSize) / 2;  // 150 is center X
        int drawY = 170 - (pieceHeight * holdCellSize) / 2; // 170 is center Y

        // Adjust for I-piece
        if (tempHold.id == 3) {
            drawX -= 5;
            drawY += 5;
        }
        // Adjust for O-piece
        else if (tempHold.id == 4) {
            drawY += 5;
        }

        for (Position item : tiles) {
            DrawRectangle((item.COL - minCol) * holdCellSize + drawX,
                (item.ROW - minRow) * holdCellSize + drawY,
                holdCellSize - 1, holdCellSize - 1,
                tempHold.colours[tempHold.id]);
        }
        DrawText("HOLDING", 100, 215, 18, YELLOW);
    }
    else {
        DrawText("EMPTY", 110, 160, 20, LIGHTGRAY);
    }
}

void Manager::DrawControlsPanel() {
    // Controls Help (Bottom-left) - X: 50, Y: 500
    DrawRectangleRounded({ 50, 500, 200, 150 }, 0.3f, 6, PanelBlue);
    DrawText("CONTROLS", 80, 470, 20, WHITE);
    DrawText("H = Hold", 70, 520, 16, WHITE);
    DrawText("Space = Hard Drop", 70, 550, 16, WHITE);
    DrawText("Ctrl+Z = Undo", 70, 580, 16, WHITE);
}

void Manager::DrawNextPiecesPanel(const PieceQueue& pieceQueue) {
    // Next Pieces (Top-right) - X: 850, Y: 100
    DrawRectangleRounded({ 850, 100, 300, 320 }, 0.3f, 6, PanelBlue);
    DrawText("NEXT PIECES", 900, 70, 24, WHITE);

    vector<Piece> nextThree = pieceQueue.GetNextThree();

    // Draw each next piece vertically stacked
    for (int i = 0; i < nextThree.size() && i < 3; i++) {
        int yOffset = 140 + (i * 100); // Space them vertically

        // Draw label
        DrawText(TextFormat("Piece %d:", i + 1), 860, yOffset - 10, 18, WHITE);

        // Create temporary piece for drawing
        Piece tempPiece = nextThree[i];
        tempPiece.rowOffset = 0;
        tempPiece.colOffset = 0;

        int previewCellSize = 25;
        vector<Position> tiles = tempPiece.GetCellPositions();

        // Find min/max for centering
        int minRow = 100, maxRow = -100;
        int minCol = 100, maxCol = -100;
        for (const Position& item : tiles) {
            if (item.ROW < minRow) minRow = item.ROW;
            if (item.ROW > maxRow) maxRow = item.ROW;
            if (item.COL < minCol) minCol = item.COL;
            if (item.COL > maxCol) maxCol = item.COL;
        }
        int pieceWidth = maxCol - minCol + 1;
        int pieceHeight = maxRow - minRow + 1;

        // Center in each slot (300px wide box starting at X=850)
        // Each slot is roughly 100px tall, centered at yOffset + 50
        int drawX = 1000 - (pieceWidth * previewCellSize) / 2;  // 1000 is center X (850 + 300/2)
        int drawY = (yOffset + 40) - (pieceHeight * previewCellSize) / 2; // Center of 100px slot

        // Adjust for I-piece
        if (tempPiece.id == 3) {
            drawX -= 5;
            drawY += 5;
        }
        // Adjust for O-piece
        else if (tempPiece.id == 4) {
            drawY += 5;
        }

        for (Position item : tiles) {
            DrawRectangle((item.COL - minCol) * previewCellSize + drawX,
                (item.ROW - minRow) * previewCellSize + drawY,
                previewCellSize - 1, previewCellSize - 1,
                tempPiece.colours[tempPiece.id]);
        }
    }
}

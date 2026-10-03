#include "ui/Manager.h"
#include "ui/Colours.h"
#include <iostream>
using namespace std;

Manager::Manager() : buttonScale(0.2f), musicX(0), musicY(0), pauseX(0), pauseY(0),
isHoveringMusic(false), isHoveringPause(false), isHoveringGhost(false),
ghostToggleX(0.0f), ghostToggleY(0.0f), isHoveringReplay(false), isHoveringMenu(false) {
    LoadTextures();
    replayButton = { 0,0,200,60 };
    menuButton = { 0,0,200,60 };
}

Manager::~Manager() {
    UnloadTextures();
}

void Manager::LoadTextures() {
    musicOnTex = LoadTexture("assets/images/music-on.png");
    musicOffTex = LoadTexture("assets/images/music-off.png");
    pauseTex = LoadTexture("assets/images/pause.png");
    playTex = LoadTexture("assets/images/play.png");

    int scaledWidth = musicOnTex.width * buttonScale;

    //pos for controls (in the right panel)
    musicX = 900;
    musicY = 560;  

    pauseX = 1025;   
    pauseY = 560; 

    ghostToggleX = 950.0f;  
    ghostToggleY = 650.0f; 
}

void Manager::Draw(bool musicOn, bool gamePaused, int score, Font& font, bool ghostEnabled,
    float ghostAnimationProgress, double time, int lines, bool gameOver,
    Leaderboard& leaderboard, const Game& game, const PieceQueue& pieceQueue) {

    // left side
    DrawHoldPanel(game);
    DrawControlsPanel();

    //game stats panel - mid left
    DrawRectangleRounded({ 50, 280, 200, 180 }, 0.3f, 6, PanelBlue);
    DrawText("GAME STATS", 65, 250, 20, WHITE);

    // Score
    DrawTextEx(font, "Score:", { 70, 310 }, 28, 1, WHITE);
    char scoreText[20];
    //converting int to string
    snprintf(scoreText, sizeof(scoreText), "%d", score);
    DrawTextEx(font, scoreText, { 160, 310 }, 28, 1, WHITE);

    // Time
    DrawTextEx(font, "Time:", { 70, 350 }, 28, 1, WHITE);
    char timeText[20];
    int minutes = (int)time / 60;
    int seconds = (int)time % 60;
    snprintf(timeText, sizeof(timeText), "%02d:%02d", minutes, seconds);
    DrawTextEx(font, timeText, { 150, 350 }, 28, 1, WHITE);

    // Lines
    DrawTextEx(font, "Lines:", { 70, 390 }, 28, 1, WHITE);
    char linesText[20];
    snprintf(linesText, sizeof(linesText), "%d", lines);
    DrawTextEx(font, linesText, { 160, 390 }, 28, 1, WHITE);


    //right side
    DrawNextPiecesPanel(pieceQueue);

    //game controls panel
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
    float startX = ghostToggleX + 15.0f;  //left pos of the circle
    float endX = ghostToggleX + toggleWidth - 15.0f; //right pos 
    float progress = ghostAnimationProgress;
    //start=0   0.5=middle  1=end
    if (progress > 1.0f) progress = 1.0f;

    float circleX;
    //drawing circle according to the toggle and progress - animation
    if (ghostEnabled) {
        circleX = startX + (endX - startX) * progress;
    }
    else {
        circleX = endX - (endX - startX) * progress;
    }

    Color textColor = BLACK;
    Vector2 TextSize = MeasureTextEx(font, fullText, 14, 1);

    //toggle k hisaab se likhna hai (right/left)
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
        DrawGameOverScreen(font, score, dummyRestart, leaderboard);
        return;
    }
}

void Manager::Update(bool& musicOn, bool& gamePaused, bool isCountingDown, bool& ghostEnabled,
    float& ghostAnimationProgress, bool gameOver, bool& restartRequested, bool& returnToMenuRequested) {
    
    Vector2 mouse = GetMousePosition();

    // Only update game over screen if game is over - obviously
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
            ghostAnimationProgress = 0.0f;  //starting animation from the beginning
        }
    }
}



void Manager::UnloadTextures() {
    UnloadTexture(musicOnTex);
    UnloadTexture(musicOffTex);
    UnloadTexture(pauseTex);
    UnloadTexture(playTex);
}

void Manager::CreateParticles() {
    particles.clear();  //remove all the old ones

    //now creating 100
    for (int i = 0; i < 100; i++) {
        GameOverParticle p;
        p.position = { 600.0f, 300.0f }; //all start from center of the screen 
        float angle = GetRandomValue(0, 360) * DEG2RAD; //getting random angle and converting to radian
        float speed = GetRandomValue(2, 8); //random speed of each
        p.velocity = { cosf(angle) * speed, sinf(angle) * speed }; //cos is for x directon and sin is for y direction

        //getting random colours
        int colorChoice = GetRandomValue(0, 4); 
        switch (colorChoice) {
        case 0: p.color = GameOverBrightRed; break;
        case 1: p.color = GameOverBrightCyan; break;
        case 2: p.color = GameOverBorder; break;    
        case 3: p.color = GameOverPurple; break;
        case 4: p.color = ParticleGold; break; 
        }

        p.size = GetRandomValue(3, 8);  //smallest can be 3 pixels in diameter and largest can be of 8
        p.life = GetRandomValue(50, 100) / 100.0f; //for how long is it visible
        particles.push_back(p);
    }
}

void Manager::UpdateParticles(float deltaTime) {

    for (GameOverParticle& particle : particles) {

        //moving the particles according to the speed and time passed
        //velocity is per frame so multiplying frameTime by 60 (since we have set 60fps)
        particle.position.x += particle.velocity.x * deltaTime * 60.0f;
        particle.position.y += particle.velocity.y * deltaTime * 60.0f;

        //every second decrease 0.3 of the life
        particle.life -= deltaTime * 0.3f;

        // Apply gravity
        particle.velocity.y += 0.2f * deltaTime * 60.0f;

        // Fade out
        particle.color.a = static_cast<unsigned char>(particle.life * 255);
    }

    // Remove dead particles
    //looping from the bottom
    for (int i = particles.size() - 1; i >= 0; i--) {
        if (particles[i].life <= 0) {
            particles.erase(particles.begin() + i);
        }
    }

    // Add new particles occasionally
    if (particles.size() < 50 && GetRandomValue(0, 100) < 20) { //get random here becoz we don't want to add in every frame
        GameOverParticle p;
        p.position = { (float)GetRandomValue(200, 1000), (float)GetRandomValue(100, 700) };
        float angle = GetRandomValue(0, 360) * DEG2RAD;
        float speed = GetRandomValue(1, 3);
        p.velocity = { cosf(angle) * speed, sinf(angle) * speed };
        p.color = ParticleGold;
        p.size = GetRandomValue(2, 5);
        p.life = GetRandomValue(30, 80) / 100.0f;
        particles.push_back(p);
    }
}

void Manager::DrawParticles() {
    for (const auto& particle : particles) {
        DrawCircle(particle.position.x, particle.position.y, particle.size, particle.color);     //size is radius in pixels
        //border around the circle of the same color jsut a bit transparent
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

void Manager::DrawGameOverScreen(Font& font, int score, bool& restartRequested, Leaderboard& leaderboard) {
    //a dark overlay over the game screen
    DrawRectangle(0, 0, 1200, 800, GameOverShadow);

    int centerX = 1200 / 2;     //center calculation
    int centerY = 800 / 2;

    //main box
    int panelWidth = 800;
    int panelHeight = 600;
    int panelX = centerX - panelWidth / 2;
    int panelY = centerY - panelHeight / 2;


    // outer border - gold
    DrawRectangleRoundedLines(
        { (float)panelX, (float)panelY, (float)panelWidth, (float)panelHeight },
        0.1f, 8, GameOverBorder
    );

    // Dark blue panel - main game over box
    DrawRectangleRounded({ (float)panelX, (float)panelY, (float)panelWidth, (float)panelHeight },
        0.1f, 8, GameOverDarkBlue);

    //inner border - blue
    int innerPadding = 20;
    //*2 becoz of both sides
    DrawRectangleRoundedLines(
        { (float)panelX + innerPadding, (float)panelY + innerPadding,
          (float)panelWidth - innerPadding * 2, (float)panelHeight - innerPadding * 2 },
        0.1f, 8, BlueBorder );

    //line separators
    DrawRectangle(panelX + 50, panelY + 230, panelWidth - 100, 2, GameOverLineGold);
    DrawRectangle(panelX + 50, panelY + 470, panelWidth - 100, 2, GameOverLineGold);

    //title
    const char* gameOverText = "GAME OVER";
    Vector2 titleSize = MeasureTextEx(font, gameOverText, 72, 3);
    //shadow
    DrawTextEx(font, gameOverText,
        { centerX - titleSize.x / 2 + 3, (float)panelY + 28 + 3 },
        72, 3, BLACK);
    //original text
    DrawTextEx(font, gameOverText,
        { centerX - titleSize.x / 2, (float)panelY + 28 },
        72, 3, GameOverBrightRed);

    //scores
    int scoresY = panelY + 140;
    int spacing = panelWidth * 0.35;  //between left/right scores

    //final score
    const char* finalScoreLabel = "FINAL SCORE";
    Vector2 finalLabelSize = MeasureTextEx(font, finalScoreLabel, 36, 2);
    int finalScoreX = centerX - spacing / 2 - finalLabelSize.x / 2;

    DrawTextEx(font, finalScoreLabel,
        { (float)finalScoreX, (float)scoresY },
        36, 2, GameOverTextWhite);

    char finalScoreText[50];
    snprintf(finalScoreText, sizeof(finalScoreText), "%d", score);
    Vector2 finalScoreSize = MeasureTextEx(font, finalScoreText, 48, 3);
    int finalScoreValueX = centerX - spacing / 2 - finalScoreSize.x / 2;

    DrawTextEx(font, finalScoreText,
        { (float)finalScoreValueX, (float)scoresY + 35 },
        48, 3, GameOverBrightCyan);

    //best score on the right
    int bestScore = leaderboard.GetHighestScore();
    const char* bestScoreLabel = "BEST SCORE";
    Vector2 bestLabelSize = MeasureTextEx(font, bestScoreLabel, 36, 2);
    int bestScoreX = centerX + spacing / 2 - bestLabelSize.x / 2;

    DrawTextEx(font, bestScoreLabel,
        { (float)bestScoreX, (float)scoresY },
        36, 2, GameOverTextWhite);

    char bestScoreText[50];
    snprintf(bestScoreText, sizeof(bestScoreText), "%d", bestScore);
    Vector2 bestScoreSize = MeasureTextEx(font, bestScoreText, 48, 3);
    int bestScoreValueX = centerX + spacing / 2 - bestScoreSize.x / 2;

    DrawTextEx(font, bestScoreText,
        { (float)bestScoreValueX, (float)scoresY + 35 },
        48, 3, GameOverBestScoreGold);  

    //leaderboard
    const char* leaderTitle = "TOP 5 SCORES";
    Vector2 leaderTitleSize = MeasureTextEx(font, leaderTitle, 28, 2);
    DrawTextEx(font, leaderTitle,
        { centerX - leaderTitleSize.x / 2, (float)panelY + 250 },
        28, 2, ScoreColour);

    vector<int> topScores = leaderboard.GetTopScores(5);

    for (int i = 0; i < 5; i++) {
        float yPos = panelY + 290 + (i * 35);   //line has 35 pixels gap

        char rank[10];
        //printing 1,2,3,4,5
        snprintf(rank, sizeof(rank), "%d.", i + 1);
        DrawTextEx(font, rank,
            { (float)centerX - 100, yPos },
            24, 1, ScoreColour);  

        if (i < topScores.size()) {
            char scoreDisplayText[20];
            snprintf(scoreDisplayText, sizeof(scoreDisplayText), "%d", topScores[i]);

            //of topscore is current score it will be in green
            Color scoreColor = (topScores[i] == score) ? CurrentScoreHighlight : ScoreColour;
            DrawTextEx(font, scoreDisplayText,
                { (float)centerX - 30, yPos },
                24, 1, scoreColor);
        }
        else {
            DrawTextEx(font, "--",
                { (float)centerX - 30, yPos },
                24, 1, EmptyScoreSlot);
        }
    }

    //buttons
    int buttonY = panelY + 520;

    //replay button
    replayButton = { (float)centerX - 200, (float)buttonY, 150, 50 };
    Color replayColor = isHoveringReplay ?  GameOverBrightCyan: PanelBlue;
    DrawRectangleRounded(replayButton, 0.3f, 8, replayColor);
    DrawRectangleRoundedLines(replayButton, 0.3f, 8, GameOverBorder);

    const char* replayText = "REPLAY";
    Vector2 replayTextSize = MeasureTextEx(font, replayText, 28, 2);
    DrawTextEx(font, replayText,
        { replayButton.x + replayButton.width / 2 - replayTextSize.x / 2,
          replayButton.y + replayButton.height / 2 - replayTextSize.y / 2 },
        28, 2, WHITE);

    //menu button
    menuButton = { (float)centerX + 50, (float)buttonY, 150, 50 };
    Color menuColor = isHoveringMenu ? GameOverPurpleHover : GameOverPurple;
    DrawRectangleRounded(menuButton, 0.3f, 8, menuColor);
    DrawRectangleRoundedLines(menuButton, 0.3f, 8, GameOverBorder);

    const char* menuText = "MAIN MENU";
    Vector2 menuTextSize = MeasureTextEx(font, menuText, 28, 2);
    DrawTextEx(font, menuText,
        { menuButton.x + menuButton.width / 2 - menuTextSize.x / 2,
          menuButton.y + menuButton.height / 2 - menuTextSize.y / 2 },
        28, 2, WHITE);

    const char* quote = "Great effort! Ready for another round?";
    Vector2 quoteSize = MeasureTextEx(font, quote, 20, 1);
    DrawTextEx(font, quote,
        { centerX - quoteSize.x / 2, (float)buttonY - 40 },
        20, 1, GameOverQuote);

    DrawParticles();    //drawing particles on TOP of the game over screen
}


void Manager::DrawHoldPanel(const Game& game) {
    // Hold Area (Top-left)
    DrawRectangleRounded({ 50, 100, 200, 140 }, 0.3f, 6, PanelBlue);
    DrawText("HOLD", 110, 70, 24, WHITE);

    if (game.IsHolding()) { //if holding get that piece
        Piece tempHold = game.GetHoldPiece();
        tempHold.rowOffset = 0;
        tempHold.colOffset = 0;

        int holdCellSize = 25;  //thora chota 
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
            //calculating positions for every piece
            //-1 -1 for border
            DrawRectangle((item.COL - minCol) * holdCellSize + drawX,
                (item.ROW - minRow) * holdCellSize + drawY,
                holdCellSize - 1, holdCellSize - 1,
                tempHold.colours[tempHold.id]);
        }
        DrawText("HOLDING", 100, 215, 18, YELLOW);
    }
    else {
        DrawText("EMPTY", 110, 160, 20, LIGHTGRAY); //not holding any piece
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

        // Draw label for every piece
        DrawText(TextFormat("Piece %d:", i + 1), 860, yOffset - 10, 18, WHITE);

        // Create temporary piece for drawing
        Piece tempPiece = nextThree[i];
        tempPiece.rowOffset = 0;
        tempPiece.colOffset = 0;

        int previewCellSize = 25;
        vector<Position> tiles = tempPiece.GetCellPositions();

        // Find min/max for centering same as in hold
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

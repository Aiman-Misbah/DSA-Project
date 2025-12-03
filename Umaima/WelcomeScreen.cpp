#include "WelcomeScreen.h"
#include "Colours.h"
#include <iostream>
#include <random>

using namespace std;

WelcomeScreen::WelcomeScreen() :
    timeElapsed(0),
    animationComplete(false),
    wigglePhase(false),      // Initialize wiggle phase
    isHoveringPlay(false),
    buttonColor(PanelBlue), showInstructions(false) {
    Load();
}

WelcomeScreen::~WelcomeScreen() {
    Unload();
}

void WelcomeScreen::Load() {
    font = LoadFontEx("Font/monogram.ttf", 64, 0, 0);

    // Create background - MATCH WINDOW SIZE
    Image img = GenImageColor(1200, 800, DarkBlue);
    background = LoadTextureFromImage(img);
    UnloadImage(img);

    // Create letters T-E-T-R-I-S with CENTERED position
    float totalWidth = 6 * 100.0f - 50.0f;  // "TETRIS" = 6 letters
    float startX = (1200.0f - totalWidth) / 2.0f;  // Center calculation
    float startY = 150.0f;
    float letterSpacing = 100.0f;

    CreateLetterT(startX, startY);
    CreateLetterE(startX + letterSpacing, startY);
    CreateLetterT(startX + letterSpacing * 2, startY);
    CreateLetterR(startX + letterSpacing * 3, startY);
    CreateLetterI(startX + letterSpacing * 4 - 25, startY);
    CreateLetterS(startX + letterSpacing * 5 - 50, startY);

    // Button position - CENTERED
    playButtonX = (1200 - 200) / 2;  // Center 200px button in 1200px window
    playButtonY = 450;
    instructionsButtonX = (1200 - 200) / 2;
    instructionsButtonY = 550;
}

void WelcomeScreen::CreateLetterT(float startX, float startY) {
    vector<Vector2> positions = {
        {startX, startY}, {startX + 30.0f, startY}, {startX + 60.0f, startY},  // Top bar
        {startX + 30.0f, startY + 30.0f}, {startX + 30.0f, startY + 60.0f}, {startX + 30.0f, startY + 90.0f}, {startX + 30.0f, startY + 120.0f}  // Vertical
    };

    for (const auto& pos : positions) {
        FallingBlock block;
        block.targetPosition = pos;
        block.originalPosition = pos; // Store original position for wiggle
        block.position = { pos.x, -100.0f };
        block.speed = 300.0f + (rand() % 100);
        block.color = TetrisRed; // Red
        block.arrived = false;
        block.delay = (rand() % 100) / 100.0f;
        block.wiggleTime = 0;
        block.wiggleOffset = (rand() % 100) / 25.0f; // Random offset for variety
        blocks.push_back(block);
    }
}

// In CreateLetterE, CreateLetterR, CreateLetterI, CreateLetterS - 
// Update each block creation to include the new fields:

void WelcomeScreen::CreateLetterE(float startX, float startY) {
    vector<Vector2> positions = {
        {startX, startY}, {startX + 30.0f, startY}, {startX + 60.0f, startY},  // Top
        {startX, startY + 30.0f},  // Middle left
        {startX, startY + 60.0f}, {startX + 30.0f, startY + 60.0f}, {startX + 60.0f, startY + 60.0f},  // Bottom
        {startX, startY + 90.0f}, {startX, startY + 120}, {startX + 30.0f, startY + 120}, {startX + 60.0f, startY + 120} // Extra for height
    };

    for (const auto& pos : positions) {
        FallingBlock block;
        block.targetPosition = pos;
        block.originalPosition = pos;
        block.position = { pos.x, -100.0f };
        block.speed = 300.0f + (rand() % 100);
        block.color = TetrisGreen; // Green
        block.arrived = false;
        block.delay = (rand() % 100) / 100.0f;
        block.wiggleTime = 0;
        block.wiggleOffset = (rand() % 100) / 25.0f;
        blocks.push_back(block);
    }
}

void WelcomeScreen::CreateLetterR(float startX, float startY) {
    vector<Vector2> positions = {
        {startX, startY}, {startX + 30.0f, startY}, {startX + 60.0f, startY},  // Top
        {startX, startY + 30.0f}, {startX + 60.0f, startY + 30.0f},  // Middle
        {startX, startY + 60.0f}, {startX + 30.0f, startY + 60.0f},  // Bottom left
        {startX + 60.0f, startY + 90.0f}, {startX + 60.0f, startY + 120.0f},  // Leg
        {startX, startY + 90.0f},{startX, startY + 120.0f} // Vertical
    };

    for (const auto& pos : positions) {
        FallingBlock block;
        block.targetPosition = pos;
        block.originalPosition = pos;
        block.position = { pos.x, -100.0f };
        block.speed = 300.0f + (rand() % 100);
        block.color = TetrisBlue; // Blue
        block.arrived = false;
        block.delay = (rand() % 100) / 100.0f;
        block.wiggleTime = 0;
        block.wiggleOffset = (rand() % 100) / 25.0f;
        blocks.push_back(block);
    }
}

void WelcomeScreen::CreateLetterI(float startX, float startY) {
    vector<Vector2> positions = {
        {startX + 30.0f, startY},  // Top
        {startX + 30.0f, startY + 30.0f}, {startX + 30.0f, startY + 60.0f},{startX + 30.0f, startY + 90.0f},  // Middle
        {startX + 30.0f, startY + 120.0f}  // Bottom
    };

    for (const auto& pos : positions) {
        FallingBlock block;
        block.targetPosition = pos;
        block.originalPosition = pos;
        block.position = { pos.x, -100.0f };
        block.speed = 300.0f + (rand() % 100);
        block.color = TetrisYellow; // Yellow
        block.arrived = false;
        block.delay = (rand() % 100) / 100.0f;
        block.wiggleTime = 0;
        block.wiggleOffset = (rand() % 100) / 25.0f;
        blocks.push_back(block);
    }
}

void WelcomeScreen::CreateLetterS(float startX, float startY) {
    vector<Vector2> positions = {
        {startX, startY}, {startX + 30.0f, startY}, {startX + 60.0f, startY},  // Top right
        {startX, startY + 30.0f},  // Middle
        {startX, startY + 60.0f}, {startX + 30.0f, startY + 60.0f}, {startX + 60.0f, startY + 60.0f},
        {startX + 60.0f, startY + 90.0f},
        {startX, startY + 120.0f}, {startX + 30.0f, startY + 120.0f}, {startX + 60.0f, startY + 120.0f}  // Bottom
    };

    for (const auto& pos : positions) {
        FallingBlock block;
        block.targetPosition = pos;
        block.originalPosition = pos;
        block.position = { pos.x, -100.0f };
        block.speed = 300.0f + (rand() % 100);
        block.color = TetrisPurple; // Purple
        block.arrived = false;
        block.delay = (rand() % 100) / 100.0f;
        block.wiggleTime = 0;
        block.wiggleOffset = (rand() % 100) / 25.0f;
        blocks.push_back(block);
    }
}

void WelcomeScreen::Update(bool& startGame, bool& showInstructionsScreen) {
    timeElapsed += GetFrameTime();

    // Update falling blocks
    bool allArrived = true;
    for (auto& block : blocks) {
        if (block.delay > 0) {
            block.delay -= GetFrameTime();
            allArrived = false;
            continue;
        }

        if (!block.arrived) {
            block.position.y += block.speed * GetFrameTime();
            if (block.position.y >= block.targetPosition.y) {
                block.position.y = block.targetPosition.y;
                block.arrived = true;
            }
            allArrived = false;
        }
    }

    // Start wiggle phase when all blocks have arrived
    if (allArrived && !wigglePhase) {
        wigglePhase = true;
        animationComplete = true;
    }

    // Wiggle animation
    if (wigglePhase) {
        for (auto& block : blocks) {
            block.wiggleTime += GetFrameTime();

            // Wiggle effect: small circular motion around original position
            float wiggleX = sinf(block.wiggleTime * 6.0f + block.wiggleOffset) * 2.0f;
            float wiggleY = cosf(block.wiggleTime * 5.0f + block.wiggleOffset) * 1.5f;

            block.position.x = block.originalPosition.x + wiggleX;
            block.position.y = block.originalPosition.y + wiggleY;
        }
    }

    // Check for clicks on buttons (only when animation complete)
    if (animationComplete) {
        Vector2 mouse = GetMousePosition();

        Rectangle playBounds = { (float)playButtonX, (float)playButtonY, 200, 80 };
        isHoveringPlay = CheckCollisionPointRec(mouse, playBounds);

        Rectangle instructionsBounds = { (float)instructionsButtonX, (float)instructionsButtonY, 200, 80 };
        isHoveringInstructions = CheckCollisionPointRec(mouse, instructionsBounds);

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            if (CheckCollisionPointRec(mouse, playBounds)) {
                startGame = true;
            }
            else if (CheckCollisionPointRec(mouse, instructionsBounds)) {
                showInstructionsScreen = true;  // NEW: Show instructions
            }
        }
    }
}
void WelcomeScreen::Draw() {
    // Draw background
    DrawTexture(background, 0, 0, WHITE);

    // Draw falling blocks
    for (const auto& block : blocks) {
        if (block.delay <= 0) {
            DrawRectangle(block.position.x, block.position.y, 28, 28, block.color);
            // Draw border
            DrawRectangleLines(block.position.x, block.position.y, 28, 28, BLACK);
        }
    }

    // Draw buttons only when animation is complete
    if (animationComplete) {
        // PLAY button
        Color playButtonColor = isHoveringPlay ? LIGHTGRAY : buttonColor;
        DrawRectangleRounded({ (float)playButtonX, (float)playButtonY, 200, 80 }, 0.3f, 6, playButtonColor);

        const char* playText = "PLAY";
        Vector2 playTextSize = MeasureTextEx(font, playText, 38, 2);
        DrawTextEx(font, playText,
            { playButtonX + (200 - playTextSize.x) / 2,
              playButtonY + (80 - playTextSize.y) / 2 },
            38, 2, WHITE);

        // NEW: INSTRUCTIONS button
        Color instructionsButtonColor = isHoveringInstructions ? LIGHTGRAY : buttonColor;
        DrawRectangleRounded({ (float)instructionsButtonX, (float)instructionsButtonY, 200, 80 }, 0.3f, 6, instructionsButtonColor);

        const char* instructionsText = "INSTRUCTIONS";
        Vector2 instructionsTextSize = MeasureTextEx(font, instructionsText, 24, 1);
        DrawTextEx(font, instructionsText,
            { instructionsButtonX + (200 - instructionsTextSize.x) / 2,
              instructionsButtonY + (80 - instructionsTextSize.y) / 2 },
            24, 1, WHITE);

        // Basic instructions below buttons
        const char* basicInstructions = "Use ARROW KEYS to move and rotate pieces";
        Vector2 basicInstrSize = MeasureTextEx(font, basicInstructions, 20, 1);
        DrawTextEx(font, basicInstructions,
            { (1200 - basicInstrSize.x) / 2, 650 },
            20, 1, WHITE);
    }
}


// NEW: Draw instructions screen
void WelcomeScreen::DrawInstructions() {
    // Draw semi-transparent overlay
    DrawRectangle(0, 0, 1200, 800, { 0, 0, 0, 200 });  // Changed from 1000 to 1200

    // Draw instructions box - CENTERED in 1200px window
    float boxWidth = 700.0f;
    float boxHeight = 550.0f;
    float boxX = (1200.0f - boxWidth) / 2.0f;  // Center calculation
    float boxY = 125.0f;

    DrawRectangleRounded({ boxX, boxY, boxWidth, boxHeight }, 0.3f, 6, buttonColor);
    DrawRectangleRoundedLines({ boxX, boxY, boxWidth, boxHeight }, 0.3f, 3, WHITE);

    // Title - centered in the window
    const char* title = "HOW TO PLAY";
    Vector2 titleSize = MeasureTextEx(font, title, 48, 2);
    DrawTextEx(font, title, { (1200.0f - titleSize.x) / 2.0f, boxY + 30 }, 48, 2, WHITE);

    // Instructions content
    vector<string> instructions = {
        "CONTROLS:",
        "← → ARROWS  : Move piece left/right",
        "↑ ARROW     : Rotate piece",
        "↓ ARROW     : Soft drop (move down faster)",
        "SPACEBAR    : Hard drop (instant drop)",
        "",
        "GAME FEATURES:",
        "- Ghost piece shows where piece will land",
        "- Wall kicks help with rotation near walls",
        "- Clear multiple lines for higher scores",
        "- Music can be toggled on/off",
        "- Game can be paused anytime",
        "",
        "SCORING:",
        "1 line  : 100 points",
        "2 lines : 200 points",
        "3 lines : 500 points",
        "4 lines : 800 points",
        "5+ lines: 800 + 100 per extra line"
    };

    float yPos = boxY + 100;
    for (const string& line : instructions) {
        Vector2 lineSize = MeasureTextEx(font, line.c_str(), 22, 1);
        DrawTextEx(font, line.c_str(), { (1200.0f - lineSize.x) / 2.0f, yPos }, 22, 1, WHITE);
        yPos += 30;
    }

    // Back button - centered below instructions
    Rectangle backBounds = { (1200.0f - 200.0f) / 2.0f, boxY + boxHeight - 80, 200, 60 };
    bool isHoveringBack = CheckCollisionPointRec(GetMousePosition(), backBounds);
    Color backColor = isHoveringBack ? LIGHTGRAY : InstructionsBackButton;

    DrawRectangleRounded(backBounds, 0.3f, 6, backColor);
    const char* backText = "BACK TO MENU";
    Vector2 backTextSize = MeasureTextEx(font, backText, 24, 1);
    DrawTextEx(font, backText,
        { backBounds.x + (backBounds.width - backTextSize.x) / 2,
          backBounds.y + (backBounds.height - backTextSize.y) / 2 },
        24, 1, WHITE);
}

void WelcomeScreen::Unload() {
    UnloadFont(font);
    UnloadTexture(background);
}

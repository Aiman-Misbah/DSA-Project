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

    // Try to load the wallpaper
    background = LoadTexture("Images/wallpaper.jpg");

    // Check if texture loaded successfully
    if (background.id == 0) {
        cout << "ERROR: Could not load Images/wallpaper.jpg!" << endl;
        cout << "Creating fallback background..." << endl;

        // Create a simple blue gradient as fallback using correct Raylib function
        Image fallbackImg = GenImageColor(1200, 800, DarkBlue);

        // Manually create vertical gradient
        for (int y = 0; y < 800; y++) {
            float ratio = (float)y / 800.0f;
            Color color = {
                (unsigned char)(50 + ratio * 50),    // R: 50 to 100
                (unsigned char)(100 + ratio * 50),   // G: 100 to 150
                (unsigned char)(180 + ratio * 40),   // B: 180 to 220
                255
            };

            for (int x = 0; x < 1200; x++) {
                ImageDrawPixel(&fallbackImg, x, y, color);
            }
        }

        background = LoadTextureFromImage(fallbackImg);
        UnloadImage(fallbackImg);

        cout << "Using fallback background" << endl;
    }
    else {
        cout << "Successfully loaded wallpaper.jpg: "
            << background.width << "x" << background.height << endl;

        // Check image size
        if (background.width != 1200 || background.height != 800) {
            cout << "Image is " << background.width << "x" << background.height
                << " - will be scaled to fit 1200x800 window" << endl;
        }
    }

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
                showInstructionsScreen = true;  // Show instructions
            }
        }
    }

    // ADD THIS SECTION for handling back button in instructions screen
    if (showInstructionsScreen) {
        Vector2 mouse = GetMousePosition();

        // Calculate the same back button bounds as in DrawInstructions()
        float boxWidth = 1150.0f;
        float boxHeight = 750.0f;
        float boxX = (1200.0f - boxWidth) / 2.0f;
        float boxY = 25.0f;

        Rectangle backBounds = {
            (1200.0f - 300.0f) / 2.0f,
            boxY + boxHeight - 70,
            300,
            60
        };

        // Check for back button click
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) &&
            CheckCollisionPointRec(mouse, backBounds)) {
            showInstructionsScreen = false;  // Go back to main menu
        }
    }
}

// Add this new method to WelcomeScreen.cpp:

void WelcomeScreen::UpdateInstructions(bool& showInstructionsScreen) {
    Vector2 mouse = GetMousePosition();

    // Calculate the same back button bounds as in DrawInstructions()
    float boxWidth = 1150.0f;
    float boxHeight = 750.0f;
    float boxX = (1200.0f - boxWidth) / 2.0f;
    float boxY = 25.0f;

    Rectangle backBounds = {
        (1200.0f - 300.0f) / 2.0f,
        boxY + boxHeight - 70,
        300,
        60
    };

    // Check for back button click
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) &&
        CheckCollisionPointRec(mouse, backBounds)) {
        showInstructionsScreen = false;  // Go back to main menu
    }
}

void WelcomeScreen::Draw() {
    // Draw background - SCALED TO FIT
    // Calculate scaling to fill the window while maintaining aspect ratio
    float scaleX = 1200.0f / background.width;
    float scaleY = 800.0f / background.height;
    float scale = max(scaleX, scaleY);  // Use max to fill the entire window

    float scaledWidth = background.width * scale;
    float scaledHeight = background.height * scale;

    // Calculate position to center the scaled image
    float posX = (1200 - scaledWidth) / 2.0f;
    float posY = (800 - scaledHeight) / 2.0f;

    // Draw the scaled background
    DrawTextureEx(background, Vector2{ posX, posY }, 0.0f, scale, WHITE);

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

    }
}

// Update in WelcomeScreen.cpp - DrawInstructions() method:

void WelcomeScreen::DrawInstructions() {
    // Draw semi-transparent overlay
    DrawRectangle(0, 0, 1200, 800, { 0, 0, 0, 180 });

    // Draw large box - max size for window
    float boxWidth = 1150.0f;
    float boxHeight = 750.0f;
    float boxX = (1200.0f - boxWidth) / 2.0f;
    float boxY = 25.0f;



    DrawRectangleRounded({ boxX, boxY, boxWidth, boxHeight }, 0.3f, 6, instructionBlue);
    DrawRectangleRoundedLines({ boxX, boxY, boxWidth, boxHeight }, 0.3f, 3, GameOverBorder);

    // Title
    const char* title = "HOW TO PLAY TETRIS";
    Vector2 titleSize = MeasureTextEx(font, title, 42, 2);
    DrawTextEx(font, title,
        { (1200.0f - titleSize.x) / 2.0f, boxY + 30 },
        42, 2, GameOverBrightRed);

    // Draw divider line
    DrawRectangle(boxX + 50, boxY + 90, boxWidth - 100, 2, GameOverLineGold);

    // Good balance of information - not too minimal, not too wordy
    vector<string> leftColumn = {
        "KEYBOARD CONTROLS",
        "",
        "Arrow Keys     Move piece left/right",
        "Up Arrow       Rotate piece",
        "Down Arrow     Soft drop (faster)",
        "Spacebar       Hard drop (instant)",
        "H Key          Hold/swap current piece",
        "Ctrl + Z       Undo last locked piece",
        "",
        "MOUSE CONTROLS",
        "",
        "Music Button   Toggle game music",
        "Pause Button   Pause/resume game",
        "Ghost Toggle   Show/hide ghost guide"
    };

    vector<string> rightColumn = {
        "GAME FEATURES",
        "",
        "Ghost piece shows where piece will land",
        "Hold piece system - save for later",
        "Undo last locked piece",
        "Preview next 3 pieces",
        "7-bag randomizer system",
        "Line clear animations",
        "",
        "SCORING",
        "",
        "1 line cleared   100 points",
        "2 lines cleared  200 points",
        "3 lines cleared  500 points",
        "4 lines cleared  800 points",
        "5+ lines         800 + 100 per line"
    };

    // Calculate text sizes first to ensure fit
    float leftX = boxX + 60;
    float rightX = boxX + boxWidth / 2 + 30;
    float yPos = boxY + 120;

    // Left column - with proper spacing
    for (const string& line : leftColumn) {
        if (line.empty()) {
            yPos += 20;
            continue;
        }

        bool isHeader = (line == "KEYBOARD CONTROLS" || line == "MOUSE CONTROLS");

        Color textColor = isHeader ? GameOverBrightCyan : GameOverTextWhite;
        int fontSize = isHeader ? 30 : 26;

        // Check if line will fit
        Vector2 lineSize = MeasureTextEx(font, line.c_str(), fontSize, 1);
        if (lineSize.x > (boxWidth / 2 - 80)) {
            // If too long, use smaller font
            fontSize = 24;
        }

        DrawTextEx(font, line.c_str(), { leftX, yPos }, fontSize, 1, textColor);
        yPos += isHeader ? 40 : 34;
    }

    // Right column
    yPos = boxY + 120;

    for (const string& line : rightColumn) {
        if (line.empty()) {
            yPos += 20;
            continue;
        }

        bool isHeader = (line == "GAME FEATURES" || line == "SCORING");

        Color textColor = isHeader ? GameOverBrightCyan : GameOverTextWhite;
        int fontSize = isHeader ? 30 : 26;

        // Check if line will fit
        Vector2 lineSize = MeasureTextEx(font, line.c_str(), fontSize, 1);
        if (lineSize.x > (boxWidth / 2 - 80)) {
            // If too long, use smaller font
            fontSize = 24;
        }

        DrawTextEx(font, line.c_str(), { rightX, yPos }, fontSize, 1, textColor);
        yPos += isHeader ? 35 : 30;
    }

    // Tips at bottom with better spacing
    float tipsY = boxY + boxHeight - 150;

    const char* tip1 = "TIP: Use ghost piece for precise placement!";
    const char* tip2 = "TIP: Hold difficult pieces for better positioning!";

    Vector2 tip1Size = MeasureTextEx(font, tip1, 22, 1);
    Vector2 tip2Size = MeasureTextEx(font, tip2, 22, 1);

    DrawTextEx(font, tip1,
        { (1200.0f - tip1Size.x) / 2.0f, tipsY },
        22, 1, TetrisYellow);

    DrawTextEx(font, tip2,
        { (1200.0f - tip2Size.x) / 2.0f, tipsY + 28 },
        22, 1, TetrisYellow);

    // Back button - properly sized
    Rectangle backBounds = {
        (1200.0f - 300.0f) / 2.0f,
        boxY + boxHeight - 70,
        300,
        60
    };

    bool isHoveringBack = CheckCollisionPointRec(GetMousePosition(), backBounds);
    Color backColor = isHoveringBack ? GameOverPurpleHover : GameOverPurple;

    DrawRectangleRounded(backBounds, 0.3f, 8, backColor);
    DrawRectangleRoundedLines(backBounds, 0.3f, 8, GameOverBorder);

    const char* backText = "BACK TO MAIN MENU";
    Vector2 backTextSize = MeasureTextEx(font, backText, 26, 1);
    DrawTextEx(font, backText,
        { backBounds.x + (backBounds.width - backTextSize.x) / 2,
          backBounds.y + (backBounds.height - backTextSize.y) / 2 },
        26, 1, WHITE);
}


void WelcomeScreen::Unload() {
    UnloadFont(font);
    UnloadTexture(background);
}
#include "WelcomeScreen.h"
#include <iostream>
#include <random>

using namespace std;

WelcomeScreen::WelcomeScreen() :
    timeElapsed(0),
    animationComplete(false),
    wigglePhase(false),      // Initialize wiggle phase
    isHoveringPlay(false),
    buttonColor({ 59, 85, 162, 255 }) {
    Load();
}

WelcomeScreen::~WelcomeScreen() {
    Unload();
}

void WelcomeScreen::Load() {
    font = LoadFontEx("Font/monogram.ttf", 64, 0, 0);

    // Create background
    Image img = GenImageColor(1000, 750, { 44, 44, 127, 255 });
    background = LoadTextureFromImage(img);
    UnloadImage(img);

    // Create letters T-E-T-R-I-S with falling blocks
    float startX = 225.0f;
    float startY = 150.0f;
    float letterSpacing = 100.0f;

    CreateLetterT(startX, startY);
    CreateLetterE(startX + letterSpacing, startY);
    CreateLetterT(startX + letterSpacing * 2, startY);
    CreateLetterR(startX + letterSpacing * 3, startY);
    CreateLetterI(startX + letterSpacing * 4 - 25, startY);
    CreateLetterS(startX + letterSpacing * 5 - 50, startY);

    // Button position
    playButtonX = (1000 - 200) / 2;
    playButtonY = 500;
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
        block.color = { 247, 87, 87, 255 }; // Red
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
        block.color = { 87, 247, 87, 255 }; // Green
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
        block.color = { 87, 87, 247, 255 }; // Blue
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
        block.color = { 247, 247, 87, 255 }; // Yellow
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
        block.color = { 247, 87, 247, 255 }; // Purple
        block.arrived = false;
        block.delay = (rand() % 100) / 100.0f;
        block.wiggleTime = 0;
        block.wiggleOffset = (rand() % 100) / 25.0f;
        blocks.push_back(block);
    }
}

void WelcomeScreen::Update(bool& startGame) {
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
            float wiggleX = sinf(block.wiggleTime * 6.0f + block.wiggleOffset) * 2.0f; // 2 pixel wiggle
            float wiggleY = cosf(block.wiggleTime * 5.0f + block.wiggleOffset) * 1.5f; // 1.5 pixel wiggle

            block.position.x = block.originalPosition.x + wiggleX;
            block.position.y = block.originalPosition.y + wiggleY;
        }
    }

    // Check for click on play button
    if (animationComplete) {
        Vector2 mouse = GetMousePosition();
        Rectangle playBounds = { (float)playButtonX, (float)playButtonY, 200, 80 };
        isHoveringPlay = CheckCollisionPointRec(mouse, playBounds);

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            if (CheckCollisionPointRec(mouse, playBounds)) {
                startGame = true;
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

    // Draw PLAY button only when animation is complete
    if (animationComplete) {
        Color playButtonColor = isHoveringPlay ? LIGHTGRAY : buttonColor;
        DrawRectangleRounded({ (float)playButtonX, (float)playButtonY, 200, 80 }, 0.3f, 6, playButtonColor);

        const char* playText = "PLAY";
        Vector2 playTextSize = MeasureTextEx(font, playText, 38, 2);
        DrawTextEx(font, playText,
            { playButtonX + (200 - playTextSize.x) / 2,
              playButtonY + (80 - playTextSize.y) / 2 },
            38, 2, WHITE);

        // Instructions
        const char* instructions = "Use ARROW KEYS to move and rotate pieces";
        Vector2 instrSize = MeasureTextEx(font, instructions, 24, 1);
        DrawTextEx(font, instructions,
            { (1000 - instrSize.x) / 2, 600 },
            24, 1, WHITE);
    }
}

void WelcomeScreen::Unload() {
    UnloadFont(font);
    UnloadTexture(background);
}

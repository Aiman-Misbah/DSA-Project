#pragma once
#include <raylib.h>
#include <vector>
#include <string>
#include <iostream>

using namespace std;

struct FallingBlock {
    Vector2 position;
    Vector2 targetPosition;
    Vector2 originalPosition;
    float speed;
    Color color;
    bool arrived;
    float delay;
    float wiggleTime;
    float wiggleOffset;
};

class WelcomeScreen {
private:
    Font font;
    Texture2D background;
    vector<FallingBlock> blocks;
    float timeElapsed;
    bool animationComplete;
    bool wigglePhase;

    // Buttons
    int playButtonX, playButtonY;
    int instructionsButtonX, instructionsButtonY;
    bool isHoveringPlay;
    bool isHoveringInstructions;
    Color buttonColor;
    bool showInstructions;  // NEW: Track if instructions are shown

public:
    WelcomeScreen();
    ~WelcomeScreen();

    void Load();
    void Update(bool& startGame, bool& showInstructionsScreen);
    void Draw();
    void Unload();
    bool GetShowInstructions() const { return showInstructions; }  // NEW: Getter
    void DrawInstructions();  // NEW: Draw instructions screen

private:
    void CreateLetterT(float startX, float startY);
    void CreateLetterE(float startX, float startY);
    void CreateLetterR(float startX, float startY);
    void CreateLetterI(float startX, float startY);
    void CreateLetterS(float startX, float startY);
};

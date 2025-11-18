#pragma once
#include <raylib.h>
#include <vector>

using namespace std;

struct FallingBlock {
    Vector2 position;
    Vector2 targetPosition;
    Vector2 originalPosition; // Add this to store the exact target position
    float speed;
    Color color;
    bool arrived;
    float delay;
    float wiggleTime;        // Add for wiggle animation
    float wiggleOffset;      // Add for random wiggle timing
};

class WelcomeScreen {
private:
    Font font;
    Texture2D background;
    vector<FallingBlock> blocks;
    float timeElapsed;
    bool animationComplete;
    bool wigglePhase;        // Add to track if we're in wiggle phase

    // Button
    int playButtonX, playButtonY;
    bool isHoveringPlay;
    Color buttonColor;

public:
    WelcomeScreen();
    ~WelcomeScreen();

    void Load();
    void Update(bool& startGame);
    void Draw();
    void Unload();

private:
    void CreateLetterT(float startX, float startY);
    void CreateLetterE(float startX, float startY);
    void CreateLetterR(float startX, float startY);
    void CreateLetterI(float startX, float startY);
    void CreateLetterS(float startX, float startY);
};

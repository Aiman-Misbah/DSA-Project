#pragma once
#include <raylib.h>
#include <vector>
#include <iostream>
using namespace std;

class Manager {
private:
    Texture2D musicOnTex;
    Texture2D musicOffTex;
    Texture2D pauseTex;
    Texture2D playTex;
    Color myBlue;

    float buttonScale;
    int musicX, musicY;
    int pauseX, pauseY;
    float ghostToggleX, ghostToggleY;
    bool isHoveringMusic;
    bool isHoveringPause;
    bool isHoveringGhost;
    float musicAnimationProgress;
    bool isMusicAnimating;


public:
    Manager();
    ~Manager();

    void LoadTextures();
    void Update(bool& musicOn, bool& gamePaused, bool isCountingDown, bool& ghostEnabled, float& ghostAnimationProgress);
    void Draw(bool musicOn, bool gamePaused, int score, Font& font, bool ghostEnabled, float ghostAnimationProgress);
    void UnloadTextures();
};

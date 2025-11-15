#pragma once
#include <raylib.h>

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

    bool isHoveringMusic;
    bool isHoveringPause;

public:
    Manager();
    ~Manager();

    void LoadTextures();
    void Update(bool& musicOn, bool& gamePaused);
    void Draw(bool musicOn, bool gamePaused, int score, Font& font);
    void UnloadTextures();
};

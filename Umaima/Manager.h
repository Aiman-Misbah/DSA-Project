#pragma once
#include <raylib.h>
#include <vector>
#include "Leaderboard.h"
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

    void DrawGameOverScreen(Font& font, int score, bool& gameOver, bool& restartRequested, bool& returnToMenuRequested);
    void UpdateGameOverScreen(Vector2 mousePos, bool& gameOver, bool& restartRequested, bool& returnToMenuRequested);

    // Game Over screen variables
    Rectangle replayButton;
    Rectangle menuButton;
    bool isHoveringReplay;
    bool isHoveringMenu;

    // Particle system
    struct GameOverParticle {
        Vector2 position;
        Vector2 velocity;
        Color color;
        float size;
        float life;
    };
    std::vector<GameOverParticle> particles;
    void UpdateParticles(float deltaTime);
    void DrawParticles();
    void CreateParticles();

    float leaderboardX, leaderboardY;
    float scoreDisplayX, scoreDisplayY;

public:
    Manager();
    ~Manager();

    void LoadTextures();
    void Update(bool& musicOn, bool& gamePaused, bool isCountingDown, bool& ghostEnabled,
        float& ghostAnimationProgress, bool gameOver, bool& restartRequested, bool& returnToMenuRequested);

    // Draw method signature needs to change
    void Draw(bool musicOn, bool gamePaused, int score, Font& font, bool ghostEnabled,
        float ghostAnimationProgress, double time, int lines, bool gameOver);
    void UnloadTextures();
};

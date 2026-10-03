#pragma once
#include <raylib.h>
#include "../leaderboard/Leaderboard.h"
#include "../game/Game.h"
#include "../data_structures/PieceQueue.h"
#include <vector>
#include <iostream>
using namespace std;

class Manager {
private:
    Texture2D musicOnTex;   //control are images
    Texture2D musicOffTex;
    Texture2D pauseTex;
    Texture2D playTex;

    float buttonScale;      //button ko kitna chota bara krna hai
    int musicX, musicY;     //buttons k positions
    int pauseX, pauseY;
    float ghostToggleX, ghostToggleY;
    
    bool isHoveringMusic;
    bool isHoveringPause;
    bool isHoveringGhost;


    void DrawGameOverScreen(Font& font, int score, bool& restartRequested, Leaderboard& leaderboard);
    void UpdateGameOverScreen(Vector2 mousePos, bool& gameOver, bool& restartRequested, bool& returnToMenuRequested);

    //game over screen k variables
    Rectangle replayButton;
    Rectangle menuButton;
    bool isHoveringReplay;
    bool isHoveringMenu;

    // Particle system for effects
    struct GameOverParticle {
        Vector2 position;   
        Vector2 velocity;   //direction and speed
        Color color;
        float size;
        float life;   //how long the particles live (aer visible)
    };

    vector<GameOverParticle> particles;

    void CreateParticles();     //creating new ones and storing in the vector
    void UpdateParticles(float deltaTime);  //updating their pos
    void DrawParticles();       //drawing them

    // UI elements
    void DrawHoldPanel(const Game& game);   //top left
    void DrawControlsPanel();       //bottom left
    void DrawNextPiecesPanel(const PieceQueue& pieceQueue); //top right

public:
    Manager();
    ~Manager();

    void LoadTextures();    //loading images

    //drawing everything
    void Draw(bool musicOn, bool gamePaused, int score, Font& font, bool ghostEnabled,
        float ghostAnimationProgress, double time, int lines, bool gameOver,
        Leaderboard& leaderboard, const Game& game, const PieceQueue& pieceQueue);

    //checkign for button clicks and hovers
     void Update(bool& musicOn, bool& gamePaused, bool isCountingDown, bool& ghostEnabled, 
                float& ghostAnimationProgress, bool gameOver, bool& restartRequested, bool& returnToMenuRequested);
    
     
    void UnloadTextures();

};

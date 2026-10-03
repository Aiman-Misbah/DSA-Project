#pragma once
#include <raylib.h>
#include "../game/Position.h"
#include <vector>
#include <string>
#include <iostream>
using namespace std;

struct FallingBlock {       //each block in the animation
    Position position;       //current position of the block
    Position targetPosition; //final desination / kahaan tak pohunchna hai
    Position originalPosition;   //as a reference point for the wiggling
    float speed;    //block k girne ki speed
    Color color;    
    bool arrived;   //block has reached its target position or not
    float delay;    //delay before the block starts fallign
    float wiggleTime;   //current time of the wiggling
    float wiggleOffset; //phase shift for each block's animation
};

class WelcomeScreen {
private:
    Font font;
    Texture2D background;   //for the background image
    vector<FallingBlock> blocks;    //all the falling blocks
    float timeElapsed;  //how much time has passed since the WelcomeScreen started
    bool animationComplete; //have all the blocks fallen
    bool wigglePhase;       //are the blocks wiggling or not

    //dono buttons k coordinated
    Position playButtonPos, instructionsButtonPos;
    bool isHoveringPlay;    //hovering flags for both
    bool isHoveringInstructions;
    Color buttonColor;
    bool showInstructions;  //are instructions being shown are not (instruction screen)
    
    //creating the letters through animation
    void CreateLetterT(float startX, float startY);
    void CreateLetterE(float startX, float startY);
    void CreateLetterR(float startX, float startY);
    void CreateLetterI(float startX, float startY);
    void CreateLetterS(float startX, float startY);


public:
    WelcomeScreen();
    ~WelcomeScreen();

    void Load();    //loading all teh resources (font, picture waghaira)
    void Update(bool& startGame, bool& showInstructionsScreen);     //update the screen for the animation and check which button is clicked and update accordingly
    void Draw();    //drawing everything on the welcome screen
    void Unload();
    void DrawInstructions();
    void UpdateInstructions(bool& showInstructionScreen);   //checking if the back button is pressed or not - want to go back to the welcome screen or not

    
};

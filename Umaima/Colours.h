#pragma once
#include <raylib.h>
#include <iostream>
#include <vector>
using namespace std;

// Existing colors - KEEP THESE
extern const Color DarkGray;
extern const Color Green;
extern const Color Red;
extern const Color Orange;
extern const Color Yellow;
extern const Color Purple;
extern const Color Cyan;
extern const Color Blue;


extern const Color LightBlue;
extern const Color DarkBlue;
extern const Color PanelBlue;
extern const Color DarkOverlay;

// NEW - Add ONLY these missing custom colors:
extern const Color TetrisRed;             // T letter color {247, 87, 87, 255}
extern const Color TetrisGreen;           // E letter color {87, 247, 87, 255}
extern const Color TetrisBlue;            // R letter color {87, 87, 247, 255}
extern const Color TetrisYellow;          // I letter color {247, 247, 87, 255}
extern const Color TetrisPurple;          // S letter color {247, 87, 247, 255}

// Game Over screen 
// 
// 
// s - Manager.cpp
extern const Color instructionBlue;  // {162, 59, 85, 255}
extern const Color GameOverShadow;          // {0, 0, 0, 150}
extern const Color ScoreColour;
extern const Color GameOverBorder;          // {255, 215, 0, 100}
extern const Color CurrentScoreHighlight;   // {100, 255, 100, 255}
extern const Color EmptyScoreSlot;          // {150, 150, 150, 200}
extern const Color GameOverQuote;           // {200, 200, 255, 200}
extern const Color ParticleGold;            // {255, 215, 0, 200}
extern const Color GhostHover;              // {250, 250, 250, 255}


// ============ ADD THESE TO COLOURS.H ============
extern const Color GameOverDarkBlue;
extern const Color GameOverBrightRed;
extern const Color GameOverBrightCyan;
extern const Color GameOverPurple;
extern const Color GameOverPurpleHover;
extern const Color GameOverTextWhite;
extern const Color GameOverLineGold;
// Add this to Colours.h:
extern const Color GameOverBestScoreGold;
extern const Color BlueBorder;

vector<Color> GetCellColours();
// Colours.cpp
#include <iostream>
#include <raylib.h>
#include <vector>
using namespace std;

// Existing colors - KEEP THESE
const Color DarkGray = { 26, 31, 40, 255 };
const Color Green = { 47, 230, 23, 255 };
const Color Red = { 232, 18, 18, 255 };
const Color Orange = { 226, 116, 17, 255 };
const Color Yellow = { 237, 234, 4, 255 };
const Color Purple = { 166, 0, 247, 255 };
const Color Cyan = { 21, 204, 209, 255 };
const Color Blue = { 13, 64, 216, 255 };
const Color LightBlue = { 59, 85, 162, 255 };
const Color DarkBlue = { 44, 44, 127, 255 };
const Color PanelBlue = { 59, 85, 162, 255 };
const Color DarkOverlay = { 0, 0, 0, 100 };

// NEW - Add ONLY these missing custom colors:
const Color TetrisRed = { 247, 87, 87, 255 };             // From WelcomeScreen.cpp
const Color TetrisGreen = { 87, 247, 87, 255 };           // From WelcomeScreen.cpp
const Color TetrisBlue = { 87, 87, 247, 255 };            // From WelcomeScreen.cpp
const Color TetrisYellow = { 247, 247, 87, 255 };         // From WelcomeScreen.cpp
const Color TetrisPurple = { 247, 87, 247, 255 };         // From WelcomeScreen.cpp

// Game Over screen colors - From Manager.cpp
const Color InstructionsBackButton = { 162, 59, 85, 255 };
const Color ReplayButtonHover = { 100, 200, 100, 255 };
const Color ReplayButtonGlow = { 100, 255, 100, 100 };
const Color MenuButtonHover = { 200, 100, 100, 255 };
const Color MenuButtonGlow = { 255, 100, 100, 100 };
const Color GameOverShadow = { 0, 0, 0, 180 };
const Color ScoreColour = { 200, 200, 255, 255 };
const Color GameOverBorder = { 255, 215, 0, 100 };
const Color CurrentScoreHighlight = { 100, 255, 100, 255 };
const Color EmptyScoreSlot = { 150, 150, 150, 200 };
const Color GameOverQuote = { 200, 200, 255, 200 };
const Color ParticleGold = { 255, 215, 0, 200 };
const Color GhostHover = { 250, 250, 250, 255 };

vector<Color> GetCellColours() {
    return vector<Color>{ DarkGray, Green, Red, Orange, Yellow, Purple, Cyan, Blue };
}

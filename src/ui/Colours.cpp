#include <iostream>
#include <raylib.h>
#include <vector>
using namespace std;

const Color DarkGray = { 26, 31, 40, 255 };
const Color Green = { 47, 230, 23, 255 };
const Color Red = { 232, 18, 18, 255 };
const Color Orange = { 226, 116, 17, 255 };
const Color Yellow = { 237, 234, 4, 255 };
const Color Purple = { 166, 0, 247, 255 };
const Color Cyan = { 21, 204, 209, 255 };
const Color Blue = { 13, 64, 216, 255 };
const Color DarkBlue = { 31, 68, 94, 255 };
const Color PanelBlue = { 37, 150, 190, 255 };
const Color DarkOverlay = { 0, 0, 0, 100 };

const Color TetrisRed = { 246, 143, 142, 255 };   
const Color TetrisGreen = { 87, 247, 87, 255 };  
const Color TetrisBlue = { 87, 87, 247, 255 };    
const Color TetrisYellow = { 248, 225, 125, 255 };
const Color TetrisPurple = { 247, 87, 247, 255 }; 

const Color instructionBlue = { 35, 65, 110, 255 };
const Color instructionBG = { 0, 0, 0, 180 };
const Color GameOverShadow = { 0, 0, 0, 180 };
const Color ScoreColour = { 200, 200, 255, 255 };
const Color GameOverBorder = { 255, 215, 0, 100 };
const Color CurrentScoreHighlight = { 100, 255, 100, 255 };
const Color EmptyScoreSlot = { 150, 150, 150, 200 };
const Color GameOverQuote = { 200, 200, 255, 200 };
const Color ParticleGold = { 255, 215, 0, 200 };
const Color GhostHover = { 150, 150, 150, 255 };

const Color GameOverDarkBlue = { 25, 35, 65, 245 };   
const Color GameOverBrightRed = { 255, 60, 60, 255 };   
const Color GameOverBrightCyan = { 80, 220, 255, 255 }; 
const Color GameOverPurple = { 180, 80, 220, 255 };     
const Color GameOverPurpleHover = { 220, 120, 255, 255 };
const Color GameOverTextWhite = { 240, 240, 255, 255 }; 
const Color GameOverLineGold = { 255, 215, 0, 100 };    
const Color GameOverBestScoreGold = { 255, 215, 0, 255 };  
const Color BlueBorder = { 100, 150, 255, 150 };

vector<Color> GetCellColours() {
    return vector<Color>{ DarkGray, Green, Red, Orange, Yellow, Purple, Cyan, Blue };
}

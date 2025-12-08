#pragma once
#include <raylib.h>
#include <iostream>
#include <vector>
using namespace std;

//for the vector of 8 colours
extern const Color DarkGray;
extern const Color Green;
extern const Color Red;
extern const Color Orange;
extern const Color Yellow;
extern const Color Purple;
extern const Color Cyan;
extern const Color Blue;

extern const Color DarkBlue;	//main background colour
extern const Color PanelBlue;	//panel boxes colour
extern const Color DarkOverlay;	//semi-transparent overlay for pause and countdown screens


//for the welcome screen
extern const Color TetrisRed;             
extern const Color TetrisGreen; 
extern const Color TetrisBlue;    
extern const Color TetrisYellow;  
extern const Color TetrisPurple;   


extern const Color instructionBlue;  //instruction screen box
extern const Color instructionBG; //instruction screen background
extern const Color GameOverShadow;	//overlay on the game for game over screen
extern const Color ScoreColour;		//for scores in the list
extern const Color GameOverBorder;	//gold-ish color for border    
extern const Color CurrentScoreHighlight;  //green highlight
extern const Color EmptyScoreSlot;	//grey color for empty entries  
extern const Color GameOverQuote;	//light blue for the text at the bottom    
extern const Color ParticleGold;            
extern const Color GhostHover;   //hover on the ghost toggle - grayish          

extern const Color GameOverDarkBlue; //game over box ka bg
extern const Color GameOverBrightRed; //game over title
extern const Color GameOverBrightCyan;	//current score and replay button
extern const Color GameOverPurple;	//menu button
extern const Color GameOverPurpleHover;
extern const Color GameOverTextWhite;	//off-white labels
extern const Color GameOverLineGold;	//separators
extern const Color GameOverBestScoreGold;	//best score
extern const Color BlueBorder;	//inner border

vector<Color> GetCellColours();

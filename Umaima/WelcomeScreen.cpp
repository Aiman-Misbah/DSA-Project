#include "WelcomeScreen.h"
#include "Colours.h"
#include <iostream>
#include <random>
using namespace std;

WelcomeScreen::WelcomeScreen() :
	timeElapsed(0),
	animationComplete(false),
	wigglePhase(false),
	isHoveringPlay(false),
	buttonColor(PanelBlue), showInstructions(false) {
	Load();
}

WelcomeScreen::~WelcomeScreen() {
	Unload();
}

void WelcomeScreen::Load() {
	font = LoadFontEx("Font/monogram.ttf", 64, 0, 0);
	background = LoadTexture("Images/wallpaper.jpg");

	//checking if the image has been loaded successfully
	if (background.id == 0) {
		cout << "ERROR: Could not load Images/wallpaper.jpg!" << endl;
		cout << "Creating fallback background..." << endl;

		//just in case the image is not loaded properly
		Image solid = GenImageColor(1200, 800, DarkBlue);	//creates a solid colour image in RAM
		background = LoadTextureFromImage(solid);	//uploads it from RAM to graphics card
		UnloadImage(solid);	//removes/deletes from RAM
	}
	else {
		cout << "Successfully loaded wallpaper.jpg: "
			<< background.width << "x" << background.height << endl;

		// Check image size
		if (background.width != 1200 || background.height != 800) {
			cout << "Image is " << background.width << "x" << background.height
				<< " - will be scaled to fit 1200x800 window" << endl;
		}
	}

	// Create letters T-E-T-R-I-S with CENTERED position
	float totalWidth = 6 * 100.0f - 50.0f;  // "TETRIS" = 6 letters
	float startX = (1200.0f - totalWidth) / 2.0f;  // Center calculation
	float startY = 150.0f;
	float letterSpacing = 100.0f;

	//according to the x-offset and y-offset not the row and col
	CreateLetterT(startX, startY);	
	CreateLetterE(startX + letterSpacing, startY);
	CreateLetterT(startX + letterSpacing * 2, startY);
	CreateLetterR(startX + letterSpacing * 3, startY);
	CreateLetterI(startX + letterSpacing * 4 - 25, startY);	//wrna i and r k beech ka gap ziada ho rha tha - i is patla that's why
	CreateLetterS(startX + letterSpacing * 5 - 50, startY);

	playButtonPos = Position(450, (1200 - 200) / 2);   //button itself is 200 pixels wide
	instructionsButtonPos = Position(550, (1200 - 200) / 2);
}

void WelcomeScreen::CreateLetterT(float startX, float startY) {

	vector<Position> positions = {
		// Position(ROW, COL) where ROW = y, COL = x
		Position(startY, startX),                     // Top-left: row=startY, col=startX
		Position(startY, startX + 30.0f),             // Top-middle  
		Position(startY, startX + 60.0f),             // Top-right

		// Vertical stem
		Position(startY + 30.0f, startX + 30.0f),     // Row+30, Col+30
		Position(startY + 60.0f, startX + 30.0f),     // Row+60, Col+30
		Position(startY + 90.0f, startX + 30.0f),     // Row+90, Col+30
		Position(startY + 120.0f, startX + 30.0f)     // Row+120, Col+30
	};

	for (const Position& targetPos : positions) {
		FallingBlock block;
		block.targetPosition = targetPos;	//each position is a target position for each block in T
		block.originalPosition = targetPos; //for wiggling
		block.position = Position(-100.0f, targetPos.COL);  // Start above screen: row=-100, same col
		block.speed = 300.0f + (rand() % 100);	//random speed for each block
		block.color = TetrisRed;
		block.arrived = false;
		block.delay = (rand() % 100) / 100.0f;	//before the block starts falling
		block.wiggleTime = 0;	//wiggling not started yet
		block.wiggleOffset = (rand() % 100) / 25.0f;	//random phase out of 4
		blocks.push_back(block);	//storing that block
	}
}

//same is for every letter
void WelcomeScreen::CreateLetterE(float startX, float startY) {

	vector<Position> positions = {
		// Top bar - Position(ROW=y, COL=x)
		Position(startY, startX),                     // Top-left
		Position(startY, startX + 30.0f),             // Top-middle  
		Position(startY, startX + 60.0f),             // Top-right

		// Middle left
		Position(startY + 30.0f, startX),             // Upper-Middle-left

		// Bottom bar
		Position(startY + 60.0f, startX),             // Middle-left
		Position(startY + 60.0f, startX + 30.0f),     // Middle-middle
		Position(startY + 60.0f, startX + 60.0f),     // Middle-right

		Position(startY + 90.0f, startX),             // Lower-Middle
		Position(startY + 120.0f, startX),            // Lower-Left
		Position(startY + 120.0f, startX + 30.0f),    // Lower-Middle
		Position(startY + 120.0f, startX + 60.0f)     // Lower-Right
	};

	for (const Position& targetPos : positions) {
		FallingBlock block;
		block.targetPosition = targetPos;
		block.originalPosition = targetPos;
		block.position = Position(-100.0f, targetPos.COL);  // Start above screen: row=-100, same col
		block.speed = 300.0f + (rand() % 100);
		block.color = TetrisGreen;
		block.arrived = false;
		block.delay = (rand() % 100) / 100.0f;
		block.wiggleTime = 0;
		block.wiggleOffset = (rand() % 100) / 25.0f;
		blocks.push_back(block);
	}
}

void WelcomeScreen::CreateLetterR(float startX, float startY) {

	vector<Position> positions = {
		// Top bar
		Position(startY, startX),                     // Top-left
		Position(startY, startX + 30.0f),             // Top-middle
		Position(startY, startX + 60.0f),             // Top-right

		// Middle
		Position(startY + 30.0f, startX),             // Middle-left
		Position(startY + 30.0f, startX + 60.0f),     // Middle-right

		// Bottom left
		Position(startY + 60.0f, startX),             // Bottom-left
		Position(startY + 60.0f, startX + 30.0f),     // Bottom-middle

		// Leg
		Position(startY + 90.0f, startX + 60.0f),     // Leg top
		Position(startY + 120.0f, startX + 60.0f),    // Leg bottom

		// Vertical
		Position(startY + 90.0f, startX),             // Vertical top
		Position(startY + 120.0f, startX)             // Vertical bottom
	};

	for (const Position& targetPos : positions) {
		FallingBlock block;
		block.targetPosition = targetPos;
		block.originalPosition = targetPos;
		block.position = Position(-100.0f, targetPos.COL);  // Start above screen
		block.speed = 300.0f + (rand() % 100);
		block.color = TetrisBlue;
		block.arrived = false;
		block.delay = (rand() % 100) / 100.0f;
		block.wiggleTime = 0;
		block.wiggleOffset = (rand() % 100) / 25.0f;
		blocks.push_back(block);
	}
}

void WelcomeScreen::CreateLetterI(float startX, float startY) {

	vector<Position> positions = {
		// Vertical line of I - all at same column (startX + 30)
		Position(startY, startX + 30.0f),             // Top
		Position(startY + 30.0f, startX + 30.0f),     // Middle 1
		Position(startY + 60.0f, startX + 30.0f),     // Middle 2
		Position(startY + 90.0f, startX + 30.0f),     // Middle 3
		Position(startY + 120.0f, startX + 30.0f)     // Bottom
	};

	for (const Position& targetPos : positions) {
		FallingBlock block;
		block.targetPosition = targetPos;
		block.originalPosition = targetPos;
		block.position = Position(-100.0f, targetPos.COL);  // Start above screen
		block.speed = 300.0f + (rand() % 100);
		block.color = TetrisYellow;
		block.arrived = false;
		block.delay = (rand() % 100) / 100.0f;
		block.wiggleTime = 0;
		block.wiggleOffset = (rand() % 100) / 25.0f;
		blocks.push_back(block);
	}
}

void WelcomeScreen::CreateLetterS(float startX, float startY) {

	vector<Position> positions = {
		// Top right part
		Position(startY, startX),                     // Top-left
		Position(startY, startX + 30.0f),             // Top-middle
		Position(startY, startX + 60.0f),             // Top-right

		// Middle
		Position(startY + 30.0f, startX),             // Upper-Middle-left

		Position(startY + 60.0f, startX),             // Middle-left
		Position(startY + 60.0f, startX + 30.0f),     // Middle-middle
		Position(startY + 60.0f, startX + 60.0f),     // Middle-right

		Position(startY + 90.0f, startX + 60.0f),     // Lower-Middle-right

		// Bottom line
		Position(startY + 120.0f, startX),            // Bottom-line left
		Position(startY + 120.0f, startX + 30.0f),    // Bottom-line middle
		Position(startY + 120.0f, startX + 60.0f)     // Bottom-line right
	};

	for (const Position& targetPos : positions) {
		FallingBlock block;
		block.targetPosition = targetPos;
		block.originalPosition = targetPos;
		block.position = Position(-100.0f, targetPos.COL);  // Start above screen
		block.speed = 300.0f + (rand() % 100);
		block.color = TetrisPurple;
		block.arrived = false;
		block.delay = (rand() % 100) / 100.0f;
		block.wiggleTime = 0;
		block.wiggleOffset = (rand() % 100) / 25.0f;
		blocks.push_back(block);
	}
}


void WelcomeScreen::Update(bool& startGame, bool& showInstructionsScreen) {
	timeElapsed += GetFrameTime();	//how much time has passed since the window was opened	

	// Update falling blocks
	bool allArrived = true;		//assuming that all the blocks have arrived at their target position
	for (auto& block : blocks) {
		if (block.delay > 0) {	//if delay is not 0 wait while moving on to the next block
			block.delay -= GetFrameTime();
			allArrived = false;		//not all have arrived
			continue;
		}

		if (!block.arrived) {	//increases in every frame
			block.position.ROW += block.speed * GetFrameTime();	//updating the block's y-offset according to the speed
			if (block.position.ROW >= block.targetPosition.ROW) {	//agar ziada neeche chale jaye to snap back to position
				block.position.ROW = block.targetPosition.ROW;
				block.arrived = true;
			}
			allArrived = false;	//all have not reached yet
		}
	}

	// Start wiggle phase when all blocks have arrived
	if (allArrived && !wigglePhase) {
		wigglePhase = true;		//start wiggling
		animationComplete = true;	//after falling animation show buttons
	}

	if (wigglePhase) {
		for (auto& block : blocks) {
			block.wiggleTime += GetFrameTime();	//how long has this block been wigglinng

			// Wiggle effect: small circular motion around original position
			//timer*6 = oscialling 6 times per second
			//adding offset for uniqueness - so that all the blocks don't oscillate in the same direction
			float wiggleX = sinf(block.wiggleTime * 6.0f + block.wiggleOffset) * 2.0f;	 //*2 for amplitude
			float wiggleY = cosf(block.wiggleTime * 5.0f + block.wiggleOffset) * 1.5f;

			block.position.COL = block.originalPosition.COL + wiggleX;
			block.position.ROW = block.originalPosition.ROW + wiggleY;
		}
	}

	// Check for clicks on buttons (only when animation complete)
	if (animationComplete) {
		Vector2 mouse = GetMousePosition();

		Rectangle playBounds = {
			(float)playButtonPos.COL,(float)playButtonPos.ROW,200, 80};

		//checking if mouse is in the rectangle
		isHoveringPlay = CheckCollisionPointRec(mouse, playBounds);

		Rectangle instructionsBounds = {
			(float)instructionsButtonPos.COL,
			(float)instructionsButtonPos.ROW,200, 80	};

		isHoveringInstructions = CheckCollisionPointRec(mouse, instructionsBounds);

		//checking for collisions/button presses
		if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
			if (CheckCollisionPointRec(mouse, playBounds)) {
				startGame = true;
			}
			else if (CheckCollisionPointRec(mouse, instructionsBounds)) {
				showInstructionsScreen = true;  
			}
		}
	}

}


void WelcomeScreen::UpdateInstructions(bool& showInstructionsScreen) {
	Vector2 mouse = GetMousePosition();

	// Calculate the same back button bounds as in DrawInstructions()
	float boxWidth = 1150.0f;
	float boxHeight = 750.0f;
	float boxX = (1200.0f - boxWidth) / 2.0f;
	float boxY = 25.0f;

	Rectangle backBounds = {
		(1200.0f - 300.0f) / 2.0f,
		boxY + boxHeight - 70,
		300,
		60
	};

	// Check for back button click
	if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) &&
		CheckCollisionPointRec(mouse, backBounds)) {
		showInstructionsScreen = false;  // Go back to main menu - no more instruction showing
	}
}


void WelcomeScreen::Draw() {

	// Calculate scaling to fill the window while maintaining aspect ratio
	float scaleX = 1200.0f / background.width;
	float scaleY = 800.0f / background.height;
	float scale = max(scaleX, scaleY);  // Use max to fill the entire window

	float scaledWidth = background.width * scale;
	float scaledHeight = background.height * scale;

	// Calculate position to center the scaled image
	float posX = (1200 - scaledWidth) / 2.0f;
	float posY = (800 - scaledHeight) / 2.0f;

	//Drawing picture
	//0.0 is rotation
	DrawTextureEx(background, Vector2{ posX, posY }, 0.0f, scale, WHITE);

	//Drawing the falling blocks
	for (const auto& block : blocks) {
		if (block.delay <= 0) {
			DrawRectangle(block.position.COL, block.position.ROW, 28, 28, block.color);
			// Draw border around the blocks
			DrawRectangleLines(block.position.COL, block.position.ROW, 28, 28, BLACK);
		}
	}

	// Draw buttons only when animation is complete
	if (animationComplete) {
		Color playButtonColor = isHoveringPlay ? LIGHTGRAY : buttonColor;
		DrawRectangleRounded({(float)playButtonPos.COL,
			(float)playButtonPos.ROW,200, 80}, 0.3f, 6, playButtonColor);

		const char* playText = "PLAY";
		Vector2 playTextSize = MeasureTextEx(font, playText, 38, 2);
		DrawTextEx(font, playText,
			{ playButtonPos.COL + (200 - playTextSize.x) / 2,
			  playButtonPos.ROW + (80 - playTextSize.y) / 2 },
			38, 2, WHITE);

		Color instructionsButtonColor = isHoveringInstructions ? LIGHTGRAY : buttonColor;
		DrawRectangleRounded({
			(float)instructionsButtonPos.COL,    // X coordinate
			(float)instructionsButtonPos.ROW,    // Y coordinate
			200, 80
			}, 0.3f, 6, instructionsButtonColor);

		const char* instructionsText = "INSTRUCTIONS";
		Vector2 instructionsTextSize = MeasureTextEx(font, instructionsText, 24, 1);
		DrawTextEx(font, instructionsText,
			{ instructionsButtonPos.COL + (200 - instructionsTextSize.x) / 2,
			  instructionsButtonPos.ROW + (80 - instructionsTextSize.y) / 2 },
			24, 1, WHITE);
	}
}


void WelcomeScreen::DrawInstructions() {
	//setting background colour
	DrawRectangle(0, 0, 1200, 800, instructionBG);
	//DrawRectangle(x,y,width, height, color)

	// Draw large box - for the instruction
	float boxWidth = 1150.0f;
	float boxHeight = 750.0f;
	float boxX = (1200.0f - boxWidth) / 2.0f;
	float boxY = 25.0f;

	//0.3 is roundness and 6 are segments (more means smoother curves)
	DrawRectangleRounded({ boxX, boxY, boxWidth, boxHeight }, 0.3f, 6, instructionBlue);
	//outline of the box with a gold colour
	DrawRectangleRoundedLines({ boxX, boxY, boxWidth, boxHeight }, 0.3f, 3, GameOverBorder);	  //here 3 means thickness in pixels

	// Title
	const char* title = "HOW TO PLAY TETRIS";
	//calculating the dimension of the text - returns vector2 .x(width) and .y(height)
	Vector2 titleSize = MeasureTextEx(font, title, 42, 2);	//42 is size and 2 is spacing

	//centering the text horizontally
	DrawTextEx(font, title,
		{ (1200.0f - titleSize.x) / 2.0f, boxY + 30 },
		42, 2, GameOverBrightRed);

	// Draw divider line
	DrawRectangle(boxX + 50, boxY + 90, boxWidth - 100, 2, GameOverLineGold);

	//instructions
	vector<string> leftColumn = {
		"KEYBOARD CONTROLS",
		"",				//line gap
		"<-/-> Arrows   Move piece left/right",
		"Up Arrow       Rotate piece",
		"Down Arrow     Soft drop (faster)",
		"Spacebar       Hard drop (instant)",
		"H Key          Hold/swap current piece",
		"Ctrl + Z       Undo last locked piece",
		"",
		"MOUSE CONTROLS",
		"",
		"Music Button   Toggle game music",
		"Pause Button   Pause/resume game",
		"Ghost Toggle   Show/hide ghost guide"
	};

	vector<string> rightColumn = {
		"GAME FEATURES",
		"",
		"Ghost piece shows where piece will land",
		"Hold piece system - save for later",
		"Undo last locked piece",
		"Preview next 3 pieces",
		"7-bag randomizer system",
		"Line clear animations",
		"",	
		"SCORING",
		"",
		"1 line cleared   100 points",
		"2 lines cleared  200 points",
		"3 lines cleared  500 points",
		"4 lines cleared  800 points",
		"5+ lines         800 + 100 per line"
	};

	//starting of both columns horizontally
	float leftX = boxX + 60;
	float rightX = boxX + boxWidth / 2 + 30;
	float yPos = boxY + 120;		//both are starting same vertically

	// Left column - with proper spacing
	for (const string& line : leftColumn) {
		if (line.empty()) {
			yPos += 20;	//gap for empty line
			continue;
		}

		//headers with a different colour and a bigger font
		bool isHeader = (line == "KEYBOARD CONTROLS" || line == "MOUSE CONTROLS");

		Color textColor = isHeader ? GameOverBrightCyan : GameOverTextWhite;
		int fontSize = isHeader ? 30 : 26;

		// Check if line will fit
		Vector2 lineSize = MeasureTextEx(font, line.c_str(), fontSize, 1);
		if (lineSize.x > (boxWidth / 2 - 80)) {
			// If too long, use smaller font
			fontSize = 24;
		}

		DrawTextEx(font, line.c_str(), { leftX, yPos }, fontSize, 1, textColor);
		yPos += isHeader ? 40 : 34;	//adding y-offset for every line
	}

	// Right column - ressetting the y-offset for this col
	yPos = boxY + 120;

	//same cheezein hi hain
	for (const string& line : rightColumn) {
		if (line.empty()) {
			yPos += 20;
			continue;
		}

		bool isHeader = (line == "GAME FEATURES" || line == "SCORING");

		Color textColor = isHeader ? GameOverBrightCyan : GameOverTextWhite;
		int fontSize = isHeader ? 30 : 26;

		Vector2 lineSize = MeasureTextEx(font, line.c_str(), fontSize, 1);
		if (lineSize.x > (boxWidth / 2 - 80)) {
			fontSize = 24;
		}

		DrawTextEx(font, line.c_str(), { rightX, yPos }, fontSize, 1, textColor);
		yPos += isHeader ? 35 : 30;	//thora sa kam bcoz yahaan ziada likhna hai jagah itni nhi hai
	}

	// Tips at the bottom
	float tipsY = boxY + boxHeight - 150; //neeche se 150 pixels oopar

	const char* tip1 = "TIP: Use ghost piece for precise placement!";
	const char* tip2 = "TIP: Hold difficult pieces for better positioning!";

	Vector2 tip1Size = MeasureTextEx(font, tip1, 22, 1);
	Vector2 tip2Size = MeasureTextEx(font, tip2, 22, 1);

	DrawTextEx(font, tip1,
		{ (1200.0f - tip1Size.x) / 2.0f, tipsY },
		22, 1, TetrisYellow);

	DrawTextEx(font, tip2,
		{ (1200.0f - tip2Size.x) / 2.0f, tipsY + 28 },
		22, 1, TetrisYellow);

	// Back button
	Rectangle backBounds = { (1200.0f - 300.0f) / 2.0f,
		boxY + boxHeight - 70, 300,60 };

	//checking if the mouse is inside that rectangle
	bool isHoveringBack = CheckCollisionPointRec(GetMousePosition(), backBounds);
	//if inside that rectangle, means hovering 
	Color backColor = isHoveringBack ? GameOverPurpleHover : GameOverPurple;

	//drawing the purple button with a gold border 
	DrawRectangleRounded(backBounds, 0.3f, 8, backColor);
	DrawRectangleRoundedLines(backBounds, 0.3f, 8, GameOverBorder);

	const char* backText = "BACK TO MAIN MENU";
	Vector2 backTextSize = MeasureTextEx(font, backText, 26, 1);
	DrawTextEx(font, backText,
		{ backBounds.x + (backBounds.width - backTextSize.x) / 2,
		  backBounds.y + (backBounds.height - backTextSize.y) / 2 },
		26, 1, WHITE);
}


void WelcomeScreen::Unload() {
	UnloadFont(font);
	UnloadTexture(background);
}

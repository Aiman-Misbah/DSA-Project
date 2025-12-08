#include <iostream>
#include "Game.h"
#include <random>
#include <vector>
#include <algorithm>
using namespace std;

Game::Game() : pieceQueue(5) {
    cout << "=== TETRIS GAME STARTED ===" << endl;
    cout << "Initializing piece queue..." << endl;

    // Use piece queue for ALL pieces
    current = pieceQueue.Dequeue();
    current.rowOffset = 0;      //center at teh top row
    current.colOffset = 6;

    if (!PieceFits()) {
        GameOver = true;
    }
    else {
        // If it fits, move it above for drop-in effect
        current.rowOffset = -3;
        GameOver = false;
    }


    score = 0;
    previousScore = 0;
    previousLinesCleared = 0;

	previousPieceQueue = pieceQueue.GetAllPieces(); //saving initial queue for undo

    InitAudioDevice();      //initializing audio system
    music = LoadMusicStream("Sounds/music.mp3");
    musicOn = true;         //music is on by default
    PlayMusicStream(music); //start playing
    RotateSound = LoadSound("Sounds/rotate.mp3");
    ClearSound = LoadSound("Sounds/clear.mp3");

    isCountingDown = true;  //start mein countdown krna hai
    countdownNumber = 3;
    countdownStartTime = GetTime(); //tracking when it is started
    ghostPiece = current.GetGhostPiece(board);
    showGhost = true;
    isDropping = false;     //space is not pressed so false
    gameStartTime = 0;      //game abhi start nhi hua
    totalPlayTime = 0;      //game ka time
    isTimeTracking = false;
    totalLinesCleared = 0;

    // Initialize undo stack for locked pieces
    lockedPieceStack = UndoStack(1);

    // Initialize hold
    isHolding = false;
    holdPiece = Piece();
    
    //jsut for checking
    cout << "Leaderboard initialized with " << leaderboard.GetTopScores(100).size() << " scores" << endl;

    cout << "=== GAME INITIALIZATION COMPLETE ===" << endl;
}

double Game::GetPlayTime() const {
    if (!isTimeTracking) return totalPlayTime;  //if not trackign time return that time wrna you need to calculate it
    return totalPlayTime + (GetTime() - gameStartTime);
}

void Game::StartTimeTracking() {
    if (!isTimeTracking) {
        gameStartTime = GetTime();
        isTimeTracking = true;
    }
}

void Game::StopTimeTracking() {
    if (isTimeTracking) {
        totalPlayTime += (GetTime() - gameStartTime);
        isTimeTracking = false;
    }
}

void Game::StartCountdown() {
    isCountingDown = true;
    countdownNumber = 3;
    countdownStartTime = GetTime();
    StopTimeTracking();
}

void Game::UpdateCountdown() {
    if (!isCountingDown) return;    //not counting to leave

    double currentTime = GetTime();
    double elapsed = currentTime - countdownStartTime;  //kitna time hogya since it started

    if (elapsed >= 1.0 && countdownNumber > 1) {    //har second decrease the no
        countdownNumber--;
        countdownStartTime = currentTime;
    }
    else if (elapsed >= 1.0 && countdownNumber == 1) {
        isCountingDown = false; //1 tak pohunch gaye counting stop game start
        countdownNumber = 0;
        StartTimeTracking();
    }
}

Game::~Game() {
    UnloadSound(RotateSound);
    UnloadSound(ClearSound);
    UnloadMusicStream(music);
    CloseAudioDevice();     //audio system ka kaam khatam
}

void Game::ToggleMusic() {
    musicOn = !musicOn;     //music button pr jb click hota hai tb
    if (musicOn) {
        ResumeMusicStream(music);
    }
    else {
        PauseMusicStream(music);
    }
}


void Game::UpdateGhostPiece() {     //updating ghost's position with the original piece
    ghostPiece = current.GetGhostPiece(board);
}

void Game::Draw() {     //drawing the three things
    board.Draw();
    if (showGhost) {
        ghostPiece.DrawGhost(50, 290);
    }
    current.Draw(50, 290);
}

void Game::SaveBoardState() {
    if (GameOver || isCountingDown) return;     //don't save during these kionke faida nhi hai

    //saving every thing
    previousScore = score;      
    previousLinesCleared = totalLinesCleared;
    previousBoardState = board.GetBoardState();
	previousPieceQueue = pieceQueue.GetAllPieces();

}

void Game::RestoreBoardState() {        //restoring jaisa pehle tha
    board.SetBoardState(previousBoardState);
}

void Game::UndoLastLock() {
    if (lockedPieceStack.IsEmpty() || previousBoardState.empty()) {
        return; //nothing to undo
    }

    cout << "=== UNDO LAST LOCK ===" << endl;

    // Get the last locked piece
    Piece lastLockedPiece = lockedPieceStack.Pop();

    //restore the board - including the previously cleared lines
    RestoreBoardState();

    //restore the queue by clearing it and setting it again with previous one
    if (!previousPieceQueue.empty()) {
        pieceQueue.ClearAndSetPieces(previousPieceQueue);
    }

    // Reset the piece to top position to make it fall again
    UndoStack::ResetPieceToTop(lastLockedPiece);

    lastLockedPiece.rowOffset = -3;
    lastLockedPiece.colOffset = 6;

    // Set this as the current falling piece
    current = lastLockedPiece;

    //restore score and lines
    score = previousScore;
    totalLinesCleared = previousLinesCleared;

    // Update ghost piece
    UpdateGhostPiece();

    cout << "Undo complete! Board, queue, and score reverted. Hold piece unchanged." << endl;
    cout << "Piece queue restored to size: " << previousPieceQueue.size() << endl;
}


void Game::UndoLastLockedPiece() {
    UndoLastLock();
}

void Game::ToggleHold() {
    if (!GameOver && !isCountingDown) { //only during game
        if (!IsHolding()) {
            // FIRST TIME: Store current piece and get next piece
            holdPiece = current;
            current = pieceQueue.Dequeue(); //get the next piece as current

            // Check if new piece fits before spawning above
            current.rowOffset = 0;  // Test at row 0 first
            current.colOffset = 6;

            if (!PieceFits()) {
                GameOver = true;
            }
            else {
                // If it fits, move it above for drop-in effect
                current.rowOffset = -3;
            }

            isHolding = true;
            cout << "Piece held! Getting next piece from queue." << endl;
        }
        else {
            //swap current piece with hold piece
            Piece temp = current;
            current = holdPiece;
            holdPiece = temp;

            //phir se wohi checking
            current.rowOffset = 0;
            current.colOffset = 6;

            if (!PieceFits()) {
                GameOver = true;
            }
            else {
                // If it fits, move it above for drop-in effect
                current.rowOffset = -3;
            }

            cout << "Swapped with hold piece! Current: " << current.id << ", Hold: " << holdPiece.id << endl;
        }
        UpdateGhostPiece();
    }
}


void Game::MoveLeft() {
    if (!GameOver && !isCountingDown) { //move only during game
        current.Move(0, -1);    //move left if not valid move back
        if (HasCollided() || !PieceFits()) {
            current.Move(0, 1);
        }
        UpdateGhostPiece(); //updating the ghost piece as well
    }
}

//same with all the other movements
void Game::MoveRight() {
    if (!GameOver && !isCountingDown) {
        current.Move(0, 1);
        if (HasCollided() || !PieceFits()) {
            current.Move(0, -1);
        }
        UpdateGhostPiece();
    }
}

void Game::MoveDown() {
    if (!GameOver && !isCountingDown) {
        current.Move(1, 0);
        if (HasCollided() || !PieceFits()) {
            current.Move(-1, 0);
            LockPiece();        //ab yahaan se nhi hil skta
        }
        UpdateGhostPiece();
    }
}

void Game::HardDrop() {
    if (!GameOver && !isCountingDown && !isDropping) {
        isDropping = true;
    }
}

void Game::UpdateHardDrop() {
    if (!isDropping) return;

    current.Move(1, 0);     //move down till locked
    if (HasCollided() || !PieceFits()) {
        current.Move(-1, 0);
        LockPiece();
        isDropping = false;
        return;
    }
}

bool Game::HasCollided() {
    vector<Position> tiles = current.GetCellPositions();
    //checking each and every block of the piece for collision aik ki bhi hui to it is considered as collision of whole
    for (Position item : tiles) {
        if (item.ROW >= 0 && board.CollisionDetected(item.ROW, item.COL)) {
            return true;
        }
        if (item.COL < 0 || item.COL >= 15) {   //side boundaries
            return true;
        }
    }
    return false;
}

void Game::RotatePiece() {
    if (!GameOver && !isCountingDown) {
        //if rotated successfully play the sound and update ghost
        bool rotationSuccess = current.RotateWithWallKicks(board);
        if (rotationSuccess) {
            PlaySound(RotateSound);
            UpdateGhostPiece();
        }
    }
}

void Game::LockPiece() {
    cout << "=== PIECE LOCKED ===" << endl;

    SaveBoardState();   // Save state before locking for undo

    // CLEAR the stack first (only keep most recent)
    lockedPieceStack.Clear();
    lockedPieceStack.Push(current);

    vector<Position> tiles = current.GetCellPositions();
    for (Position item : tiles) {
        board.SetCell(item.ROW, item.COL, current.id);
        //setting that id on the board (for row clearing)
    }

    //checking for completed rows
    int rowsCleared = board.ClearRows();
    if (rowsCleared > 0) {
        PlaySound(ClearSound);
        UpdateScore(rowsCleared);
        cout << "Cleared " << rowsCleared << " rows! Current score: " << score << endl;
        
        AddLineClearMessage(rowsCleared);   //uske mutaabik msg
    }

    current = pieceQueue.Dequeue();     //next piece from queue

    //wohi testing to se it if it fits
    current.rowOffset = 0;  // Test at row 0 first
    current.colOffset = 6;

    if (!PieceFits()) {
        GameOver = true;
        leaderboard.AddScore(score);   //saving the score as final
        cout << "Score " << score << " added to leaderboard. Bset: " << leaderboard.GetHighestScore() << endl;
        StopMusicStream(music);
    }
    else {
        // If it fits, move it above the board for the drop-in effect
        current.rowOffset = -3;
    }

    UpdateGhostPiece();
}
bool Game::PieceFits() {
    //similar to collision wala but this time checking for empty cells
    vector<Position> tiles = current.GetCellPositions();
    for (Position item : tiles) {
        if (item.ROW >= 0 && !board.isCellEmpty(item.ROW, item.COL)) {
            return false;
        }
    }
    return true;
}

void Game::HandleInput() {
    // Handle Ctrl+Z for undoing last locked piece
    if ((IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL)) && IsKeyPressed(KEY_Z)) {
        UndoLastLock();
    }

    // Press 'H' to toggle hold (hold/swap)
    if (IsKeyPressed(KEY_H)) {
        ToggleHold();
    }

    if (IsKeyPressed(KEY_LEFT)) {
        MoveLeft();
    }

    if (IsKeyPressed(KEY_RIGHT)) {
        MoveRight();
    }

    if (IsKeyPressed(KEY_DOWN)) {
        MoveDown();
    }

    if (IsKeyPressed(KEY_UP)) {
        RotatePiece();
    }

    if (IsKeyPressed(KEY_SPACE)) {
        HardDrop();
    }

    // Update ghost piece if any movement happened
    bool moved = (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_RIGHT) ||
        IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_UP) ||
        IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_H));

    if (moved) {
        UpdateGhostPiece();
    }
}

void Game::Reset() {
    cout << "=== GAME RESET ===" << endl;
    board.Initialize();     //clearing the board and starting anew

    // Reset piece queue and get new pieces
    pieceQueue.Clear();
    pieceQueue.FillStartup();
    current = pieceQueue.Dequeue();
    current.rowOffset = -3;
    current.colOffset = 6;
    UpdateGhostPiece();     //updating the ghost piece according to the new piece

    // Reset hold
    isHolding = false;
    holdPiece = Piece();

    // Reset game ki saari cheezein
    GameOver = false;
    score = 0;
    previousScore = 0;
    previousLinesCleared = 0;

    //clearing everything related to undo
    previousPieceQueue.clear();
    previousBoardState.clear();
    lockedPieceStack.Clear();

    totalPlayTime = 0;
    isTimeTracking = false;
    gameStartTime = 0;
    totalLinesCleared = 0;

     PlayMusicStream(music);    //restart the music - not resume

    cout << "Game reset complete!" << endl;
}

void Game::UpdateScore(int lines) {
    if (lines > 0) {
        totalLinesCleared += lines;     //tracking all the lines cleared
    }
    //increasing score accordingly
    if (lines == 1)
        score += 100;
    else if (lines == 2)
        score += 200;
    else if (lines == 3)
        score += 500;
    else if (lines == 4)
        score += 800;
    else if (lines > 4) {
        lines -= 4;
        lines *= 100;
        score += 800 + lines;
    }

}



void Game::AddLineClearMessage(int linesCleared) {

    if (linesCleared <= 0) return;  //no lines - no msg

    LineClearMessage msg;
    msg.isActive = true;
    msg.displayTime = 0.0f;
    msg.duration = 2.0f; // Show for 2 seconds

    // Custom messages for different line clears
    switch (linesCleared) {
    case 1:
        msg.text = "SINGLE!";
        msg.color = RED;
        break;
    case 2:
        msg.text = "DOUBLE!!";
        msg.color = GREEN;
        break;
    case 3:
        msg.text = "TRIPLE!!!";
        msg.color = YELLOW;
        break;
    case 4:
        msg.text = "TETRIS!!!!";
        msg.color = ORANGE;
        break;
    default:
        msg.text = TextFormat("MEGA %d LINES!", linesCleared);
        msg.color = PURPLE;

        break;
    }

    activeMessages = msg;
}

void Game::UpdateMessages(float deltaTime) {

    if (activeMessages.isActive) {
        activeMessages.displayTime += deltaTime;    //updating time passed
        if (activeMessages.displayTime >= activeMessages.duration) {    //time hogya to done
            activeMessages.isActive = false;
        }
    }

}

void Game::DrawMessages() {
    // Draw active messages on top of the board
    int boardX = 290;
    int boardY = 50;
    int boardWidth = 15 * 35;
    int boardHeight = 20 * 35;

    if (activeMessages.isActive) {

        // Calculate position (center of board)
        int centerX = boardX + boardWidth / 2;
        int centerY = boardY + boardHeight / 2;


        // Calculate alpha (fade out effect)
        float progress = activeMessages.displayTime / activeMessages.duration;
        float alpha = 1.0f - progress; // Fade from 1.0(visible) to 0.0(invisible)
        Color textColor = activeMessages.color;
        textColor.a = static_cast<unsigned char>(alpha * 255);

        // Calculate font size (pulse effect)
        //30 is base size - add/sub 10 for pulse
        int fontSize = 30 + static_cast<int>(10 * sin(activeMessages.displayTime * 5.0f));



        // Draw text
        Vector2 textSize = MeasureTextEx(GetFontDefault(), activeMessages.text.c_str(), fontSize, 2);
        DrawTextEx(GetFontDefault(), activeMessages.text.c_str(),
            { centerX - textSize.x / 2, centerY - textSize.y / 2 },
            fontSize, 2, textColor);
        }

}

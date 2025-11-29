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
    GameOver = false;

    score = 0;
    previousScore = 0;
    previousLinesCleared = 0;

    InitAudioDevice();
    music = LoadMusicStream("Sounds/music.mp3");
    musicOn = true;
    PlayMusicStream(music);
    RotateSound = LoadSound("Sounds/rotate.mp3");
    ClearSound = LoadSound("Sounds/clear.mp3");

    // Initialize features
    isCountingDown = true;
    countdownNumber = 3;
    countdownStartTime = GetTime();
    ghostPiece = current.GetGhostPiece(board);
    showGhost = true;
    isDropping = false;
    gameStartTime = 0;
    totalPlayTime = 0;
    isTimeTracking = false;
    lastPauseTime = 0;
    totalLinesCleared = 0;

    // Initialize undo stack for locked pieces
    lockedPieceStack = UndoStack(1);

    // Initialize hold
    isHolding = false;
    holdPiece = Piece();

    cout << "=== GAME INITIALIZATION COMPLETE ===" << endl;
}

double Game::GetPlayTime() const {
    if (!isTimeTracking) return totalPlayTime;
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
    if (!isCountingDown) return;

    double currentTime = GetTime();
    double elapsed = currentTime - countdownStartTime;

    if (elapsed >= 1.0 && countdownNumber > 1) {
        countdownNumber--;
        countdownStartTime = currentTime;
    }
    else if (elapsed >= 1.0 && countdownNumber == 1) {
        isCountingDown = false;
        countdownNumber = 0;
        StartTimeTracking();
    }
}

Game::~Game() {
    UnloadSound(RotateSound);
    UnloadSound(ClearSound);
    UnloadMusicStream(music);
    CloseAudioDevice();
}

void Game::ToggleMusic() {
    musicOn = !musicOn;
    if (musicOn) {
        ResumeMusicStream(music);
    }
    else {
        PauseMusicStream(music);
    }
}

Piece Game::GetRandomPiece() {
    if (pieceBag.GetSize() == 0) {
        // Refill the bag if empty
        pieceBag.AddPiece(IPiece());
        pieceBag.AddPiece(JPiece());
        pieceBag.AddPiece(LPiece());
        pieceBag.AddPiece(OPiece());
        pieceBag.AddPiece(SPiece());
        pieceBag.AddPiece(TPiece());
        pieceBag.AddPiece(ZPiece());
    }

    int index = rand() % pieceBag.GetSize();
    Piece p = pieceBag.GetPiece(index);
    pieceBag.RemovePiece(index);
    return p;
}

void Game::UpdateGhostPiece() {
    ghostPiece = current.GetGhostPiece(board);
}

void Game::Draw() {
    board.Draw();
    if (showGhost) {
        ghostPiece.DrawGhost(11, 11);
    }
    current.Draw(11, 11);

    // Draw hold piece area
    DrawRectangleRounded({ 550, 215, 150, 120 }, 0.3f, 6, Color{ 59, 85, 162, 255 });
    DrawText("HOLD", 580, 180, 20, WHITE);

    // Draw hold piece if currently holding
    if (IsHolding()) {
        Piece tempHold = GetHoldPiece();
        tempHold.rowOffset = 0;
        tempHold.colOffset = 0;

        // Draw hold piece with smaller size
        int holdCellSize = 25;
        vector<Position> tiles = tempHold.GetCellPositions();
        for (Position item : tiles) {
            DrawRectangle(item.COL * holdCellSize + 590,
                item.ROW * holdCellSize + 250,
                holdCellSize - 1, holdCellSize - 1,
                tempHold.colours[tempHold.id]);
        }

        // Show hold status
        DrawText("HOLDING", 575, 280, 16, YELLOW);
    }
    else {
        DrawText("EMPTY", 580, 250, 16, LIGHTGRAY);
        
    }

    // Draw the next THREE pieces from queue visually
    vector<Piece> nextThree = pieceQueue.GetNextThree();

    // Draw background for next pieces area
    DrawRectangleRounded({ 720, 215, 170, 360 }, 0.3f, 6, Color{ 59, 85, 162, 255 });

    // Draw title
    DrawText("NEXT PIECES", 730, 180, 20, WHITE);

    // Draw each of the next three pieces in different positions
    for (int i = 0; i < nextThree.size() && i < 3; i++) {
        int yOffset = 230 + (i * 120); // Space them vertically

        // Draw label for each piece
        DrawText(TextFormat("Next %d:", i + 1), 730, yOffset - 25, 18, WHITE);

        // Create a temporary piece for drawing with smaller size
        Piece tempPiece = nextThree[i];
        tempPiece.rowOffset = 0;
        tempPiece.colOffset = 0;

        // Set smaller size for preview pieces (25px instead of 35px)
        int previewCellSize = 25;

        // Adjust position for each piece type to center them properly
        int xPos, yPos;

        // Special positioning for different piece types
        switch (tempPiece.id) {
        case 1: // L-Piece
            xPos = 770;
            yPos = yOffset + 15;
            break;
        case 2: // J-Piece
            xPos = 770;
            yPos = yOffset + 15;
            break;
        case 3: // I-Piece (long piece)
            xPos = 750;
            yPos = yOffset + 20;
            break;
        case 4: // O-Piece (square)
            xPos = 770;
            yPos = yOffset + 5;
            break;
        case 5: // S-Piece
            xPos = 770;
            yPos = yOffset + 15;
            break;
        case 6: // T-Piece
            xPos = 770;
            yPos = yOffset + 15;
            break;
        case 7: // Z-Piece
            xPos = 770;
            yPos = yOffset + 15;
            break;
        default:
            xPos = 760;
            yPos = yOffset + 10;
            break;
        }

        // Draw the piece preview with smaller size
        vector<Position> tiles = tempPiece.GetCellPositions();
        for (Position item : tiles) {
            DrawRectangle(item.COL * previewCellSize + xPos,
                item.ROW * previewCellSize + yPos,
                previewCellSize - 1, previewCellSize - 1,
                tempPiece.colours[tempPiece.id]);
        }
    }
}

void Game::SaveBoardState() {
    if (GameOver || isCountingDown) return;

    previousScore = score;
    previousLinesCleared = totalLinesCleared;
    previousBoardState = board.GetBoardState();
}

void Game::RestoreBoardState() {
    board.SetBoardState(previousBoardState);
}

void Game::UndoLastLock() {
    if (lockedPieceStack.IsEmpty() || previousBoardState.empty()) {
        return;
    }

    // Get the last locked piece
    Piece lastLockedPiece = lockedPieceStack.Pop();

    // RESTORE THE ENTIRE BOARD STATE (this will restore any cleared lines)
    RestoreBoardState();

    // Reset the piece to top position to make it fall again
    UndoStack::ResetPieceToTop(lastLockedPiece);

    // Set this as the current falling piece
    current = lastLockedPiece;

    // ❌ REMOVE THIS COMPLEX AVL LOGIC:
    // if (!previousScores.root) {
    //     score = 0;
    // } else {
    //     score = currentScoreBeforeLock;
    // }

    // ✅ REPLACE WITH SIMPLE VARIABLE ASSIGNMENT:
    score = previousScore;
    totalLinesCleared = previousLinesCleared;

    // Update ghost piece
    UpdateGhostPiece();

    cout << "Last locked piece restored! Score reverted to: " << score << endl;
}


void Game::UndoLastLockedPiece() {
    UndoLastLock();
}

void Game::ToggleHold() {
    if (!GameOver && !isCountingDown) {
        if (!IsHolding()) {
            // FIRST TIME: Store current piece and get next piece
            holdPiece = current;
            current = pieceQueue.Dequeue();
            isHolding = true;
            cout << "Piece held! Getting next piece from queue." << endl;
        }
        else {
            // SWAP: Exchange current piece with hold piece
            Piece temp = current;
            current = holdPiece;
            holdPiece = temp;

            // Reset current piece position to top
            current.rowOffset = 0;
            current.colOffset = 3;
            cout << "Swapped with hold piece! Current: " << current.id << ", Hold: " << holdPiece.id << endl;
        }
        UpdateGhostPiece();
    }
}

void Game::ToggleHoldPiece() {
    ToggleHold();
}

void Game::MoveLeft() {
    if (!GameOver && !isCountingDown) {
        current.Move(0, -1);
        if (HasCollided() || !PieceFits()) {
            current.Move(0, 1);
        }
        UpdateGhostPiece();
    }
}

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
            LockPiece();
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

    current.Move(1, 0);
    if (HasCollided() || !PieceFits()) {
        current.Move(-1, 0);
        LockPiece();
        isDropping = false;
        return;
    }
}

bool Game::HasCollided() {
    vector<Position> tiles = current.GetCellPositions();
    for (Position item : tiles) {
        if (item.ROW >= 0 && board.CollisionDetected(item.ROW, item.COL)) {
            return true;
        }
        if (item.COL < 0 || item.COL >= 15) {
            return true;
        }
    }
    return false;
}

void Game::RotatePiece() {
    if (!GameOver && !isCountingDown) {
        bool rotationSuccess = current.RotateWithWallKicks(board);
        if (rotationSuccess) {
            PlaySound(RotateSound);
            UpdateGhostPiece();
        }
    }
}

void Game::LockPiece() {
    cout << "=== PIECE LOCKED ===" << endl;

    // Save state before locking
    SaveBoardState();

    // CLEAR the stack first (only keep most recent)
    lockedPieceStack.Clear();

    // Save the current piece to locked piece stack
    lockedPieceStack.Push(current);

    vector<Position> tiles = current.GetCellPositions();
    for (Position item : tiles) {
        board.SetCell(item.ROW, item.COL, current.id);
    }

    // Get next piece from queue immediately
    current = pieceQueue.Dequeue();


    if (!PieceFits()) {
        GameOver = true;
    }

    int rowsCleared = board.ClearRows();
    if (rowsCleared > 0) {
        PlaySound(ClearSound);
        UpdateScore(rowsCleared);
        cout << "Cleared " << rowsCleared << " rows! Current score: " << score << endl;
    }
    UpdateGhostPiece();
}

bool Game::PieceFits() {
    vector<Position> tiles = current.GetCellPositions();
    for (Position item : tiles) {
        if (item.ROW >= 0 && !board.isCellEmpty(item.ROW, item.COL)) {
            return false;
        }
    }
    return true;
}

void Game::HandleInput() {
    int key = GetKeyPressed();
    if (GameOver && key != 0) {
        GameOver = false;
        Reset();
    }

    bool pieceMoved = false;

    // Handle Ctrl+Z for undoing last locked piece
    if ((IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL)) && IsKeyPressed(KEY_Z)) {
        UndoLastLock();
        pieceMoved = true;
    }

    // Press 'Q' to display piece queue manually
    if (key == KEY_Q) {
        cout << "=== MANUAL QUEUE DISPLAY ===" << endl;
        DisplayPieceQueue();
    }

    // Press 'H' to toggle hold (hold/release)
    if (key == KEY_H) {
        ToggleHold();
        pieceMoved = true;
    }

    switch (key) {
    case KEY_LEFT:
        MoveLeft();
        pieceMoved = true;
        break;
    case KEY_RIGHT:
        MoveRight();
        pieceMoved = true;
        break;
    case KEY_DOWN:
        MoveDown();
        pieceMoved = true;
        break;
    case KEY_UP:
        RotatePiece();
        pieceMoved = true;
        break;
    case KEY_SPACE:
        HardDrop();
        pieceMoved = true;
        break;
    }

    if (pieceMoved && showGhost) {
        UpdateGhostPiece();
    }
}


void Game::Reset() {
    cout << "=== GAME RESET ===" << endl;
    board.Initialize();

    // Reset piece queue and get new pieces
    pieceQueue.Clear();
    pieceQueue.FillStartup();
    current = pieceQueue.Dequeue();

    // Reset hold
    isHolding = false;

    holdPiece = Piece();

    GameOver = false;
    score = 0;
    // ... rest of reset method
}

void Game::UpdateScore(int lines) {
    if (lines > 0) {
        totalLinesCleared += lines;
    }
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

void Game::DisplayPieceQueue() {
    pieceQueue.DisplayNextThree();
}

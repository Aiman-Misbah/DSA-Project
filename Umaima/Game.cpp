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
    current.rowOffset = 0;
    current.colOffset = 5;

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

	previousPieceQueue = pieceQueue.GetAllPieces();

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
    
    cout << "Leaderboard initialized with " << leaderboard.GetTopScores(100).size() << " scores" << endl;

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
    // Draw the game board (centered)
    board.Draw();

    if (showGhost) {
        ghostPiece.DrawGhost(50, 290);
    }
    current.Draw(50, 290);

    // ============ LEFT PANEL ============
    // Hold Area (Top-left) - X: 50, Y: 100
    DrawRectangleRounded({ 50, 100, 200, 140 }, 0.3f, 6, PanelBlue);
    DrawText("HOLD", 110, 70, 24, WHITE);

    // For HOLD piece (simpler version):
    if (IsHolding()) {
        Piece tempHold = GetHoldPiece();
        tempHold.rowOffset = 0;
        tempHold.colOffset = 0;

        int holdCellSize = 25;
        vector<Position> tiles = tempHold.GetCellPositions();

        // Find min/max for centering
        int minRow = 100, maxRow = -100;
        int minCol = 100, maxCol = -100;
        for (const Position& item : tiles) {
            if (item.ROW < minRow) minRow = item.ROW;
            if (item.ROW > maxRow) maxRow = item.ROW;
            if (item.COL < minCol) minCol = item.COL;
            if (item.COL > maxCol) maxCol = item.COL;
        }

        int pieceWidth = maxCol - minCol + 1;
        int pieceHeight = maxRow - minRow + 1;

        // Center in hold box (200x140 at 50,100)
        int drawX = 150 - (pieceWidth * holdCellSize) / 2;  // 150 is center X
        int drawY = 170 - (pieceHeight * holdCellSize) / 2; // 170 is center Y

        // Adjust for I-piece
        if (tempHold.id == 3) {
            drawX -= 5;
            drawY += 5;
        }
        // Adjust for O-piece
        else if (tempHold.id == 4) {
            drawY += 5;
        }

        for (Position item : tiles) {
            DrawRectangle((item.COL - minCol) * holdCellSize + drawX,
                (item.ROW - minRow) * holdCellSize + drawY,
                holdCellSize - 1, holdCellSize - 1,
                tempHold.colours[tempHold.id]);
        }
        DrawText("HOLDING", 100, 215, 18, YELLOW);
    }
    else {
        DrawText("EMPTY", 110, 160, 20, LIGHTGRAY);
    }

    // Controls Help (Bottom-left) - X: 50, Y: 500
    DrawRectangleRounded({ 50, 500, 200, 150 }, 0.3f, 6, PanelBlue);
    DrawText("CONTROLS", 80, 470, 20, WHITE);
    DrawText("H = Hold", 70, 520, 16, WHITE);
    DrawText("Space = Hard Drop", 70, 550, 16, WHITE);
    DrawText("Ctrl+Z = Undo", 70, 580, 16, WHITE);

    // ============ RIGHT PANEL ============
    // Next Pieces (Top-right) - X: 850, Y: 100
    DrawRectangleRounded({ 850, 100, 300, 320 }, 0.3f, 6, PanelBlue);
    DrawText("NEXT PIECES", 900, 70, 24, WHITE);

    vector<Piece> nextThree = pieceQueue.GetNextThree();

    // Draw each next piece vertically stacked
    // Draw each next piece vertically stacked
    for (int i = 0; i < nextThree.size() && i < 3; i++) {
        int yOffset = 140 + (i * 100); // Space them vertically

        // Draw label
        DrawText(TextFormat("Piece %d:", i + 1), 860, yOffset - 10, 18, WHITE);

        // Create temporary piece for drawing
        Piece tempPiece = nextThree[i];
        tempPiece.rowOffset = 0;
        tempPiece.colOffset = 0;

        int previewCellSize = 25;
        vector<Position> tiles = tempPiece.GetCellPositions();

        // Find min/max for centering
        int minRow = 100, maxRow = -100;
        int minCol = 100, maxCol = -100;
        for (const Position& item : tiles) {
            if (item.ROW < minRow) minRow = item.ROW;
            if (item.ROW > maxRow) maxRow = item.ROW;
            if (item.COL < minCol) minCol = item.COL;
            if (item.COL > maxCol) maxCol = item.COL;
        }

        int pieceWidth = maxCol - minCol + 1;
        int pieceHeight = maxRow - minRow + 1;

        // Center in each slot (300px wide box starting at X=850)
        // Each slot is roughly 100px tall, centered at yOffset + 50
        int drawX = 1000 - (pieceWidth * previewCellSize) / 2;  // 1000 is center X (850 + 300/2)
        int drawY = (yOffset + 40) - (pieceHeight * previewCellSize) / 2; // Center of 100px slot

        // Adjust for I-piece
        if (tempPiece.id == 3) {
            drawX -= 5;
            drawY += 5;
        }
        // Adjust for O-piece
        else if (tempPiece.id == 4) {
            drawY += 5;
        }

        for (Position item : tiles) {
            DrawRectangle((item.COL - minCol) * previewCellSize + drawX,
                (item.ROW - minRow) * previewCellSize + drawY,
                previewCellSize - 1, previewCellSize - 1,
                tempPiece.colours[tempPiece.id]);
        }
    }
    // Game Controls (Bottom-right) - X: 850, Y: 460
    // This will be drawn by Manager
}

void Game::SaveBoardState() {
    if (GameOver || isCountingDown) return;

    previousScore = score;
    previousLinesCleared = totalLinesCleared;
    previousBoardState = board.GetBoardState();

	previousPieceQueue = pieceQueue.GetAllPieces();

}

void Game::RestoreBoardState() {
    board.SetBoardState(previousBoardState);
}

void Game::UndoLastLock() {
    if (lockedPieceStack.IsEmpty() || previousBoardState.empty()) {
        return;
    }

    cout << "=== UNDO LAST LOCK ===" << endl;

    // Get the last locked piece
    Piece lastLockedPiece = lockedPieceStack.Pop();

    // RESTORE THE ENTIRE BOARD STATE (this will restore any cleared lines)
    RestoreBoardState();

    if (!previousPieceQueue.empty()) {
        pieceQueue.ClearAndSetPieces(previousPieceQueue);
    }

    // Reset the piece to top position to make it fall again
    UndoStack::ResetPieceToTop(lastLockedPiece);

    // ADD THIS LINE to also set it above the board:
    lastLockedPiece.rowOffset = -3;
    lastLockedPiece.colOffset = 5;

    // Set this as the current falling piece
    current = lastLockedPiece;

    // RESTORE score and lines
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
    if (!GameOver && !isCountingDown) {
        if (!IsHolding()) {
            // FIRST TIME: Store current piece and get next piece
            holdPiece = current;
            current = pieceQueue.Dequeue();

            // Check if new piece fits before spawning above
            current.rowOffset = 0;  // Test at row 0 first
            current.colOffset = 5;

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
            // SWAP: Exchange current piece with hold piece
            Piece temp = current;
            current = holdPiece;
            holdPiece = temp;

            // Check if swapped piece fits before spawning above
            current.rowOffset = 0;  // Test at row 0 first
            current.colOffset = 5;

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

    int rowsCleared = board.ClearRows();
    if (rowsCleared > 0) {
        PlaySound(ClearSound);
        UpdateScore(rowsCleared);
        cout << "Cleared " << rowsCleared << " rows! Current score: " << score << endl;
        
        AddLineClearMessage(rowsCleared);
    }


    // Get next piece from queue
    current = pieceQueue.Dequeue();

    // CHECK FOR GAME OVER BEFORE spawning above
    // Test if the piece would fit at spawn position (row 0)
    current.rowOffset = 0;  // Test at row 0 first
    current.colOffset = 5;

    if (!PieceFits()) {
        GameOver = true;
        leaderboard.AddScore(score);
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


    bool pieceMoved = false;

    // Handle Ctrl+Z for undoing last locked piece
    if ((IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL)) && IsKeyPressed(KEY_Z)) {
        UndoLastLock();
        pieceMoved = true;
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
    current.rowOffset = -3;
    current.colOffset = 5;

    // Reset hold
    isHolding = false;
    holdPiece = Piece();

    GameOver = false;
    score = 0;
    previousScore = 0;
    previousLinesCleared = 0;

    previousPieceQueue.clear();

    // Clear board state
    previousBoardState.clear();

    // Clear undo stack on reset
    lockedPieceStack.Clear();

    UpdateGhostPiece();
    totalPlayTime = 0;
    isTimeTracking = false;
    gameStartTime = 0;
    totalLinesCleared = 0;

        PlayMusicStream(music);

    cout << "Game reset complete!" << endl;

    
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


// Add this after other methods, before Draw()

void Game::AddLineClearMessage(int linesCleared) {

    if (linesCleared <= 0) return;

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

    activeMessages.push_back(msg);
}

void Game::UpdateMessages(float deltaTime) {
    // Update all active messages
    for (auto& msg : activeMessages) {
        if (msg.isActive) {
            msg.displayTime += deltaTime;
            if (msg.displayTime >= msg.duration) {
                msg.isActive = false;
            }
        }
    }

    // Remove inactive messages
    activeMessages.erase(
        remove_if(activeMessages.begin(), activeMessages.end(),
            [](const LineClearMessage& msg) { return !msg.isActive; }),
        activeMessages.end()
    );
}

void Game::DrawMessages() {
    // Draw all active messages on top of the board
    int boardX = 290;
    int boardY = 50;
    int boardWidth = 15 * 35;
    int boardHeight = 20 * 35;

    for (const auto& msg : activeMessages) {
        if (!msg.isActive) continue;

        // Calculate position (center of board)
        int centerX = boardX + boardWidth / 2;
        int centerY = boardY + boardHeight / 2;

        // Calculate alpha (fade out effect)
        float progress = msg.displayTime / msg.duration;
        float alpha = 1.0f - progress; // Fade from 1.0 to 0.0
        Color textColor = msg.color;
        textColor.a = static_cast<unsigned char>(alpha * 255);

        // Calculate font size (pulse effect)
        int fontSize = 30 + static_cast<int>(10 * sin(msg.displayTime * 5.0f));

        // Draw text
        Vector2 textSize = MeasureTextEx(GetFontDefault(), msg.text.c_str(), fontSize, 2);
        DrawTextEx(GetFontDefault(), msg.text.c_str(),
            { centerX - textSize.x / 2, centerY - textSize.y / 2 },
            fontSize, 2, textColor);
    }
}

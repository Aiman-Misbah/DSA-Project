#pragma once
#include <iostream>
#include "Board.h"
#include "Pieces.cpp"
#include "ScoreAVL.h"
#include "UndoStack.h"
#include "PieceQueue.h"
#include "Leaderboard.h"
#include <vector>
using namespace std;

class Game {
private:
    float lastMoveTime;
    float moveDelay = 0.1f;  // 0.1 seconds between moves (adjust as needed)
    float lastDownMoveTime;
    float downMoveDelay = 0.05f;  // Faster for down movement

    Board board;
    Piece current;
    bool HasCollided();
    void RotatePiece();
    void LockPiece();
    bool PieceFits();
    void UpdateScore(int lines);
    void MoveLeft();
    void MoveRight();
    Sound RotateSound;
    Sound ClearSound;
    double countdownStartTime;
    bool isCountingDown;
    int countdownNumber;
    Piece ghostPiece;
    void UpdateGhostPiece();
    double gameStartTime;
    double totalPlayTime;
    bool isTimeTracking;
    int totalLinesCleared;

    // Undo functionality for last locked piece
    UndoStack lockedPieceStack{ 1 };

    int previousScore;       
    int previousLinesCleared;



    void SaveBoardState();
    void UndoLastLock();

    // Piece Queue for next pieces
    PieceQueue pieceQueue;
    vector<Piece> previousPieceQueue;

    // Hold functionality
    void ToggleHold();
    Piece holdPiece;
    bool isHolding;

    vector<vector<int>> previousBoardState; // For undoing last locked piece

    void RestoreBoardState();

    struct LineClearMessage {
        string text;
        float displayTime = 0.0f;
        float duration = 2.0f;
        Color color = WHITE;
        bool isActive = false;
    };

    vector<LineClearMessage> activeMessages;

    void AddLineClearMessage(int linesCleared);

    Leaderboard leaderboard;

public:
    Game();
    void Draw();
    void HandleInput();
    void MoveDown();
    bool GameOver;
    int score;
    ~Game();
    Music music;
    bool musicOn;
    void ToggleMusic();
    void StartCountdown();
    void UpdateCountdown();
    bool IsCountingDown() const { return isCountingDown; }
    int GetCountdownNumber() const { return countdownNumber; }
    void ToggleGhostPiece() { showGhost = !showGhost; }
    bool showGhost;
    void HardDrop();
    bool isDropping;
    void UpdateHardDrop();
    double GetPlayTime() const;
    void StartTimeTracking();
    void StopTimeTracking();
    int GetTotalLinesCleared() const { return totalLinesCleared; }

    // Public undo method for last locked piece
    void UndoLastLockedPiece();


    // Hold methods
    bool IsHolding() const { return isHolding; }
    Piece GetHoldPiece() const { return holdPiece; }

    void UpdateMessages(float deltaTime);
    void DrawMessages();
    void Reset();

    Leaderboard& GetLeaderboard() { return leaderboard; }

    // In Game.h public section:
    const PieceQueue& GetPieceQueue() const { return pieceQueue; }
};
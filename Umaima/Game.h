#pragma once
#include <iostream>
#include "Board.h"
#include "Pieces.cpp"
#include "LinkedList.h"
#include "ScoreAVL.h"
#include "UndoStack.h"
#include "PieceQueue.h"
#include <vector>
#include <raylib.h>
using namespace std;

class Game {
private:
    Board board;
    LinkedList pieceBag;
    Piece current;
    bool HasCollided();
    void RotatePiece();
    void LockPiece();
    bool PieceFits();
    void Reset();
    void UpdateScore(int lines);
    void MoveLeft();
    void MoveRight();
    Piece GetRandomPiece();
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
    double lastPauseTime;
    int totalLinesCleared;

    // Undo functionality for last locked piece
    UndoStack lockedPieceStack{ 1 };

    int previousScore;        // Store previous scores in AVL
    int previousLinesCleared;



    void SaveBoardState();
    void UndoLastLock();

    // Piece Queue for next pieces
    PieceQueue pieceQueue;

    // Hold functionality
    void ToggleHold();
    Piece holdPiece;
    bool isHolding;
    bool canUseHold;

	vector<vector<int>> previousBoardState; // For undoing last locked piece

    void RestoreBoardState();

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

    // Method to display piece queue
    void DisplayPieceQueue();

    // Hold methods
    void ToggleHoldPiece();
    bool IsHolding() const { return isHolding; }
    bool CanUseHold() const { return canUseHold; }
    Piece GetHoldPiece() const { return holdPiece; }
};

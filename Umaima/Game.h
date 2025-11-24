#pragma once
#include <iostream>
#include "Board.h"
#include "Pieces.cpp"
#include "LinkedList.h"
#include "ScoreAVL.h"  // ADD: Include AVL tree
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
    Piece next;
    bool HasCollided();
    void RotatePiece();
    void LockPiece();
    bool PieceFits();
    void Reset();
    void UpdateScore(int lines, int down);
    void MoveLeft();
    void MoveRight();
    Piece GetRandomPiece();
    Sound RotateSound;
    Sound ClearSound;
    double countdownStartTime;      // YOUR: Countdown timer
    bool isCountingDown;            // YOUR: Countdown state
    int countdownNumber;            // YOUR: Countdown number
    Piece ghostPiece;               // YOUR: Ghost piece
    void UpdateGhostPiece();        // YOUR: Update ghost
    double gameStartTime;
    double totalPlayTime;
    bool isTimeTracking;
    double lastPauseTime;
    int totalLinesCleared;

    UndoStack undoStack;
    PieceQueue pieceQueue;
    void SaveUndoState();
    void UndoMove();

public:
    Game();
    void Draw();
    void HandleInput();
    void MoveDown();
    bool GameOver;
    int score;                      // KEEP: Your score variable name
    ScoreAVL scores;                // ADD: AVL tree for scores
    ~Game();
    Music music;
    bool musicOn;
    void ToggleMusic();
    void StartCountdown();          // YOUR: Countdown methods
    void UpdateCountdown();
    bool IsCountingDown() const { return isCountingDown; }
    int GetCountdownNumber() const { return countdownNumber; }
    void ToggleGhostPiece() { showGhost = !showGhost; }
    bool showGhost;                 // YOUR: Ghost piece toggle
    void HardDrop();                // YOUR: Hard drop
    bool isDropping;                // YOUR: Drop state
    void UpdateHardDrop();          // YOUR: Update hard drop
    double GetPlayTime() const;
    void StartTimeTracking();
    void StopTimeTracking();
    int GetTotalLinesCleared() const{ return totalLinesCleared;  }
};

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
    Board board;        //linked list of 20 rows
    Piece current;      //currently falling piece
    bool HasCollided(); //checking for hits
    void RotatePiece();
    void LockPiece();   //locking it the board at the bottom
    bool PieceFits();       //checking if the piece fits on the 
    void UpdateScore(int lines);
    void MoveLeft();
    void MoveRight();
    Sound RotateSound;
    Sound ClearSound;
    double countdownStartTime;  //track time when the countdown has started
    bool isCountingDown;
    int countdownNumber;    //3 - 2 - 1
    Piece ghostPiece;
    void UpdateGhostPiece();    //changing its position according to the current piece
    double gameStartTime;   //when the game has started
    double totalPlayTime;   //game ka time
    bool isTimeTracking;    //is time stopped or not
    int totalLinesCleared;  //completed rows

    // Undo functionality for last locked piece
    UndoStack lockedPieceStack{ 1 };

    //for restoring after undo
    int previousScore;     
    int previousLinesCleared;
    void SaveBoardState(); 
    void UndoLastLock();
    vector<vector<int>> previousBoardState;
    void RestoreBoardState();

    PieceQueue pieceQueue;      //next pieces
    vector<Piece> previousPieceQueue;   //for undo

    // Hold functionality
    void ToggleHold();
    Piece holdPiece;
    bool isHolding;


    struct LineClearMessage {   //SINGLE DOUBLE waghaira
        string text;
        float displayTime = 0.0f;   //message kab se display ho rha hai
        float duration = 2.0f;
        Color color = WHITE;    //default colour (for now)
        bool isActive = false;  //message dikhna bhi chahye ya nhi
    };

    LineClearMessage activeMessages;

    void AddLineClearMessage(int linesCleared);

    Leaderboard leaderboard;        //for final and best scores

public:
    Game();
    ~Game();
    void Draw();    //drawing the board, current and ghost pieces
    void HandleInput();
    void MoveDown();
    bool GameOver;  //game khatam hogya ya nhi
    int score;      //current score during game
    Music music;
    bool musicOn;
    void ToggleMusic();     //for the music button - pause & play
    void StartCountdown();  
    void UpdateCountdown();
    bool IsCountingDown() const { return isCountingDown; }
    int GetCountdownNumber() const { return countdownNumber; }
    void ToggleGhostPiece() { showGhost = !showGhost; }
    bool showGhost;         //ghost dikhaana hai ya nhi
    void HardDrop();        //spacebar wali cheez
    bool isDropping;        //is it dropping or not
    void UpdateHardDrop();
    double GetPlayTime() const;
    void StartTimeTracking();
    void StopTimeTracking();
    int GetTotalLinesCleared() const { return totalLinesCleared; }

    void UndoLastLockedPiece();

    bool IsHolding() const { return isHolding; }
    Piece GetHoldPiece() const { return holdPiece; }

    void UpdateMessages(float deltaTime);   //updating the timers and checking if duration is complete
    void DrawMessages();    //draws message with the animation
    void Reset();
        
    //used for drawing it in Manager.cpp
    Leaderboard& GetLeaderboard() { return leaderboard; } 
    const PieceQueue& GetPieceQueue() const { return pieceQueue; }
};

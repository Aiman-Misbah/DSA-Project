#pragma once
#include "ScoreAVL.h"
#include <iostream>
#include "Board.h"
#include "Pieces.cpp"
#include "LinkedList.h"
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

public:
    Game();
    void Draw();
    void HandleInput();
    void MoveDown();
    bool GameOver;
    ScoreAVL scores;
    int currentScore; // ongoing session score
    ~Game();
    Music music;
    bool musicOn;           // ADD: Music state
    void ToggleMusic();     // ADD: Toggle method
};
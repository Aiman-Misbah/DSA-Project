#pragma once
#include <iostream>
#include "Board.h"
#include "Pieces.cpp"
#include "LinkedList.h"  // Add this
#include <vector>
#include <raylib.h>
using namespace std;

class Game {
private:
    Board board;
    LinkedList pieceBag;  // Replace vector with LinkedList
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
    int score;
    ~Game();
    Music music;
};
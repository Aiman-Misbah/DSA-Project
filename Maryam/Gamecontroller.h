#pragma once
#include "Board.h"
#include "Piece.h"
#include "PieceQueue.h"
#include "UndoStack.h"

class GameController {
private:
    Board* board;           // pointer to existing Board instance (not owned)
    PieceQueue queue;       // upcoming pieces (by value)
    UndoStack undo;         // movement undo stack (snapshots of current piece)

    Piece current;          // current active piece
    Piece nextPiece;        // next preview piece

    // validate a candidate (returns true if candidate does NOT collide)
    bool IsValidPosition(const Piece& candidate) const;

public:
    GameController(Board* b, int queueSize = 5, int undoCapacity = 50);

    // initialize queue and spawn first pieces
    void Init();

    // spawn next piece (after locking)
    void SpawnFirstPieces();
    void SpawnNextPiece();

    // movement API
    void SaveSnapshot();      // push current piece state to undo stack
    void UndoLastMovement();  // pop and restore

    void MoveLeft();
    void MoveRight();
    void MoveDown();
    void Rotate();

    // lock piece into board and spawn next; returns rows cleared
    int LockPieceAndSpawnNext();

    // getters for drawing/external use
    Piece& GetCurrent();
    Piece& GetNext();
};

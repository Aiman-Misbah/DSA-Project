#pragma once
#include "GameController.h"
#include "Pieces.cpp"   // ensure concrete piece constructors are available
#include <vector>

GameController::GameController(Board* b, int queueSize, int undoCapacity)
    : board(b), queue(queueSize), undo(undoCapacity) {
    // Intentionally empty
}

void GameController::Init() {
    queue.FillStartup();
    SpawnFirstPieces();
    undo.Clear();
}

void GameController::SpawnFirstPieces() {
    current = queue.Dequeue();
    nextPiece = queue.Peek();
}

void GameController::SpawnNextPiece() {
    current = queue.Dequeue();
    nextPiece = queue.Peek();
}

bool GameController::IsValidPosition(const Piece& candidate) const {
    Piece temp = candidate;
    // check if any tile collides with board
    std::vector<Position> tiles = temp.GetCellPositions();
    for (const Position& p : tiles) {
        if (board->CollisionDetected(p.ROW, p.COL)) return false;
    }
    return true;
}

void GameController::SaveSnapshot() {
    undo.Push(current);
}

void GameController::UndoLastMovement() {
    if (undo.IsEmpty()) return;
    Piece prev = undo.Pop();
    current = prev;
}

void GameController::MoveLeft() {
    SaveSnapshot();
    current.Move(0, -1);
    if (!IsValidPosition(current)) {
        UndoLastMovement();
    }
}

void GameController::MoveRight() {
    SaveSnapshot();
    current.Move(0, 1);
    if (!IsValidPosition(current)) {
        UndoLastMovement();
    }
}

void GameController::MoveDown() {
    SaveSnapshot();
    current.Move(1, 0);
    if (!IsValidPosition(current)) {
        // revert last move (common behavior: move back up)
        UndoLastMovement();
    }
}

void GameController::Rotate() {
    SaveSnapshot();
    // try rotation using existing Piece::RotateWithWallKicks (call will internally try offsets)
    bool success = current.RotateWithWallKicks(*board);
    if (!success) {
        // RotateWithWallKicks already backtracked to original if failed, but we also restore from snapshot to be safe
        UndoLastMovement();
    }
}

int GameController::LockPieceAndSpawnNext() {
    // write current piece into board
    std::vector<Position> tiles = current.GetCellPositions();
    for (const Position& p : tiles) {
        board->SetCell(p.ROW, p.COL, current.id);
    }

    // spawn next piece
    SpawnNextPiece();

    // clear rows and return number cleared
    int cleared = board->ClearRows();
    return cleared;
}

Piece& GameController::GetCurrent() {
    return current;
}

Piece& GameController::GetNext() {
    return nextPiece;
}

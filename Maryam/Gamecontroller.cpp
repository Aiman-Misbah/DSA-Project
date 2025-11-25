#pragma once
#include "GameController.h"
#include "Pieces.cpp"   // ensure concrete piece constructors are available
#include <vector>
#include <iostream>

GameController::GameController(Board* b, int queueSize, int undoCapacity)
    : board(b), queue(queueSize), undo(undoCapacity), canHold(true) {
    // Intentionally empty
}

void GameController::Init() {
    queue.FillStartup();
    SpawnFirstPieces();
    undo.Clear();
    canHold = true;
    holdPiece = Piece();
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
        UndoLastMovement();
    }
}

void GameController::Rotate() {
    SaveSnapshot();
    bool success = current.RotateWithWallKicks(*board);
    if (!success) {
        UndoLastMovement();
    }
}

// NEW: Hold current piece functionality
void GameController::HoldCurrentPiece() {
    if (!canHold) {
        std::cout << "Cannot hold - already used hold this turn!" << std::endl;
        return;
    }

    std::cout << "Holding current piece..." << std::endl;

    if (!HasHoldPiece()) {
        // First time holding - just store current piece and get next one
        holdPiece = current;
        SpawnNextPiece();
        std::cout << "Stored piece in hold. ID: " << holdPiece.id << std::endl;
    }
    else {
        // Swap current piece with hold piece
        Piece temp = current;
        current = holdPiece;
        holdPiece = temp;

        // Reset current piece position to top
        current.rowOffset = 0;
        current.colOffset = 3;
        std::cout << "Swapped with hold piece. Current ID: " << current.id << ", Hold ID: " << holdPiece.id << std::endl;
    }

    canHold = false; // Can only hold once per piece
}

int GameController::LockPieceAndSpawnNext() {
    // write current piece into board
    std::vector<Position> tiles = current.GetCellPositions();
    for (const Position& p : tiles) {
        board->SetCell(p.ROW, p.COL, current.id);
    }

    // spawn next piece
    SpawnNextPiece();

    // NEW: Reset hold ability after locking piece
    canHold = true;
    std::cout << "Hold ability reset for new piece" << std::endl;

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
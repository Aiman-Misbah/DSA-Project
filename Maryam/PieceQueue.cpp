#pragma once
#include "PieceQueue.h"
#include <cstdlib>
#include <ctime>

// Include concrete piece definitions so CreateRandomPiece can construct them.
// Your project already uses this pattern (Game.cpp includes Pieces.cpp), so this matches it.
#include "Pieces.cpp"

Piece PieceQueue::CreateRandomPiece() {
    int r = rand() % 7; // 0..6
    switch (r) {
    case 0: { LPiece p; return p; }
    case 1: { JPiece p; return p; }
    case 2: { IPiece p; return p; }
    case 3: { OPiece p; return p; }
    case 4: { SPiece p; return p; }
    case 5: { TPiece p; return p; }
    default: { ZPiece p; return p; }
    }
}

PieceQueue::PieceQueue(int cap) : capacity(cap) {
    srand((unsigned int)time(nullptr));
    FillStartup();
}

PieceQueue::~PieceQueue() {
    q.clear();
}

bool PieceQueue::IsEmpty() const {
    return q.empty();
}

bool PieceQueue::IsFull() const {
    return (int)q.size() >= capacity;
}

void PieceQueue::FillStartup() {
    q.clear();
    while (!IsFull()) {
        q.push_back(CreateRandomPiece());
    }
}

Piece PieceQueue::Dequeue() {
    if (IsEmpty()) {
        // keep behavior consistent: return a random piece and refill
        Piece p = CreateRandomPiece();
        q.push_back(CreateRandomPiece());
        return p;
    }

    Piece front = q.front();
    q.erase(q.begin());
    // push a new random piece so queue keeps its capacity
    q.push_back(CreateRandomPiece());
    return front;
}

Piece PieceQueue::Peek() const {
    if (IsEmpty()) return Piece();
    return q.front();
}

void PieceQueue::Clear() {
    q.clear();
}

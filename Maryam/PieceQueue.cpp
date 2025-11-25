#pragma once
#include "PieceQueue.h"
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <algorithm>

// Include concrete piece definitions so CreateRandomPiece can construct them.
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
    srand(static_cast<unsigned int>(time(nullptr)));  // Fixed time conversion
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

    // Display next three pieces after dequeue
    DisplayNextThree();

    return front;
}

Piece PieceQueue::Peek() const {
    if (IsEmpty()) return Piece();
    return q.front();
}

void PieceQueue::Clear() {
    q.clear();
}

// Display next three pieces in console
void PieceQueue::DisplayNextThree() const {
    std::cout << "=== NEXT 3 PIECES IN QUEUE ===" << std::endl;

    if (q.empty()) {
        std::cout << "Queue is empty!" << std::endl;
        return;
    }

    // Display up to 3 pieces or whatever is available
    int displayCount = std::min(3, (int)q.size());

    for (int i = 0; i < displayCount; i++) {
        std::cout << i + 1 << ". ";

        switch (q[i].id) {
        case 1: std::cout << "L-Piece"; break;
        case 2: std::cout << "J-Piece"; break;
        case 3: std::cout << "I-Piece"; break;
        case 4: std::cout << "O-Piece"; break;
        case 5: std::cout << "S-Piece"; break;
        case 6: std::cout << "T-Piece"; break;
        case 7: std::cout << "Z-Piece"; break;
        default: std::cout << "Unknown Piece"; break;
        }

        // Display piece position info
        std::cout << " (Position: " << q[i].rowOffset << "," << q[i].colOffset << ")";
        std::cout << std::endl;
    }

    if (q.size() > 3) {
        std::cout << "... and " << (q.size() - 3) << " more pieces" << std::endl;
    }

    std::cout << "===============================" << std::endl;
}

// Get the next three pieces for display
std::vector<Piece> PieceQueue::GetNextThree() const {
    std::vector<Piece> nextThree;

    if (q.empty()) {
        return nextThree;
    }

    int displayCount = std::min(3, (int)q.size());

    for (int i = 0; i < displayCount; i++) {
        nextThree.push_back(q[i]);
    }

    return nextThree;
}
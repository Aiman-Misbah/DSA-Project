#pragma once
#include "PieceQueue.h"
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <algorithm>
using namespace std;

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
    cout << "=== NEXT 3 PIECES IN QUEUE ===" << endl;

    if (q.empty()) {
        cout << "Queue is empty!" << endl;
        return;
    }

    // Display up to 3 pieces or whatever is available
    int displayCount = min(3, (int)q.size());

    for (int i = 0; i < displayCount; i++) {
        cout << i + 1 << ". ";

        switch (q[i].id) {
        case 1: cout << "L-Piece"; break;
        case 2: cout << "J-Piece"; break;
        case 3: cout << "I-Piece"; break;
        case 4: cout << "O-Piece"; break;
        case 5: cout << "S-Piece"; break;
        case 6: cout << "T-Piece"; break;
        case 7: cout << "Z-Piece"; break;
        default: cout << "Unknown Piece"; break;
        }

        // Display piece position info
        cout << " (Position: " << q[i].rowOffset << "," << q[i].colOffset << ")";
        cout << endl;
    }

    if (q.size() > 3) {
        cout << "... and " << (q.size() - 3) << " more pieces" << endl;
    }

    cout << "===============================" << endl;
}

// Get the next three pieces for display
vector<Piece> PieceQueue::GetNextThree() const {
    vector<Piece> nextThree;

    if (q.empty()) {
        return nextThree;
    }

    int displayCount = min(3, (int)q.size());

    for (int i = 0; i < displayCount; i++) {
        nextThree.push_back(q[i]);
    }

    return nextThree;
}

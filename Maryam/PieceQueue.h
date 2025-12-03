#pragma once
#include <vector>
#include "Piece.h"
#include <iostream>
using namespace std;

class PieceQueue {
private:

    vector<Piece> q;
    int capacity;

    // helper: create a random concrete piece (implemented in cpp)
    Piece CreateRandomPiece();

    // NEW: Create random piece that doesn't appear in recent window
    Piece CreateRandomPieceNoRepeat();

    // NEW: Check if a piece would appear in the last (windowSize-1) positions
    bool WouldCauseRepeat(int pieceId, int windowSize) const;

    // NEW: Check both window sizes (2 and 3)
    bool WouldCauseAnyRepeat(int pieceId) const;

public:
    PieceQueue(int cap = 5);
    ~PieceQueue();

    bool IsEmpty() const;
    bool IsFull() const;

    // Dequeue: returns the front Piece by value, and pushes a new random piece at back
    Piece Dequeue();

    // Peek front without removing
    Piece Peek() const;

    // Fill queue to capacity (call at startup or after clear)
    void FillStartup();

    // Clear queue
    void Clear();

    // NEW: Display next three pieces in console
    void DisplayNextThree() const;

    // NEW: Get the next three pieces for display
    vector<Piece> GetNextThree() const;

    vector<Piece> GetAllPieces() const { return q; }  // Get copy of all pieces
    void ClearAndSetPieces(const vector<Piece>& newPieces);  // Set the queue
};

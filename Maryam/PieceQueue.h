#pragma once
#include <vector>
#include "Piece.h"

class PieceQueue {
private:
    std::vector<Piece> q;
    int capacity;

    // helper: create a random concrete piece (implemented in cpp)
    Piece CreateRandomPiece();

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
    std::vector<Piece> GetNextThree() const;
};
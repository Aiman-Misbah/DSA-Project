#pragma once
#include "Queue.h"      // Your custom Queue
#include "LinkedList.h" // Your LinkedList for bag
#include <iostream>
using namespace std;

class PieceQueue {
private:
    Queue q;           // YOUR custom Queue!
    LinkedList bag;    // LinkedList for 7-bag
    int capacity;

    // Piece generation
    Piece CreateRandomPiece();
    Piece CreateRandomPieceNoRepeat();

    // Window checking
    bool WouldCauseRepeat(int pieceId, int windowSize) const;
    bool WouldCauseAnyRepeat(int pieceId) const;

public:
    PieceQueue(int cap = 5);

    bool IsEmpty() const;
    bool IsFull() const;
    Piece Dequeue();
    Piece Peek() const;
    void FillStartup();
    void Clear();
    void DisplayNextThree() const;
    vector<Piece> GetNextThree() const;
    vector<Piece> GetAllPieces() const;
    void ClearAndSetPieces(const vector<Piece>& newPieces);
};

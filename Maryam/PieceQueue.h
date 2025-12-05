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

    // SIMPLIFIED: No repeat checking needed
    Piece CreateRandomPiece();

public:
    PieceQueue(int cap = 5);

    bool IsEmpty() const;
    bool IsFull() const;
    Piece Dequeue();
    void FillStartup();
    void Clear();
    vector<Piece> GetNextThree() const;
    vector<Piece> GetAllPieces() const;
    void ClearAndSetPieces(const vector<Piece>& newPieces);
};
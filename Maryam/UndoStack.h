#pragma once
#include <vector>
#include "Piece.h"
#include <iostream>
using namespace std;

class UndoStack {
private:
    
    
    vector<Piece> stack;
    int capacity;

public:
    UndoStack(int cap = 50);
    ~UndoStack();

    void Push(const Piece& snapshot);
    bool IsEmpty() const;
    bool IsFull() const;
    Piece Pop();
    void Clear();
    int Size() const;

    // Reset piece to top position for falling effect
    static void ResetPieceToTop(Piece& piece);
};

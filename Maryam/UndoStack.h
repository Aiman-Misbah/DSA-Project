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
    UndoStack(int cap = 1);
    ~UndoStack();

    void Push(const Piece& snapshot);
    bool IsEmpty() const;
    bool IsFull() const;
    Piece Pop();
    void Clear();

    // Reset piece to top position for falling effect
    static void ResetPieceToTop(Piece& piece);
};
#pragma once
#include <vector>
#include "Piece.h"

class UndoStack {
private:
    std::vector<Piece> stack;
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
};

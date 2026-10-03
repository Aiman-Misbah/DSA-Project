#include "data_structures/UndoStack.h"
#include <iostream>
using namespace std;

UndoStack::UndoStack(int cap) : capacity(cap) {
    stack.reserve(capacity);    //vector ka apna method hai to create that much space
}

UndoStack::~UndoStack() {
    stack.clear();
}

void UndoStack::Push(const Piece& snapshot) {
    if (IsFull()) {
        stack.erase(stack.begin());   //stack.clear()
    }
    stack.push_back(snapshot);
}

bool UndoStack::IsEmpty() const {
    return stack.empty();
}

bool UndoStack::IsFull() const {
    return (int)stack.size() >= capacity;
}

Piece UndoStack::Pop() {
    if (IsEmpty()) return Piece();
    Piece top = stack.back();
    stack.pop_back();
    return top;
}

void UndoStack::Clear() {
    stack.clear();
}


// Reset piece to top position for falling effect
void UndoStack::ResetPieceToTop(Piece& piece) {
    piece.rowOffset = -3;  // Start above the visible board
    piece.colOffset = 5;   // Center position
}

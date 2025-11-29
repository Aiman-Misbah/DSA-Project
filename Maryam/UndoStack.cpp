#pragma once
#include "UndoStack.h"
#include <iostream>
using namespace std;

UndoStack::UndoStack(int cap) : capacity(cap) {
    stack.reserve(capacity);
}

UndoStack::~UndoStack() {
    stack.clear();
}

void UndoStack::Push(const Piece& snapshot) {
    if (IsFull()) {
        // drop oldest snapshot to make room
        stack.erase(stack.begin());
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

int UndoStack::Size() const {
    return (int)stack.size();
}

// Reset piece to top position for falling effect
void UndoStack::ResetPieceToTop(Piece& piece) {
    piece.rowOffset = -2;  // Start above the visible board
    piece.colOffset = 3;   // Center position
}

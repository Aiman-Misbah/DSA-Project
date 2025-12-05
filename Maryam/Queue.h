#pragma once
#include "Piece.h"
#include <iostream>
using namespace std;

class QueueNode {
public:
    Piece data;
    QueueNode* next;

    QueueNode(Piece piece) : data(piece), next(nullptr) {}
};

class Queue {
private:
    QueueNode* front;
    QueueNode* rear;
    int size;
    int capacity;

public:
    Queue(int cap = 5);
    ~Queue();

    // Core queue operations
    void enqueue(Piece piece);
    Piece dequeue();
    Piece peek() const;

    // Utility
    bool isEmpty() const;
    bool isFull() const;
    int getSize() const;
    void clear();

    // For display
    vector<Piece> getAllPieces() const;
    vector<Piece> getNextThree() const;
};
#pragma once
#include "Piece.h"
#include <iostream>
using namespace std;

class QueueNode {   //queue for the next pieces
public:
    Piece data;
    QueueNode* next;

    QueueNode(Piece piece) : data(piece), next(NULL) {}
};

class Queue {
private:
    QueueNode* front;
    QueueNode* rear;
    int size;       //how many pieces currently in the queue
    int capacity;   //kitne aaskte hain (5)

public:
    Queue(int cap = 5);
    ~Queue();

    void enqueue(Piece piece);
    Piece dequeue();
    Piece peek() const;

    bool isEmpty() const;
    bool isFull() const;
    int getSize() const;    //returns current no of elements
    void clear();   //empties the queue

    
    vector<Piece> getAllPieces() const; //for undo (restoring)
    vector<Piece> getNextThree() const; //for the display
};

#pragma once
#include "Queue.h"  
#include "LinkedList.h"
#include <iostream>
using namespace std;

class PieceQueue {
private:
    Queue q;        //next pieces' queue
    LinkedList bag; //the piece bag
    int capacity;

    Piece CreateRandomPiece();  //randomly choosing one piece from the bag

public:
    PieceQueue(int cap = 5);

    bool IsEmpty() const;
    bool IsFull() const;
    Piece Dequeue();
    void FillStartup(); //fills queue with 5 random pieces
    void Clear();       //clears both bag and queue
    vector<Piece> GetNextThree() const; //same as in Queue files
    vector<Piece> GetAllPieces() const;
    void ClearAndSetPieces(const vector<Piece>& newPieces); //restoring queue after undo
};

#include "PieceQueue.h"
#include "Pieces.cpp"
#include <cstdlib>
#include <ctime>
#include <iostream>
using namespace std;


Piece PieceQueue::CreateRandomPiece() {
    // Refill bag if empty
    if (bag.GetSize() == 0) {
        bag.AddPiece(LPiece());
        bag.AddPiece(JPiece());
        bag.AddPiece(IPiece());
        bag.AddPiece(OPiece());
        bag.AddPiece(SPiece());
        bag.AddPiece(TPiece());
        bag.AddPiece(ZPiece());
    }

    // Get random piece from bag
    int index = rand() % bag.GetSize();
    Piece p = bag.GetPiece(index);
    bag.RemovePiece(index);  // Remove that from the bag as well
    return p;
}

PieceQueue::PieceQueue(int cap) : q(cap), capacity(cap) {
    srand(time(NULL));
    FillStartup();
}

void PieceQueue::FillStartup() {
    q.clear();  //first clearing the queue

    while (!IsFull()) {
        q.enqueue(CreateRandomPiece()); //then add random pieces one by one till full
    }

}

Piece PieceQueue::Dequeue() {   //never to be empty
    if (IsEmpty()) {
        Piece p = CreateRandomPiece();
        q.enqueue(p);
        return p;
    }

    //agar hata rahe hain to add bhi krenge (for the agla piece)
    Piece front = q.dequeue();
    q.enqueue(CreateRandomPiece()); 
    return front;
}

bool PieceQueue::IsEmpty() const {
    return q.isEmpty();
}

bool PieceQueue::IsFull() const {
    return q.isFull();
}

void PieceQueue::Clear() {
    q.clear();
    bag.Clear();
}


vector<Piece> PieceQueue::GetNextThree() const {
    return q.getNextThree();
}

vector<Piece> PieceQueue::GetAllPieces() const {
    return q.getAllPieces();
}

void PieceQueue::ClearAndSetPieces(const vector<Piece>& newPieces) {
    q.clear();  //first clear the current queue
    for (const Piece& piece : newPieces) {
        if (!q.isFull()) {
            q.enqueue(piece);   //then store the previous one in the current one
        }
    }
    cout << "Queue manually set to " << q.getSize() << " pieces" << endl;
}

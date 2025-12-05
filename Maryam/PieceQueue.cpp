#include "PieceQueue.h"
#include <cstdlib>
#include <ctime>
#include <iostream>
using namespace std;

#include "Pieces.cpp"

// SIMPLIFIED: Just get piece from bag, no repeat checks
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
    bag.RemovePiece(index);  // Removes from linked list bag!
    return p;
}

// Constructor
PieceQueue::PieceQueue(int cap) : q(cap), capacity(cap) {
    srand(time(nullptr));
    FillStartup();
}

// SIMPLIFIED: Always use CreateRandomPiece, not CreateRandomPieceNoRepeat
void PieceQueue::FillStartup() {
    q.clear();

    cout << "=== QUEUE INITIALIZED ===" << endl;

    while (!IsFull()) {
        q.enqueue(CreateRandomPiece());  // NO repeat checks!
    }

}

// SIMPLIFIED: Always get fresh piece, no repeat checks
Piece PieceQueue::Dequeue() {
    if (IsEmpty()) {
        Piece p = CreateRandomPiece();
        q.enqueue(CreateRandomPiece());  // NO repeat checks!
        return p;
    }

    Piece front = q.dequeue();
    q.enqueue(CreateRandomPiece());  // NO repeat checks!
    return front;
}

// Rest of the functions stay the same...
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
    q.clear();
    for (const auto& piece : newPieces) {
        if (!q.isFull()) {
            q.enqueue(piece);
        }
    }
    cout << "Queue manually set to " << q.getSize() << " pieces" << endl;
}
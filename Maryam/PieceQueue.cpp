#include "PieceQueue.h"
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <algorithm>
#include <set>
using namespace std;

#include "Pieces.cpp"

// Window checking methods (same as before)
bool PieceQueue::WouldCauseRepeat(int pieceId, int windowSize) const {
    vector<Piece> pieces = q.getAllPieces();
    if (pieces.empty()) return false;

    int checkCount = min(windowSize - 1, (int)pieces.size());
    for (int i = 0; i < checkCount; i++) {
        int index = pieces.size() - 1 - i;
        if (pieces[index].id == pieceId) {
            return true;
        }
    }
    return false;
}

bool PieceQueue::WouldCauseAnyRepeat(int pieceId) const {
    return WouldCauseRepeat(pieceId, 2) || WouldCauseRepeat(pieceId, 3);
}

// Piece generation (same logic, different queue calls)
Piece PieceQueue::CreateRandomPiece() {
    if (bag.GetSize() == 0) {
        bag.AddPiece(LPiece());
        bag.AddPiece(JPiece());
        bag.AddPiece(IPiece());
        bag.AddPiece(OPiece());
        bag.AddPiece(SPiece());
        bag.AddPiece(TPiece());
        bag.AddPiece(ZPiece());
    }

    int index = rand() % bag.GetSize();
    Piece p = bag.GetPiece(index);
    bag.RemovePiece(index);
    return p;
}

Piece PieceQueue::CreateRandomPieceNoRepeat() {
    Piece newPiece = CreateRandomPiece();

    for (int i = 0; i < 10; i++) {
        if (!WouldCauseAnyRepeat(newPiece.id)) {
            return newPiece;
        }
        newPiece = CreateRandomPiece();
    }
    return newPiece;
}

// Queue operations using YOUR custom Queue
PieceQueue::PieceQueue(int cap) : q(cap), capacity(cap) {
    srand(time(nullptr));
    FillStartup();
}

bool PieceQueue::IsEmpty() const {
    return q.isEmpty();
}

bool PieceQueue::IsFull() const {
    return q.isFull();
}

void PieceQueue::FillStartup() {
    q.clear();

    cout << "=== QUEUE INITIALIZED ===" << endl;

    while (!IsFull()) {
        if (q.isEmpty()) {
            q.enqueue(CreateRandomPiece());
        }
        else {
            q.enqueue(CreateRandomPieceNoRepeat());
        }
    }

    DisplayNextThree();
}

Piece PieceQueue::Dequeue() {
    if (IsEmpty()) {
        Piece p = CreateRandomPiece();
        q.enqueue(CreateRandomPieceNoRepeat());
        return p;
    }

    Piece front = q.dequeue();  // O(1) dequeue!
    q.enqueue(CreateRandomPieceNoRepeat());
    DisplayNextThree();
    return front;
}

Piece PieceQueue::Peek() const {
    return q.peek();
}

void PieceQueue::Clear() {
    q.clear();
    bag.Clear();
}

// Display using your Queue's methods
void PieceQueue::DisplayNextThree() const {
    cout << "=== NEXT 3 PIECES IN QUEUE ===" << endl;

    vector<Piece> nextThree = q.getNextThree();
    if (nextThree.empty()) {
        cout << "Queue is empty!" << endl;
        return;
    }

    for (int i = 0; i < nextThree.size(); i++) {
        cout << i + 1 << ". ";
        switch (nextThree[i].id) {
        case 1: cout << "L-Piece"; break;
        case 2: cout << "J-Piece"; break;
        case 3: cout << "I-Piece"; break;
        case 4: cout << "O-Piece"; break;
        case 5: cout << "S-Piece"; break;
        case 6: cout << "T-Piece"; break;
        case 7: cout << "Z-Piece"; break;
        default: cout << "Unknown"; break;
        }
        cout << " (ID: " << nextThree[i].id << ")" << endl;
    }

    cout << "===============================" << endl;
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

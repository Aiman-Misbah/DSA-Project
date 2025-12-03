#include "PieceQueue.h"
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <algorithm>
#include <set>
using namespace std;

// Include concrete piece definitions so CreateRandomPiece can construct them.
#include "Pieces.cpp"

// NEW: Helper function to check if a piece would appear more than once in the last N pieces
bool PieceQueue::WouldCauseRepeat(int pieceId, int windowSize) const {
    if (q.empty()) return false;

    // Check the last (windowSize-1) pieces (or fewer if queue is smaller)
    int checkCount = min(windowSize - 1, (int)q.size());

    for (int i = 0; i < checkCount; i++) {
        int index = q.size() - 1 - i; // Start from the end and go backwards
        if (q[index].id == pieceId) {
            return true; // Piece already appears in the window
        }
    }

    return false;
}

// NEW: Check both window sizes (2 and 3)
bool PieceQueue::WouldCauseAnyRepeat(int pieceId) const {
    // Check for window size 2 (piece shouldn't appear in last 1 position)
    if (WouldCauseRepeat(pieceId, 2)) {
        return true;
    }

    // Check for window size 3 (piece shouldn't appear in last 2 positions)
    if (WouldCauseRepeat(pieceId, 3)) {
        return true;
    }

    return false;
}

// NEW: Modified version that ensures piece doesn't appear in last 1 OR 2 positions
Piece PieceQueue::CreateRandomPieceNoRepeat() {
    // Try to generate a piece that doesn't appear in the recent window
    const int MAX_ATTEMPTS = 50; // Higher because this is more restrictive
    Piece newPiece;

    for (int attempt = 0; attempt < MAX_ATTEMPTS; attempt++) {
        newPiece = CreateRandomPiece();

        // Check if this piece appears in the last 1 OR 2 positions
        if (!WouldCauseAnyRepeat(newPiece.id)) {
            return newPiece; // Good piece found
        }

        // Debug: Log when we're having trouble finding a piece
        if (attempt == MAX_ATTEMPTS / 2) {
            cout << "Debug: Having difficulty finding unique piece..." << endl;
            if (!q.empty()) {
                cout << "Recent pieces: ";
                int showCount = min(3, (int)q.size());
                for (int i = q.size() - showCount; i < q.size(); i++) {
                    if (i >= 0) cout << q[i].id << " ";
                }
                cout << endl;
            }
        }
    }

    // If we can't find a unique piece after many attempts,
    // just return whatever we got (fallback to prevent infinite loop)
    cout << "Warning: Could not find piece without repeats in window after "
        << MAX_ATTEMPTS << " attempts" << endl;
    return newPiece;
}

Piece PieceQueue::CreateRandomPiece() {
    int r = rand() % 7; // 0..6
    switch (r) {
    case 0: { LPiece p; return p; }
    case 1: { JPiece p; return p; }
    case 2: { IPiece p; return p; }
    case 3: { OPiece p; return p; }
    case 4: { SPiece p; return p; }
    case 5: { TPiece p; return p; }
    default: { ZPiece p; return p; }
    }
}

PieceQueue::PieceQueue(int cap) : capacity(cap) {
    srand(static_cast<unsigned int>(time(nullptr)));  // Fixed time conversion
    FillStartup();
}

PieceQueue::~PieceQueue() {
    q.clear();
}

bool PieceQueue::IsEmpty() const {
    return q.empty();
}

bool PieceQueue::IsFull() const {
    return (int)q.size() >= capacity;
}

void PieceQueue::FillStartup() {
    q.clear();

    cout << "=== QUEUE INITIALIZED WITH NO REPEATS IN 2 OR 3 PIECE WINDOW ===" << endl;

    // Fill the queue with pieces, ensuring no repeats in recent window
    while (!IsFull()) {
        if (q.empty()) {
            // First piece: always random
            q.push_back(CreateRandomPiece());
        }
        else {
            // For subsequent pieces, ensure no repeats in window
            q.push_back(CreateRandomPieceNoRepeat());
        }
    }

    DisplayNextThree();
}

Piece PieceQueue::Dequeue() {
    if (IsEmpty()) {
        // keep behavior consistent: return a random piece and refill
        Piece p = CreateRandomPiece();
        // Use no-repeat version when adding new piece
        q.push_back(CreateRandomPieceNoRepeat());
        return p;
    }

    Piece front = q.front();
    q.erase(q.begin());

    // Use no-repeat version when adding new piece
    q.push_back(CreateRandomPieceNoRepeat());

    // Display next three pieces after dequeue
    DisplayNextThree();

    return front;
}

Piece PieceQueue::Peek() const {
    if (IsEmpty()) return Piece();
    return q.front();
}

void PieceQueue::Clear() {
    q.clear();
}

// Helper function to check for repeats in a window
bool CheckWindowForRepeats(const vector<Piece>& pieces, int start, int windowSize) {
    if (start + windowSize > pieces.size()) return false;

    set<int> seen;
    for (int i = start; i < start + windowSize; i++) {
        if (seen.count(pieces[i].id) > 0) {
            return true; // Repeat found in this window
        }
        seen.insert(pieces[i].id);
    }
    return false;
}

// Display next three pieces in console
void PieceQueue::DisplayNextThree() const {
    cout << "=== NEXT 3 PIECES IN QUEUE ===" << endl;

    if (q.empty()) {
        cout << "Queue is empty!" << endl;
        return;
    }

    // Display up to 3 pieces or whatever is available
    int displayCount = min(3, (int)q.size());

    for (int i = 0; i < displayCount; i++) {
        cout << i + 1 << ". ";

        switch (q[i].id) {
        case 1: cout << "L-Piece"; break;
        case 2: cout << "J-Piece"; break;
        case 3: cout << "I-Piece"; break;
        case 4: cout << "O-Piece"; break;
        case 5: cout << "S-Piece"; break;
        case 6: cout << "T-Piece"; break;
        case 7: cout << "Z-Piece"; break;
        default: cout << "Unknown Piece"; break;
        }

        // Display piece position info
        cout << " (Position: " << q[i].rowOffset << "," << q[i].colOffset << ")";
        cout << endl;
    }

    if (q.size() > 3) {
        cout << "... and " << (q.size() - 3) << " more pieces" << endl;
    }

    // Check for violations in the entire queue using sliding windows
    bool hasWindow2Violation = false;
    bool hasWindow3Violation = false;

    // Check window size 2
    for (int i = 0; i <= (int)q.size() - 2; i++) {
        if (q[i].id == q[i + 1].id) {
            hasWindow2Violation = true;
            cout << "VIOLATION: Window size 2 - pieces at " << i << " and " << i + 1
                << " are both " << q[i].id << endl;
        }
    }

    // Check window size 3
    for (int i = 0; i <= (int)q.size() - 3; i++) {
        set<int> windowPieces;
        bool hasRepeat = false;

        for (int j = 0; j < 3; j++) {
            if (windowPieces.count(q[i + j].id) > 0) {
                hasRepeat = true;
                break;
            }
            windowPieces.insert(q[i + j].id);
        }

        if (hasRepeat) {
            hasWindow3Violation = true;
            cout << "VIOLATION: Window size 3 - pieces at " << i << "," << i + 1 << "," << i + 2
                << " have repeats" << endl;
        }
    }

    if (!hasWindow2Violation && !hasWindow3Violation) {
        cout << "Queue OK: No repeats in any 2 or 3 piece window" << endl;
    }
    else if (hasWindow2Violation && !hasWindow3Violation) {
        cout << "Queue has window-2 violations" << endl;
    }
    else if (!hasWindow2Violation && hasWindow3Violation) {
        cout << "Queue has window-3 violations" << endl;
    }

    cout << "===============================" << endl;
}

// Get the next three pieces for display
vector<Piece> PieceQueue::GetNextThree() const {
    vector<Piece> nextThree;

    if (q.empty()) {
        return nextThree;
    }

    int displayCount = min(3, (int)q.size());

    for (int i = 0; i < displayCount; i++) {
        nextThree.push_back(q[i]);
    }

    return nextThree;
}

void PieceQueue::ClearAndSetPieces(const vector<Piece>& newPieces) {
    q.clear();
    q = newPieces;
    // Ensure we don't exceed capacity
    if ((int)q.size() > capacity) {
        q.resize(capacity);
    }
    cout << "Queue updated - Size: " << q.size() << endl;

    // Check for violations after setting pieces
    bool hasViolation = false;

    // Check window size 2
    for (int i = 0; i < (int)q.size() - 1; i++) {
        if (q[i].id == q[i + 1].id) {
            hasViolation = true;
            cout << "VIOLATION: Window size 2 - pieces at " << i << " and " << i + 1
                << " are both " << q[i].id << endl;
        }
    }

    // Check window size 3
    for (int i = 0; i <= (int)q.size() - 3; i++) {
        set<int> windowPieces;
        bool windowHasRepeat = false;

        for (int j = 0; j < 3; j++) {
            if (windowPieces.count(q[i + j].id) > 0) {
                windowHasRepeat = true;
                break;
            }
            windowPieces.insert(q[i + j].id);
        }

        if (windowHasRepeat) {
            hasViolation = true;
            cout << "VIOLATION: Window size 3 - pieces at " << i << "," << i + 1 << "," << i + 2
                << " have repeats" << endl;
        }
    }
}

#include "Queue.h"
#include <vector>
using namespace std;

Queue::Queue(int cap) : front(nullptr), rear(nullptr), size(0), capacity(cap) {}

Queue::~Queue() {
    clear();
}

void Queue::enqueue(Piece piece) {
    if (isFull()) return;

    QueueNode* newNode = new QueueNode(piece);

    if (isEmpty()) {
        front = rear = newNode;
    }
    else {
        rear->next = newNode;
        rear = newNode;
    }
    size++;
}

Piece Queue::dequeue() {
    if (isEmpty()) return Piece();

    QueueNode* temp = front;
    Piece data = front->data;

    front = front->next;
    if (front == nullptr) {
        rear = nullptr;
    }

    delete temp;
    size--;
    return data;
}

Piece Queue::peek() const {
    if (isEmpty()) return Piece();
    return front->data;
}

bool Queue::isEmpty() const {
    return front == nullptr;
}

bool Queue::isFull() const {
    return size >= capacity;
}

int Queue::getSize() const {
    return size;
}

void Queue::clear() {
    while (!isEmpty()) {
        dequeue();
    }
}

vector<Piece> Queue::getAllPieces() const {
    vector<Piece> pieces;
    QueueNode* current = front;
    while (current != nullptr) {
        pieces.push_back(current->data);
        current = current->next;
    }
    return pieces;
}

vector<Piece> Queue::getNextThree() const {
    vector<Piece> nextThree;
    QueueNode* current = front;
    int count = 0;

    while (current != nullptr && count < 3) {
        nextThree.push_back(current->data);
        current = current->next;
        count++;
    }
    return nextThree;
}

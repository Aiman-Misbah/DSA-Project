#include "Queue.h"
#include <iostream>
#include <vector>
using namespace std;

Queue::Queue(int cap) : front(NULL), rear(NULL), size(0), capacity(cap) {}

Queue::~Queue() {
    clear();
}

void Queue::enqueue(Piece piece) {
    if (isFull()) return;   //agar full hogyi hai to add nhi krskte wrna krdo

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
    if (isEmpty()) return Piece();  //if empty returning default piece - means 

    QueueNode* temp = front;
    Piece data = front->data;

    front = front->next;
    if (front == NULL) {
        rear = NULL;
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
    return front == NULL;
}

bool Queue::isFull() const {
    return size >= capacity;
}

int Queue::getSize() const {
    return size;
}

void Queue::clear() {
    while (!isEmpty()) {    //emptying everything
        dequeue();
    }
}

//we are using vector instead of Queue itself kionke phir copy constructor bana pr rha tha then another one in pieceQueue and phir Game ki file mein masle aarhe the
vector<Piece> Queue::getAllPieces() const {     //for undo
    vector<Piece> pieces;   //copying the queue
    QueueNode* current = front;
    while (current != NULL) {
        pieces.push_back(current->data);
        current = current->next;
    }
    return pieces;
}

vector<Piece> Queue::getNextThree() const { //copying first three 
    vector<Piece> nextThree;
    QueueNode* current = front;
    int count = 0;

    while (current != NULL && count < 3) {
        nextThree.push_back(current->data);
        current = current->next;
        count++;
    }
    return nextThree;
}

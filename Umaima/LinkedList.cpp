#include "LinkedList.h"
#include <iostream>
using namespace std;

LinkedList::LinkedList() : head(NULL), tail(NULL), size(0) {}

LinkedList::~LinkedList() {
    Clear();
}

void LinkedList::AddPiece(Piece piece) {
    Node* newNode = new Node(piece);

    if (head == NULL) {
        head = newNode;
        tail = newNode;
    }
    else {
        tail->next = newNode;
        tail = newNode;
    }
    size++;
}

Piece LinkedList::GetPiece(int index) {
    if (index < 0 || index >= size) {
        // Return default piece or handle error
        return Piece();
    }

    Node* current = head;
    for (int i = 0; i < index; i++) {
        current = current->next;
    }
    return current->data;
}

void LinkedList::RemovePiece(int index) {
    if (index < 0 || index >= size) return;

    Node* toDelete = NULL;

    if (index == 0) {
        toDelete = head;
        head = head->next;
        if (head == NULL) tail = NULL;
    }
    else {
        Node* current = head;
        for (int i = 0; i < index - 1; i++) {
            current = current->next;
        }
        toDelete = current->next;
        current->next = toDelete->next;

        if (toDelete == tail) {
            tail = current;
        }
    }

    delete toDelete;
    size--;
}

int LinkedList::GetSize() {
    return size;
}

void LinkedList::Clear() {
    Node* current = head;
    while (current != NULL) {
        Node* next = current->next;
        delete current;
        current = next;
    }
    head = NULL;
    tail = NULL;
    size = 0;
}
#pragma once
#include "Piece.h"
#include <iostream>
using namespace std;

class Node {
public:
    Piece data;
    Node* next;

    Node(Piece piece) : data(piece), next(NULL) {}
};

class LinkedList {
private:
    Node* head;
    Node* tail;
    int size;

public:
    LinkedList();
    ~LinkedList();
    void AddPiece(Piece piece);
    Piece GetPiece(int index);
    void RemovePiece(int index);
    int GetSize();
    void Clear();
};

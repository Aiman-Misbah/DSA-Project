#pragma once
#include <raylib.h>
#include <vector>
#include <iostream>
#include "Colours.h"
using namespace std;

// Node for linked list row
struct RowNode {
    int rowData[10];  // Each row has 10 cells
    RowNode* next;

    RowNode() : next(NULL) {
        for (int i = 0; i < 10; i++) rowData[i] = 0;
    }
};

class Board {
private:
    RowNode* head;
    RowNode* tail;
    int rows;
    int cols;
    int cell; //size of a cell in pixels
    vector<Color> colours;

    // Linked list management methods
    void AddRow();
    void RemoveRow(int rowIndex);
    RowNode* GetRow(int index);
    bool isRowFull(RowNode* rowNode);
    void ClearRow(RowNode* rowNode);
    void MoveRowsDown(int startRow, int numRows);

public:
    Board();
    ~Board();
    void Initialize();
    void Print();
    void Draw();
    bool CollisionDetected(int r, int c);
    bool isCellEmpty(int r, int c);
    int ClearRows();  // This will use linked list operations
    void SetCell(int row, int col, int value);
    int GetCell(int row, int col);
};

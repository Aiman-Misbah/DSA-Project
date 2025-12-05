#pragma once
#include <raylib.h>
#include <vector>
#include <iostream>
#include "Colours.h"
using namespace std;

// Node for linked list row
struct RowNode {
    int rowData[15];  // Each row has 10 cells
    RowNode* next;

    RowNode() : next(NULL) {
        for (int i = 0; i < 15; i++) rowData[i] = 0;
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
    RowNode* GetRow(int index);
    bool isRowFull(RowNode* rowNode);
    void ClearRow(RowNode* rowNode);

public:
    Board();
    ~Board();
    void Initialize();
    void Draw();
    bool CollisionDetected(int r, int c);
    bool isCellEmpty(int r, int c);
    int ClearRows();  // This will use linked list operations
    void SetCell(int row, int col, int value);

	vector<vector<int>> GetBoardState();
	void SetBoardState(const vector<vector<int>>& state);
};
#pragma once
#include <raylib.h>
#include <vector>
#include <iostream>
#include "Colours.h"
using namespace std;

// Each node represents a row
struct RowNode {
    int rowData[15];  // Each row has 15 cells/cols
    RowNode* next;

    RowNode() : next(NULL) { //initializing every cell as 0
        for (int i = 0; i < 15; i++) rowData[i] = 0;
    }
};

class Board {
private:
    RowNode* head;      //first row in the linked list
    RowNode* tail;      //last row
    int rows;           //total number of rows (20)
    int cols;           //total number of cols (15)
    int cell;           //size of a cell in pixels
    vector<Color> colours;

    // Linked list wale methods
    void AddRow();
    RowNode* GetRow(int index);
    bool isRowFull(RowNode* rowNode);
    void ClearRow(RowNode* rowNode);

public:
    Board();
    ~Board();
    void Initialize();  //resetting the board for game restart
    void Draw();
    bool CollisionDetected(int r, int c);   //if that cell is occupied or out of bounds
    bool isCellEmpty(int r, int c);
    int ClearRows();    //for completed rows and moving above rows down
    void SetCell(int row, int col, int value);  //when piece is locked

	vector<vector<int>> GetBoardState();  //saving for undo
	void SetBoardState(const vector<vector<int>> state); //restoring after undo
};

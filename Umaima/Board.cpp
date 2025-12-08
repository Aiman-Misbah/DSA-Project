#include "Board.h"
#include <iostream>
#include <vector>
using namespace std;

Board::Board() : head(NULL), tail(NULL), rows(20), cols(15), cell(35) {
    colours = GetCellColours();
    Initialize();
}

Board::~Board() {
    RowNode* current = head;
    while (current != NULL) {
        RowNode* next = current->next;
        delete current;
        current = next;
    }
}

void Board::Initialize() {
    //same as destructor - clearing everything and starting anew
    RowNode* current = head;
    while (current != NULL) {
        RowNode* next = current->next;
        delete current;
        current = next;
    }
    head = tail = NULL;

    for (int i = 0; i < rows; i++) {
        AddRow();
    }
}

void Board::AddRow() {
    RowNode* newNode = new RowNode();
    if (head == NULL) {
        head = tail = newNode;
    }
    else {
        tail->next = newNode;
        tail = newNode;
    }
}

RowNode* Board::GetRow(int index) {
    //getting the row at that index (if it is not NULL)
    RowNode* current = head;
    for (int i = 0; i < index && current != NULL; i++) {
        current = current->next;
    }
    return current;
}



void Board::Draw() {    //drawing the entire board
    RowNode* current = head;
    int rowIndex = 0;   //starting from the first row
    while (current != NULL && rowIndex < rows) {
        for (int j = 0; j < cols; j++) {    //each col in each row
            int cellVal = current->rowData[j];
            //290 and 50 are pixels of x and y respectively
            DrawRectangle(j * cell + 290, rowIndex * cell + 50, cell - 1, cell - 1, colours[cellVal]);
        }
        current = current->next;
        rowIndex++;
    }
}

bool Board::CollisionDetected(int r, int c) {
    //if it is out of boundaries
    if (r < 0 || c < 0 || c >= cols) {
        return true;
    }
    if (r >= rows) {
        return true;
    }
    //if it is occupied collision detected and vice versa
    return !isCellEmpty(r, c);
}

bool Board::isCellEmpty(int r, int c) {
    //checking if it is occupied
    RowNode* rowNode = GetRow(r); //getting that row and checking the col in that row
    if (rowNode == NULL) return true;
    return rowNode->rowData[c] == 0;
}

void Board::SetCell(int row, int col, int value) {
    //setting that cell to a value according to the id of the piece
    RowNode* rowNode = GetRow(row);
    if (rowNode != NULL) {
        rowNode->rowData[col] = value;
    }
}



bool Board::isRowFull(RowNode* rowNode) {
    if (rowNode == NULL) return false;

    for (int c = 0; c < cols; c++) {
        if (rowNode->rowData[c] == 0) { //if even one col is empty it is not full
            return false;
        }
    }
    return true;
}

void Board::ClearRow(RowNode* rowNode) {
    if (rowNode == NULL) return;
    //restting that row with 0 in every col
    for (int c = 0; c < cols; c++) {
        rowNode->rowData[c] = 0;
    }
}


int Board::ClearRows() {    //for completed rows
    
    int completed = 0;  //how many rows are completed

    //identify full rows
    vector<bool> keepRow(rows, true);   //tracking which rows to keep (true) and which to remove (false) - creates a vector of 20 bool all initialized with true rn

    for (int r = 0; r < rows; r++) {
        RowNode* currentRow = GetRow(r);
        if (currentRow != NULL && isRowFull(currentRow)) {
            keepRow[r] = false; //if it is full we are getting rid of it and incremented the counter
            completed++;
        }
    }

    if (completed == 0) return 0;   //if none are completed to koi faida nhi

    vector<vector<int>> nonFullRows; //copying the rows that are to be kept - 2D because of the rows x cols

    // Collect all non-full rows in order
    for (int r = 0; r < rows; r++) {
        if (keepRow[r]) {   //if it is to be kept (not full) then copy it into nonFullRows
            RowNode* currentRow = GetRow(r);
            vector<int> rowData(cols);  //us row k saare cols copy krke store krein then store the row itself
            for (int c = 0; c < cols; c++) {
                rowData[c] = currentRow->rowData[c];
            }
            nonFullRows.push_back(rowData);
        }
    }

    // Clear the entire board first
    for (int r = 0; r < rows; r++) {
        RowNode* currentRow = GetRow(r);
        ClearRow(currentRow);
    }

    // Fill from bottom with non-full rows
    int writeRow = rows - 1;    //starting from last row of the board
    for (int i = nonFullRows.size() - 1; i >= 0; i--) { //starting from the last row to be kept
        RowNode* currentRow = GetRow(writeRow); //kis row mein likhna hai
        for (int c = 0; c < cols; c++) {    //copying the whole row into it
            currentRow->rowData[c] = nonFullRows[i][c];
        }
        writeRow--; //move up
    }

    // Top rows remain empty (already cleared above)
    return completed;   //kitni rows clear hui hain
}



vector<vector<int>> Board::GetBoardState() {    //saves the current board state for undo
    vector<vector<int>> state;  //2D for rows x cols
    RowNode* current = head;
    int rowIndex = 0;
    while (current != NULL && rowIndex < rows) {
        vector<int> row;
        for (int c = 0; c < cols; c++) {
            row.push_back(current->rowData[c]); //storing cols in a row
        }
        state.push_back(row);   //storing row itself
        current = current->next;
        rowIndex++;
    }
    return state;
}

void Board::SetBoardState(const vector<vector<int>> state) {   //restoring after undo - getting the state as well jismein waapis jaana hai

    // Clear the board first
    Initialize();

    // Restore the state
    for (int r = 0; r < state.size() && r < rows; r++) {
        for (int c = 0; c < state[r].size() && c < cols; c++) {
            SetCell(r, c, state[r][c]);
        }
    }
}

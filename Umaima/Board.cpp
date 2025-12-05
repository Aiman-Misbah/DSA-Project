#include "Board.h"
#include <iostream>
#include <vector>
using namespace std;

Board::Board() : head(NULL), tail(NULL), rows(20), cols(15), cell(35) {
    colours = GetCellColours();
    Initialize();
}

Board::~Board() {
    // Clean up linked list
    RowNode* current = head;
    while (current != NULL) {
        RowNode* next = current->next;
        delete current;
        current = next;
    }
}

void Board::Initialize() {
    // Clear existing linked list
    RowNode* current = head;
    while (current != NULL) {
        RowNode* next = current->next;
        delete current;
        current = next;
    }
    head = tail = NULL;

    // Create new linked list with empty rows
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
    RowNode* current = head;
    for (int i = 0; i < index && current != NULL; i++) {
        current = current->next;
    }
    return current;
}



void Board::Draw() {
    RowNode* current = head;
    int rowIndex = 0;
    while (current != NULL && rowIndex < rows) {
        for (int j = 0; j < cols; j++) {
            int cellVal = current->rowData[j];
            DrawRectangle(j * cell + 290, rowIndex * cell + 50, cell - 1, cell - 1, colours[cellVal]);
        }
        current = current->next;
        rowIndex++;
    }
}

bool Board::CollisionDetected(int r, int c) {
    if (r < 0 || c < 0 || c >= cols) {
        return true;
    }
    if (r >= rows) {
        return true;
    }
    return !isCellEmpty(r, c);
}

bool Board::isCellEmpty(int r, int c) {
    RowNode* rowNode = GetRow(r);
    if (rowNode == NULL) return true;
    return rowNode->rowData[c] == 0;
}

void Board::SetCell(int row, int col, int value) {
    RowNode* rowNode = GetRow(row);
    if (rowNode != NULL) {
        rowNode->rowData[col] = value;
    }
}



bool Board::isRowFull(RowNode* rowNode) {
    if (rowNode == NULL) return false;

    for (int c = 0; c < cols; c++) {
        if (rowNode->rowData[c] == 0) {
            return false;
        }
    }
    return true;
}

void Board::ClearRow(RowNode* rowNode) {
    if (rowNode == NULL) return;

    for (int c = 0; c < cols; c++) {
        rowNode->rowData[c] = 0;
    }
}


int Board::ClearRows() {
    int completed = 0;

    // First pass: identify full rows
    vector<bool> keepRow(rows, true);
    for (int r = 0; r < rows; r++) {
        RowNode* currentRow = GetRow(r);
        if (currentRow != NULL && isRowFull(currentRow)) {
            keepRow[r] = false;
            completed++;
        }
    }

    if (completed == 0) return 0;

    // Create temporary storage for NON-FULL rows only
    vector<vector<int>> nonFullRows;

    // Collect all non-full rows in order
    for (int r = 0; r < rows; r++) {
        if (keepRow[r]) {
            RowNode* currentRow = GetRow(r);
            vector<int> rowData(cols);
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
    int writeRow = rows - 1;
    for (int i = nonFullRows.size() - 1; i >= 0; i--) {
        RowNode* currentRow = GetRow(writeRow);
        for (int c = 0; c < cols; c++) {
            currentRow->rowData[c] = nonFullRows[i][c];
        }
        writeRow--;
    }

    // Top rows remain empty (already cleared above)
    return completed;
}



vector<vector<int>> Board::GetBoardState() {
    vector<vector<int>> state;
    RowNode* current = head;
    int rowIndex = 0;
    while (current != NULL && rowIndex < rows) {
        vector<int> row;
        for (int c = 0; c < cols; c++) {
            row.push_back(current->rowData[c]);
        }
        state.push_back(row);
        current = current->next;
        rowIndex++;
    }
    return state;
}

void Board::SetBoardState(const vector<vector<int>>& state) {
    // Clear the board first
    Initialize();

    // Restore the state
    for (int r = 0; r < state.size() && r < rows; r++) {
        for (int c = 0; c < state[r].size() && c < cols; c++) {
            SetCell(r, c, state[r][c]);
        }
    }
}
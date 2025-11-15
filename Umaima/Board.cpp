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

void Board::RemoveRow(int rowIndex) {
    if (rowIndex == 0) {
        // Remove head
        RowNode* temp = head;
        head = head->next;
        delete temp;
        if (head == NULL) tail = NULL;
    }
    else {
        // Find previous node
        RowNode* prev = GetRow(rowIndex - 1);
        if (prev != NULL && prev->next != NULL) {
            RowNode* toDelete = prev->next;
            prev->next = toDelete->next;
            if (toDelete == tail) tail = prev;
            delete toDelete;
        }
    }
}

void Board::Draw() {
    RowNode* current = head;
    int rowIndex = 0;
    while (current != NULL && rowIndex < rows) {
        for (int j = 0; j < cols; j++) {
            int cellVal = current->rowData[j];
            DrawRectangle(j * cell + 11, rowIndex * cell + 11, cell - 1, cell - 1, colours[cellVal]);
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

int Board::GetCell(int row, int col) {
    RowNode* rowNode = GetRow(row);
    if (rowNode == NULL) return -1;
    return rowNode->rowData[col];
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

void Board::MoveRowsDown(int startRow, int numRows) {
    // Move data from upper rows to lower rows
    for (int r = startRow; r > 0; r--) {
        RowNode* currentRow = GetRow(r);
        RowNode* aboveRow = GetRow(r - 1);

        if (currentRow != NULL && aboveRow != NULL) {
            for (int c = 0; c < cols; c++) {
                currentRow->rowData[c] = aboveRow->rowData[c];
            }
        }
    }

    // Clear the top row
    RowNode* topRow = GetRow(0);
    if (topRow != NULL) {
        ClearRow(topRow);
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
void Board::Print() {
    RowNode* current = head;
    int rowIndex = 0;
    while (current != NULL) {
        cout << "Row " << rowIndex << ": ";
        for (int j = 0; j < cols; j++) {
            cout << current->rowData[j] << " ";
        }
        cout << endl;
        current = current->next;
        rowIndex++;
    }
}

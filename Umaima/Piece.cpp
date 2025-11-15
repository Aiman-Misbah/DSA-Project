#include <iostream>
#include "Piece.h"
#include "Colours.cpp"
#include "Board.h"
using namespace std;

Piece::Piece() {
	size = 30;
	state = 0;
	colours = GetCellColours();
	id = 0;
	rowOffset = 0;
	colOffset = 0;

}

void Piece::Draw(int x, int y) {
	vector<Position> tiles = GetCellPositions();
	for (Position item : tiles) {
		DrawRectangle(item.COL * size+y, item.ROW * size+x, size - 1, size - 1, colours[id]);
	}
}

void Piece::Move(int r, int c) {
	rowOffset += r;
	colOffset += c;
}

vector<Position> Piece::GetCellPositions() {
	vector<Position> tiles = cells[state];
	vector<Position> newTiles;
	for (Position item : tiles) {
		Position pos = Position(item.ROW + rowOffset, item.COL + colOffset);
		newTiles.push_back(pos);
	}
	return newTiles;
}

void Piece::Rotate() {
	state++;
	if (state == 4) {
		state = 0;
	}
}

void Piece::UndoRotation() {
	state--;
	if (state == -1) {
		state = 3;
	}
}

bool Piece::HasCollision(Board& board)  {
    vector<Position> tiles = GetCellPositions();
    for (Position item : tiles) {
        if (board.CollisionDetected(item.ROW, item.COL)) {
            return true;
        }
    }
    return false;
}

bool Piece::TryRotationWithOffset(Board& board, int rowOffset, int colOffset, int attempts) {
    //  BASE CASE: No more attempts
    if (attempts <= 0) {
        return false;
    }

    // Store original position for backtracking
    int originalRow = rowOffset;
    int originalCol = colOffset;

    // Try moving to the offset position
    Move(rowOffset, colOffset);

    // Check if this position is valid
    if (!HasCollision(board)) {
        return true; //  Success!
    }

    //  BACKTRACK: Position not valid, undo movement
    Move(-rowOffset, -colOffset);

    //  RECURSION: Try with smaller offset
    return TryRotationWithOffset(board, rowOffset / 2, colOffset / 2, attempts - 1);
}

//  NEW: Main recursive rotation with wall kicks
bool Piece::RotateWithWallKicks(Board& board) {
    // Store original state for backtracking
    int originalState = state;
    int originalRow = rowOffset;
    int originalCol = colOffset;

    // Try the rotation first
    Rotate();

    // If rotation is valid at current position, success!
    if (!HasCollision(board)) {
        return true;
    }

    // Define wall kick patterns to try (row, col offsets)
    vector<Position> wallKicks = {
        Position(0, -1),  // Left
        Position(0, 1),   // Right
        Position(-1, 0),  // Up
        Position(1, 0),   // Down
        Position(-1, -1), // Up-Left
        Position(-1, 1),  // Up-Right
        Position(1, -1),  // Down-Left
        Position(1, 1)    // Down-Right
    };

    // Try each wall kick position recursively
    for (Position& kick : wallKicks) {
        if (TryRotationWithOffset(board, kick.ROW, kick.COL, 3)) {
            return true; //  Found valid position!
        }
    }

    //  BACKTRACK: All attempts failed, undo the rotation completely
    state = originalState;
    rowOffset = originalRow;
    colOffset = originalCol;

    return false; //  No valid position found
}
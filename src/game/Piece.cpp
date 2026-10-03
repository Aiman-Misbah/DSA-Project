#include <iostream>
#include "game/Piece.h"
#include "ui/Colours.h"
#include "game/Board.h"
using namespace std;

Piece::Piece() {
    size = 35;          //in pixels
    state = 0;
    colours = GetCellColours();     //returns 8 colours - 7 for pieces and 1 for background
    id = 0;     //default cell is empty/DarkGray
    rowOffset = 0;      //just default values
    colOffset = 0;

}

void Piece::Draw(int x, int y) { 
    vector<Position> tiles = GetCellPositions(); //actual positions
    for (Position item : tiles) {
        // Check if this tile is at or below row 0 (top of visible board)
        if (item.ROW >= 0) {
            DrawRectangle(item.COL * size + y, item.ROW * size + x,
                size - 1, size - 1, colours[id]);
            // -1 for the border - thora sa gap
        }
    }
}

void Piece::Move(int r, int c) {    //updating the offsets accordingly
    rowOffset += r;
    colOffset += c;
}

vector<Position> Piece::GetCellPositions() {    //calculates actual positions of the blocks of the piece according to the board
    vector<Position> tiles = cells[state];  //that rotation k saare cells k positions 
    vector<Position> newTiles;
    for (Position item : tiles) {
        Position pos = Position(item.ROW + rowOffset, item.COL + colOffset);
        newTiles.push_back(pos);
    }
    return newTiles;    //storing the actual positions of each block and returning them
}

void Piece::Rotate() {  //for rotating the block the state is incremented and if state has exceeded 3 (0-3 states) then looping it back to 0 and so on
    state++;
    if (state == 4) {
        state = 0;
    }
}


bool Piece::HasCollision(Board& board) {
    //checking if it is outside the board boundaries or it is overlapping any of the filled cells
    vector<Position> tiles = GetCellPositions();
    for (Position item : tiles) {
        if (board.CollisionDetected(item.ROW, item.COL)) {  //aik ki bhi collision aayi so it's true wrna false
            return true;
        }
    }
    return false;
}


bool Piece::TryRotationWithOffset(Board& board, int rowOffset, int colOffset, int attempts) {

    //if no more attempts, stop trying
    if (attempts <= 0) {
        return false;
    }

    //move the piece and if there is no collision it is a success wrna undo that movement
    Move(rowOffset, colOffset);

    if (!HasCollision(board)) {
        return true; 
    }
    Move(-rowOffset, -colOffset);

    //recursive call with smaller offset to try and move it
    return TryRotationWithOffset(board, rowOffset / 2, colOffset / 2, attempts - 1);
}

bool Piece::RotateWithWallKicks(Board& board) { //if collision occurs tries with 8 offset positions recursively
    //saving for backtracking
    int originalState = state;
    int originalRow = rowOffset;
    int originalCol = colOffset;

    //updating the state for simple rotation first
    Rotate();

    //if no collision using simple rotation to phir theek hai
    if (!HasCollision(board)) {
        return true;
    }

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

    for (Position& kick : wallKicks) {
        if (TryRotationWithOffset(board, kick.ROW, kick.COL, 3)) {
            return true; 
        }
    }

    //if each has failed restore the original things
    state = originalState;
    rowOffset = originalRow;
    colOffset = originalCol;

    return false;
}

Piece Piece::GetGhostPiece(Board& board) {  //calculating the position of the ghost at the bottom
    Piece ghost = *this; //creating a copy of the piece and moving it down

    vector<Position> tiles = ghost.GetCellPositions();
    bool isAboveBoard = true;   //if the piece is above board don't show it
    for (Position item : tiles) {
        if (item.ROW >= 0) {
            isAboveBoard = false;
            break;
        }
    }
    
    //when the piece is not visible no calculation is done - wohi piece return kardo as the original piece itself
    if (isAboveBoard) {
        return ghost;
    }

    //jab tak there is no collsion move it down
    while (!ghost.HasCollision(board)) {
        ghost.Move(1, 0);
    }
    //agar collision then move up once and return that piece
    ghost.Move(-1, 0);

    return ghost;
}

void Piece::DrawGhost(int x, int y) {
    vector<Position> tiles = GetCellPositions();
    Color ghostColor = colours[id]; //same colour as the piece and make it a bit transparent (r,g,a)
    ghostColor.a = 50;  // 50/255

    for (Position item : tiles) {
        //same as the Draw method
        if (item.ROW >= 0) {
            DrawRectangle(item.COL * size + y, item.ROW * size + x,
                size - 1, size - 1, ghostColor);

            DrawRectangleLines(item.COL * size + y, item.ROW * size + x,
                size - 1, size - 1, Fade(WHITE, 0.3f));
            //to draw borders for the ghost to add definition - normal pieces k borders are dark so to differentiate the ghost piece have a bit of white borders
        }
    }
}

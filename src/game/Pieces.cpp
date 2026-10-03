#include "game/Pieces.h"

LPiece::LPiece() {
    id = 1;
    cells[0] = { Position(0,2), Position(1,0), Position(1,1), Position(1,2) };
    cells[1] = { Position(0,1), Position(1,1), Position(2,1), Position(2,2) };
    cells[2] = { Position(1,0), Position(1,1), Position(1,2), Position(2,0) };
    cells[3] = { Position(0,0), Position(0,1), Position(1,1), Position(2,1) };
}

JPiece::JPiece() {
    id = 2;
    cells[0] = { Position(0,0), Position(1,0), Position(1,1), Position(1,2) };
    cells[1] = { Position(0,1), Position(0,2), Position(1,1), Position(2,1) };
    cells[2] = { Position(1,0), Position(1,1), Position(1,2), Position(2,2) };
    cells[3] = { Position(0,1), Position(1,1), Position(2,0), Position(2,1) };
}

IPiece::IPiece() {
    id = 3;
    cells[0] = { Position(1,0), Position(1,1), Position(1,2), Position(1,3) };
    cells[1] = { Position(0,2), Position(1,2), Position(2,2), Position(3,2) };
    cells[2] = { Position(2,0), Position(2,1), Position(2,2), Position(2,3) };
    cells[3] = { Position(0,1), Position(1,1), Position(2,1), Position(3,1) };
}

OPiece::OPiece() {
    id = 4;
    cells[0] = { Position(0,0), Position(0,1), Position(1,0), Position(1,1) };
    cells[1] = { Position(0,0), Position(0,1), Position(1,0), Position(1,1) };
    cells[2] = { Position(0,0), Position(0,1), Position(1,0), Position(1,1) };
    cells[3] = { Position(0,0), Position(0,1), Position(1,0), Position(1,1) };
}

SPiece::SPiece() {
    id = 5;
    cells[0] = { Position(0,1), Position(0,2), Position(1,0), Position(1,1) };
    cells[1] = { Position(0,1), Position(1,1), Position(1,2), Position(2,2) };
    cells[2] = { Position(1,1), Position(1,2), Position(2,0), Position(2,1) };
    cells[3] = { Position(0,0), Position(1,0), Position(1,1), Position(2,1) };
}

TPiece::TPiece() {
    id = 6;
    cells[0] = { Position(0,1), Position(1,0), Position(1,1), Position(1,2) };
    cells[1] = { Position(0,1), Position(1,1), Position(1,2), Position(2,1) };
    cells[2] = { Position(1,0), Position(1,1), Position(1,2), Position(2,1) };
    cells[3] = { Position(0,1), Position(1,0), Position(1,1), Position(2,1) };
}

ZPiece::ZPiece() {
    id = 7;
    cells[0] = { Position(0,0), Position(0,1), Position(1,1), Position(1,2) };
    cells[1] = { Position(0,2), Position(1,1), Position(1,2), Position(2,1) };
    cells[2] = { Position(1,0), Position(1,1), Position(2,1), Position(2,2) };
    cells[3] = { Position(0,1), Position(1,0), Position(1,1), Position(2,0) };
}

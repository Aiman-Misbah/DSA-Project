#include <iostream>
#include "Position.h"
#include "Piece.h"
using namespace std;

class LPiece :public Piece {
public:
	LPiece() {	//har piece ki apni id hai usi id se colour bhi pata chal rha hai
		id = 1;

		//for each rotation state the relative positions according to the box have been hardocoded
		cells[0] = { Position(0,2), Position(1,0), Position(1,1), Position(1,2) };
		cells[1] = { Position(0,1), Position(1,1), Position(2,1), Position(2,2) };
		cells[2] = { Position(1,0), Position(1,1),Position(1,2),Position(2,0) };
		cells[3] = { Position(0,0),Position(0,1),Position(1,1), Position(2,1) };
	}
};


class JPiece :public Piece {
public:
	JPiece() {
		id = 2;
		cells[0] = { Position(0,0), Position(1,0), Position(1,1), Position(1,2) };
		cells[1] = { Position(0,1), Position(0,2), Position(1,1), Position(2,1) };
		cells[2] = { Position(1,0), Position(1,1), Position(1,2), Position(2,2) };
		cells[3] = { Position(0,1), Position(1,1), Position(2,0), Position(2,1) };
	}
};

class IPiece :public Piece {
public:
	IPiece() {
		id = 3;
		cells[0] = { Position(1,0), Position(1,1), Position(1,2), Position(1,3) };
		cells[1] = { Position(0,2), Position(1,2), Position(2,2), Position(3,2) };
		cells[2] = { Position(2,0), Position(2,1), Position(2,2), Position(2,3) };
		cells[3] = { Position(0,1), Position(1,1), Position(2,1), Position(3,1) };
	}
};

class OPiece :public Piece {
public:
	OPiece() {
		id = 4;
		cells[0] = { Position(0,0), Position(0,1), Position(1,0), Position(1,1) };
		cells[1] = { Position(0,0), Position(0,1), Position(1,0), Position(1,1) };
		cells[2] = { Position(0,0), Position(0,1), Position(1,0), Position(1,1) };
		cells[3] = { Position(0,0), Position(0,1), Position(1,0), Position(1,1) };
	}
};

class SPiece :public Piece {
public:
	SPiece() {
		id = 5;
		cells[0] = { Position(0,1), Position(0,2), Position(1,0), Position(1,1) };
		cells[1] = { Position(0,1), Position(1,1), Position(1,2), Position(2,2) };
		cells[2] = { Position(1,1), Position(1,2), Position(2,0), Position(2,1) };
		cells[3] = { Position(0,0), Position(1,0), Position(1,1), Position(2,1) };
	}
};

class TPiece :public Piece {
public:
	TPiece() {
		id = 6;
		cells[0] = { Position(0,1), Position(1,0), Position(1,1), Position(1,2) };
		cells[1] = { Position(0,1), Position(1,1), Position(1,2), Position(2,1) };
		cells[2] = { Position(1,0), Position(1,1), Position(1,2), Position(2,1) };

		cells[3] = { Position(0,1), Position(1,0), Position(1,1), Position(2,1) };
	}
};

class ZPiece :public Piece {
public:
	ZPiece() {
		id = 7;
		cells[0] = { Position(0,0), Position(0,1), Position(1,1), Position(1,2) };
		cells[1] = { Position(0,2), Position(1,1), Position(1,2), Position(2,1) };
		cells[2] = { Position(1,0), Position(1,1), Position(2,1), Position(2,2) };
		cells[3] = { Position(0,1), Position(1,0), Position(1,1), Position(2,0) };
	}
};

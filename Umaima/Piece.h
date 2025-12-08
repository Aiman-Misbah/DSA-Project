#pragma once
#include <iostream>
#include <vector>
#include <map>
#include "Position.h"
#include "Colours.h"
#include "Board.h"
using namespace std;

//parent class for the pieces

class Piece {
private:
	int size;		//size of each cell
	int state;		//current rotation state (total 4 hain - 0 is by default)

public:
	Piece();
	int id;			//each has its own id
	map<int, vector<Position>> cells;	//map is like dictionary in Python - right now it is taking the rotation state and the coordinates of the cells of that rotation state
	vector<Color> colours;
	void Draw(int x, int y);
	int rowOffset;		//from the original position - for movement on the board accordingly
	int colOffset;
	void Move(int r, int c);
	vector<Position> GetCellPositions();	//get actual position from the offsets/relative positions on the board
	void Rotate();

	bool RotateWithWallKicks(Board& board);   //to try if rotation is possible - side mein wall agar aajaye to
	bool TryRotationWithOffset(Board& board, int rowOffset, int colOffset, int attempts);  //if blocked tries smaller offsets for rotation
	bool HasCollision(Board& board);
	void DrawGhost(int x, int y);
	Piece GetGhostPiece(Board& board);

};

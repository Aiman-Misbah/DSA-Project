#pragma once
#include <iostream>
#include <vector>
#include <map>
#include "Position.h"
#include "Colours.h"
#include "Board.h"
using namespace std;


class Piece {
private:
	int size;
	int state;

public:
	Piece();
	int id;
	map<int, vector<Position>> cells;
	vector<Color> colours;
	void Draw(int x, int y);
	int rowOffset;
	int colOffset;
	void Move(int r, int c);
	vector<Position> GetCellPositions();
	void Rotate();
	void UndoRotation();

	//  NEW: Recursive rotation with backtracking
	bool RotateWithWallKicks( Board& board);
	bool TryRotationWithOffset(Board& board, int rowOffset, int colOffset, int attempts);
	bool HasCollision(Board& board);

};
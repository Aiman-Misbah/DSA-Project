#pragma once
#include <iostream>
using namespace std;

//for the position of each cell/block

class Position {
public:
	Position(int row=0, int col=0);
	int ROW;
	int COL;
};

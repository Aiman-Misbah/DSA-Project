#pragma once
#include <iostream>
#include <raylib.h>
#include <vector>
using namespace std;

//extern keyword isliye bcz then we can use this constant easily in other files as well
const Color DarkGray = { 26, 31, 40, 255 };
const Color Green = { 47, 230,23,255 };
const Color Red = { 232, 18, 18, 255 };
const Color Orange = { 226, 116, 17, 255 };
const Color Yellow = { 237, 234, 4, 255 };
const Color Purple = { 166, 0, 247, 255 };
const Color Cyan = { 21, 204, 209, 255 };
const Color Blue = { 13, 64, 216, 255 };
const Color LightBlue = { 59, 85, 162, 255 };
const Color DarkBlue = { 44, 44, 127, 255 };

// UI Colors - ADD THESE TWO LINES
const Color PanelBlue = { 59, 85, 162, 255 };
const Color DarkOverlay = { 0, 0, 0, 100 };

vector<Color> GetCellColours() {
	return vector <Color>{ DarkGray, Green, Red, Orange, Yellow, Purple, Cyan, Blue };
}

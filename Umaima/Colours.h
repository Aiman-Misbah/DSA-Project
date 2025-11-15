#pragma once
#include <raylib.h>
#include <iostream>
#include <vector>
using namespace std;

// declare the colors (not define)
extern const Color DarkGray;
extern const Color Green;
extern const Color Red;
extern const Color Orange;
extern const Color Yellow;
extern const Color Purple;
extern const Color Cyan;
extern const Color Blue;
extern const Color LightBlue;
extern const Color DarkBlue;

vector<Color> GetCellColours();

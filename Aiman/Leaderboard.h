#pragma once
#include "ScoreAVL.h"
#include <iostream>
#include <vector>
using namespace std;

class Leaderboard {
private:
    ScoreAVL scoresTree;

public:

    void AddScore(int score);
    vector<int> GetTopScores(int count);
    int GetHighestScore();
    void Reset();
};

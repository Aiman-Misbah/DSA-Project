#pragma once
#include "../data_structures/ScoreAVL.h"
#include <iostream>
#include <vector>
using namespace std;

class Leaderboard {
private:
    ScoreAVL scoresTree;    //leaderboard is basically the AVL tree

public:

    void AddScore(int score);
    vector<int> GetTopScores(int count);
    int GetHighestScore();
    void Reset();   //clearing the leaderboard
};

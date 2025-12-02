// Leaderboard.h
#pragma once
#include "ScoreAVL.h"
#include <vector>
#include <string>
#include <fstream>
using namespace std;

class Leaderboard {
private:
    ScoreAVL scoresTree;
    string filename = "leaderboard.txt";

    void SaveScoresToFile();
    void LoadScoresFromFile();

public:
    Leaderboard();
    ~Leaderboard();

    void AddScore(int score);
    vector<int> GetTopScores(int count);
    int GetHighestScore();
    void Reset();
    void Display();  // For debugging
};

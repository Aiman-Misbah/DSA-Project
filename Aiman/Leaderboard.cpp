#include "Leaderboard.h"
#include <iostream>
#include <algorithm>
using namespace std;


void Leaderboard::AddScore(int score) {
    scoresTree.Insert(score);
}

vector<int> Leaderboard::GetTopScores(int count) {
    return scoresTree.GetTopScores(count);
}

int Leaderboard::GetHighestScore() {
    return scoresTree.GetHighestScore();
}

void Leaderboard::Reset() {
    scoresTree.Clear();
}

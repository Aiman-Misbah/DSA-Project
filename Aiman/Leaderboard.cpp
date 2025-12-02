// Leaderboard.cpp
#include "Leaderboard.h"
#include <iostream>
#include <algorithm>
#include <fstream>
using namespace std;

Leaderboard::Leaderboard() {
    LoadScoresFromFile();
}

Leaderboard::~Leaderboard() {
    SaveScoresToFile();
}

void Leaderboard::AddScore(int score) {
    scoresTree.Insert(score);
    SaveScoresToFile();
}

vector<int> Leaderboard::GetTopScores(int count) {
    return scoresTree.GetTopScores(count);
}

int Leaderboard::GetHighestScore() {
    return scoresTree.GetHighestScore();
}

void Leaderboard::LoadScoresFromFile() {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "No existing leaderboard file. Starting fresh." << endl;
        return;
    }

    int score;
    while (file >> score) {
        scoresTree.Insert(score);
    }
    file.close();

    cout << "Loaded " << scoresTree.GetTopScores(100).size() << " scores from leaderboard." << endl;
}

void Leaderboard::SaveScoresToFile() {
    ofstream file(filename);
    if (!file.is_open()) {
        cout << "Error: Could not save leaderboard!" << endl;
        return;
    }

    // Get all scores and save them
    vector<int> allScores = scoresTree.GetTopScores(100); // Get top 100 or all
    for (int score : allScores) {
        file << score << endl;
    }

    file.close();
}

void Leaderboard::Reset() {
    scoresTree.Clear();
    remove(filename.c_str());
    cout << "Leaderboard reset!" << endl;
}

void Leaderboard::Display() {
    vector<int> topScores = GetTopScores(10);
    cout << "=== LEADERBOARD ===" << endl;
    for (int i = 0; i < topScores.size(); i++) {
        cout << i + 1 << ". " << topScores[i] << endl;
    }
    if (topScores.empty()) {
        cout << "No scores yet!" << endl;
    }
    cout << "===================" << endl;
}

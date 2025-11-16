#pragma once
#include <iostream>
using namespace std;

struct AVLScoreNode {
    int score;
    AVLScoreNode* left;
    AVLScoreNode* right;
    int height;

    AVLScoreNode(int val) : score(val), left(NULL), right(NULL), height(1) {}
};

class ScoreAVL {
public:
    AVLScoreNode* root;

    // Helper functions
    int Height(AVLScoreNode* node);
    int GetBalance(AVLScoreNode* node);
    AVLScoreNode* RightRotate(AVLScoreNode* y);
    AVLScoreNode* LeftRotate(AVLScoreNode* x);
    AVLScoreNode* InsertNode(AVLScoreNode* node, int val);
    void InOrderPrint(AVLScoreNode* node);
    void ClearNodes(AVLScoreNode* node);

public:
    ScoreAVL();
    ~ScoreAVL();

    void Insert(int val);
    void PrintScores();   // Print all scores in ascending order
    void Clear();         // Clear all nodes
};

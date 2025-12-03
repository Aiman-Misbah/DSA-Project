#pragma once
#include <iostream>
#include <vector>
using namespace std;

struct AVLScoreNode {
    int score;
    int count;  // NEW: Handle duplicate scores
    AVLScoreNode* left;
    AVLScoreNode* right;
    int height;

    AVLScoreNode(int val) : score(val), count(1), left(NULL), right(NULL), height(1) {}
};

class ScoreAVL {
private:
    AVLScoreNode* root;

    // Helper functions
    int Height(AVLScoreNode* node);
    void UpdateHeight(AVLScoreNode* node);
    int GetBalance(AVLScoreNode* node);
    AVLScoreNode* RightRotate(AVLScoreNode* y);
    AVLScoreNode* LeftRotate(AVLScoreNode* x);
    AVLScoreNode* InsertNode(AVLScoreNode* node, int val);
    void ReverseInOrder(AVLScoreNode* node, vector<int>& result, int& remaining);
    void ClearNodes(AVLScoreNode* node);

public:
    ScoreAVL();
    ~ScoreAVL();

    void Insert(int val);
    vector<int> GetTopScores(int n);
    int GetHighestScore();
    void Clear();
    bool IsEmpty() { return root == NULL; }
};

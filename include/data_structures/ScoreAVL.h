#pragma once
#include <iostream>
#include <vector>
using namespace std;

struct AVLScoreNode {
    int score;
    AVLScoreNode* left;
    AVLScoreNode* right;
    int height;

    AVLScoreNode(int val) : score(val), left(NULL), right(NULL), height(1) {}
};

class ScoreAVL {
private:
    AVLScoreNode* root;

    int Height(AVLScoreNode* node);
    void UpdateHeight(AVLScoreNode* node);
    int GetBalance(AVLScoreNode* node);
    AVLScoreNode* RightRotate(AVLScoreNode* y);
    AVLScoreNode* LeftRotate(AVLScoreNode* x);
    AVLScoreNode* InsertNode(AVLScoreNode* node, int val);
    void ReverseInOrder(AVLScoreNode* node, vector<int>& result, int& remaining);   //getting the scores from highest to lowest
    void ClearNodes(AVLScoreNode* node);    //deleting everything

public:
    ScoreAVL();
    ~ScoreAVL();

    void Insert(int val);
    vector<int> GetTopScores(int n);
    int GetHighestScore();
    void Clear();   //clearing for resetting - after every run not every game
};

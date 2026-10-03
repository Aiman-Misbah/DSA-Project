#include "data_structures/ScoreAVL.h"
#include <iostream>
#include <algorithm>
using namespace std;

ScoreAVL::ScoreAVL() : root(NULL) {}
ScoreAVL::~ScoreAVL() { Clear(); }

int ScoreAVL::Height(AVLScoreNode* node) {
    return node ? node->height : 0;
}

void ScoreAVL::UpdateHeight(AVLScoreNode* node) {
    if (node) {
        node->height = 1 + max(Height(node->left), Height(node->right));
    }
}

int ScoreAVL::GetBalance(AVLScoreNode* node) {
    return node ? Height(node->left) - Height(node->right) : 0;
}

AVLScoreNode* ScoreAVL::RightRotate(AVLScoreNode* y) {
    AVLScoreNode* x = y->left;
    AVLScoreNode* T2 = x->right;

    x->right = y;
    y->left = T2;

    // Update heights
    UpdateHeight(y);
    UpdateHeight(x);

    return x;
}

AVLScoreNode* ScoreAVL::LeftRotate(AVLScoreNode* x) {
    AVLScoreNode* y = x->right;
    AVLScoreNode* T2 = y->left;

    y->left = x;
    x->right = T2;

    UpdateHeight(x);
    UpdateHeight(y);

    return y;
}

AVLScoreNode* ScoreAVL::InsertNode(AVLScoreNode* node, int val) {
    if (!node) return new AVLScoreNode(val);

    if (val < node->score)
        node->left = InsertNode(node->left, val);
    else if (val > node->score)
        node->right = InsertNode(node->right, val);
    else {

        return node;  
    }

    UpdateHeight(node);

    int balance = GetBalance(node);


    if (balance > 1 && val < node->left->score)
        return RightRotate(node);

    if (balance < -1 && val > node->right->score)
        return LeftRotate(node);

    if (balance > 1 && val > node->left->score) {
        node->left = LeftRotate(node->left);
        return RightRotate(node);
    }

    if (balance < -1 && val < node->right->score) {
        node->right = RightRotate(node->right);
        return LeftRotate(node);
    }
    return node;
}

void ScoreAVL::Insert(int val) {
    root = InsertNode(root, val);
}

void ScoreAVL::ReverseInOrder(AVLScoreNode* node, vector<int>& result, int& remaining) {
    if (!node || remaining <= 0) return;    //if none so stop there

    // Right first (larger values)
    ReverseInOrder(node->right, result, remaining);

    result.push_back(node->score);
    remaining--;

    // Then left (smaller values)
    ReverseInOrder(node->left, result, remaining);
}

vector<int> ScoreAVL::GetTopScores(int n) {
    vector<int> topScores;  //creating and returning a vector of the top scores
    if (n <= 0) return topScores;   //empty hi return krdo

    int remaining = n;
    ReverseInOrder(root, topScores, remaining);
    return topScores;
}

int ScoreAVL::GetHighestScore() {
    if (!root) return 0;

    //rightmost is the maximum score
    AVLScoreNode* current = root;
    while (current->right) {
        current = current->right;
    }
    return current->score;
}

void ScoreAVL::ClearNodes(AVLScoreNode* node) {
    if (!node) return;
    ClearNodes(node->left);
    ClearNodes(node->right);
    delete node;
}

void ScoreAVL::Clear() {
    ClearNodes(root);
    root = NULL;
}

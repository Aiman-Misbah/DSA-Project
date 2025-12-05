#include "ScoreAVL.h"
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

    // Perform rotation
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

    // Perform rotation
    y->left = x;
    x->right = T2;

    // Update heights
    UpdateHeight(x);
    UpdateHeight(y);

    return y;
}

AVLScoreNode* ScoreAVL::InsertNode(AVLScoreNode* node, int val) {
    // 1. Perform normal BST insertion
    if (!node) return new AVLScoreNode(val);

    if (val < node->score)
        node->left = InsertNode(node->left, val);
    else if (val > node->score)
        node->right = InsertNode(node->right, val);
    else {

        return node;  // Tree structure unchanged, no rebalancing needed
    }

    // 2. Update height of this ancestor node
    UpdateHeight(node);

    // 3. Get the balance factor
    int balance = GetBalance(node);

    // 4. Check if this node became unbalanced

    // Left Left Case
    if (balance > 1 && val < node->left->score)
        return RightRotate(node);

    // Right Right Case
    if (balance < -1 && val > node->right->score)
        return LeftRotate(node);

    // Left Right Case
    if (balance > 1 && val > node->left->score) {
        node->left = LeftRotate(node->left);
        return RightRotate(node);
    }

    // Right Left Case
    if (balance < -1 && val < node->right->score) {
        node->right = RightRotate(node->right);
        return LeftRotate(node);
    }

    // Return the (unchanged) node pointer
    return node;
}

void ScoreAVL::Insert(int val) {
    root = InsertNode(root, val);
}

void ScoreAVL::ReverseInOrder(AVLScoreNode* node, vector<int>& result, int& remaining) {
    if (!node || remaining <= 0) return;

    // Right first (larger values)
    ReverseInOrder(node->right, result, remaining);

    // Add this score 'count' times (for duplicates)
    for (int i = 0; i < node->count && remaining > 0; i++) {
        result.push_back(node->score);
        remaining--;
    }

    // Then left (smaller values)
    ReverseInOrder(node->left, result, remaining);
}

vector<int> ScoreAVL::GetTopScores(int n) {
    vector<int> topScores;
    if (n <= 0) return topScores;

    int remaining = n;
    ReverseInOrder(root, topScores, remaining);
    return topScores;
}

int ScoreAVL::GetHighestScore() {
    if (!root) return 0;

    // Find rightmost node (maximum value in BST)
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
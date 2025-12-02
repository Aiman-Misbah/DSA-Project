#include "ScoreAVL.h"
#include <iostream>
using namespace std;

// Constructor & Destructor
ScoreAVL::ScoreAVL() : root(NULL) {}
ScoreAVL::~ScoreAVL() { Clear(); }

// Height helper
int ScoreAVL::Height(AVLScoreNode* node) {
    if (!node) return 0;
    return node->height;
}

// Balance factor
int ScoreAVL::GetBalance(AVLScoreNode* node) {
    if (!node) return 0;
    return Height(node->left) - Height(node->right);
}

// Right rotation
AVLScoreNode* ScoreAVL::RightRotate(AVLScoreNode* y) {
    AVLScoreNode* x = y->left;
    AVLScoreNode* T2 = x->right;

    // Perform rotation
    x->right = y;
    y->left = T2;

    // Update heights
    y->height = max(Height(y->left), Height(y->right)) + 1;
    x->height = max(Height(x->left), Height(x->right)) + 1;

    return x; // new root
}

// Left rotation
AVLScoreNode* ScoreAVL::LeftRotate(AVLScoreNode* x) {
    AVLScoreNode* y = x->right;
    AVLScoreNode* T2 = y->left;

    // Perform rotation
    y->left = x;
    x->right = T2;

    // Update heights
    x->height = max(Height(x->left), Height(x->right)) + 1;
    y->height = max(Height(y->left), Height(y->right)) + 1;

    return y; // new root
}

// Recursive insertion
AVLScoreNode* ScoreAVL::InsertNode(AVLScoreNode* node, int val) {
    if (!node) return new AVLScoreNode(val);

    if (val < node->score)
        node->left = InsertNode(node->left, val);
    else
        node->right = InsertNode(node->right, val);

    // Update height
    node->height = 1 + max(Height(node->left), Height(node->right));

    // Check balance
    int balance = GetBalance(node);

    // Left Left
    if (balance > 1 && val < node->left->score)
        return RightRotate(node);

    // Right Right
    if (balance < -1 && val > node->right->score)
        return LeftRotate(node);

    // Left Right
    if (balance > 1 && val > node->left->score) {
        node->left = LeftRotate(node->left);
        return RightRotate(node);
    }

    // Right Left
    if (balance < -1 && val < node->right->score) {
        node->right = RightRotate(node->right);
        return LeftRotate(node);
    }

    return node;
}

// Public insert
void ScoreAVL::Insert(int val) {
    root = InsertNode(root, val);
}

// In-order traversal
void ScoreAVL::InOrderPrint(AVLScoreNode* node) {
    if (!node) return;
    InOrderPrint(node->left);
    cout << node->score << " ";
    InOrderPrint(node->right);
}

void ScoreAVL::PrintScores() {
    InOrderPrint(root);
    cout << endl;
}

// Clear tree
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

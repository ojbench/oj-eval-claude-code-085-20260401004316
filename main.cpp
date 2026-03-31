#include <iostream>
#include <vector>
using namespace std;

struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

// Forward declaration
void deleteTree(TreeNode* node);

// Build tree from extended preorder traversal
TreeNode* buildTree(vector<int>& preorder, int& index) {
    if (index >= preorder.size() || preorder[index] == -1) {
        index++;
        return nullptr;
    }

    TreeNode* root = new TreeNode(preorder[index++]);
    root->left = buildTree(preorder, index);
    root->right = buildTree(preorder, index);
    return root;
}

// Delete a tree to avoid memory leak
void deleteTree(TreeNode* node) {
    if (!node) return;
    deleteTree(node->left);
    deleteTree(node->right);
    delete node;
}

// Check if a subtree contains only 0s (and should be pruned)
// Returns true if the subtree contains at least one 1
bool containsOne(TreeNode* node) {
    if (!node) return false;

    bool leftContainsOne = containsOne(node->left);
    bool rightContainsOne = containsOne(node->right);

    // Prune left child if it doesn't contain 1
    if (!leftContainsOne) {
        deleteTree(node->left);
        node->left = nullptr;
    }

    // Prune right child if it doesn't contain 1
    if (!rightContainsOne) {
        deleteTree(node->right);
        node->right = nullptr;
    }

    // This node contains 1 if itself is 1 or any of its children contains 1
    return node->val == 1 || leftContainsOne || rightContainsOne;
}

// Prune the tree - remove all subtrees with only 0s
TreeNode* pruneTree(TreeNode* root) {
    if (!root) return nullptr;

    if (!containsOne(root)) {
        deleteTree(root);
        return nullptr;
    }

    return root;
}

// Output preorder traversal with null nodes marked as -1
void printPreorder(TreeNode* node, vector<int>& result) {
    if (!node) {
        result.push_back(-1);
        return;
    }

    result.push_back(node->val);
    printPreorder(node->left, result);
    printPreorder(node->right, result);
}

int main() {
    vector<int> preorder;
    int val;

    // Read input until -2
    while (cin >> val && val != -2) {
        preorder.push_back(val);
    }

    int index = 0;
    TreeNode* root = buildTree(preorder, index);

    root = pruneTree(root);

    vector<int> result;
    printPreorder(root, result);

    for (size_t i = 0; i < result.size(); i++) {
        if (i > 0) cout << " ";
        cout << result[i];
    }
    cout << endl;

    deleteTree(root);

    return 0;
}

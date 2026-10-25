#ifndef BST_H
#define BST_H

// ─────────────────────────────────────────────
//  DATA STRUCTURE: BINARY SEARCH TREE (BST)
//  Used for: Menu item storage and search.
//  Supports sorted display (in-order traversal)
//  and O(log n) search by item name.
// ─────────────────────────────────────────────

#include "MenuItem.h"
#include <string>
using namespace std;

struct BSTNode {
    MenuItem  item;
    BSTNode*  left;
    BSTNode*  right;

    BSTNode(const MenuItem& m) : item(m), left(nullptr), right(nullptr) {}
};

class MenuBST {
private:
    BSTNode* root;

    // Insert helper
    BSTNode* insert(BSTNode* node, const MenuItem& item) {
        if (!node) return new BSTNode(item);
        if (item.name < node->item.name)
            node->left  = insert(node->left,  item);
        else if (item.name > node->item.name)
            node->right = insert(node->right, item);
        // duplicate name: update price
        else
            node->item.price = item.price;
        return node;
    }

    // In-order traversal (alphabetical order)
    void inOrder(BSTNode* node) const {
        if (!node) return;
        inOrder(node->left);
        node->item.display();
        inOrder(node->right);
    }

    // Search by name (case-sensitive)
    BSTNode* search(BSTNode* node, const string& name) const {
        if (!node) return nullptr;
        if (name == node->item.name) return node;
        if (name <  node->item.name) return search(node->left,  name);
        return search(node->right, name);
    }

    // Search by partial name (substring match, collect results)
    void partialSearch(BSTNode* node, const string& keyword,
                       vector<MenuItem>& results) const {
        if (!node) return;
        partialSearch(node->left, keyword, results);
        if (node->item.name.find(keyword) != string::npos)
            results.push_back(node->item);
        partialSearch(node->right, keyword, results);
    }

    // Filter by category
    void filterCategory(BSTNode* node, const string& cat,
                        vector<MenuItem>& results) const {
        if (!node) return;
        filterCategory(node->left, cat, results);
        if (node->item.category == cat)
            results.push_back(node->item);
        filterCategory(node->right, cat, results);
    }

    // Destructor helper
    void destroy(BSTNode* node) {
        if (!node) return;
        destroy(node->left);
        destroy(node->right);
        delete node;
    }

    // Search by item ID
    BSTNode* searchById(BSTNode* node, int id) const {
        if (!node) return nullptr;
        if (node->item.id == id) return node;
        BSTNode* left  = searchById(node->left,  id);
        if (left) return left;
        return searchById(node->right, id);
    }

public:
    MenuBST() : root(nullptr) {}
    ~MenuBST() { destroy(root); }

    void insert(const MenuItem& item) {
        root = insert(root, item);
    }

    // Display full menu in alphabetical order
    void displayMenu() const {
        cout << "\n  ─────────────────────────────────────────────────────\n";
        cout << "  🍽️   FULL MENU (Alphabetical Order — BST In-Order)\n";
        cout << "  ─────────────────────────────────────────────────────\n";
        inOrder(root);
        cout << "  ─────────────────────────────────────────────────────\n";
    }

    // Exact name search
    MenuItem* findByName(const string& name) {
        BSTNode* node = search(root, name);
        return node ? &node->item : nullptr;
    }

    // Exact ID search
    MenuItem* findById(int id) {
        BSTNode* node = searchById(root, id);
        return node ? &node->item : nullptr;
    }

    // Partial / keyword search
    vector<MenuItem> searchByKeyword(const string& keyword) const {
        vector<MenuItem> results;
        partialSearch(root, keyword, results);
        return results;
    }

    // Category filter
    vector<MenuItem> getByCategory(const string& cat) const {
        vector<MenuItem> results;
        filterCategory(root, cat, results);
        return results;
    }

    bool isEmpty() const { return root == nullptr; }
};

#endif // BST_H

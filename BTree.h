#ifndef BTREE_H
#define BTREE_H

#include "Node.h"

class BTree {
private:
  Node* root;
  int degree;
  int const printLevel(Node* node, int level, ostream& out); //only class uses this
  int const getHeight(Node* node);

public:
  BTree(int _degree) : root(nullptr), degree(_degree) {}
  ~BTree();

  void insert(int val);
  void splitRoot();

  void const printLevel(int level, ostream& out);
  int const getHeight();
};

BTree::~BTree() {
  if (root != nullptr) {
    delete root;
  }
}

void const BTree::printLevel(int level, ostream& out) {
  if (printLevel(root, level, out) == 0) {
    out << "Empty";
  }
  out << endl;
}

int const BTree::printLevel(Node* node, int level, ostream& out) {
  if (node == nullptr) {
    return 0;
  }
  if (level == 1) {
    node->print(out);
    // out << "| ";
    return 1;
  }

  int sum = 0;
  for (int i = 0; i < node->numChildren; i++) {
    sum += printLevel(node->children[i], level - 1, out);
  }
  return sum;
}

int const BTree::getHeight() {
  return getHeight(root);
}

int const BTree::getHeight(Node* node) {
  if (node == nullptr) {
    return 0;
  }

  int maxHeight = 0;
  for (int i = 0; i < node->numChildren; i++) {
    int heightOfChild = getHeight(node->children[i]);

    if (heightOfChild > maxHeight)
      maxHeight = heightOfChild;
  }

  return maxHeight + 1;
}

void BTree::insert(int val) {
  if (root == nullptr) {
    root = new Node(degree, val);
    return;
  }
  if (!root->insert(val)) { // if we have to split the root, then do so
    splitRoot();
  }
}

void BTree::splitRoot() {
  // part 1
  Node* parent = new Node(degree);
  Node* left = root;
  Node* right = new Node(degree);
  
  int middleIndex = (degree - 1) / 2;
  parent->values[0] = left->values[middleIndex];
  parent->numVals++;

  left->values[middleIndex] = 0;
  left->numVals--;
  
  // part 2
  for (int i = middleIndex + 1; i < degree; i++) {
    right->values[i - middleIndex - 1] = left->values[i];
    right->numVals++;
    left->values[i] = 0;
    left->numVals--;

    if (left->numChildren > 0) { // if root is not a leaf
      right->children[i - middleIndex - 1] = left->children[i];
      right->numChildren++;
      left->children[i] = nullptr;
      left->numChildren--;
    }
  }

  if (left->numChildren > 0) { // if root is not a leaf
    right->children[right->numChildren] = left->children[degree];
    right->numChildren++;
    left->children[degree] = nullptr;
    left->numChildren--;
  }
  
  // part 3
  parent->children[0] = left;
  parent->children[1] = right;
  parent->numChildren += 2;

  root = parent;
}

#endif
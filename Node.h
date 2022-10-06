#ifndef NODE_H
#define NODE_H

#include <ostream>

using namespace std;

struct Node {
  int* values; // dynamic array of values
  Node** children; // dynamic array of pointers
  int degree;
  int numVals;
  int numChildren;

  Node(int _degree) : values(new int[_degree] {0}), children(new Node*[_degree + 1] {nullptr}), degree(_degree), numVals(0), numChildren(0) {}
  Node(int _degree, int _val) : values(new int[_degree] {0}), children(new Node*[_degree + 1] {nullptr}), degree(_degree), numVals(0), numChildren(0) {
    insert(_val);
  }
  ~Node() {
    delete[] values;
    
    for (int i = 0; i <= degree; i++) {
      if (children[i] != nullptr) {
        delete children[i];
      }
    }
    delete[] children;
  }

  bool insert(int val);

  void const print(ostream& out);
};

void const Node::print(ostream& out) {
  for (int i = 0; i < numVals; i++) {
    out << values[i] << " ";
  }
}

bool Node::insert(int val) {
  if (numChildren > 0) {
    /* 
      if interior node:
      1. Find the child to insert the value into
      2. insert the value into that child
      3. check the size of the child's value array. If too big, split
    */
    // part 1
    int indexToInsert = 0;
    if (val < values[0]) {
      indexToInsert = 0;
    }
    else if (values[indexToInsert] == val) {
      return true;
    }
    else {
      for (indexToInsert = 1; indexToInsert < numChildren; indexToInsert++) {
        if (values[indexToInsert] > val) {
          break;
        }
        else if (values[indexToInsert] == val) {
          return true;
        }
      }
    }

    // part 2
    if (indexToInsert == numChildren) {
      indexToInsert--;
    }

    // part 3
    if (!children[indexToInsert]->insert(val)) {
      /*
        to split the child node:
        1. insert the median into this node
          1. find the index to insert the element
          2. push everything to the right of index by one
          3. add the value to the values array
        2. make the right child node
          1. make a new node for the right side
          2. insert all numbers to the right of the median into it
          3. delete all numbers to the right of the median in the original node
          4. insert all children to the right of the median into it
          5. set all children[median + 1..] = nullptr
          6. insert the new node (right) children[indexToInsert + 1]
          7. adjust numVals and numChildren of all nodes
        3. insert the right child node into this node's children array
          1. index to insert the child ptr = (1.1) + 1
          2. push every ptr to the right of the index by one
          3. add the ptr to the children array
      */
      // part 1
      int middleIndex = (degree - 1) / 2;

      for (int i = degree - 1; i > indexToInsert; i--) {
        values[i] = values[i - 1];
      }
  
      values[indexToInsert] = children[indexToInsert]->values[middleIndex];
      numVals++;

      children[indexToInsert]->values[middleIndex] = 0;
      children[indexToInsert]->numVals--;
      
      // part 2
      Node* left = children[indexToInsert];
      Node* right = new Node(degree);

      for (int i = middleIndex + 1; i < degree; i++) {
        right->values[i - middleIndex - 1] = left->values[i];
        right->numVals++;
        left->values[i] = 0;
        left->numVals--;
        
        if (left->numChildren > 0) { // if splitting an interior node
          right->children[i - middleIndex - 1] = left->children[i];
          right->numChildren++;
          left->children[i] = nullptr;
          left->numChildren--;
        }
      }

      if (left->numChildren > 0) { // if splitting an interior node
        right->children[right->numChildren] = left->children[degree];
        right->numChildren++;
        left->children[degree] = nullptr;
        left->numChildren--;
      }
      
      // part 3
      for (int i = degree; i > indexToInsert + 1; i--) {
        children[i] = children[i - 1];
      }
  
      children[indexToInsert + 1] = right;
      numChildren++;
    }
  }
  else {
    /*
      if leaf node:
      1. find the index to insert the element
      2. push everything to the right of index by one
      3. add the value to the values array
    */
    // part 1
    int indexToInsert;
    for (indexToInsert = 0; indexToInsert < numVals; indexToInsert++) {
      if (values[indexToInsert] > val) {
        break;
      }
      else if (values[indexToInsert] == val) {
        return true;
      }
    }

    // part 2
    for (int i = degree - 1; i > indexToInsert; i--) {
      values[i] = values[i - 1];
    }

    // part 3
    values[indexToInsert] = val;
    numVals++;
  }
  
  /*
    return whether or not the size is too big:
    true -> not too big
    false -> too big, need to split
  */
  return numVals < degree;
}

#endif

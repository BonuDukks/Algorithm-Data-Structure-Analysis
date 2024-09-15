#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

class Node {
 private:
  int value;
  Node* leftChild;
  Node* rightChild;

 public:
  void setValue(int valueInsert) { value = valueInsert; }
  int getValue() { return value; }
  Node* getLeftChild() { return leftChild; }
  Node* getRightChild() { return rightChild; }
  void setRightChild(Node* rightNode) { rightChild = rightNode; }
  void setLeftChild(Node* leftNode) { leftChild = leftNode; }
  Node(int valueInsert) {
    value = valueInsert;
    leftChild = nullptr;
    rightChild = nullptr;
  }
};

class AvlTree {
 private:
  Node* root;

 public:
  AvlTree() { root = nullptr; }

  void insert(int value) {
    // Insert node at root if tree is empty.
    if (root == nullptr) {
      root = new Node(value);
    } else if (!find(value, root)) {  // If node not found in tree, insert it.
      root = insertHelperFunction(root, value);
    }
  }

  // Helper function for inserting node.
  Node* insertHelperFunction(Node* currentNode, int value) {
    // Reached insertion location.
    if (currentNode == nullptr) {
      return new Node(value);
    }

    // Traverse tree to find location.
    if (value < currentNode->getValue()) {
      currentNode->setLeftChild(
          insertHelperFunction(currentNode->getLeftChild(), value));
    } else if (value > currentNode->getValue()) {
      currentNode->setRightChild(
          insertHelperFunction(currentNode->getRightChild(), value));
    }

    // Balance after inserting.
    return balance(currentNode);
  }

  Node* balance(Node* currentNode) {
    // Get balance factor of current node.
    int balanceFactor = getBalanceFactor(currentNode);

    // Perform Rotations if balance factor is not part of AVL set.
    if (balanceFactor > 1) {
      if (getBalanceFactor(currentNode->getLeftChild()) >= 0) {
        return rightRotation(currentNode);
      } else {
        return leftRightRotation(currentNode);
      }
    }

    if (balanceFactor < -1) {
      if (getBalanceFactor(currentNode->getRightChild()) <= 0) {
        return leftRotation(currentNode);
      } else {
        return rightLeftRotation(currentNode);
      }
    }

    return currentNode;
  }

  // Searches tree to see if value already in it.
  bool find(int value, Node* currentNode) {
    if (currentNode == nullptr) {
      return false;
    }

    if (currentNode->getValue() == value) {
      return true;
    }

    if (value < currentNode->getValue()) {
      return find(value, currentNode->getLeftChild());
    } else {
      return find(value, currentNode->getRightChild());
    }
  }

  // Calculate height of current node
  int height(Node* currentNode) {
    if (currentNode == nullptr) {
      return -1;
    }
    return 1 + max(height(currentNode->getLeftChild()),
                   height(currentNode->getRightChild()));
  }

  // Calculate balance factor of current node.
  int getBalanceFactor(Node* currentNode) {
    if (currentNode == nullptr) {
      return 0;
    }
    return height(currentNode->getLeftChild()) -
           height(currentNode->getRightChild());
  }

  // Right Right Rotation
  Node* rightRotation(Node* currentNode) {
    Node* newRoot = currentNode->getLeftChild();
    Node* tempNode = newRoot->getRightChild();

    newRoot->setRightChild(currentNode);
    currentNode->setLeftChild(tempNode);

    return newRoot;
  }

  // Left Left Rotation
  Node* leftRotation(Node* currentNode) {
    Node* newRoot = currentNode->getRightChild();
    Node* tempNode = newRoot->getLeftChild();

    newRoot->setLeftChild(currentNode);
    currentNode->setRightChild(tempNode);

    return newRoot;
  }

  // Left Right Rotation
  Node* leftRightRotation(Node* currentNode) {
    currentNode->setLeftChild(leftRotation(currentNode->getLeftChild()));
    return rightRotation(currentNode);
  }

  // Right Left Rotation
  Node* rightLeftRotation(Node* currentNode) {
    currentNode->setRightChild(rightRotation(currentNode->getRightChild()));
    return leftRotation(currentNode);
  }

  // Delete Node
  void deleteNode(int value) { root = deleteNodeHelper(root, value); }

  // Helper function for delete node function
  Node* deleteNodeHelper(Node* currentNode, int value) {
    // If tree has no node to delete, return nullptr
    if (currentNode == nullptr) {
      return nullptr;
    }

    // Traverse down tree
    if (value < currentNode->getValue()) {
      currentNode->setLeftChild(
          deleteNodeHelper(currentNode->getLeftChild(), value));
    } else if (value > currentNode->getValue()) {
      currentNode->setRightChild(
          deleteNodeHelper(currentNode->getRightChild(), value));
    } else {
      // If Node has no children
      if (currentNode->getLeftChild() == nullptr &&
          currentNode->getRightChild() == nullptr) {
        delete currentNode;
        return nullptr;
      }

      // If node has 1 child
      if (currentNode->getLeftChild() == nullptr) {
        Node* tempNode = currentNode->getRightChild();
        delete currentNode;
        return tempNode;
      } else if (currentNode->getRightChild() == nullptr) {
        Node* tempNode = currentNode->getLeftChild();
        delete currentNode;
        return tempNode;
      }

      // If node has 2 children, get successor, switch values and dleete
      // succcessor.
      Node* successor = findSuccessor(currentNode->getLeftChild());
      currentNode->setValue(successor->getValue());
      currentNode->setRightChild(deleteNodeHelper(currentNode->getRightChild(),
                                                  successor->getValue()));
    }

    // Balance tree after deleting node.
    return balance(currentNode);
  }

  // Finds largest element which is smaller than the current root.
  Node* findSuccessor(Node* currentNode) {
    while (currentNode->getRightChild() != nullptr) {
      currentNode = currentNode->getRightChild();
    }
    return currentNode;
  }

  // Pre Order Printing
  void printPreOrder() const {
    // Print EMPTY if tree is empty.
    if (root == nullptr) {
      cout << "EMPTY" << endl;
      return;
    } else {
      printPreOrderHelper(root);
      cout << endl;
    }
  }

  // Post Order Printing
  void printPostOrder() const {
    // Print EMPTY if tree is empty
    if (root == nullptr) {
      cout << "EMPTY" << endl;
      return;
    } else {
      printPostOrderHelper(root);
      cout << endl;
    }
  }

  // In Order Printing
  void printInOrder() const {
    // Print EMPTY if tree is empty
    if (root == nullptr) {
      cout << "EMPTY" << endl;
      return;
    } else {
      printInOrderHelper(root);
      cout << endl;
    }
  }

  // Pre Order Printing helper function
  void printPreOrderHelper(Node* currentNode) const {
    if (currentNode == nullptr) {
      return;
    }
    cout << currentNode->getValue() << " ";
    printPreOrderHelper(currentNode->getLeftChild());
    printPreOrderHelper(currentNode->getRightChild());
  }

  // Post Order Printing helper function
  void printPostOrderHelper(Node* currentNode) const {
    if (currentNode == nullptr) {
      return;
    }
    printPostOrderHelper(currentNode->getLeftChild());
    printPostOrderHelper(currentNode->getRightChild());
    cout << currentNode->getValue() << " ";
  }

  // In Order Printing helper function
  void printInOrderHelper(Node* currentNode) const {
    if (currentNode == nullptr) {
      return;
    }
    printInOrderHelper(currentNode->getLeftChild());
    cout << currentNode->getValue() << " ";
    printInOrderHelper(currentNode->getRightChild());
  }
};

int main() {
  AvlTree avlTree;

  string inputLine;
  // Read input line
  getline(cin, inputLine);

  stringstream ss(inputLine);
  string move;

  vector<string> moves;

  // Split input string into individual moves
  while (ss >> move) {
    moves.push_back(move);
  }

  // Read each A or D move.
  for (int i = 0; i < moves.size() - 1; i++) {
    char action = moves[i][0];             // Gets the Action (Add or Delete)
    int value = stoi(moves[i].substr(1));  // Gets the value.

    if (action == 'A') {
      avlTree.insert(value);
    } else if (action == 'D') {
      avlTree.deleteNode(value);
    }
  }

  // Get the print method.
  string printMethod = moves.back();

  // Call Print Order function depending on input.
  if (printMethod == "PRE") {
    avlTree.printPreOrder();
  } else if (printMethod == "POST") {
    avlTree.printPostOrder();
  } else if (printMethod == "IN") {
    avlTree.printInOrder();
  }

  return 0;
}
#include "tree_node.h"
#include <vector>
#include <iostream>

using namespace std;

int main() {
    Node* root = new Node("root", 0);
    Node* A1 = new Node("A1", 1);
    Node* A2 = new Node("A2", 2);
    Node* B1 = new Node("B1", 3);
    Node* B2 = new Node("B2", 4);
    Node* B3 = new Node("B3", 5);

    root->addChild(A1);
    root->addChild(A2);

    A2->addChild(B1);
    A2->addChild(B2);
    A2->addChild(B3);

    root->print();
    cout << endl;

    return 0;
}
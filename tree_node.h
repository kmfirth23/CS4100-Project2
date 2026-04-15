#ifndef TREE_NODE_H
#define TREE_NODE_H

#include <string>
#include <vector>
#include <iostream>
using namespace std;

class Node{
    public:
        string name;
        string weight;
        vector<Node*> children;

    Node(string n, int w) {
        name = n;
        weight = w;
    }

    void addChild(Node* child) {
        children.push_back(child);
    }

    void print() {
        cout << name;
        if (!children.empty()) {
            cout << "[";
            for (long unsigned int i = 0; i < children.size(); i++) {
                children[i] -> print ();
                if (i != children.size() - 1) {
                    cout << ",";
                }
            }
            cout << "]";
        }
    }
};

#endif
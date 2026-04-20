#ifndef TREE_NODE_H
#define TREE_NODE_H

#include <string>
#include <vector>
#include <iostream>
using namespace std;

class Node{
    public:
        string name;
        int weight;
        string parent;
        vector<Node*> children;

    Node(string n, int w, string p = "") {
        name = n;
        weight = w;
        parent = p;
    }

    void print() {
        cout << name;
        if (!children.empty()) {
            cout << "[";
            for (size_t i = 0; i < children.size(); i++) {
                children[i]->print();
                if (i != children.size() - 1) {
                    cout << ",";
                }
            }
            cout << "]";
        }
    }
};

#endif
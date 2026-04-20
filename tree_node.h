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

    Node(string n, int w, string p = "") {
        name = n;
        weight = w;
        parent = p;
    }
};

#endif
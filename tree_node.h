/*
 * @file tree_node.h
 * @author Kennedy Firth and Megan Penn
 * @brief - Class for the nodes to build a tree 
 * @date 2026-04-20
*/

#ifndef TREE_NODE_H
#define TREE_NODE_H

#include <string>
#include <vector>
#include <iostream>
using namespace std;
//class for Nodes
class Node{
    public:
        string name;
        int weight;
        string parent;
        vector<Node*> children;
    /** 
     * @brief constructor
     * @param n the name of the node
     * @param w the weight
     * @param p the parent of this node
     */

    Node(string n, int w, string p = "") {
        name = n;
        weight = w;
        parent = p;
    }
    /**
     * @brief the print function for the tree
     */
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
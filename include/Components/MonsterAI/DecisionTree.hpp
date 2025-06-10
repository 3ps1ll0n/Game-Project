#pragma once

#include <functional>
#include <vector>
#include <map>
#include <string>

class Link;

struct Node
{
    std::vector<Link*> links;
    std::function<void()> action;
};

struct Link
{
    Node* from;
    Node* to;
    std::function<bool()> condition;

    Link(Node* f, Node* t, std::function<bool()> c)
    {
        from = f;
        to = t;
        condition = c;    
    }
};

class DecisionTree
{
private:
    std::vector<Node*> nodes;
    Node* currentNode;
public:
    DecisionTree() 
    {
        nodes = {};
        currentNode = NULL;
    }
    ~DecisionTree() {}

    Node* getNode(int index)
    {
        return nodes[index];
    }

    Node* addNode()
    {
        auto node = new Node();
        nodes.push_back(node);
        return node;
    }

    Node* setCurrentNode(int index)
    {
        currentNode = nodes[index];
        return currentNode;
    }

    void update()
    {
        for (auto link : currentNode->links)
        {
            if(link->condition()) currentNode = link->to;
        }
    }
};

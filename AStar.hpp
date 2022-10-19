#pragma once

#include "Handle.hpp"
#include <vector>
#include <memory>
#include <algorithm>


enum class NodeStates { start = 1, end = 2, notTraversable = 3, notVisited = 4, unreachable, path};

struct Node
{   
    //posx in window
    int posx;
    //posy in window
    int posy;bool contains(std::vector<Node>& vec, Node& node);
    //x index in vector
    int x;
    //y index in vector
    int y;
    //indicates what type of node it currently is
    NodeStates nodeState;
    //points to the parent of this node
    Node* parent;

    //how far away from start node
    int gCost;
    //how far away from end node
    int hCost;


    Node(int posx, int posy, int x, int y){
        this->posx = posx;
        this->posy = posy;
        this->x = x;
        this->y = y;
        this->nodeState = NodeStates::notVisited;
        this->gCost = 0;
        this->hCost = 0;
        this->parent = nullptr;
    }

    Node(const Node& node){
        this->posx = node.posx;
        this->posy = node.posy;
        this->x = node.x;
        this->y = node.y;
        this->nodeState = node.nodeState;
        this->gCost = node.gCost;
        this->hCost = node.hCost;
    }

    int fCost(){
        return gCost + hCost;
    }

     bool operator==(const Node& node){
        return std::tie(posx, posy, x, y, nodeState) 
             == std::tie(node.posx, node.posy, node.x, node.y, node.nodeState);
    }

};

class AStar{

    //all nodes
    std::vector<std::vector<Node>> mNodes;
    //curently open nodes
    std::vector<Node*> mOpen;
    //closed nodes
    std::vector<Node*> mClosed;
    //vector that holds the path generated by the astar
    std::vector<Node*> mPath;

    //points to starting node
    Node* start;
    //points to ending node
    Node* end;

public:
    AStar(int screenWidht, int screenHeight, int cellSize);

    std::vector<std::vector<Node>>& getNodes(){
        return mNodes;
    }

    //returns valid neighbours around node
    std::vector<Node*> getNeigbours(const Node& currentNode);

    //returns path
    std::vector<Node*>* getPath(){
        return &mPath;
    }

    //tries to trace path and returns true on success
    bool tracePath();

    //resets astar values
    void reset(int screenWidht, int screenHeight, int cellSize);

private:
    //returns true if node is in closed
    bool isNodeInClosed(Node& node);   

    //returns distance from node A to node B
    int getDistance(Node& from, Node& to);

    //returns true if node is contained in vector
    bool contains(std::vector<Node*>& vec, Node& node);

    //astar algorhytm to find path from starting node to ending node
    Node* pathfind(Node& startingNode, Node& endingNode);

};
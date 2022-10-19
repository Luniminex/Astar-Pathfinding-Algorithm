#include "AStar.hpp"

AStar::AStar(int screenWidht, int screenHeight, int cellSize){
    int nx = screenWidht / cellSize - 2;
    int ny = screenHeight / cellSize - 2;

    for(int i = 0; i < ny; i++){
        mNodes.push_back(std::vector<Node>(nx, Node(0, 0, 0, 0)));
        for(int j = 0; j < nx; j++){
            mNodes.at(i).at(j).posx = j * cellSize + cellSize;
            mNodes.at(i).at(j).posy = i * cellSize + cellSize;
            mNodes.at(i).at(j).x = j;
            mNodes.at(i).at(j).y = i;
        }
    }
}

std::vector<Node*> AStar::getNeigbours(const Node& currentNode){
    std::vector<Node*> neighbours;
    for(int i = -1; i <= 1; i++){
        for(int j = -1; j <= 1; j++){
                //its current node
            if(j == 0 && i == 0)
                continue;
                //its out of bounds
            if(static_cast<unsigned>(currentNode.x) + j > mNodes.at(0).size() - 1)
                continue;
                //its out of bounds
            if(currentNode.x + j < 0)
                continue;
                //its out of bounds
            if(static_cast<unsigned>(currentNode.y) + i > mNodes.size() - 1)
                continue;
                //its out of bounds
            if(currentNode.y + i < 0)
                continue;
            neighbours.push_back(&mNodes.at(currentNode.y + i).at(currentNode.x + j));
        }
    }
    return neighbours;
}

Node* AStar::pathfind(Node& startingNode, Node& endingNode){
    this->start = &startingNode;
    this->end = &endingNode;
    
    //clears closed and open nodes possibly from last use
    mClosed.clear();
    mOpen.clear();

    //sets hcost of starting node
    start->hCost = getDistance(*start, endingNode);

    //pushes back starting node into openlist
    mOpen.push_back(start);

    //sets current node to starting node
    Node* current = mOpen.at(0);
    current->parent = nullptr;

    std::vector<Node*> neighbours;
    //reserves up to maximum possible neighbours 
    neighbours.reserve(8);

    while(mOpen.size() > 0){
        //finds node in open nodes with lowest fcost and sets it to current
        current = mOpen.at(0);
        for(auto& node : mOpen){
            if(node->fCost() < current->fCost() || node->fCost() == current->fCost() && node->hCost < current->hCost){
                current = node;
            }
        }
        //removes current from open and then adds it to closed
        for(unsigned int i = 0; i < mOpen.size(); i++){
            if(current == mOpen.at(i)){
                mOpen.erase(mOpen.begin() + i);
                break;
            }
        }
        mClosed.push_back(current);

        //if current is endingNode, path was found
        if(*current == endingNode)
            return &endingNode;
 
        //gets neighbours of current node
        neighbours.clear();
        neighbours = getNeigbours(*current);

        //goes through all neighbours of current node
        for(auto& neighbour : neighbours){
            //if you cant walk on the node or the node is in closed, continue to next neighbour
            if(neighbour->nodeState == NodeStates::notTraversable || isNodeInClosed(*neighbour)){
                continue;
            }
            
            //calculate path cost
            int newMvmCost = current->gCost + getDistance(*current, *neighbour);
            //if new path cost is lower than current or if he is not in open set
            if(newMvmCost < neighbour->gCost || !contains(mOpen, *neighbour)){
                neighbour->gCost = newMvmCost;
                neighbour->hCost = getDistance(*neighbour, *end);
                neighbour->parent = current;

                if(!contains(mOpen, *neighbour)){
                    mOpen.push_back(neighbour);
                }
            }

        }

    }
    //if path wasnt found, returns nullptr
    return nullptr;
}

bool AStar::isNodeInClosed(Node& node){
    for(auto& node_ : mClosed){
        if(node == *node_)
            return true;
    }
    return false;
 }

int AStar::getDistance(Node& from, Node& to){
    int distX = abs(from.x - to.x);
    int distY = abs(from.y - to.y);

    if(distX > distY){
        return 14*distY + 10*(distX - distY);
    }
    return 14*distX + 10*(distY - distX);
 }

bool AStar::contains(std::vector<Node*>& vec, Node& node){
    for(auto& n : vec){
        if(*n == node){
            return true;
        }
    }
    return false;
 }

bool AStar::tracePath(){
    int startx;
    int starty;
    int endx;
    int endy;
    bool foundStart = false;
    bool foundEnd = false;
    for(unsigned int i = 0; i < mNodes.size() - 1; i++){
        for(unsigned int j = 0; j < mNodes.at(0).size() - 1; j++){
            if(mNodes.at(i).at(j).nodeState == NodeStates::start){
                startx = j;
                starty = i;
                foundStart = true;
            }
            if(mNodes.at(i).at(j).nodeState == NodeStates::end){
                endx = j;
                endy = i;
                foundEnd = true;
            }                
        }
    }

    //if either end or start node wasnt placed, returns 0
    if(!foundEnd || !foundStart)
        return 0;

    //if astar found path, returns the end node and should be able to trace path through parents
    Node* node = pathfind(mNodes.at(starty).at(startx), mNodes.at(endy).at(endx));
    if(node == nullptr){
        return 0;
    }

    //creates valid path
    mPath.push_back(node);
    while(node->parent != nullptr){
        node = node->parent;
        mPath.push_back(node);
    }

    //reverses path so it goes from start->end node
    std::reverse(mPath.begin(), mPath.end());
    return 1;
 }

 void AStar::reset(int screenWidht, int screenHeight, int cellSize){
    mNodes.clear();
    mOpen.clear();
    mClosed.clear();
    mPath.clear();

    int nx = screenWidht / cellSize - 2;
    int ny = screenHeight / cellSize - 2;

    for(int i = 0; i < ny; i++){
        mNodes.push_back(std::vector<Node>(nx, Node(0, 0, 0, 0)));
        for(int j = 0; j < nx; j++){
            mNodes.at(i).at(j).posx = j * cellSize + cellSize;
            mNodes.at(i).at(j).posy = i * cellSize + cellSize;
            mNodes.at(i).at(j).x = j;
            mNodes.at(i).at(j).y = i;
        }
    }
    start = nullptr;
    end = nullptr;
 }
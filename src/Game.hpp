#pragma once

#include <SDL2/SDL.h>
#include "Handle.hpp"
#include "AStar.hpp"

struct Grid {
public:
    //NO cells on x
    int nx;
    //NO cells on Y
    int ny;
    int cellSize;
};

enum PlaceMode {start = 1, end = 2, block = 3, none = 4};

enum class GameState { 
    //user is placing nodes
    building, 
    //signal to AStar algorhytm to start and find the path
    astar,
    //starts tracing path AStar generated
    tracing,
    //path is traced and program is now waiting for user input 
    waiting,
    //singal to reset all values
    reset, 
    //signal to quit
    quit
};

class Game {
    
    Grid mGrid;

    SDL_Event mEvnt;
    SDL_Window &mWindow;
    SDL_Renderer &mRenderer;

    int windowWidht;
    int windowHeight;
    int cellSize;

    AStar astar;
    
    //indicates in which state the game is
    GameState gameState;

    //indicates what block to place
    int mPlaceMode;
public:
    Game(SDL_Window &window, SDL_Renderer &renderer, int windowWidht, int windowHeight, int cellsize);

    //game loop
    void loop(Handle &handle);

private:
    //handles inputs
    void handleInput(SDL_Event &event);

    //handles mouse inputs
    void handleMouseInput(SDL_Event &event);

    //handles keyboard input
    void handleKeyboardInput(SDL_Event &event);

    //draws grid
    void drawGrid(Handle &handle);

    //fills grid with rectangles
    void fillGrid(Handle &handle);

    //places a node at position
    void placeNode(NodeStates type, int posx, int posy);

    //changes what type of node is placed
    void changeMode();

    //places node
    void chooseWhichToPlace(int type, int x, int y);

    //calculates path from starting node to ending node
    std::vector<Node*>* findPath();

    //resets everything
    void reset();

    //makes path visible 
    void showPath(std::vector<Node*> path);

};
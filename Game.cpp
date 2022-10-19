#include "Game.hpp"

Game::Game(SDL_Window& window, SDL_Renderer& renderer, int windowWidht, int windowHeight, int cellSize) :
  mWindow(window), mRenderer(renderer), astar(windowWidht, windowHeight, cellSize) {
    gameState = GameState::building;
    mGrid.nx = windowWidht / cellSize;
    mGrid.ny = windowHeight / cellSize;
    mGrid.cellSize = cellSize;
    mPlaceMode = PlaceMode::start;
    this->windowWidht = windowWidht;
    this->windowHeight = windowHeight;
    this->cellSize = cellSize;
}

void Game::handleInput(SDL_Event& event){

    switch (event.type)
    {
    case SDL_QUIT:
        gameState = GameState::quit;
        break;
    case SDL_MOUSEBUTTONDOWN:
        handleMouseInput(event);
        break;
    case SDL_KEYDOWN:
        handleKeyboardInput(event);
        break;
    default:
        break;
    }

}

void Game::handleMouseInput(SDL_Event &event){
    int x, y;
    switch(event.button.button)
    {
    case SDL_BUTTON_LEFT:
    {
        SDL_GetMouseState(&x, &y);
        chooseWhichToPlace(mPlaceMode, x, y);
        break;
    }
    case SDL_BUTTON_RIGHT:
        SDL_GetMouseState(&x, &y);
        chooseWhichToPlace(mPlaceMode, x, y);
        break;
    default:
        break;
    }
}

void Game::handleKeyboardInput(SDL_Event &event){
    switch (event.key.keysym.sym)
    {
    case SDLK_SPACE:
        //you are only able to pathfind when you are in building state
        if(gameState == GameState::building)
            gameState = GameState::astar;
        break;
    case SDLK_g:
        changeMode();
        break;
    case SDLK_r:
        reset();
        break;
    default:
        break;
    }
}


void Game::loop(Handle& handle){
    //sets fps per second
    const int fps = 60;
    //how much one frame should take
    const int frameDelay = 1000 / fps;

    //time when frame starts
    Uint32 frameStart;
    //current frame time
    Uint32 frameTime;

    //main loop
    while (gameState != GameState::quit)
    {   //events
        while(SDL_PollEvent(&mEvnt)){
            handleInput(mEvnt);
        }

        frameStart = SDL_GetTicks();

        SDL_SetRenderDrawColor(handle.getRenderer(), 180, 180, 180, 255);
        SDL_RenderClear(handle.getRenderer());

        //if user wishes to pathfind
        if(gameState == GameState::astar){
            std::vector<Node*>* temp = findPath();
            //show path only when there is a valid path
            if(temp != nullptr){
                showPath(*temp);
            }
            else{
                gameState = GameState::building;
            }
        }

        fillGrid(handle);

        SDL_SetRenderDrawColor(handle.getRenderer(), 0, 0, 0, 255);
        drawGrid(handle);
       
        SDL_RenderPresent(handle.getRenderer());

        //calculates how much time did the frame take
        frameTime = SDL_GetTicks() - frameStart;

        if(frameDelay > frameTime){
            SDL_Delay(frameDelay - frameTime);
        }
    }
    
} 

void Game::drawGrid(Handle& handle){
    for(int i = 0; i <= mGrid.nx; i++){
        handle.drawLine(i * mGrid.cellSize + mGrid.cellSize, mGrid.cellSize, 
                        i * mGrid.cellSize + mGrid.cellSize, mGrid.ny * mGrid.cellSize - mGrid.cellSize);
    }
    for(int i = 0; i <= mGrid.ny; i++){
        handle.drawLine(mGrid.cellSize, i * mGrid.cellSize + mGrid.cellSize, 
                        mGrid.nx * mGrid.cellSize - mGrid.cellSize, i * mGrid.cellSize + mGrid.cellSize);
    }
}

void Game::fillGrid(Handle& handle){
    SDL_Rect rect;
    rect.w = mGrid.cellSize;
    rect.h = mGrid.cellSize;
    for(int i = 1; i < mGrid.ny - 1; i++){
        for(int j = 1; j < mGrid.nx - 1; j++){
            rect.x = j * mGrid.cellSize;
            rect.y = i * mGrid.cellSize;
            switch (astar.getNodes().at(i-1).at(j-1).nodeState)
            {
            case NodeStates::notVisited:
                SDL_SetRenderDrawColor(handle.getRenderer(), 255, 255, 255, 255);
                break;
            case NodeStates::unreachable:
               SDL_SetRenderDrawColor(handle.getRenderer(), 255, 0, 0, 255);
               break;
            case NodeStates::start:
               SDL_SetRenderDrawColor(handle.getRenderer(), 0, 0, 180, 255);
               break;
            case NodeStates::end:
                SDL_SetRenderDrawColor(handle.getRenderer(), 0, 0, 255, 255);
                break;
            case NodeStates::path:
                SDL_SetRenderDrawColor(handle.getRenderer(), 0, 255, 0, 255);
                break;
            case NodeStates::notTraversable:
                SDL_SetRenderDrawColor(handle.getRenderer(), 0, 0, 0, 255);
                break;
            default:
                break;
            }
            
            handle.drawRect(rect);
        }
    }
}

void Game::placeNode(NodeStates type, int posx, int posy){
    
    //calculates bondaries inside grid where you can place nodes
    bool containedInX = posx > mGrid.cellSize && posx < mGrid.cellSize + ((mGrid.nx - 2 ) * mGrid.cellSize);
    bool containedInY = posy > mGrid.cellSize && posy < mGrid.cellSize + ((mGrid.ny - 2 ) * mGrid.cellSize);
    
    //if outside, exit function
    if(!containedInX || !containedInY){
        return;
    }

    //place starting or ending node
    if(type == NodeStates::start || type == NodeStates::end){
        //check if there is already one placed
        for(unsigned int i = 0; i < astar.getNodes().size(); i++){
            for(unsigned int j = 0; j < astar.getNodes().at(i).size(); j++){
                if(astar.getNodes().at(i).at(j).nodeState == type){
                    astar.getNodes().at(i).at(j).nodeState = NodeStates::notVisited;
                }
            }
        }
        int row = (posy / mGrid.cellSize) - 1;
        int collumn = (posx / mGrid.cellSize) - 1;
        astar.getNodes().at(row).at(collumn).nodeState = type;
    }
    /////
    if(type == NodeStates::notTraversable || type == NodeStates::notVisited){
        int row = (posy / mGrid.cellSize) - 1;
        int collumn = (posx / mGrid.cellSize) - 1;
        astar.getNodes().at(row).at(collumn).nodeState = type;
    }
}

void Game::changeMode(){
    mPlaceMode += 1;
    if(mPlaceMode > 4 ){
        mPlaceMode = 1;
    }
}

void Game::chooseWhichToPlace(int type, int x, int y){
    //chooses which node to place from building mode selected
    switch (type)
    {
    case PlaceMode::start:
        placeNode(NodeStates::start, x, y);
        break;
    case PlaceMode::end:
        placeNode(NodeStates::end, x, y);
        break;
    case PlaceMode::block:
        placeNode(NodeStates::notTraversable, x, y);
        break;
    case PlaceMode::none:
        placeNode(NodeStates::notVisited, x, y);
        break;
    default:
        break;
    }
}

std::vector<Node*>* Game::findPath(){
    
    //if it wasnt able to tracepath, return nullptr
    if(astar.tracePath() == 0)
        return nullptr;


    gameState = GameState::tracing;
    
    //returns valid path that can be shown
    return astar.getPath();
}

void Game::reset(){
    astar.reset(windowWidht, windowHeight, cellSize);
    gameState = GameState::building;
}

void Game::showPath(std::vector<Node*> path){
    //for all nodes in path changes state to path
    for(auto& a : path){
        a->nodeState = NodeStates::path;
    }

}
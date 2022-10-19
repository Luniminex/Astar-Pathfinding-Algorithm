#include "Handle.hpp"
#include "Game.hpp"

#define WINDOW_WIDHT 1024
#define WINDOW_HEIGHT 768
#define CELLSIZE 64


int main(){

    Handle handle;

    if(handle.initLibs() != 1){
        std::cout << "Failed to initialize SDL libraries" << std::endl;
        return 1;
    }
    if(handle.createWindow("A*", WINDOW_WIDHT, WINDOW_HEIGHT)!= 1){
        std::cout << "Could not create a window" << std::endl;
        return 1;
    }
    if(handle.createRenderer() != 1){
        std::cout << "Could not create a renderer" << std::endl;
        return 1;
    }

    Game game(*handle.getWindow(), *handle.getRenderer(), WINDOW_WIDHT, WINDOW_HEIGHT, CELLSIZE);

    game.loop(handle);

    handle.quit();

    return 0;
}


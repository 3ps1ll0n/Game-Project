#include "Game.hpp"
#include <iostream>
#include <wtypes.h>

// g++ src/*.cpp src/jsoncpp/*.cpp -o bin/prog -I include -I include/Components -I include/SDL2 -L lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer

int main(int argc, char *argv[])
{
    Game* game = new Game();

    RECT desck;
    const HWND hDesktop = GetDesktopWindow();
    GetWindowRect(hDesktop, &desck);

    std::cout << desck.bottom  << " | " << desck.right << std::endl;

    game->init("Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, desck.right, desck.bottom);

    Uint32 fps = 1000/60;

    Uint32 start = 0;
    Uint32 frameTime = 0;
    double dt = 0;

    while(game->isRunning())
    {
        dt = (SDL_GetTicks() - start) * 1e-3;
        start = SDL_GetTicks();

        game->eventHandler();
        game->update(dt);
        game->render();

        
        frameTime = SDL_GetTicks() - start;

        if(frameTime < fps) {
            SDL_Delay(fps - frameTime);
            //std::cout << fps - dt << std::endl;
        }
    }

    game->clear();

    return 0;
}
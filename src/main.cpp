#include "Game.hpp"

//g++ src/*.cpp src/jsoncpp/*.cpp -o bin/prog -I include -I include/SDL2 -L lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf

int main(int argc, char *argv[])
{
    Game* game = new Game();
    game->init("Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 700, 600);

    while(game->isRunning())
    {
        SDL_GetTicks();

        game->eventHandler();
        game->update(0);
        game->render();
    }

    game->clear();

    return 0;
}
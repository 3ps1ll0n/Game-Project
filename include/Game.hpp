#pragma once

#include <SDL.h>
#include "Camera.hpp"
#include "Log.hpp"
#include "ECS.hpp"

class Game
{
private:
    SDL_Window* window;

    int windowWidth, windowHeight;

    bool running;

    Entity* player;
    Camera* cam;
public:
    Game(/* args */);
    ~Game();

    void init(const char* title, int xpos, int ypos, int width, int height);
    void eventHandler();
    void update(double dt);
    void render();
    void clear();
    bool isRunning(){return running;};

    static SDL_Renderer* renderer;
    static bool debugMode;
    static Entity* tileMap;
    static Manager* entityManager;
};


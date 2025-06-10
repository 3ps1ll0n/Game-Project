#ifndef GAME_H
#define GAME_H

#include <SDL.h>
#include "Camera.hpp"
#include "Log.hpp"
#include "ECS.hpp"
#include "HitBoxesManager.hpp"

class Game
{
private:
    SDL_Window* window;

    int windowWidth, windowHeight;

    bool running;

    static Entity* player;
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

    static Entity* getPlayer(){return player;}

    static SDL_Renderer* renderer;
    static bool debugMode;
    static Entity* tileMap;
    static Manager* entityManager;
    static HitBoxesManager* hitBoxesManager;
};

#endif
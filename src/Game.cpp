#include "Game.hpp"

#include <iostream>

Game::Game(){}

Game::~Game(){}

void Game::init(const char* title, int xpos, int ypos, int width, int height)
{
    if(SDL_Init(SDL_INIT_EVERYTHING) == 0)
    {
        window = SDL_CreateWindow(title, xpos, ypos, width, height, 0);
        if(!window) std::cout << "ERROR WHILE CREATING THE WINDOW" << std::endl;

        renderer = SDL_CreateRenderer(window, -1, 0);
        if(!renderer) std::cout << "ERROR windowWHILE CREATING THE REMDERER" << std::endl;

        running = true;
    }
    else
    {
        std::cout << "CANNOT INIT SDL SUBSYSTEM" << std::endl;
        running = false;
    } 
}

void Game::eventHandler()
{
    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT:
            running = false;
            break;
        
        default:
            break;
        }
    }
    
}

void Game::update(double dt)
{

}

void Game::render()
{
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
}

void Game::clear()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}

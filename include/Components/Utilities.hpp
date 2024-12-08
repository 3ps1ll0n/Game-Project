#pragma once

#include "SDL.h"
#include <math.h>
#include <iostream>

struct Vector2D{
    double x;
    double y;

    void reset()
    {
        x = 0.0;
        y = 0.0;
    }

    void diplay(SDL_Renderer* r, int xPos, int yPos, double factor, SDL_Color color)
    {
        SDL_SetRenderDrawColor(r, color.r, color.g, color.b, color.a);
        SDL_RenderDrawLine(r, xPos, yPos, xPos + (x * factor), yPos + (y * factor));
    }

    void normalize()
    {
        if(x == 0 && y == 0) return;
        double nX =  x / (sqrt(pow(x, 2) + pow(y, 2)));
        double nY =  y / (sqrt(pow(x, 2) + pow(y, 2)));       

        x = nX;
        y = nY;
    }

    Vector2D operator *=(double m)
    {
        x *= m;
        y *= m;
        return *this;
    }
};

struct Cooldown{
    double dt;
    double cooldown;

    void update(double dt)
    {
        this->dt += dt;
    }

    bool canExecute()
    {
        if(dt >= cooldown) 
        {
            dt = 0;
            return true;
        }
        return false;
    }
};

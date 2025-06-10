#pragma once

#include "SDL.h"
#include <math.h>
#include "ECS.hpp"
#include "TextureManager.hpp"

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

struct Point{
    int x = 0;
    int y = 0;
    Point(int x, int y)
    :x(x), y(y) {}

    Point operator *=(int m)
    {
        x*= m;
        y *= m;
        return *this;
    }

    Point operator +=(int add)
    {
        x += add;
        y += add;
        return *this;
    }
};

struct Cooldown{
    double dt = 0;;
    double cooldown;

    void update(double dt)
    {
        this->dt += dt;
    }

    bool execute()
    {
        if(dt >= cooldown) 
        {
            dt = 0;
            return true;
        }
        return false;
    }

    bool canExecute()
    {
        if(dt >= cooldown) 
        {
            return true;
        }
        return false;
    }
};

inline void DrawCircle(SDL_Renderer* renderer, int32_t centreX, int32_t centreY, int32_t radius)
{
    const int32_t diameter = (radius * 2);

    int32_t x = (radius - 1);
    int32_t y = 0;
    int32_t tx = 1;
    int32_t ty = 1;
    int32_t error = (tx - diameter);

    while (x >= y)
    {
        // Each of the following renders an octant of the circle
        SDL_RenderDrawPoint(renderer, centreX + x, centreY - y);
        SDL_RenderDrawPoint(renderer, centreX + x, centreY + y);
        SDL_RenderDrawPoint(renderer, centreX - x, centreY - y);
        SDL_RenderDrawPoint(renderer, centreX - x, centreY + y);
        SDL_RenderDrawPoint(renderer, centreX + y, centreY - x);
        SDL_RenderDrawPoint(renderer, centreX + y, centreY + x);
        SDL_RenderDrawPoint(renderer, centreX - y, centreY - x);
        SDL_RenderDrawPoint(renderer, centreX - y, centreY + x);

        if (error <= 0)
        {
            ++y;
            error += ty;
            ty += 2;
        }

        if (error > 0)
        {
            --x;
            tx += 2;
            error += (tx - diameter);
        }
    }
}
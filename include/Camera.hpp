#pragma once

class Camera
{
private:
    int x, y;
    int w, h;
public:
    Camera(/* args */)
    {
        x = 0; 
        y = 0;
    }
    ~Camera();

    void setPos(int xPos, int yPos)
    {
        x = xPos;
        y = yPos;
    }

    int getX() {return x;}
    int getY() {return y;}
    int getW() {return w;}
    int getH() {return h;}
    void setSize(int w, int h)
    {
        this->w = w;
        this->h = h;
    }
};
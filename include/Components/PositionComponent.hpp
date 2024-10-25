#pragma once

#include "ECS.hpp"

class PositionComponent : public Component
{
private:
    double x, y;
public:
    PositionComponent(){};
    ~PositionComponent(){};

    void init()
    {
        x = 0;
        y = 0;
    }
    void update(double dt){};
    void render(Camera* cam){};

    void setPos(double xPos, double yPos)
    {
        x = xPos;
        y = yPos;
    }

    void setX(double xPos){x = xPos;}
    void setY(double yPos){y = yPos;}

    void moveX(double xMovement){x += xMovement;}
    void moveY(double yMovement){y += yMovement;}

    double getX(){return x;}
    double getY(){return y;}


};
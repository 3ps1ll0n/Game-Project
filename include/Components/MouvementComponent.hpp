#pragma once

#include "ECS.hpp"
#include "PositionComponent.hpp"
#include "StatsComponent.hpp"
#include "HitBoxComponent.hpp"
#include "TileMapComponent.hpp"
#include "SoundEngine.hpp"

#include <iostream>

class MouvementComponent : public Component 
{
private:
    double xVelocity, yVelocity;
    double oldX, oldY;
    Sound* stepSound;

    static const int steps = 6;
public:
    MouvementComponent(){};
    ~MouvementComponent(){};

    void init()
    {
        xVelocity = 0;
        yVelocity = 0;
        stepSound = new Sound(0.2, [&]()
        {
            if((int)xVelocity == 0 && (int)yVelocity == 0) return false;
            return true;
        });
    };
    void update(double dt)
    {
        double smallDt = dt/steps;
        PositionComponent* pos = &entity->getComponent<PositionComponent>();
        TileMapComponent* tileMap = &Game::tileMap->getComponent<TileMapComponent>();
        double entitySpeed = entity->getComponent<StatsComponent>().getSpeed();

        for(int i = 0; i < steps; i++)
        {
            oldX = pos->getX();
            oldY = pos->getY();
            //if(entity->getComponent<StatsComponent>() != nullptr );
            //pos->setPos(pos->getX() + ((xVelocity * smallDt) * entitySpeed), pos->getY() + (yVelocity * smallDt) * entitySpeed);
            pos->moveX(xVelocity * smallDt * entitySpeed);
            entity->getComponent<HitBoxComponent>().update(dt);
            if(tileMap->applyCollider(entity)) pos->setX(oldX);
            
            pos->moveY(yVelocity * smallDt * entitySpeed);
            entity->getComponent<HitBoxComponent>().update(dt);
            if(tileMap->applyCollider(entity)) pos->setY(oldY);

            entity->getComponent<HitBoxComponent>().update(dt);
        }

        stepSound->play("step");
        stepSound->update(dt);
    };
    void render(Camera* cam){};

    void setVelocity(double xVelocity, double yVelocity)
    {
        this->xVelocity = xVelocity;
        this->yVelocity = yVelocity;
    };


    void setXVelocity(double xVelocity){this->xVelocity = xVelocity;}
    void setYVelocity(double yVelocity){this->yVelocity = yVelocity;}
    void resetVelocity()
    {
        xVelocity = 0.0;
        yVelocity = 0.0;
    }

    
};

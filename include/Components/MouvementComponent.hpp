#pragma once

#include "ECS.hpp"
#include "PositionComponent.hpp"
#include "StatsComponent.hpp"
#include "HitBoxComponent.hpp"
#include "TileMapComponent.hpp"
#include "SoundEngine.hpp"
#include "Utilities.hpp"

#include <iostream>

class MouvementComponent : public Component 
{
private:
    //double xVelocity, yVelocity;
    //double oldX, oldY;
    Vector2D velocity;
    Vector2D oldPosition;
    bool isDashing;
    Cooldown canDash;
    Cooldown dashing;
    Sound* stepSound;

    static const int steps = 6;
public:
    MouvementComponent(){};
    ~MouvementComponent(){};

    void init()
    {
        velocity.x = 0;
        velocity.y = 0;
        isDashing = false;
        canDash = {1.0, 0.75};
        dashing = {0, 0.2};
        stepSound = new Sound(0.2, [&]()
        {
            if((int)velocity.x == 0 && (int)velocity.y == 0) return false;
            return true;
        });
    };
    void update(double dt)
    {
        double smallDt = dt/steps;
        PositionComponent* pos = &entity->getComponent<PositionComponent>();
        TileMapComponent* tileMap = &Game::tileMap->getComponent<TileMapComponent>();
        double entitySpeed = entity->getComponent<StatsComponent>().getSpeed();

        velocity.normalize();
        if(!isDashing)velocity *= entitySpeed;
        else velocity *= (entitySpeed * 1.75);

        for(int i = 0; i < steps; i++)
        {
            oldPosition.x = pos->getX();
            oldPosition.y = pos->getY();
            //if(entity->getComponent<StatsComponent>() != nullptr );
            //pos->setPos(pos->getX() + ((xVelocity * smallDt) * entitySpeed), pos->getY() + (yVelocity * smallDt) * entitySpeed);
            pos->moveX(velocity.x * smallDt);
            entity->getComponent<HitBoxComponent>().update(dt);
            if(tileMap->applyCollider(entity)) pos->setX(oldPosition.x);
            
            pos->moveY(velocity.y * smallDt);
            entity->getComponent<HitBoxComponent>().update(dt);
            if(tileMap->applyCollider(entity)) pos->setY(oldPosition.y);

            entity->getComponent<HitBoxComponent>().update(dt);
        }

        stepSound->play("step");
        stepSound->update(dt);
        canDash.update(dt);
        dashing.update(dt);

        if(isDashing && dashing.canExecute()){
            velocity.reset();
            isDashing = false;
        }

    };

    void dash()
    {
        if(canDash.canExecute()){
            isDashing = true;
            dashing.dt = 0;
            velocity.x *= 1.75;
            velocity.y *= 1.75;
        }
    }

    void render(Camera* cam)
    {
        if(Game::debugMode)
        {
            PositionComponent* pos = &entity->getComponent<PositionComponent>();
            //SpriteComponent* sprite = &entity->getComponent<SpriteComponent>();
            velocity.diplay(Game::renderer, pos->getX() + 30 - cam->getX(), pos->getY() + 30 - cam->getY(), 0.05, {255, 0 ,255, 0});
        }
    };

    void setVelocity(double xVelocity, double yVelocity)
    {
        velocity.x = xVelocity;
        velocity.y = yVelocity;
    };


    void setXVelocity(double xVelocity){
        if(isDashing) return;
        velocity.x = xVelocity;
    }
    void setYVelocity(double yVelocity){
        if(isDashing) return;
        velocity.y = yVelocity;
    }
    void resetVelocity() {
        if(isDashing) return;
        velocity.reset();
    }
};

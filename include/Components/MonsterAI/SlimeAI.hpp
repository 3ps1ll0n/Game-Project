#pragma once

#include "ECS.hpp"
#include "Utilities.hpp"
#include "StateManager.hpp"
#include "AnimatorComponent.hpp"
#include "TileMapComponent.hpp"
#include "HitBoxesManager.hpp"

class SlimeAI : public Component
{
private:
    StateManager stateManager;
    Entity* target;
    Entity* tileMap;
    PositionComponent* targetPos;
    PositionComponent* thisPos;
    MouvementComponent* moove;
    Cooldown jumping;
    Cooldown attacking;

    static const int detectionRange = 500;
    static const int attackRange = 50;
public:
    SlimeAI() {}
    ~SlimeAI() {}

    void init() 
    {
        stateManager = StateManager();
        jumping = Cooldown();
        attacking = Cooldown();
        thisPos = &entity->getComponent<PositionComponent>();
        moove = &entity->getComponent<MouvementComponent>();

        jumping.dt = 0;
        attacking.dt = 0;

        jumping.cooldown = 0.5;
        attacking.cooldown = 0.75;

        auto nodeIdle = stateManager.addState([](){return true;});
        auto nodeWander = stateManager.addState([this]()
        {
            if(abs(targetPos->getX() - thisPos->getX()) > detectionRange && abs(targetPos->getY() - thisPos->getY()) > detectionRange) return true;
            return false;
        });
        auto nodeChase = stateManager.addState([this]()
        {
            if(abs(targetPos->getX() - thisPos->getX()) <= detectionRange && abs(targetPos->getY() - thisPos->getY()) <= detectionRange && attacking.canExecute()) return true;
            return false;
        });
        auto nodeAttack = stateManager.addState([this]()
        {
            if(abs(targetPos->getX() - thisPos->getX()) <= attackRange && abs(targetPos->getY() - thisPos->getY()) <= attackRange && jumping.canExecute()) return true;
            return false;
        });

        stateManager.setCurrentState(0);

        nodeIdle->action = [this]()
        {
            moove->resetVelocity();
            return;
        };

        nodeChase->action = [this]()
        {
            auto hb = target->getComponent<HitBoxComponent>().getHitBox(0);
            jumping.execute();
            Vector2D* speed = moove->getVelocity();
            auto pathFinder = &entity->getComponent<PathFinderComponent>();
            auto animator = &entity->getComponent<AnimatorComponent>();
            if(animator->getCurrentFrameIndex() > 2 && animator->getCurrentFrameIndex() <= 6) {
                if(speed->x == 0 && speed->y == 0){
                    auto point = pathFinder->getNextPoint();

                    speed->x = point->x - thisPos->getX();
                    speed->y = point->y - thisPos->getY();

                    speed->normalize();
                    //speed *= entity->getComponent<StatsComponent>().getSpeed() * 0.6;

                    speed->x *= entity->getComponent<StatsComponent>().getSpeed() * 0.6;
                    speed->y *= entity->getComponent<StatsComponent>().getSpeed() * 0.6;
                }
            }
            else
            {
                pathFinder->setTargetPos(new Point(targetPos->getX() + hb->w/2, targetPos->getY() + hb->h/2));
                pathFinder->calculatePath();
                speed->reset();
            }
        };
        
        nodeAttack->action = [this]()
        {
            moove->getVelocity()->reset();
            if(attacking.canExecute() && jumping.canExecute())
            {
                attacking.execute();
                auto hb = target->getComponent<HitBoxComponent>().getHitBox(0);
                HitBoxesManager::createDamageHitBox(thisPos->getX() + hb->w/2, thisPos->getY() + hb->h/2, 35, 10, 2, 0.75);
            }
        };

        nodeWander->action = []()
        {
            return;
        };
    }

    void setTarget(Entity* target)
    {
        this->target = target;
        targetPos = &target->getComponent<PositionComponent>();
    }

    void setTileMap(Entity* tileMap)
    {
        this->tileMap = tileMap;
    }

    void render(SDL_Renderer* renderer, Camera* cam) {}
    
    void update(double dt) 
    {
        moove->getOldPos()->x = thisPos->getX();
        moove->getOldPos()->y = thisPos->getY();

        HitBox* hb = target->getComponent<HitBoxComponent>().getHitBox(0);

        stateManager.update();
        jumping.update(dt);
        attacking.update(dt);

        //thisPos->setX(thisPos->getX() + moove->getVelocity()->x * dt);
        entity->getComponent<HitBoxComponent>().update(dt);

        //Correct x Pos in case of a collision
        if(tileMap->getComponent<TileMapComponent>().applyCollider(entity)) thisPos->setX(moove->getOldPos()->x);

        //thisPos->setY(thisPos->getY() + moove->getVelocity()->y * dt);
        entity->getComponent<HitBoxComponent>().update(dt);
        //Correct y Pos in case of a collision
        if(tileMap->getComponent<TileMapComponent>().applyCollider(entity)) thisPos->setY(moove->getOldPos()->y);

        entity->getComponent<HitBoxComponent>().update(dt);
    }

    bool getIsJumping()
    {
        return !jumping.canExecute();
    }

    bool getIsAttacking()
    {
        return !attacking.canExecute();
    }
};
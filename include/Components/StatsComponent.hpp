#pragma once

#include "ECS.hpp"
#include "Utilities.hpp"
#include "SoundEngine.hpp"

class StatsComponent : public Component
{
private:
    double speed, maxHP, hp, def, atk;
    Cooldown damageable;
public:
    StatsComponent() {};
    ~StatsComponent() {};

    void init() 
    {
        speed = 700;
        maxHP = 10;
        hp = maxHP;
        def = 0;
        atk = 1;

        damageable = Cooldown();
        damageable.cooldown = 1.0;
    };
    void update(double dt) 
    {
        damageable.update(dt);
    }

    void render(SDL_Renderer* renderer, Camera* cam) {};

    double getSpeed(){return speed;}
    double getMaxHP(){return maxHP;}
    double getDef(){return def;}
    double getAtk(){return atk;}

    bool hasBeenDamaged() {return !damageable.canExecute();}
    bool canBeDamaged() {return damageable.canExecute();}

    void dealDamage(double damage)
    {
        if(canBeDamaged())
        {
            damageable.execute();
            hp -= (100*damage)/(def + 100);
            //SDL_Delay(500);
            playSound("hit");
        }

    }
};
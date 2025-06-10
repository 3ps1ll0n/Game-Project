#pragma once

#include "ECS.hpp"


class CreateMonster
{
public:
    static Entity* Slime(Manager* entityManager, Entity* tileMap, int xPos, int yPos);
};

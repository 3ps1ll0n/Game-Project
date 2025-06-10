#pragma once

#include <bitset>
#include <array>
#include <vector>
#include <algorithm>
#include <memory>

#include "Log.hpp"
#include "Camera.hpp"
#include "SDL.h"

using ComponentID = int;

class Component;
class Entity;

inline  ComponentID getComponentID(){
    static int id = 0;
    return id++;
}

template <typename T> ComponentID getComponentTypeID(){
    static ComponentID id = getComponentID();
    return id;
}

const std::size_t maxComponents = 32;
using ComponentBitSet = std::bitset<maxComponents>;
using ComponentArray = std::array<Component*, maxComponents>;

class Component
{
public:
    Entity* entity;

    virtual void init() {};
    virtual void update(double dt) {};
    virtual void render(SDL_Renderer* renderer, Camera* cam) {};

    virtual ~Component() {};
};

class Entity
{
private:
    bool active;
    std::vector<std::unique_ptr<Component>> components = {};

    ComponentBitSet componentsBitSet;
    ComponentArray componentArray;
public:
    void update(double dt)
    {
        for(auto& c : components) c->update(dt);
    };

    void render(SDL_Renderer* renderer, Camera* cam)
    {
       for(auto& c : components) c->render(renderer, cam); 
    };

    bool isActive() {return active;}

    void destroy(){active = false;}

    template<typename T> bool hasComponent()
    {
        return componentsBitSet[getComponentTypeID<T>()];
    }

    template<typename T, typename... TArgs> T& addComponent(TArgs&&... mArgs)
    {
        T* c = new T(std::forward<TArgs>(mArgs)...);
        c->entity = this;
        std::unique_ptr<Component> uPtr { c };
        components.emplace_back(std::move(uPtr));

        componentArray[getComponentTypeID<T>()] = c;
        componentsBitSet[getComponentTypeID<T>()] = true;

        c->init();
        return *c;
    }

    template<typename T> T& getComponent()
    {
        auto ptr(componentArray[getComponentTypeID<T>()]);
        return *static_cast<T*>(ptr);
    }
};

class Manager
{
private:
    std::vector<std::unique_ptr<Entity>> entities = {};
public:        
    void update(double dt)
    {
        for(auto& e : entities) e->update(dt);
    }
    void render(SDL_Renderer* renderer, Camera* cam)
    {
        for(auto& e : entities) e->render(renderer, cam);
    }

    void refresh()
    {
        entities.erase(std::remove_if(std::begin(entities), std::end(entities),
        [](const std::unique_ptr<Entity> &mEntity)
        {
            return !mEntity->isActive();
        }),
        std::end(entities));
    }
    
    Entity& addEntity()
    {
        Entity* e = new Entity();
        std::unique_ptr<Entity> uPtr{ e };
        entities.emplace_back(std::move(uPtr));
        return *e;
    }
};
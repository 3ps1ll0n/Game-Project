#include "Game.hpp"
#include "ECS.hpp"
#include "Components.hpp"
#include "SoundEngine.hpp"
#include "Monster.hpp"

#include <iostream>

SDL_Renderer* Game::renderer = nullptr;
bool Game::debugMode = false;
Entity* Game::tileMap = nullptr;
Manager* Game::entityManager = new Manager();

Game::Game(){}

Game::~Game(){}

void Game::init(const char* title, int xpos, int ypos, int width, int height)
{
    Log::clearFile("debugLog.txt");
    Log::writeLog("debugLog.txt", "======================{LOG}======================");
    if(SDL_Init(SDL_INIT_EVERYTHING) == 0)
    {
        window = SDL_CreateWindow(title, xpos, ypos, width, height, SDL_WINDOW_RESIZABLE | SDL_WINDOW_MAXIMIZED);
        if(!window) Log::writeLog("debugLog.txt", "ERROR WHILE CREATING THE WINDOW");
        windowWidth = width;
        windowHeight = height;

        renderer = SDL_CreateRenderer(window, -1, 0);
        if(!renderer) Log::writeLog("debugLog.txt", "ERROR windowWHILE CREATING THE REMDERER");

        initMixer();

        player = new Entity();
        entityManager = new Manager();
        cam = new Camera();
        cam->setSize(width, height);
        running = true;

        player->addComponent<MouvementComponent>();
        player->addComponent<PositionComponent>();
        player->addComponent<SpriteRendererComponent>().setSprite("assets/chevalier_002.png");
        player->addComponent<StatsComponent>();
        player->addComponent<AnimatorComponent>().addSpriteSheet("assets/CharAni-Sheet4.png", 
        {"Idle", "Walk"},
        {4, 8},
        {0.100, 0.100},
        32,
        32
        );

        player->getComponent<AnimatorComponent>().addConditions("Idle", [](){
        return true;
        });
        player->getComponent<AnimatorComponent>().addConditions("Walk", [&](){
            auto moveC = &player->getComponent<MouvementComponent>();
            if(moveC->getVelocity().x != 0 || moveC->getVelocity().y != 0) return true;
            return false;
        });

        player->getComponent<AnimatorComponent>().setCurrentSpriteSheet("Walk");
        
        auto& pHB = player->addComponent<HitBoxComponent>();
        pHB.addHitBoxType(BODY); //Add a kind of hitbox in the player
        pHB.addHitBox(BODY, new HitBox(0, 0, 60, 60));


        tileMap = &entityManager->addEntity();
        auto& tmc = tileMap->addComponent<TileMapComponent>();
        tmc.Generate(500, 50);

        player->getComponent<PositionComponent>().setPos(tmc.getSize()/2 * tmc.getTileSize(), tmc.getSize()/2 * tmc.getTileSize());
        
        CreateMonster::Slime(tmc.getSize()/2 * tmc.getTileSize(), tmc.getSize()/2 * tmc.getTileSize());

        loadSound("assets/Sounds/Effect");
        loadMusic("assets/Sounds/Music/Battlefield Symphony.mp3");
        playMusic();

        std::cout << sizeof(Entity) << std::endl;
    }
    else
    {
        Log::writeLog("debugLog.txt", "CANNOT INIT SDL SUBSYSTEM");
        running = false;
    } 
}

void Game::eventHandler()
{
    const Uint8* keyboard = SDL_GetKeyboardState(NULL);
    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT:
            running = false;
            break;
        default:
            break;
        }
    }

    double speed = player->getComponent<StatsComponent>().getSpeed();

    if(keyboard[SDL_SCANCODE_D]){
        player->getComponent<MouvementComponent>().setXVelocity(speed);
    }   
    else if(keyboard[SDL_SCANCODE_A]){
        player->getComponent<MouvementComponent>().setXVelocity(-speed);
    } else {
        player->getComponent<MouvementComponent>().setXVelocity(0.0);
    }

    if(keyboard[SDL_SCANCODE_W]){
        player->getComponent<MouvementComponent>().setYVelocity(-speed);
    }
    else if(keyboard[SDL_SCANCODE_S]){
        player->getComponent<MouvementComponent>().setYVelocity(speed);
    } else {
        player->getComponent<MouvementComponent>().setYVelocity(0.0);
    }

    if(keyboard[SDL_SCANCODE_SPACE]){
        player->getComponent<MouvementComponent>().dash();
    }
    
}

void Game::update(double dt)
{
    SDL_GetWindowSize(window, &windowWidth, &windowHeight);
    player->update(dt);
    cam->setPos(player->getComponent<PositionComponent>().getX() - (windowWidth/2), player->getComponent<PositionComponent>().getY() - (windowHeight/2));
    entityManager->update(dt);
    //player->getComponent<MouvementComponent>().resetVelocity();
    //playSound();
}

void Game::render()
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    entityManager->render(cam);
    player->render(cam);
    SDL_RenderPresent(renderer);
}

void Game::clear()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}

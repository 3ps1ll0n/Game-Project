#pragma once

#include <SDL_mixer.h>
#include <map>
#include <string>
#include <iostream>
#include <filesystem>
#include <functional>

int loadMusic(const char* filename);
int loadSound(const char* path);

void setVolume(int v);

void playMusic();
void playSound(std::string id, int channel = -1);

int initMixer();
void quitMixer();
void togglePlay();

class Sound
{
private:
    double cooldown;
    double dt;
    std::function<bool()> condition;
public:
    Sound(double cooldown, std::function<bool()> condition = NULL)
    :cooldown(cooldown), condition(condition){}

    void update(double dt) {this->dt += dt;}
    void play(std::string sound, int channel = -1);
};

/*class SoundEngine
{
private:
    std::map<std::string, Sound*> sounds;
public:
    SoundEngine();
    void update(double dt)
    {
        for(auto& it = sounds.begin(); it != sounds.end(); it++)
        {
            it->second->update(dt);
            it->second->play();
        }
    }
};*/
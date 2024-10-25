#pragma once

#include <SDL_mixer.h>
#include <map>
#include <string>
#include <iostream>
#include <filesystem>
#include <functional>

std::map<std::string, Mix_Chunk*> soundEffects = {};
Mix_Music* music;

int volume;

int loadMusic(const char* filename)
{
    Mix_Music* m = NULL;
    m = Mix_LoadMUS(filename);
    if(m == NULL){
        std::cout << "Failled to load music" << std::endl;
        return -1;
    }

    music = m;
    return 0;
}
int loadSound(const char* path)
{
    for(const auto & entry : std::filesystem::directory_iterator(path)) 
    {
        
        std::string path = entry.path().string();
        std::string file = path.substr(path.find_last_of("/\\") + 1);
        

        if(file.substr(file.find_last_of(".") + 1) == "wav") 
        {
            soundEffects.insert(std::pair(file.substr(0, file.find(".")), Mix_LoadWAV(entry.path().string().c_str())));
        }
    }
    return 0;
}

void setVolume(int v)
{
    volume = (MIX_MAX_VOLUME * v)/100;
}

void playMusic()
{
    if(Mix_PlayingMusic() == 0)
    {
        Mix_Volume(1, volume);
        if(Mix_PlayMusic(music, -1) == -1) std::cout << "ERROR PLAYING MUSIC : " << Mix_GetError() << std::endl;
    }
}
void playSound(std::string id, int channel = -1)
{
    auto effect = soundEffects.find(id);
    if(effect == soundEffects.end()) return;

    //if(Mix_Playing(channel) != 0) return;
    if(Mix_PlayChannel(channel, effect->second, 0) == -1){
        std::cout << "Can't play sound" << std::endl;
    }
}

int initMixer()
{
    Mix_Init(MIX_INIT_MP3);
    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        std::cout << "SDL_Mixer failled to init" << std::endl;
        return -1;
    }
    setVolume(80);
    return 0;
}

void quitMixer()
{
    music = NULL;

    Mix_Quit();
}

void togglePlay()
{
    if(Mix_PausedMusic() == 1) Mix_ResumeMusic();
    else Mix_PauseMusic();
}

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
    void play(std::string sound, int channel = -1)
    {
        if(dt < cooldown || (condition != NULL && !condition())) return;
        
        dt = 0.0;
        playSound(sound, channel);
    }
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
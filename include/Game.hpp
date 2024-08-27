#include <SDL.h>

class Game
{
private:
    SDL_Window* window;
    SDL_Renderer* renderer;

    bool running;

public:
    Game(/* args */);
    ~Game();

    void init(const char* title, int xpos, int ypos, int width, int height);
    void eventHandler();
    void update(double dt);
    void render();
    void clear();
    bool isRunning(){return running;};
};


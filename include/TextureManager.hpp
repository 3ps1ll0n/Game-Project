#include <SDL.h>

class TextureManager
{
public:
    static SDL_Texture* loadTexture(const char* filename);
};

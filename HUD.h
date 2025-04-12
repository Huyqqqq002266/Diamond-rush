#ifndef HUD_H
#define HUD_H

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string>

class HUD {
public:
    HUD(SDL_Renderer* renderer);
    ~HUD();

    void SetDiamonds(int count);
    void SetLives(int count);
    void SetKeys(int count);

    void Render(SDL_Renderer* renderer);

private:
    int diamondCount;
    int lives;
    int key;

    SDL_Texture* diamondTexture;
    SDL_Texture* heartTexture;
    SDL_Texture* keyTexture;
    TTF_Font* font;

    SDL_Texture* CreateTextTexture(const std::string& text, SDL_Color color, SDL_Renderer* renderer);
};

#endif

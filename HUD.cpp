#include "HUD.h"
#include <iostream>

HUD::HUD(SDL_Renderer* renderer) : diamondCount(0), lives(3), key(0) {
    diamondTexture = IMG_LoadTexture(renderer, "image/diamond-removebg-preview.png");
    if (!diamondTexture) {
        std::cerr << "Failed to load diamond image: " << IMG_GetError() << std::endl;
    }

    heartTexture = IMG_LoadTexture(renderer, "image/heart.png");
    if (!heartTexture) {
        std::cerr << "Failed to load heart image: " << IMG_GetError() << std::endl;
    }

    keyTexture = IMG_LoadTexture(renderer, "image/key.png");
    if (!keyTexture) {
        std::cerr << "Failed to load key image: " << IMG_GetError() << std::endl;
    }

    if (TTF_Init() == -1) {
        std::cerr << "TTF_Init failed: " << TTF_GetError() << std::endl;
    }

    font = TTF_OpenFont("font/Arial.ttf", 24);
    if (!font) {
        std::cerr << "Failed to load font: " << TTF_GetError() << std::endl;
    }
}

HUD::~HUD() {
    if (diamondTexture) SDL_DestroyTexture(diamondTexture);
    if (heartTexture) SDL_DestroyTexture(heartTexture);
    if (keyTexture) SDL_DestroyTexture(keyTexture);
    if (font) TTF_CloseFont(font);
}

void HUD::SetDiamonds(int count) {
    diamondCount = count;
}

void HUD::SetLives(int count) {
    lives = count;
}

void HUD::SetKeys(int count) {
    key = count;
}

SDL_Texture* HUD::CreateTextTexture(const std::string& text, SDL_Color color, SDL_Renderer* renderer) {
    SDL_Surface* surface = TTF_RenderText_Blended(font, text.c_str(), color);
    if (!surface) return nullptr;

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return texture;
}

void HUD::Render(SDL_Renderer* renderer) {
    SDL_Color black = { 0, 0, 0 };

    int offsetX = 10;
    int spacing = 100;
    int iconSize = 32;
    int baseY = 10;

    SDL_Rect diamondRect = { offsetX, baseY, iconSize, iconSize };
    SDL_RenderCopy(renderer, diamondTexture, NULL, &diamondRect);

    std::string diamondText = "x" + std::to_string(diamondCount);
    SDL_Texture* diamondTextTexture = CreateTextTexture(diamondText, black, renderer);
    if (diamondTextTexture) {
        int w, h;
        SDL_QueryTexture(diamondTextTexture, NULL, NULL, &w, &h);
        SDL_Rect textRect = { offsetX + iconSize + 5, baseY + 5, w, h };
        SDL_RenderCopy(renderer, diamondTextTexture, NULL, &textRect);
        SDL_DestroyTexture(diamondTextTexture);
    }

    SDL_Rect heartRect = { offsetX + spacing, baseY, iconSize, iconSize };
    SDL_RenderCopy(renderer, heartTexture, NULL, &heartRect);

    std::string livesText = "x" + std::to_string(lives);
    SDL_Texture* livesTextTexture = CreateTextTexture(livesText, black, renderer);
    if (livesTextTexture) {
        int w, h;
        SDL_QueryTexture(livesTextTexture, NULL, NULL, &w, &h);
        SDL_Rect textRect = { offsetX + spacing + iconSize + 5, baseY + 5, w, h };
        SDL_RenderCopy(renderer, livesTextTexture, NULL, &textRect);
        SDL_DestroyTexture(livesTextTexture);
    }

    SDL_Rect keyRect = { offsetX + spacing * 2, baseY, iconSize, iconSize };
    SDL_RenderCopy(renderer, keyTexture, NULL, &keyRect);

    std::string keyText = "x" + std::to_string(key);
    SDL_Texture* keyTextTexture = CreateTextTexture(keyText, black, renderer);
    if (keyTextTexture) {
        int w, h;
        SDL_QueryTexture(keyTextTexture, NULL, NULL, &w, &h);
        SDL_Rect textRect = { offsetX + spacing * 2 + iconSize + 5, baseY + 5, w, h };
        SDL_RenderCopy(renderer, keyTextTexture, NULL, &textRect);
        SDL_DestroyTexture(keyTextTexture);
    }
}

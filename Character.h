#ifndef CHARACTER_H
#define CHARACTER_H

#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include "CommonFunc.h"

class Character {
public:
    Character(int x, int y, SDL_Renderer* renderer);
    ~Character();

    void HandleEvent(SDL_Event& event, int tileMap[MAP_HEIGHT][MAP_WIDTH]);
    void Update(int tileMap[MAP_HEIGHT][MAP_WIDTH]);
    void Render(SDL_Renderer* renderer);
    void Reset();
    bool IsDead() const { return dead; }

private:
    void LoadSprite(const std::string& path);
    bool CheckCollision(int newX, int newY, int tileMap[MAP_HEIGHT][MAP_WIDTH]);
    void Die();

    SDL_Renderer* renderer;
    SDL_Texture* texture = nullptr;

    int x, y;
    int startX, startY;

    Direction direction;
    bool underRock = false;
    bool dead = false;
    Uint32 timeUnderRock = 0;
};

#endif // CHARACTER_H
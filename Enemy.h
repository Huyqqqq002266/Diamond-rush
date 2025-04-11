#ifndef ENEMY_H
#define ENEMY_H

#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <vector>
#include "CommonFunc.h"

enum Direction;

class Enemy {
public:
    Enemy(int x, int y, Direction dir, SDL_Renderer* renderer);
    ~Enemy();

    void Update();
    void Render(SDL_Renderer* renderer);
    void Reset();

private:
    void LoadSprite(const std::string& path);
    bool CanMoveTo(int tx, int ty);

    SDL_Renderer* renderer = nullptr;
    SDL_Texture* texture = nullptr;

    int x, y;
    int tileX, tileY;
    int startX, startY;

    Direction direction;
    bool forward = true;
    int moveDistance = 0;
    const int maxMoveDistance = 3;
};

extern std::vector<Enemy> enemies;
void LoadEnemies(SDL_Renderer* renderer);
#endif
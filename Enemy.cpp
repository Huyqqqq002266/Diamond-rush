#include "Enemy.h"
#include "map.h"
#include <iostream>
#include <cstring>

int enemyMap[MAP_HEIGHT][MAP_WIDTH] = { 0 };
std::vector<Enemy> enemies;

Enemy::Enemy(int x, int y, Direction dir, SDL_Renderer* renderer)
    : tileX(x), tileY(y), startX(x), startY(y), direction(dir), renderer(renderer) {
    this->x = x * TILE_SIZE;
    this->y = y * TILE_SIZE;
}

Enemy::~Enemy() {
    if (texture) {
        SDL_DestroyTexture(texture);
        texture = nullptr;
    }
}

void Enemy::LoadSprite(const std::string& path) {
    SDL_Surface* surface = IMG_Load(path.c_str());
    if (!surface) {
        std::cerr << "IMG_Load error: " << IMG_GetError() << std::endl;
        return;
    }

    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    if (!texture) {
        std::cerr << "SDL_CreateTexture error: " << SDL_GetError() << std::endl;
    }
}

bool Enemy::CanMoveTo(int tx, int ty) {
    if (tx < 0 || tx >= MAP_WIDTH || ty < 0 || ty >= MAP_HEIGHT) return false;

    int tileType = tileMap[ty][tx];
    return (tileType != 1 && tileType != 2 && tileType != 3 && tileType != 4);
}


void Enemy::Update() {
    int dx = 0, dy = 0;
    if (direction == HORIZONTAL) dx = forward ? 1 : -1;
    else dy = forward ? 1 : -1;

    int nextTileX = tileX + dx;
    int nextTileY = tileY + dy;

    if (CanMoveTo(nextTileX, nextTileY)) {
        tileMap[tileY][tileX] = 0;
        tileX = nextTileX;
        tileY = nextTileY;
        tileMap[tileY][tileX] = 5;

        x = tileX * TILE_SIZE;
        y = tileY * TILE_SIZE;

        moveDistance++;
        if (moveDistance >= maxMoveDistance) {
            forward = !forward;
            moveDistance = 0;
        }
    }
    else {
        forward = !forward;
        moveDistance = 0;
    }
}

void Enemy::Render(SDL_Renderer* renderer) {
    SDL_Rect renderQuad = { x, y, TILE_SIZE, TILE_SIZE };
    SDL_RenderCopy(renderer, texture, NULL, &renderQuad);
}

void Enemy::Reset() {
    tileX = startX;
    tileY = startY;
    x = tileX * TILE_SIZE;
    y = tileY * TILE_SIZE;
    forward = true;
    moveDistance = 0;
}

void LoadEnemies(SDL_Renderer* renderer) {
    enemies.clear();
    memset(enemyMap, 0, sizeof(enemyMap));
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            if (tileMap[y][x] == 5) {
                Direction dir;

                if (y == 4 && x == 5) {
                    dir = HORIZONTAL;
                }
                else if ((y == 4 && x == 2) || (y == 17 && (x == 15 || x == 17 || x == 19))) {
                    dir = VERTICAL;
                }
                else if ((x == 3 && y == 18) || (x == 10 && y == 19)) {
                    dir = VERTICAL;
                }
                else if (x == 11 && y == 18) {
                    dir = HORIZONTAL;
                }
                else if ((x == 10 && y == 16) || (x==6 && y == 8)){
                    dir = VERTICAL;
                }
                else if (x == 16 && y == 17) {
                    dir = HORIZONTAL;
                }
                else if (x == 16 && y == 18) {
                    dir = HORIZONTAL;
                }
                else {
                    continue;
                }
                enemies.emplace_back(x, y, dir, renderer);
                enemyMap[y][x] = 1;
            }
        }
    }
}

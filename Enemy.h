#ifndef ENEMY_H
#define ENEMY_H

#include <SDL.h>
#include <SDL_image.h>
#include "CommonFunc.h"

#define TILE_SIZE 32 // Kích thước 1 ô
extern int tileMap[MAP_HEIGHT][MAP_WIDTH]; // Sử dụng bản đồ toàn cục

enum Direction { HORIZONTAL, VERTICAL };


extern int enemyMap[MAP_HEIGHT][MAP_WIDTH];

class Enemy {
public:
    Enemy(int x, int y, Direction dir, SDL_Renderer* renderer);
    ~Enemy();

    void Update();
    void Render(SDL_Renderer* renderer);

private:
    int gridX, gridY;  // Tọa độ ô trong lưới
    int pixelX, pixelY; // Tọa độ theo pixel
    int dx, dy;         // Hướng di chuyển
    SDL_Texture* texture;
    const int SPEED = 22;
};

#endif

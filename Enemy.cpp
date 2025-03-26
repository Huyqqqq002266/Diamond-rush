#include "Enemy.h"
#include "Map.h"
#include <stdio.h>

Enemy::Enemy(int x, int y, Direction dir, SDL_Renderer* renderer) {
    gridX = x;
    gridY = y;
    pixelX = x * TILE_SIZE;
    pixelY = y * TILE_SIZE;

    // Xác định hướng ban đầu
    if (dir == HORIZONTAL) {
        dx = 1; dy = 0;
    }
    else {
        dx = 0; dy = 1;
    }

    // Load ảnh rắn
    texture = IMG_LoadTexture(renderer, "image/enemy.png");
    if (texture == NULL) {
        printf("Failed to load enemy texture! SDL Error: %s\n", SDL_GetError());
    }
}

Enemy::~Enemy() {
    if (texture != nullptr) {
        SDL_DestroyTexture(texture);
        texture = nullptr;
    }
}

void Enemy::Update() {
    int nextX = gridX + dx;
    int nextY = gridY + dy;

    // Nếu gặp vật cản hoặc enemy khác, quay đầu
    if (tileMap[nextY][nextX] != 0 || enemyMap[nextY][nextX] == 1) {
        dx = -dx;
        dy = -dy;
        return;
    }

    // Xóa vị trí cũ trên enemyMap và tileMap
    enemyMap[gridY][gridX] = 0;
    tileMap[gridY][gridX] = 0;

    // Cập nhật vị trí mới
    gridX = nextX;
    gridY = nextY;

    // Đánh dấu vị trí mới trên enemyMap và tileMap
    enemyMap[gridY][gridX] = 1;
    tileMap[gridY][gridX] = 5;

    // Cập nhật vị trí pixel
    pixelX = gridX * TILE_SIZE;
    pixelY = gridY * TILE_SIZE;
}

void Enemy::Render(SDL_Renderer* renderer) {
    SDL_Rect dstRect = { pixelX, pixelY, TILE_SIZE, TILE_SIZE };
    SDL_RenderCopy(renderer, texture, NULL, &dstRect);
}

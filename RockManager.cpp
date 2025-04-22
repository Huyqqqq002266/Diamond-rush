#include "RockManager.h"

void RockManager::Update() {
    for (int y = MAP_HEIGHT - 2; y >= 0; --y) {
        for (int x = 0; x < MAP_WIDTH; ++x) {
            if (tileMap[y][x] == 2) {
                if (tileMap[y + 1][x] == 0) {
                    tileMap[y][x] = 0;
                    tileMap[y + 1][x] = 2;
                }
            }
        }
    }
}

#include "Map.h"
#include <stdio.h>
#include <cstring>

Map::Map() {
    for (int i = 0; i < 9; i++) {
        tileTextures[i] = nullptr;
    }
}

Map::~Map() {
    Free();
}

int tileMap[MAP_HEIGHT][MAP_WIDTH] = {
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,1,1,4,3,4,4,3,3,1,1,1,1,1,2,0,0,0,6,0,0,0,2,1,1,1},
    {1,1,4,3,2,3,1,3,3,1,1,1,1,1,4,4,0,0,1,0,0,4,4,1,1,1},
    {1,1,5,3,4,5,0,0,3,3,1,1,1,1,1,4,4,0,0,0,0,4,4,1,1,1},
    {1,1,0,3,1,3,2,4,2,3,1,1,1,1,1,1,4,3,0,3,4,4,1,1,1,1},
    {1,1,0,2,2,2,3,1,3,2,3,2,0,3,3,1,1,4,0,4,1,1,1,1,1,1},
    {1,4,0,2,3,3,3,3,3,3,1,1,0,1,3,1,1,1,0,1,1,1,1,1,1,1},
    {1,2,1,3,3,1,1,1,1,1,1,1,1,1,3,1,1,1,0,1,1,1,1,1,1,1},
    {1,3,3,3,3,3,3,3,3,3,7,0,2,2,3,0,7,0,0,0,0,0,8,0,0,0},
    {1,1,1,3,3,3,3,1,1,1,1,1,2,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,3,0,0,0,3,2,3,0,0,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,3,0,0,0,0,2,3,0,0,0,1,1,1,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,0,2,2,3,0,0,0,0,1,1,1},
    {1,1,1,1,1,1,2,2,2,2,2,0,1,1,1,1,1,1,1,1,1,0,0,1,1,1},
    {1,1,1,1,1,3,4,4,4,4,4,3,1,1,1,1,1,1,1,1,1,0,0,0,1,1},
    {1,1,1,1,1,3,3,3,3,3,3,3,1,1,1,1,1,1,1,1,1,0,0,0,1,1},
    {1,1,1,1,1,3,3,3,3,3,3,3,1,0,0,5,0,5,0,5,1,0,0,0,1,1},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1},
    {1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};
int originalMap[MAP_HEIGHT][MAP_WIDTH];


bool Map::LoadTiles(SDL_Renderer* renderer) {
    const char* filePaths[9] = {
        "image/ground.png", "image/wall.png", "image/rock.png", "image/leaves.png",
        "image/diamond.png", "image/enemy.png", "image/treasure.png", "image/entrance.png", "image/exit.png"
    };

    for (int i = 0; i < 9; i++) {
        tileTextures[i] = IMG_LoadTexture(renderer, filePaths[i]);
        if (tileTextures[i] == NULL) {
            printf("Failed to load %s! SDL Error: %s\n", filePaths[i], SDL_GetError());
            return false;
        }
    }
    return true;
}

void Map::DrawMap(SDL_Renderer* renderer) {
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            int tileType = tileMap[y][x];
            if (tileTextures[tileType] != NULL) {
                SDL_Rect dstRect = { x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE };
                SDL_RenderCopy(renderer, tileTextures[tileType], NULL, &dstRect);
            }
        }
    }
}
void Map::SaveOriginalMap() {
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            originalMap[y][x] = tileMap[y][x];
        }
    }
}


void Map::ResetMap() {
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            tileMap[y][x] = originalMap[y][x];
        }
    }
}

void Map::Free() {
    for (int i = 0; i < 9; i++) {
        if (tileTextures[i] != NULL) {
            SDL_DestroyTexture(tileTextures[i]);
            tileTextures[i] = NULL;
        }
    }
}

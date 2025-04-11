#pragma once
#ifndef MAP_H
#define MAP_H

#include "CommonFunc.h"

extern int tileMap[MAP_HEIGHT][MAP_WIDTH];
extern int originalMap[MAP_HEIGHT][MAP_WIDTH];

class Map {
public:
    Map();
    ~Map();
    bool LoadTiles(SDL_Renderer* renderer);
    void DrawMap(SDL_Renderer* renderer);
    void SaveOriginalMap();
    void ResetMap();
    void Free();

private:
    SDL_Texture* tileTextures[9];
};

#endif

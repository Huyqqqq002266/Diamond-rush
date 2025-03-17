#ifndef TILEMAP_H
#define TILEMAP_H

#include <vector>
#include <string>
#include <SDL.h>
#include "tinyxml2.h"

class TileMap {
public:
    TileMap();
    ~TileMap();

    bool LoadFromFile(const std::string& filePath, SDL_Renderer* renderer);
    void Render(SDL_Renderer* renderer);

private:
    int mapWidth, mapHeight;   // Kích thước bản đồ (số ô)
    int tileWidth, tileHeight; // Kích thước mỗi tile
    std::vector<int> tileData; // Lưu ID của từng tile

    SDL_Texture* tileSetTexture;
};

#endif

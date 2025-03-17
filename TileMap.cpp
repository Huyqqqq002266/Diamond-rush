#include "TileMap.h"
#include <SDL_image.h>
#include <iostream>

TileMap::TileMap() : tileSetTexture(nullptr), mapWidth(0), mapHeight(0), tileWidth(0), tileHeight(0) {}

TileMap::~TileMap() {
    if (tileSetTexture) {
        SDL_DestroyTexture(tileSetTexture);
    }
}

bool TileMap::LoadFromFile(const std::string& filePath, SDL_Renderer* renderer) {
    tinyxml2::XMLDocument doc;
    if (doc.LoadFile(filePath.c_str()) != tinyxml2::XML_SUCCESS) {
        std::cerr << "Khong the doc file: " << filePath << std::endl;
        return false;
    }

    tinyxml2::XMLElement* mapElement = doc.FirstChildElement("map");
    if (!mapElement) return false;

    // Lấy kích thước bản đồ
    mapElement->QueryIntAttribute("width", &mapWidth);
    mapElement->QueryIntAttribute("height", &mapHeight);
    mapElement->QueryIntAttribute("tilewidth", &tileWidth);
    mapElement->QueryIntAttribute("tileheight", &tileHeight);

    // Lấy tileset (chứa hình ảnh)
    tinyxml2::XMLElement* tilesetElement = mapElement->FirstChildElement("tileset");
    if (!tilesetElement) return false;

    tinyxml2::XMLElement* imageElement = tilesetElement->FirstChildElement("image");
    if (!imageElement) return false;

    const char* imageSource = imageElement->Attribute("source");
    if (!imageSource) return false;

    std::string tileSetPath = "assets/" + std::string(imageSource);
    SDL_Surface* surface = IMG_Load(tileSetPath.c_str());
    if (!surface) return false;

    tileSetTexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    // Đọc dữ liệu map từ layer
    tinyxml2::XMLElement* layerElement = mapElement->FirstChildElement("layer");
    if (!layerElement) return false;

    tinyxml2::XMLElement* dataElement = layerElement->FirstChildElement("data");
    if (!dataElement) return false;

    tinyxml2::XMLElement* tileElement = dataElement->FirstChildElement("tile");
    while (tileElement) {
        int gid;
        tileElement->QueryIntAttribute("gid", &gid);
        tileData.push_back(gid - 1); // GID trong Tiled bắt đầu từ 1
        tileElement = tileElement->NextSiblingElement("tile");
    }

    return true;
}

void TileMap::Render(SDL_Renderer* renderer) {
    if (!tileSetTexture) return;

    for (int y = 0; y < mapHeight; ++y) {
        for (int x = 0; x < mapWidth; ++x) {
            int tileID = tileData[y * mapWidth + x];
            if (tileID < 0) continue;

            SDL_Rect srcRect = { (tileID % 10) * tileWidth, (tileID / 10) * tileHeight, tileWidth, tileHeight };
            SDL_Rect dstRect = { x * tileWidth, y * tileHeight, tileWidth, tileHeight };

            SDL_RenderCopy(renderer, tileSetTexture, &srcRect, &dstRect);
        }
    }
}

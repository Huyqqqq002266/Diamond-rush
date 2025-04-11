#include "Character.h"
#include <iostream>
#include "map.h"

Character::Character(int x, int y, SDL_Renderer* renderer)
    : startX(x), startY(y), renderer(renderer), direction(RIGHT) {

    this->x = x * TILE_SIZE;
    this->y = y * TILE_SIZE;
    tileMap[y][x] = 1;
    LoadSprite("character/character_right.png");
}

Character::~Character() {
    if (texture) {
        SDL_DestroyTexture(texture);
        texture = nullptr;
    }
}

void Character::LoadSprite(const std::string& path) {
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if (!loadedSurface) {
        std::cerr << "IMG_Load Error: " << IMG_GetError() << std::endl;
        return;
    }

    if (texture) SDL_DestroyTexture(texture);
    texture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
    SDL_FreeSurface(loadedSurface);

    if (!texture) {
        std::cerr << "SDL_CreateTextureFromSurface Error: " << SDL_GetError() << std::endl;
    }
}

bool Character::CheckCollision(int newX, int newY, int tileMap[MAP_HEIGHT][MAP_WIDTH]) {
    int tileX = newX / TILE_SIZE;
    int tileY = newY / TILE_SIZE;

    if (tileX < 0 || tileX >= MAP_WIDTH || tileY < 0 || tileY >= MAP_HEIGHT)
        return true;

    int tileType = tileMap[tileY][tileX];
    return (tileType == 1 || tileType == 2 || tileType == 5);
}

void Character::HandleEvent(SDL_Event& event, int tileMap[MAP_HEIGHT][MAP_WIDTH]) {
    if (event.type != SDL_KEYDOWN) return;

    int newX = x, newY = y;
    std::string spritePath;

    switch (event.key.keysym.sym) {
    case SDLK_LEFT:
        newX -= TILE_SIZE;
        direction = LEFT;
        spritePath = "character/character_left.png";
        break;
    case SDLK_RIGHT:
        newX += TILE_SIZE;
        direction = RIGHT;
        spritePath = "character/character_right.png";
        break;
    case SDLK_UP:
        newY -= TILE_SIZE;
        direction = UP;
        spritePath = "character/character_up.png";
        break;
    case SDLK_DOWN:
        newY += TILE_SIZE;
        direction = DOWN;
        spritePath = "character/character_up.png";
        break;
    default:
        return;
    }

    if (!CheckCollision(newX, newY, tileMap)) {
        int oldTileX = x / TILE_SIZE;
        int oldTileY = y / TILE_SIZE;
        x = newX;
        y = newY;
        int newTileX = x / TILE_SIZE;
        int newTileY = y / TILE_SIZE;

        tileMap[oldTileY][oldTileX] = 0;
        tileMap[newTileY][newTileX] = 1;

        if (tileMap[newTileY][newTileX] == 3 || tileMap[newTileY][newTileX] == 4)
            tileMap[newTileY][newTileX] = 1;

        LoadSprite(spritePath);
    }
}

void Character::Update(int tileMap[MAP_HEIGHT][MAP_WIDTH]) {
    int tileX = x / TILE_SIZE;
    int tileY = y / TILE_SIZE;

    underRock = (tileY > 0 && tileMap[tileY - 1][tileX] == 2);

    static Uint32 lastCheck = SDL_GetTicks();
    Uint32 now = SDL_GetTicks();
    Uint32 frameTime = now - lastCheck;
    lastCheck = now;

    if (underRock && !dead) {
        timeUnderRock += frameTime;
        if (timeUnderRock >= 3000) Die();
    }
    else {
        timeUnderRock = 0;
    }
}

void Character::Die() {
    std::cout << "Nhân vật đã chết vì bị đá đè!\n";
    dead = true;
    Reset();
}

void Character::Reset() {
    x = startX * TILE_SIZE;
    y = startY * TILE_SIZE;
    timeUnderRock = 0;
    underRock = false;
    dead = false;

    extern int originalMap[MAP_HEIGHT][MAP_WIDTH];
    for (int i = 0; i < MAP_HEIGHT; ++i)
        for (int j = 0; j < MAP_WIDTH; ++j)
            tileMap[i][j] = originalMap[i][j];

    LoadSprite("character/character_right.png");
}

void Character::Render(SDL_Renderer* renderer) {
    SDL_Rect renderQuad = { x, y, TILE_SIZE, TILE_SIZE };
    SDL_RenderCopy(renderer, texture, NULL, &renderQuad);
}

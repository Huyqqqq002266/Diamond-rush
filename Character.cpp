#include "Character.h"
#include <iostream>
#include "map.h"
#include "Enemy.h"
#include <set>

Character::Character(int x, int y, SDL_Renderer* renderer)
    : startX(x), startY(y), renderer(renderer), direction(RIGHT) {

    this->x = x * TILE_SIZE;
    this->y = y * TILE_SIZE;
    tileMap[y][x] = 3;
    LoadSprite("character/character_right.png");
    collectDiamondSound = Mix_LoadWAV("music/collect_diamond.mp3");
    leavesSound = Mix_LoadWAV("music/leaves.mp3");
}

Character::~Character() {
    if (texture) SDL_DestroyTexture(texture);
    if (collectDiamondSound) Mix_FreeChunk(collectDiamondSound);
    if (leavesSound) {
        Mix_FreeChunk(leavesSound);
        leavesSound = nullptr;
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
    return (tileType == 1 || tileType == 2);
}

void Character::HandleEvent(SDL_Event& event, int tileMap[MAP_HEIGHT][MAP_WIDTH]) {
    if (event.type != SDL_KEYDOWN) return;

    int newX = x, newY = y;
    std::string spritePath;

    switch (event.key.keysym.sym) {
    case SDLK_LEFT:
        direction = LEFT;
        spritePath = "character/character_left.png";
        if (!CheckCollision(x - TILE_SIZE, y, tileMap)) {
            newX -= TILE_SIZE;
        }
        else {
            int tileX = x / TILE_SIZE;
            int tileY = y / TILE_SIZE;
            if (tileMap[tileY][tileX - 1] == 2 && tileMap[tileY][tileX - 2] == 0) {
                tileMap[tileY][tileX - 2] = 2;
                tileMap[tileY][tileX - 1] = 0;
                newX -= TILE_SIZE;
            }
            spritePath = "character/character_push_left.png";
        }
        break;

    case SDLK_RIGHT:
        direction = RIGHT;
        spritePath = "character/character_right.png";
        if (!CheckCollision(x + TILE_SIZE, y, tileMap)) {
            newX += TILE_SIZE;
        }
        else {
            int tileX = x / TILE_SIZE;
            int tileY = y / TILE_SIZE;
            if (tileMap[tileY][tileX + 1] == 2 && tileMap[tileY][tileX + 2] == 0) {
                tileMap[tileY][tileX + 2] = 2;
                tileMap[tileY][tileX + 1] = 0;
                newX += TILE_SIZE;
            }
            spritePath = "character/character_push_right.png";
        }
        break;

    case SDLK_UP:
        newY -= TILE_SIZE;
        direction = UP;
        spritePath = "character/character_up.png";
        break;

    case SDLK_DOWN:
        newY += TILE_SIZE;
        direction = DOWN;
        spritePath = "character/character_down.png";
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

        if (tileMap[newTileY][newTileX] == 5 && !dead) {
            Die();
            return;
        }

        if (tileMap[newTileY][newTileX] == 4 || tileMap[newTileY][newTileX] == 6) {
            diamondsCollected++;

            std::pair<int, int> pos = { newTileY, newTileX };
            if (diamondsPlayed.find(pos) == diamondsPlayed.end()) {
                Mix_PlayChannel(-1, collectDiamondSound, 0);
                diamondsPlayed.insert(pos);
            }
        }
        if (tileMap[newTileY][newTileX] == 3) {
            std::pair<int, int> pos = { newTileY, newTileX };
            if (leavesPlayed.find(pos) == leavesPlayed.end()) {
                Mix_PlayChannel(-1, leavesSound, 0);
                leavesPlayed.insert(pos);
            }
        }

        if (tileMap[newTileY][newTileX] == 6) {
            key++;
        }

        if (tileMap[newTileY][newTileX] == 8) {
            if (diamondsCollected >= DIAMONDS_REQUIRED_FOR_NEXT_LEVEL) {
                LevelUp = true;
            }
            else {
                gameOverByDiamond = true;
            }
        }

        tileMap[oldTileY][oldTileX] = 0;
        tileMap[newTileY][newTileX] = 3;
        LoadSprite(spritePath);
    }
}

void Character::Update(int tileMap[MAP_HEIGHT][MAP_WIDTH]) {
    int tileX = x / TILE_SIZE;
    int tileY = y / TILE_SIZE;

    bool nowUnderRock = (tileY > 0 && tileMap[tileY - 1][tileX] == 2);

    static int lastTileX = tileX;
    static int lastTileY = tileY;

    static Uint32 lastCheck = SDL_GetTicks();
    Uint32 now = SDL_GetTicks();
    Uint32 frameTime = now - lastCheck;
    lastCheck = now;

    if (tileMap[tileY][tileX] == 5 && !dead) {
        Die();
    }

    if (nowUnderRock) {
        if (tileX != lastTileX || tileY != lastTileY || !underRock) {
            timeUnderRock = 0;
        }

        timeUnderRock += frameTime;

        if (timeUnderRock >= 500 && timeUnderRock < 2000) {
            LoadSprite("character/character_underrock.png");
        }

        if (timeUnderRock >= 2000 && !dead) {
            Die();
        }
    }

    underRock = nowUnderRock;
    lastTileX = tileX;
    lastTileY = tileY;
}

void Character::Die() {
    dead = true;
    lives--;
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

    diamondsCollected = 0;
    key = 0;
    diamondsPlayed.clear();
    LoadSprite("character/character_right.png");
    LoadEnemies(renderer);
}

void Character::Render(SDL_Renderer* renderer) {
    SDL_Rect renderQuad = { x, y, TILE_SIZE, TILE_SIZE };
    SDL_RenderCopy(renderer, texture, NULL, &renderQuad);
}

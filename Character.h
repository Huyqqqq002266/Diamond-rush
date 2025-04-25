#ifndef CHARACTER_H
#define CHARACTER_H

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <string>
#include <set>
#include "CommonFunc.h"

class Character {
public:
    Character(int x, int y, SDL_Renderer* renderer);
    ~Character();

    void HandleEvent(SDL_Event& event, int tileMap[MAP_HEIGHT][MAP_WIDTH]);
    void Update(int tileMap[MAP_HEIGHT][MAP_WIDTH]);
    void Render(SDL_Renderer* renderer);
    void Reset();
    int diamondsCollected = 0;
    int lives = 3;
    int key = 0;
    bool gameOverByDiamond = false;
    bool IsDead() const { return dead; }
    bool IsGameOver() const { return lives <= 0; }
    bool IsWin() const;

private:
    void LoadSprite(const std::string& path);
    bool CheckCollision(int newX, int newY, int tileMap[MAP_HEIGHT][MAP_WIDTH]);
    void Die();

    SDL_Renderer* renderer;
    SDL_Texture* texture = nullptr;
    Mix_Chunk* collectDiamondSound = nullptr;
    Mix_Chunk* leavesSound = nullptr;
    int x, y;
    int startX, startY;
    Direction direction;

    bool underRock = false;
    bool dead = false;
    bool isWin = false;
    bool levelUp = false;


    Uint32 timeUnderRock = 0;

    std::set<std::pair<int, int>> leavesPlayed;
    std::set<std::pair<int, int>> diamondsPlayed;
};

#endif 

#include "CommonFunc.h"
#include "BaseObject.h"
#include "Map.h"
#include "Enemy.h"
#include "Character.h"
#include <vector>

BaseObject g_background;
std::vector<Enemy> enemies;
Character* player = nullptr;

bool InitData() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) return false;
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

    g_window = SDL_CreateWindow("Diamond Rush",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (g_window == nullptr) return false;

    g_screen = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_ACCELERATED);
    if (g_screen == nullptr) return false;

    SDL_SetRenderDrawColor(g_screen, 255, 255, 255, 255);
    if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG) return false;

    return true;
}

bool LoadBackground() {
    if (!g_background.LoadImg("image/background.png", g_screen)) {
        printf("Failed to load background image!\n");
        return false;
    }
    return true;
}

void Close(Map& gameMap) {
    g_background.Free();
    gameMap.Free();
    if (player) {
        delete player;
        player = nullptr;
    }
    SDL_DestroyRenderer(g_screen);
    SDL_DestroyWindow(g_window);
    SDL_Quit();
}

int main(int argc, char* argv[]) {
    if (!InitData()) return -1;
    if (!LoadBackground()) return -1;

    Map gameMap;
    if (!gameMap.LoadTiles(g_screen)) return -1;

    gameMap.SaveOriginalMap();

    LoadEnemies(enemies, g_screen);
    player = new Character(1, 18, g_screen);

    bool is_quit = false;
    SDL_Event g_event;

    while (!is_quit) {
        while (SDL_PollEvent(&g_event) != 0) {
            if (g_event.type == SDL_QUIT) is_quit = true;
            if (player) player->HandleEvent(g_event, tileMap);
        }

        player->Update(tileMap);

        SDL_SetRenderDrawColor(g_screen, 255, 255, 255, 255);
        SDL_RenderClear(g_screen);

        g_background.Render(g_screen, NULL);
        gameMap.DrawMap(g_screen);

        for (Enemy& enemy : enemies) {
            enemy.Update();
            enemy.Render(g_screen);
        }

        player->Render(g_screen);

        SDL_RenderPresent(g_screen);
        SDL_Delay(200);
        if (player->IsDead()) {
            gameMap.ResetMap();
            LoadEnemies(enemies, g_screen);
            player->Reset();
        }
    }

    Close(gameMap);
    return 0;
}


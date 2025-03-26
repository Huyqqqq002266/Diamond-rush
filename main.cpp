#include "CommonFunc.h"
#include "BaseObject.h"
#include "Map.h"
#include "Enemy.h"
#include <vector>

int enemyMap[MAP_HEIGHT][MAP_WIDTH] = {0};
BaseObject g_background;
std::vector<Enemy> enemies;

bool InitData() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) return false;
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

    g_window = SDL_CreateWindow("Diamond Rush",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (g_window == NULL) return false;

    g_screen = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_ACCELERATED);
    if (g_screen == NULL) return false;

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

void LoadEnemies() {
    enemies.clear();
    memset(enemyMap, 0, sizeof(enemyMap));

    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            if (tileMap[y][x] == 5) {
                Direction dir;

                if ((y == 4 && x == 5)) dir = HORIZONTAL;
                else if ((y == 4 && x == 2) || (y == 17 && (x == 15 || x == 17 || x == 19)))
                    dir = VERTICAL;
                else
                    continue;

                enemies.emplace_back(x, y, dir, g_screen);
                enemyMap[y][x] = 1;
            }
        }
    }
}


void close(Map& gameMap) {
    g_background.Free();
    gameMap.Free();
    SDL_DestroyRenderer(g_screen);
    SDL_DestroyWindow(g_window);
    SDL_Quit();
}

int main(int argc, char* argv[]) {
    if (!InitData()) return -1;
    if (!LoadBackground()) return -1;

    Map gameMap;
    if (!gameMap.LoadTiles(g_screen)) return -1;

    LoadEnemies();

    bool is_quit = false;
    SDL_Event g_event;

    while (!is_quit) {
        while (SDL_PollEvent(&g_event) != 0) {
            if (g_event.type == SDL_QUIT) is_quit = true;
        }

        SDL_SetRenderDrawColor(g_screen, 255, 255, 255, 255);
        SDL_RenderClear(g_screen);

        g_background.Render(g_screen, NULL);
        gameMap.DrawMap(g_screen);

        for (Enemy& enemy : enemies) {
            enemy.Update();
            enemy.Render(g_screen);
        }

        SDL_RenderPresent(g_screen);
        SDL_Delay(250);
    }

    close(gameMap);
    return 0;
}

#include "CommonFunc.h"
#include "BaseObject.h"
#include "Map.h"
#include "Enemy.h"
#include "Character.h"
#include "RockManager.h"
#include "HUD.h"
#include "menu.h"

HUD* hud = nullptr;
BaseObject g_background;
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

    if (hud) {
        delete hud;
        hud = nullptr;
    }

    SDL_DestroyRenderer(g_screen);
    SDL_DestroyWindow(g_window);
    IMG_Quit();
    SDL_Quit();
}

int main(int argc, char* argv[]) {
    if (!InitData()) return -1;
    if (!LoadBackground()) return -1;

BACK_TO_MENU:
    Menu* menu = new Menu(g_screen);
    bool inMenu = true;
    bool isPaused = false;
    SDL_Event event;
    while (inMenu) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                delete menu;
                return 0;
            }

            int result = menu->handleEvent(event);
            switch (result) {
            case Menu::NEW_GAME:
                inMenu = false;
                break;
            case Menu::QUIT:
                delete menu;
                return 0;
            }
        }

        SDL_SetRenderDrawColor(g_screen, 0, 0, 0, 255);
        SDL_RenderClear(g_screen);
        menu->render();
        SDL_RenderPresent(g_screen);
        SDL_Delay(16);
    }
    delete menu;
    Map gameMap;
    if (!gameMap.LoadTiles(g_screen)) return -1;
    gameMap.LoadCurrentLevel(1);
    gameMap.SaveOriginalMap();
    RockManager rockManager;

    LoadEnemies(g_screen);
    player = new Character(1, 18, g_screen);
    hud = new HUD(g_screen);

    bool is_quit = false;
    
    while (!is_quit) {
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT) {
                is_quit = true;
            }

            if (!isPaused) {
                if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
                    isPaused = true;
                    menu = new Menu(g_screen);
                    menu->setPause(true);
                }

                if (player) {
                    player->HandleEvent(event, tileMap);
                }
            }
            else {
                int result = menu->handleEvent(event);
                switch (result) {
                case Menu::RESUME:
                    isPaused = false;
                    menu->setPause(false);
                    delete menu;
                    menu = nullptr;
                    break;
                case Menu::QUIT:
                    isPaused = false;
                    delete menu;
                    menu = nullptr;

                    delete player;
                    player = nullptr;
                    enemies.clear();

                    inMenu = true;
                    goto BACK_TO_MENU;

                }
            }
        }


        if (!isPaused) {
            rockManager.Update();
            player->Update(tileMap);
            if (player->LevelUp) {
                currentLevel++;
                if (currentLevel <= MAX_LEVEL) {
                    gameMap.LoadCurrentLevel(currentLevel);
                    player->Reset();
                    player->LevelUp = false;
                    LoadEnemies(g_screen);
                }
                else {
                    is_quit = true;
                }
            }

            SDL_SetRenderDrawColor(g_screen, 255, 255, 255, 255);
            SDL_RenderClear(g_screen);

            g_background.Render(g_screen, NULL);
            gameMap.DrawMap(g_screen); 

            for (Enemy& enemy : enemies) {
                enemy.Update();
                enemy.Render(g_screen);
            }

            player->Render(g_screen);
            hud->SetDiamonds(player->diamondsCollected);
            hud->SetLives(player->lives);
            hud->SetKeys(player->key);
            hud->Render(g_screen);

            SDL_RenderPresent(g_screen);
            SDL_Delay(200);
            if (player->IsGameOver()) {
                delete player;
                player = nullptr;
                enemies.clear();

                menu = new Menu(g_screen);

                bool gameOverMenu = true;
                while (gameOverMenu) {
                    while (SDL_PollEvent(&event)) {
                        if (event.type == SDL_QUIT) {
                            gameOverMenu = false;
                            is_quit = true;
                        }

                        int result = menu->handleEvent(event);
                        switch (result) {
                        case Menu::GAME_OVER_AGAIN:
                            delete menu;
                            goto BACK_TO_MENU;
                        case Menu::GAME_OVER_QUIT:
                            gameOverMenu = false;
                            is_quit = true;
                            break;
                        }
                    }

                    SDL_SetRenderDrawColor(g_screen, 0, 0, 0, 255);
                    SDL_RenderClear(g_screen);
                    menu->RenderGameOver(g_screen);
                    SDL_RenderPresent(g_screen);
                    SDL_Delay(4);
                }

                if (menu) {
                    delete menu;
                    menu = nullptr;
                }
            }
            if (player->IsDead()) {
                gameMap.ResetMap();
                LoadEnemies(g_screen);
                player->Reset();
            }
        }
        else {
            SDL_SetRenderDrawColor(g_screen, 0, 0, 0, 255);
            SDL_RenderClear(g_screen);
            menu->render();
            SDL_RenderPresent(g_screen);
            SDL_Delay(200);
        }
    }

    Close(gameMap);
    return 0;
}

#include <SDL_mixer.h>
#include<iostream>
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
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cout << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << std::endl;
    }

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

void ResetGame() {
    if (player) delete player;
    player = new Character(1, 18, g_screen);

    enemies.clear();
    LoadEnemies(g_screen);
    player->Reset();
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
    Mix_Music* bgMusic = Mix_LoadMUS("music/background_music.mp3");
    if (bgMusic == nullptr) {
        std::cout << "Failed to load background music! SDL_mixer Error: " << Mix_GetError() << std::endl;
    }
    else {
        Mix_PlayMusic(bgMusic, -1);
    }
    while (!is_quit) {
        while (SDL_PollEvent(&event)) {
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

        if (isPaused) {
            SDL_SetRenderDrawColor(g_screen, 0, 0, 0, 255);
            SDL_RenderClear(g_screen);
            if (menu) menu->render();
            SDL_RenderPresent(g_screen);
            SDL_Delay(100);
            continue;
        }
        if (player) {
            rockManager.Update();
            player->Update(tileMap);
            if (player->gameOverByDiamond) {
                delete player;
                player = nullptr;
                enemies.clear();

                menu = new Menu(g_screen);
                menu->setGameOver(true);
                bool gameOverMenu = true;

                Mix_HaltMusic();

                Mix_Chunk* gameOverSound = Mix_LoadWAV("music/game_over.mp3");
                if (gameOverSound != nullptr) {
                    Mix_PlayChannel(-1, gameOverSound, 0);
                }
                else {
                    std::cout << "Failed to load game over sound! SDL_mixer Error: " << Mix_GetError() << std::endl;
                }

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
                            menu = nullptr;

                            ResetGame();
                            Mix_PlayMusic(bgMusic, -1);
                            gameMap.LoadCurrentLevel(currentLevel);
                            player->gameOverByDiamond = false;
                            gameOverMenu = false;
                            break;

                        case Menu::QUIT:
                            gameOverMenu = false;
                            is_quit = true;
                            break;
                        }
                    }

                    SDL_SetRenderDrawColor(g_screen, 0, 0, 0, 255);
                    SDL_RenderClear(g_screen);
                    if (menu) menu->render();
                    SDL_RenderPresent(g_screen);
                    SDL_Delay(100);
                }

                continue;
            }

            if (player->IsGameOver()) {
                delete player;
                player = nullptr;
                enemies.clear();

                menu = new Menu(g_screen);
                menu->setGameOver(true);
                bool gameOverMenu = true;

                Mix_HaltMusic();

                Mix_Chunk* gameOverSound = Mix_LoadWAV("music/game_over.mp3");
                if (gameOverSound != nullptr) {
                    Mix_PlayChannel(-1, gameOverSound, 0);
                }
                else {
                    std::cout << "Failed to load game over sound! SDL_mixer Error: " << Mix_GetError() << std::endl;
                }

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
                            menu = nullptr;

                            ResetGame();

                            gameMap.LoadCurrentLevel(currentLevel);
                            gameMap.SaveOriginalMap();

                            if (hud) {
                                delete hud;
                                hud = nullptr;
                            }
                            hud = new HUD(g_screen);
                            Mix_PlayMusic(bgMusic, -1);
                            gameOverMenu = false;
                            break;
                        case Menu::GAME_OVER_QUIT:
                            delete menu;
                            menu = nullptr;

                            gameOverMenu = false;
                            inMenu = true;
                            goto BACK_TO_MENU;

                        }
                    }

                    if (gameOverMenu) {
                        SDL_SetRenderDrawColor(g_screen, 0, 0, 0, 255);
                        SDL_RenderClear(g_screen);
                        menu->RenderGameOver(g_screen);
                        SDL_RenderPresent(g_screen);
                    }
                }

                if (menu) {
                    delete menu;
                    menu = nullptr;
                }
                gameOverMenu = false;
            }

            if (player->IsDead()) {
                gameMap.ResetMap();
                LoadEnemies(g_screen);
                player->Reset();
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

        if (player) {
            player->Render(g_screen);
            hud->SetDiamonds(player->diamondsCollected);
            hud->SetLives(player->lives);
            hud->SetKeys(player->key);
            hud->Render(g_screen);
        }

        SDL_RenderPresent(g_screen);
        SDL_Delay(200);
    }

    Mix_FreeMusic(bgMusic);
    Mix_CloseAudio();
    Close(gameMap);
    return 0;
}


#include "Menu.h"
#include <iostream>

Menu::Menu(SDL_Renderer* renderer) {
    this->renderer = renderer;

    menuTexture = IMG_LoadTexture(renderer, "image/menu.png");
    levelMenuTexture = IMG_LoadTexture(renderer, "image/menu_level.png");
    settingMenuTexture = IMG_LoadTexture(renderer, "image/menu_setting.png");
    pauseMenuTexture = IMG_LoadTexture(renderer, "image/pause_menu.png");
    gameOverTexture = IMG_LoadTexture(renderer, "image/game_over.png");

    currentState = MAIN_MENU;
    isPaused = false;

    const int SCREEN_WIDTH = 832;
    const int SCREEN_HEIGHT = 672;
    const int MENU_WIDTH = 832;
    const int MENU_HEIGHT = 672;

    int menuX = (SCREEN_WIDTH - MENU_WIDTH) / 2;
    int menuY = (SCREEN_HEIGHT - MENU_HEIGHT) / 2;

    menuRect = { menuX, menuY, MENU_WIDTH, MENU_HEIGHT };
    newGameButton = { menuX + 244, menuY + 131, 357, 78 };
    levelButton = { menuX + 244, menuY + 262, 357, 78 };
    settingButton = { menuX + 244, menuY + 393, 357, 78 };
    quitButton = { menuX + 244, menuY + 525, 357, 78 };
    resumeButton = { menuX + 222, menuY + 370, 357, 78 };
    pauseQuitButton = { menuX + 222, menuY + 480, 357, 78 };
    backButton = { 20, 20, 80, 50 };
    gameOverPlayAgainButton = { 270, 340, 250, 60 };
    gameOverQuitButton = { 300, 420, 180, 60 };
}

Menu::~Menu() {
    if (menuTexture) SDL_DestroyTexture(menuTexture);
    if (levelMenuTexture) SDL_DestroyTexture(levelMenuTexture);
    if (settingMenuTexture) SDL_DestroyTexture(settingMenuTexture);
    if (pauseMenuTexture) SDL_DestroyTexture(pauseMenuTexture);
    if (gameOverTexture) SDL_DestroyTexture(gameOverTexture);
}

void Menu::setPause(bool isPaused) {
    this->isPaused = isPaused;
    if (isPaused) {
        stateStack.push(currentState);
        currentState = PAUSE_MENU;
    }
    else {
        if (!stateStack.empty()) {
            currentState = stateStack.top();
            stateStack.pop();
        }
    }
}

void Menu::RenderGameOver(SDL_Renderer* renderer) {
    if (gameOverTexture) {
        SDL_RenderCopy(renderer, gameOverTexture, NULL, &menuRect);
    }
}

void Menu::render() {
    switch (currentState) {
    case MAIN_MENU:
        SDL_RenderCopy(renderer, menuTexture, NULL, &menuRect);
        break;
    case LEVEL_MENU:
        SDL_RenderCopy(renderer, levelMenuTexture, NULL, &menuRect);
        break;
    case SETTING_MENU:
        SDL_RenderCopy(renderer, settingMenuTexture, NULL, &menuRect);
        break;
    case PAUSE_MENU:
        SDL_RenderCopy(renderer, pauseMenuTexture, NULL, &menuRect);
        break;
    case GAME_OVER:
        RenderGameOver(renderer);
        break;
    }
}

int Menu::handleEvent(SDL_Event& event) {
    if (event.type == SDL_QUIT) return QUIT;

    if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
        if (!isPaused) {
            setPause(true);
            return NONE;
        }
        else {
            setPause(false);
            return RESUME;
        }
    }

    if (event.type == SDL_MOUSEBUTTONDOWN) {
        SDL_Point mouse = { event.button.x, event.button.y };

        if (currentState == PAUSE_MENU) {
            if (SDL_PointInRect(&mouse, &resumeButton)) {
                setPause(false);
                return RESUME;
            }
            if (SDL_PointInRect(&mouse, &pauseQuitButton)) {
                return QUIT;
            }
        }

        if (currentState == LEVEL_MENU || currentState == SETTING_MENU) {
            if (SDL_PointInRect(&mouse, &backButton)) {
                if (!stateStack.empty()) {
                    currentState = stateStack.top();
                    stateStack.pop();
                    return NONE;
                }
            }
        }

        if (currentState == MAIN_MENU) {
            if (SDL_PointInRect(&mouse, &newGameButton)) return NEW_GAME;
            if (SDL_PointInRect(&mouse, &levelButton)) {
                stateStack.push(currentState);
                currentState = LEVEL_MENU;
                return NONE;
            }
            if (SDL_PointInRect(&mouse, &settingButton)) {
                stateStack.push(currentState);
                currentState = SETTING_MENU;
                return NONE;
            }
            if (SDL_PointInRect(&mouse, &quitButton)) return QUIT;
        }

        if (currentState == GAME_OVER) {
            if (SDL_PointInRect(&mouse, &gameOverPlayAgainButton)) {
                return GAME_OVER_AGAIN;
            }
            if (SDL_PointInRect(&mouse, &gameOverQuitButton)) {
                return GAME_OVER_QUIT;
            }
        }
    }

    return NONE;
}

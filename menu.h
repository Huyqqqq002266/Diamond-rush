#ifndef MENU_H
#define MENU_H

#include <SDL.h>
#include <SDL_image.h>
#include <stack>

class Menu {
public:
    enum MenuResult {
        NONE,
        NEW_GAME,
        LEVEL,
        SETTING,
        QUIT,
        RESUME,
        GAME_OVER_AGAIN,
        GAME_OVER_QUIT
    };

    enum State {
        MAIN_MENU,
        LEVEL_MENU,
        SETTING_MENU,
        PAUSE_MENU,
        GAME_OVER
    };

    Menu(SDL_Renderer* renderer);
    ~Menu();

    void render();
    int handleEvent(SDL_Event& event);
    void RenderGameOver(SDL_Renderer* renderer);
    void setPause(bool isPaused);

private:
    SDL_Renderer* renderer;
    SDL_Texture* menuTexture;
    SDL_Texture* levelMenuTexture;
    SDL_Texture* settingMenuTexture;
    SDL_Texture* pauseMenuTexture;
    SDL_Texture* gameOverTexture = nullptr;

    SDL_Rect menuRect;
    SDL_Rect newGameButton;
    SDL_Rect levelButton;
    SDL_Rect settingButton;
    SDL_Rect quitButton;
    SDL_Rect backButton;
    SDL_Rect resumeButton;
    SDL_Rect pauseQuitButton;
    SDL_Rect gameOverPlayAgainButton;
    SDL_Rect gameOverQuitButton;

    // State
    std::stack<State> stateStack;
    State currentState;
    bool isPaused;
};

#endif

#ifndef COMMON_FUNCTION_H_
#define COMMON_FUNCTION_H_
#define DIAMONDS_REQUIRED_FOR_NEXT_LEVEL 25
#define MAX_LEVEL 3

#include <windows.h>
#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

static SDL_Window* g_window = NULL;
static SDL_Renderer* g_screen = NULL;
static SDL_Event g_event;

const int SCREEN_WIDTH = 832;
const int SCREEN_HEIGHT = 672;
const int SCREEN_BPP = 32;

const int COLOR_KEY_R = 167;
const int COLOR_KEY_G = 175;
const int COLOR_KEY_B = 180;


const int MAP_HEIGHT = 21;
const int MAP_WIDTH = 26;
const int TILE_SIZE = 32;

const int RENDER_DRAW_COLOR = 0xff;

#ifndef COMMON_FUNC_H
#define COMMON_FUNC_H

enum Direction {
    LEFT,
    RIGHT,
    UP,
    DOWN,
    VERTICAL,
    HORIZONTAL
};

#endif
#endif

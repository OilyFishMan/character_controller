#ifndef SPRITE_H_
#define SPRITE_H_

#include <stdbool.h>
#include "mathematique.h"
#include <SDL2/SDL.h>

struct sprite
{
    vec2s scale;
    SDL_Color color;
};

bool draw_sprite(SDL_Renderer* renderer, struct sprite sprite, vec2s pos);

#endif // SPRITE_H_

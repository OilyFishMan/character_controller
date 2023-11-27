#include "sprite.h"
#include "int_types.h"
#include "mathematique.h"

#include <stdbool.h>
#include <SDL2/SDL.h>

bool draw_sprite(SDL_Renderer* renderer, struct sprite sprite, vec2s pos)
{
    s32 width;
    s32 height;

    if (SDL_GetRendererOutputSize(renderer, &width, &height) != 0) {
        fprintf(stderr, "%s\n", SDL_GetError());
        return false;
    }

    if (SDL_SetRenderDrawColor( renderer
                              , sprite.color.r
                              , sprite.color.g
                              , sprite.color.b
                              , sprite.color.a) != 0) {
        fprintf(stderr, "%s\n", SDL_GetError());
        return false;
    }

    if (SDL_RenderFillRect( renderer
                          , &(SDL_Rect) { width / 2 + (s32) pos.x - sprite.scale.x / 2
                                        , height / 2 - (s32) pos.y - sprite.scale.y / 2
                                        , (s32) sprite.scale.x
                                        , (s32) sprite.scale.y }) != 0) {
        fprintf(stderr, "%s\n", SDL_GetError());
        return false;
    }

    return true;
}

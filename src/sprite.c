#include "sprite.h"
#include "collision.h"
#include "int_types.h"
#include "mathematique.h"
#include "window.h"

#include <stdbool.h>
#include <SDL2/SDL.h>

bool draw_sprite(SDL_Renderer* renderer, struct sprite sprite, const vec2s pos)
{
    struct collision on_screen = aabb( pos
                                     , sprite.scale
                                     , (vec2s) { .x = 0.0f, .y = 0.0f }
                                     , (vec2s) { .x = WINDOW_WIDTH, .y = WINDOW_HEIGHT });
    if (!on_screen.hit) {
        return true;
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
                          , &(SDL_Rect) { WINDOW_WIDTH / 2 + (s32) pos.x - sprite.scale.x / 2
                                        , WINDOW_HEIGHT / 2 - (s32) pos.y - sprite.scale.y / 2
                                        , (s32) sprite.scale.x
                                        , (s32) sprite.scale.y }) != 0) {
        fprintf(stderr, "%s\n", SDL_GetError());
        return false;
    }

    return true;
}

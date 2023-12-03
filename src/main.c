#include "player.h"
#include "enemy.h"
#include "int_types.h"
#include "input.h"
#include "int_types.h"
#include "mathematique.h"
#include "time_math.h"
#include "sprite.h"
#include "window.h"

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <assert.h>
#include <limits.h>
#include <SDL2/SDL.h>

s32 main(void)
{
    srand(time(NULL));

    bool success = true;

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        fprintf(stderr, "Failed to initialize SDL2 : %s\n", SDL_GetError());
        success = false;
        goto cleanup;
    }

    SDL_Window* window = SDL_CreateWindow( WINDOW_TITLE
                                         , SDL_WINDOWPOS_UNDEFINED
                                         , SDL_WINDOWPOS_UNDEFINED
                                         , WINDOW_WIDTH, WINDOW_HEIGHT
                                         , SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

    if (window == NULL) {
        fprintf(stderr, "%s\n", SDL_GetError());
        success = false;
        goto cleanup_sdl;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer( window
                                               , -1
                                               , SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if (renderer == NULL) {
        fprintf(stderr, "%s\n", SDL_GetError());
        success = false;
        goto cleanup_window;
    }

    usize tick = 0;

    struct key_state key_states[SDL_NUM_SCANCODES] = KEY_STATES_DEFAULT;

    struct enemies* enemies = malloc(sizeof(*enemies));
    if (enemies == NULL) {
        fprintf(stderr, "Failed to allocate memory: %s\n", strerror(errno));
        goto cleanup_renderer;
    }
    enemies_init(enemies);

    struct player player;
    player_init(&player);

    vec2s box_pos = { .x = 0.0f, .y = -WINDOW_HEIGHT };
    const vec2s box_scale = { .x = WINDOW_WIDTH, .y = WINDOW_HEIGHT };

    vec2s camera = { .x = 0.0f, .y = 0.0f };

    f64 frame_begin_time = get_time();
    f64 frame_elapsed_time = DELTA_TIME;
    f64 physics_accumulator = DELTA_TIME;

    for (bool open = true; open; ++tick) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            switch (e.type) {
                case SDL_QUIT: {
                    open = false;
                    break;
                }

                case SDL_KEYDOWN: {
                    if (key_states[e.key.keysym.scancode].type == E_key_state_released) {
                        key_states[e.key.keysym.scancode].tick = tick;
                    }

                    if (e.key.repeat > 0) {
                        key_states[e.key.keysym.scancode].type = E_key_state_repeat;
                    } else {
                        key_states[e.key.keysym.scancode].type = E_key_state_down;
                    }
                    break;
                }

                case SDL_KEYUP: {
                    key_states[e.key.keysym.scancode].type = E_key_state_released;
                    break;
                }
            }
        }

        if (KEY_STATE_PRESSED_ON(key_states, SDLK_ESCAPE, tick)) {
            open = false;
        }

        player_update(&player, frame_begin_time, tick, key_states);

        f64 physics_elapsed_total = 0.0f;
        const usize physics_steps = (usize) (physics_accumulator / DELTA_TIME);
        while (physics_accumulator >= DELTA_TIME) {
            const f64 begin = get_time();
            player_physics(&player, box_pos, box_scale, frame_begin_time);
            enemies_physics(enemies, box_pos, box_scale);

            camera = player.pos;

            physics_accumulator -= DELTA_TIME;
            physics_elapsed_total += get_time() - begin;
        }
        if (physics_elapsed_total > ((f64) physics_steps) * DELTA_TIME) {
            SDL_Log("Sorry, your computer is too shit to run this simulation. (or I fucked up)\n");
            success = false;
            goto cleanup_enemies;
        }

        if (SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0) != 0) {
            fprintf(stderr, "%s\n", SDL_GetError());
            success = false;
            goto cleanup_enemies;
        }

        if (SDL_RenderClear(renderer) != 0) {
            fprintf(stderr, "%s\n", SDL_GetError());
            success = false;
            goto cleanup_enemies;
        }

        if (!draw_sprite( renderer
                        , (struct sprite) { .scale = box_scale
                                          , .color = (SDL_Color) { 128, 128, 128, 0 }}
                        , vec2_sub(box_pos, camera))) {
            success = false;
            goto cleanup_enemies;
        }

        if (!draw_sprite(renderer, player.sprite, vec2_sub(player.pos, camera))) {
            success = false;
            goto cleanup_enemies;
        }

        for (usize i = 0; i < enemies->len; ++i) {
            if (!draw_sprite(renderer, enemies->sprite, vec2_sub(enemies->pos[i], camera))) {
                success = false;
                goto cleanup_enemies;
            }
        }

        SDL_RenderPresent(renderer);

        const f64 frame_end_time = get_time();
        frame_elapsed_time = frame_end_time - frame_begin_time;
        frame_begin_time = frame_end_time;
        physics_accumulator += frame_elapsed_time;
        fprintf(stderr, "%f\n", frame_elapsed_time);
    }

cleanup_enemies:
    free(enemies);

cleanup_renderer:
    SDL_DestroyRenderer(renderer);

cleanup_window:
    SDL_DestroyWindow(window);

cleanup_sdl:
    SDL_Quit();

cleanup:
    if (success) {
        fprintf(stderr, "My application has closed successfully.\n");
        return EXIT_SUCCESS;
    } else {
        fprintf(stderr, "My application has crashed. See above error messages for more details.\n");
        return EXIT_FAILURE;
    }
}

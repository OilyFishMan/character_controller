#include "player.h"
#include "int_types.h"
#include "mathematique.h"
#include "time_math.h"
#include "collision.h"
#include "input.h"
#include "sprite.h"
#include "window.h"

#include <SDL2/SDL.h>

#define PLAYER_JUMP         (300.0f)
#define PLAYER_COYOTE_TIME  (1.0f/8.0f)
#define PLAYER_DECELERATION (1000.0f)
#define PLAYER_GRAVITY      (750.0f)
#define PLAYER_MOVEMENT     (200.0f)

void player_init(struct player* player)
{
    player->sprite = (struct sprite) { .scale = (vec2s) { .x = 20, .y = 20 }
                                     , .color = (SDL_Color) { 0, 0, 0, 0 }};

    player->jump_pressed = true;
    player->jumping = false;

    player->coyote_time = -PLAYER_COYOTE_TIME;

    player->vel = (vec2s) { .x = 0.0f, .y = 0.0f };
    player->pos = (vec2s) { .x = 0.0f, .y = 0.0f };
}

void player_update(struct player* player, const f64 current_time, const usize tick, const struct key_state* key_states)
{
    if (KEY_STATE_PRESSED(key_states, SDLK_LEFT)) {
        player->vel.x = -PLAYER_MOVEMENT;
    }

    if (KEY_STATE_PRESSED(key_states, SDLK_RIGHT)) {
        player->vel.x = PLAYER_MOVEMENT;
    }

    struct key_state jump_state = KEY_STATE(key_states, SDLK_UP);

    if (jump_state.type == E_key_state_released) {
        player->jumping = false;
    } else {
        if (jump_state.tick == tick) {
            player->jump_pressed = true;
        }

        if (!player->jumping && player->jump_pressed && current_time - player->coyote_time < PLAYER_COYOTE_TIME) {
            player->jumping = true;
            player->jump_pressed = false;
            player->coyote_time = -PLAYER_COYOTE_TIME;
            player->vel.y = PLAYER_JUMP;
        }
    }

    if (KEY_STATE_PRESSED(key_states, SDLK_DOWN) && player->vel.y > -PLAYER_JUMP) {
        player->vel.y = -PLAYER_JUMP;
    }
}

void player_physics(struct player* player, const vec2s box_pos, const vec2s box_scale, const f64 current_time)
{
    const vec2s prev_vel = player->vel;

    player->vel.x /= powf(PLAYER_DECELERATION, DELTA_TIME);
    player->vel.y -= PLAYER_GRAVITY * DELTA_TIME;

    if (player->vel.y < -PLAYER_GRAVITY) {
        player->vel.y = -PLAYER_GRAVITY;
    }

    player->pos = vec2_add(player->pos, vec2_scale(vec2_add(prev_vel, player->vel), DELTA_TIME * 0.5f));

    const struct collision collide_box = aabb(player->pos, player->sprite.scale, box_pos, box_scale);

    if (collide_box.hit) {
        if (fabsf(collide_box.intersect.y) < fabsf(collide_box.intersect.x)) {
            player->pos.y += collide_box.intersect.y;
            if (player->vel.y < 0.0f) {
                player->vel.y = 0.0f;
            }
            player->coyote_time = current_time;
        } else {
            player->pos.x += collide_box.intersect.x;
        }
    }
}

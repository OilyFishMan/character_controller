#include "enemy.h"
#include "collision.h"
#include "sprite.h"
#include "int_types.h"
#include "mathematique.h"
#include "time_math.h"

#include <stdlib.h>
#include <SDL2/SDL.h>

#define ENEMY_GRAVITY (750.0f)
#define ENEMY_SPEED (500.0f)
#define ENEMY_DECELERATION (1000.0f)

void enemies_init(struct enemies* enemies)
{
    enemies->sprite = (struct sprite) { .scale = (vec2s) { .x = 20.0f, .y = 30.0f }
                                      , .color = (SDL_Color) { 0, 255, 0, 0 } };
    enemies->len = 0;

    for (usize i = 0; i < MAX_ENEMIES; ++i) {
        enemies->dir[enemies->len] = (vec2s) { .x = ENEMY_SPEED, .y = 0.0f };
        enemies->pos[enemies->len] = (vec2s) { .x = (float) i * 10.0f - 1000.0f, .y = (float) i * 10.0f - 1000.0f };
        enemies->vel[enemies->len] = (vec2s) { .x = 0.0f, .y = 0.0f };
        ++enemies->len;
    }
}

void enemies_physics(struct enemies* enemies, const vec2s box_pos, const vec2s box_scale)
{
    for (usize i = 0; i < enemies->len; ++i) {
        const vec2s prev_vel = enemies->vel[i];

        enemies->vel[i] = vec2_add(enemies->vel[i], vec2_scale(enemies->dir[i], DELTA_TIME));
        enemies->vel[i].x /= powf(ENEMY_DECELERATION, DELTA_TIME);

        if (enemies->vel[i].x > ENEMY_SPEED) {
            enemies->vel[i].x = ENEMY_SPEED;
        }

        enemies->vel[i].y -= ENEMY_GRAVITY * DELTA_TIME;

        if (enemies->vel[i].y < -ENEMY_GRAVITY) {
            enemies->vel[i].y = -ENEMY_GRAVITY;
        }

        enemies->pos[i] = vec2_add(enemies->pos[i], vec2_scale(vec2_add(prev_vel, enemies->vel[i]), 0.5f * DELTA_TIME));

        if (enemies->pos[i].x >= box_pos.x + box_scale.x / 2.0f) {
            enemies->dir[i].x = -ENEMY_SPEED;
        } else if (enemies->pos[i].x <= box_pos.x - box_scale.x / 2.0f) {
            enemies->dir[i].x = ENEMY_SPEED;
        }

        const struct collision collide_box = aabb(enemies->pos[i], enemies->sprite.scale, box_pos, box_scale);

        if (collide_box.hit) {
            if (fabsf(collide_box.intersect.y) < fabsf(collide_box.intersect.x)) {
                enemies->pos[i].y += collide_box.intersect.y;
                enemies->vel[i].y = 0.0f;
            } else {
                enemies->pos[i].x += collide_box.intersect.x;
                enemies->vel[i].x = 0.0f;
            }
        }
    }
}

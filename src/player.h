#ifndef PLAYER_H_
#define PLAYER_H_

#include "int_types.h"
#include "sprite.h"
#include "input.h"
#include "mathematique.h"

struct player
{
    struct sprite sprite;

    struct {
        bool jump_pressed : 1;
        bool jumping : 1;
    };

    f64 coyote_time;

    vec2s vel;
    vec2s pos;
};

void player_init(struct player* player);
void player_update(struct player* player, const f64 current_time, const usize tick, const struct key_state* key_states);
void player_physics(struct player* player, const vec2s box_pos, const vec2s box_scale, const f64 current_time);

#endif // PLAYER_H_

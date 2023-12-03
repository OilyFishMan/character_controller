#ifndef ENEMY_H_
#define ENEMY_H_

#include "sprite.h"
#include "int_types.h"
#include "mathematique.h"

#define MAX_ENEMIES (1000)

struct enemies
{
    struct sprite sprite;
    usize len;
    vec2s dir[MAX_ENEMIES];
    vec2s vel[MAX_ENEMIES];
    vec2s pos[MAX_ENEMIES];
};

void enemies_init(struct enemies* enemies);
void enemies_physics(struct enemies* enemies, const vec2s box_pos, const vec2s box_scale);

#endif // ENEMY_H_

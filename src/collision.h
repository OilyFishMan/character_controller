#ifndef COLLISION_H_
#define COLLISION_H_

#include "mathematique.h"

struct collision
{
    bool hit;
    vec2s intersect;
};

struct collision aabb(const vec2s pos1, const vec2s scale1, const vec2s pos2, const vec2s scale2);

#endif // COLLISION_H_

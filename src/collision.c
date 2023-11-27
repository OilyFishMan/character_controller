#include "collision.h"
#include "mathematique.h"

#include <stdlib.h>

vec2s aabb(const vec2s pos1, const vec2s scale1, const vec2s pos2, const vec2s scale2)
{
    if ( 2.0f * fabsf(pos1.x - pos2.x) >= scale1.x + scale2.x
      || 2.0f * fabsf(pos1.y - pos2.y) >= scale1.y + scale2.y) {
        return (vec2s) { .x = 0.0f, .y = 0.0f };
    } else {
        return (vec2s) { .x = pos2.x + (pos1.x < pos2.x ? -1.0f : 1.0f) * (scale1.x / 2.0f + scale2.x / 2.0f) - pos1.x
                       , .y = pos2.y + (pos1.y < pos2.y ? -1.0f : 1.0f) * (scale1.y / 2.0f + scale2.y / 2.0f) - pos1.y };
    }
}

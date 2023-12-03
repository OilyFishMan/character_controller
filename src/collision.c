#include "collision.h"
#include "mathematique.h"

#include <stdlib.h>

struct collision aabb(const vec2s pos1, const vec2s scale1, const vec2s pos2, const vec2s scale2)
{
    const vec2s diff = vec2_sub(pos2, pos1);
    const vec2s scale_sum_half = vec2_scale(vec2_add(scale1, scale2), 0.5f);

    if ( fabsf(diff.x) >= scale_sum_half.x
      || fabsf(diff.y) >= scale_sum_half.y) {
        return (struct collision) { .hit = false };
    }

    return (struct collision) { .hit = true
                              , .intersect.x = diff.x - (diff.x > 0.0f ? scale_sum_half.x : -scale_sum_half.x)
                              , .intersect.y = diff.y - (diff.y > 0.0f ? scale_sum_half.y : -scale_sum_half.y) };
}

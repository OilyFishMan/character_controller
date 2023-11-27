#ifndef MATHEMATIQUE_H_
#define MATHEMATIQUE_H_

#define CGLM_OMIT_NS_FROM_STRUCT_API
#include <cglm/struct.h>
#include <math.h>

#define EPSILON (1e-9)

#define SQUARE(_x) \
    ({ typeof(x) _y = (_x); (_y) * (_y); })

#define CUBE(_x) \
    ({ typeof(_x) _y = (_x); (_y) * (_y) * (_y); })

#define LERP(_a, _b, _t) \
    ({ typeof(_a) _x = (_a); ((_x) + (_t) * ((_b) - (_x))); })

#endif // MATHEMATIQUE_H_

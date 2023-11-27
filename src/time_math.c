#include "time_math.h"
#include "int_types.h"

#include <time.h>

f64 get_time(void)
{
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    return (f64) ts.tv_sec + 1e-9 * (f64) ts.tv_nsec;
}

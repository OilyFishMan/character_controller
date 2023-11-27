#include "input.h"
#include "int_types.h"

#include <stdbool.h>
#include <string.h>
#include <SDL2/SDL.h>

void key_state_init(struct key_state* key_state)
{
    memset(key_state, 0, sizeof(*key_state) * SDL_NUM_SCANCODES);
}

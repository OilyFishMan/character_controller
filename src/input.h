#ifndef INPUT_H_
#define INPUT_H_

#include "int_types.h"

#include <assert.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

enum key_state_type
{
    E_key_state_released = 0,
    E_key_state_down,
    E_key_state_repeat,
};

static_assert(E_key_state_released == 0, "Oh no, something has gone horribly wrong");

struct key_state
{
    enum key_state_type type;
    usize tick;
};

#define KEY_STATES_DEFAULT ((struct key_state[SDL_NUM_SCANCODES]) { 0 })

#define KEY_STATE(_key_states, _key) ((_key_states)[SDL_GetScancodeFromKey((_key))])
#define KEY_STATE_PRESSED(_key_states, _key) (KEY_STATE(_key_states, _key).type != E_key_state_released)
#define KEY_STATE_PRESSED_ON(_key_states, _key, _tick) \
    ({ struct key_state _state = KEY_STATE(_key_states, _key); _state.tick == (_tick) && _state.type == E_key_state_down; })

#endif // INPUT_H_

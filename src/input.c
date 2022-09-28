/* -----------------------------------------------------------------------------
    INCLUDES & GLOBALS
----------------------------------------------------------------------------- */

#include <stdbool.h>
#include <GLFW/glfw3.h>
#include "../include/input.h"

#define MAX_INTEGER_BITS 128

int mouse_x = 0, mouse_y = 0;
__uint128_t printable_key_pressed = 0, function_key_pressed = 0;

/* -----------------------------------------------------------------------------
    PUBLIC FUNCTIONS
----------------------------------------------------------------------------- */

bool tecla_apertada(int tecla) {

    __uint128_t *key_pressed = NULL;

    if (tecla < MAX_INTEGER_BITS)
        key_pressed = &printable_key_pressed;

    else {
        key_pressed = &function_key_pressed;
        tecla -= MAX_INTEGER_BITS * 2;
    }

    return (*key_pressed & (((__uint128_t)1ULL) << tecla)) > 0ULL;
}

/* -----------------------------------------------------------------------------
    PRIVATE FUNCTIONS
----------------------------------------------------------------------------- */

void _mouse_position_callback(GLFWwindow* window, double x, double y) {
    mouse_x = x;
    mouse_y = y;
}

// -----------------------------------------------------------------------------

void _keyboard_key_callback(
    GLFWwindow* window, int key, int scancode, int action, int mods
) {

    __uint128_t *key_pressed = NULL;

    if (key < MAX_INTEGER_BITS)
        key_pressed = &printable_key_pressed;

    else {
        key_pressed = &function_key_pressed;
        key -= MAX_INTEGER_BITS * 2;
    }

    if (action)
        *key_pressed |= ((__uint128_t)1ULL) << key;

    else *key_pressed &= (~(((__uint128_t)1ULL) << key));
}
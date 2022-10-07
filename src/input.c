/* -----------------------------------------------------------------------------
    INCLUDES & GLOBALS
----------------------------------------------------------------------------- */

#include <stdbool.h>
#include <GLFW/glfw3.h>
#include "../include/input.h"

#define MAX_INTEGER_BITS 128

int mouse_x = 0, mouse_y = 0;
__uint128_t printable_key_pressed = 0, function_key_pressed = 0, 
            last_printable_key_pressed = 0, last_function_key_pressed = 0;

short int mouse_button_pressed = 0, last_mouse_button_pressed = 0;

double roda_do_mouse = 0;

/* -----------------------------------------------------------------------------
    FORWARD DECLARATIONS
----------------------------------------------------------------------------- */

void _keyboard_key_state(
    int *tecla, __uint128_t *key_pressed, __uint128_t *last_key_pressed
);

/* -----------------------------------------------------------------------------
    PUBLIC FUNCTIONS
----------------------------------------------------------------------------- */

bool tecla_pressionada(int tecla) {

    __uint128_t key_pressed, _;
    _keyboard_key_state(&tecla, &key_pressed, &_);

    return (key_pressed & (((__uint128_t)1ULL) << tecla)) > 0ULL;

}

// -----------------------------------------------------------------------------

bool tecla_acabou_de_ser_apertada(int tecla) {

    __uint128_t key_pressed, last_key_pressed;
    _keyboard_key_state(&tecla, &key_pressed, &last_key_pressed);

    return ((key_pressed & (((__uint128_t)1ULL) << tecla)) > 0ULL) &&
        ((last_key_pressed & (((__uint128_t)1ULL) << tecla)) == 0ULL);

}

// -----------------------------------------------------------------------------

bool tecla_acabou_de_ser_solta(int tecla) {
    
    __uint128_t key_pressed, last_key_pressed;
    _keyboard_key_state(&tecla, &key_pressed, &last_key_pressed);

    return ((key_pressed & (((__uint128_t)1ULL) << tecla)) == 0ULL) &&
        ((last_key_pressed & (((__uint128_t)1ULL) << tecla)) > 0ULL);

}

// -----------------------------------------------------------------------------

bool botao_mouse_pressionado(int botao) {
    return (mouse_button_pressed & (1 << botao)) > 0;
}

// -----------------------------------------------------------------------------

bool botao_mouse_acabou_de_ser_apertado(int botao) {
    return ((mouse_button_pressed & (1 << botao)) > 0) &&
        ((last_mouse_button_pressed & (1 << botao)) == 0);
}

// -----------------------------------------------------------------------------

bool botao_mouse_acabou_de_ser_solto(int botao) {
    return ((mouse_button_pressed & (1 << botao)) == 0) &&
        ((last_mouse_button_pressed & (1 << botao)) > 0);
}

/* -----------------------------------------------------------------------------
    PRIVATE FUNCTIONS
----------------------------------------------------------------------------- */

void _mouse_position_callback(GLFWwindow* window, double x, double y) {
    mouse_x = x;
    mouse_y = y;
}

// -----------------------------------------------------------------------------

void _keyboard_key_state(
    int *tecla, __uint128_t *key_pressed, __uint128_t *last_key_pressed
) {

    if (*tecla < MAX_INTEGER_BITS) {
        *key_pressed = printable_key_pressed;
        *last_key_pressed = last_printable_key_pressed;
    }

    else {
        *key_pressed = function_key_pressed;
        *last_key_pressed = last_function_key_pressed;
        *tecla -= MAX_INTEGER_BITS * 2;
    }

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

// -----------------------------------------------------------------------------

void _mouse_button_callback(
    GLFWwindow* window, int button, int action, int mods
) {

    if (action)
        mouse_button_pressed |= 1 << button;

    else mouse_button_pressed &= ~(1 << button);

}

// -----------------------------------------------------------------------------

void _mouse_scroll_wheel_callback(
    GLFWwindow* window, double xoffset, double yoffset
) {
    roda_do_mouse += yoffset;
}

// -----------------------------------------------------------------------------

void _update_input_system() {
    last_printable_key_pressed = printable_key_pressed;
    last_function_key_pressed = function_key_pressed;
    last_mouse_button_pressed = mouse_button_pressed;
    roda_do_mouse = 0;
}
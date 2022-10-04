#ifndef INPUT_H
#define INPUT_H

/* -----------------------------------------------------------------------------
    INCLUDES & GLOBALS
----------------------------------------------------------------------------- */

#include <GLFW/glfw3.h>

extern int mouse_x, mouse_y;

// Key Constants -------------------------------------------
#define ESC             GLFW_KEY_ESCAPE
#define ENTER           GLFW_KEY_ENTER
#define ESPACO          GLFW_KEY_SPACE
#define TAB             GLFW_KEY_TAB
#define BACKSPACE       GLFW_KEY_BACKSPACE
#define INSERT          GLFW_KEY_INSERT
#define DELETE          GLFW_KEY_DELETE
#define DIREITA         GLFW_KEY_RIGHT
#define ESQUERDA        GLFW_KEY_LEFT
#define CIMA            GLFW_KEY_UP
#define BAIXO           GLFW_KEY_DOWN
#define PAGE_UP         GLFW_KEY_PAGE_UP
#define PAGE_DOWN       GLFW_KEY_PAGE_DOWN
#define HOME            GLFW_KEY_HOME
#define END             GLFW_KEY_END
#define CAPS_LOCK       GLFW_KEY_CAPS_LOCK
#define SCROLL_LOCK     GLFW_KEY_SCROLL_LOCK
#define NUM_LOCK        GLFW_KEY_NUM_LOCK
#define PRINT_SCREEN    GLFW_KEY_PRINT_SCREEN
#define PAUSE           GLFW_KEY_PAUSE
#define F1              GLFW_KEY_F1
#define F2              GLFW_KEY_F2
#define F3              GLFW_KEY_F3
#define F4              GLFW_KEY_F4
#define F5              GLFW_KEY_F5
#define F6              GLFW_KEY_F6
#define F7              GLFW_KEY_F7
#define F8              GLFW_KEY_F8
#define F9              GLFW_KEY_F9
#define F10             GLFW_KEY_F10
#define F11             GLFW_KEY_F11
#define F12             GLFW_KEY_F12

#define BOTAO_ESQUERDO GLFW_MOUSE_BUTTON_LEFT
#define BOTAO_DIREITO  GLFW_MOUSE_BUTTON_RIGHT
#define BOTAO_MEIO     GLFW_MOUSE_BUTTON_MIDDLE

/* -----------------------------------------------------------------------------
    PUBLIC FUNCTIONS
----------------------------------------------------------------------------- */

// Teclado -------------------------------------------------
bool tecla_pressionada(int tecla);
bool tecla_acabou_de_ser_apertada(int tecla);
bool tecla_acabou_de_ser_solta(int tecla);

// Mouse ---------------------------------------------------
bool botao_mouse_pressionado(int botao);
bool botao_mouse_acabou_de_ser_apertado(int botao);
bool botao_mouse_acabou_de_ser_solto(int botao);

/* -----------------------------------------------------------------------------
    PRIVATE FUNCTIONS
----------------------------------------------------------------------------- */

void _mouse_position_callback(GLFWwindow* window, double x, double y);
void _keyboard_key_callback(
    GLFWwindow* window, int key, int scancode, int action, int mods
);

void _mouse_button_callback(
    GLFWwindow* window, int button, int action, int mods
);

void _update_input_system();

#endif
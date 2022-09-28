#ifndef JOGO_H
#define JOGO_H

/* -----------------------------------------------------------------------------
    INCLUDES & GLOBALS
----------------------------------------------------------------------------- */

#include <stdbool.h>
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include "display.h"
#include "image.h"
#include "input.h"
#include "../src/macros.h"

extern GLFWwindow *window;

/* -----------------------------------------------------------------------------
    FUNCTIONS
----------------------------------------------------------------------------- */

void abre_janela();
void fecha_janela();
bool janela_esta_aberta();
void tela_cheia();
void janela_deve_fechar();

#endif
#ifndef IMAGE_H
#define IMAGE_H

/* -----------------------------------------------------------------------------
    INCLUDES
----------------------------------------------------------------------------- */

#include "display.h"

/* -----------------------------------------------------------------------------
    PUBLIC FUNCTIONS
----------------------------------------------------------------------------- */

DISPLAY_FUNC(desenha_imagem, char *arquivo);

void fonte(char *arquivo, float tamanho);
DISPLAY_FUNC(desenha_texto, char *texto);

/* -----------------------------------------------------------------------------
    PRIVATE FUNCTIONS
----------------------------------------------------------------------------- */

void _free_sprite_hashmap();
void _free_font_hashmap();

#endif
#ifndef AUDIO_H
#define AUDIO_H

/* -----------------------------------------------------------------------------
    INCLUDES
----------------------------------------------------------------------------- */

#include <stdbool.h>

/* -----------------------------------------------------------------------------
    PUBLIC FUNCTIONS
----------------------------------------------------------------------------- */

void toca_som_em_loop(char *arquivo);
void toca_som(char *arquivo);
void para_som(char *arquivo);
void recomeca_som(char *arquivo);
bool som_esta_tocando(char *arquivo);

/* -----------------------------------------------------------------------------
    PRIVATE FUNCTIONS
----------------------------------------------------------------------------- */

void _initialize_sound_system();
void _terminate_sound_system();

#endif
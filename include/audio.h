/// @file
#ifndef AUDIO_H
#define AUDIO_H

/* -----------------------------------------------------------------------------
    INCLUDES
----------------------------------------------------------------------------- */

#include <stdbool.h>

/* -----------------------------------------------------------------------------
    PUBLIC FUNCTIONS
----------------------------------------------------------------------------- */

/*!
 * @brief Carrega um arquivo de áudio e toca ele em @e loop.
 * @param arquivo   Nome do arquivo de áudio. .MP3, .OGG e .WAV, entre outros.
 * @returns @c void
 */
void toca_som_em_loop(char *arquivo);

/*!
 * @brief Carrega um arquivo de áudio e toca ele.
 * @param arquivo   Nome do arquivo de áudio. .MP3, .OGG e .WAV, entre outros.
 * @returns @c void
 */
void toca_som(char *arquivo);

/*!
 * @brief Para a execução de um áudio.
 * @details Se o áudio não estiver tocando, nada acontece.
 * @param arquivo  Nome do arquivo de áudio.
 * @returns @c void
 */
void para_som(char *arquivo);

/*!
 * @brief Recomeça um áudio que está tocando
 * @details Se o áudio não estiver tocando, ele começa a ser tocado.
 * @param arquivo  Nome do arquivo de áudio.
 * @returns @c void
 */
void recomeca_som(char *arquivo);

/*!
 * @brief Verifica se um áudio está tocando.
 * @param arquivo  Nome do arquivo de áudio.
 * @returns @c true se o áudio estiver tocando, @c false caso contrário.
 */
bool som_esta_tocando(char *arquivo);

/* -----------------------------------------------------------------------------
    PRIVATE FUNCTIONS
----------------------------------------------------------------------------- */

void _initialize_sound_system();
void _terminate_sound_system();

#endif
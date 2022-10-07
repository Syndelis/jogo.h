/// @file
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
#include "audio.h"
#include "common.h"

/*!
 * @brief Pode ser passado para a função @ref abre_janela() para abrir o jogo em
 * tela cheia.
 */
#define TELA_CHEIA 0x0FFFFFFF

/*!
 * @var largura_janela
 * @brief Largura da janela. Útil para saber qual é o maior X possível.
 */

/*!
 * @var altura_janela
 * @brief Altura da janela. Útil para saber qual é o maior Y possível.
 */
extern int largura_janela, altura_janela;

extern GLFWwindow *window;

/* -----------------------------------------------------------------------------
    PUBLIC FUNCTIONS
----------------------------------------------------------------------------- */
/*!
 * @def abre_janela(int largura, int altura)
 * @brief Abre a janela do jogo.
 * @param largura   Largura da janela. Opcionalmente pode ser @ref TELA_CHEIA.
 * @param altura   Altura da janela. Só deve aparecer quando @p largura
 *                     não for @ref TELA_CHEIA. @b Opcional
 * @returns @c void
 */
#define abre_janela(...) _abre_janela(NARGS(__VA_ARGS__), __VA_ARGS__)

/*!
 * @brief Fecha a janela e termina o jogo.
 * @returns @c void
 */
void fecha_janela();

/*!
 * @brief Verifica se a janela do jogo está aberta.
 * @returns @c true se a janela estiver aberta, @c false caso contrário. Esse
 *          retorno pode ser controlado pela função @ref fecha_janela().
 */
bool janela_esta_aberta();

/*!
 * @brief Marca a janela para fechar. Isso significa que
 *          @ref janela_esta_aberta() retornará @c false na próxima vez que
 *         for chamada.
 * @returns @c void
 */
void janela_deve_fechar();

/* -----------------------------------------------------------------------------
    PRIVATE FUNCTIONS
----------------------------------------------------------------------------- */

// If removed, triggers a "-Wimplicit-function-declaration", but works
// nonetheless

void _abre_janela(int n, ...);

#endif
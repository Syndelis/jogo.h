/// @file
#ifndef DISPLAY_H
#define DISPLAY_H

/* -----------------------------------------------------------------------------
    INCLUDES & CONSTANTS
----------------------------------------------------------------------------- */

#include <stdarg.h>
#include "common.h"

// Color Constants -----------------------------------------

/*!
 * @enum Cores
 * @brief Cores que podem ser usadas na função @ref cor()
 */
enum Cores {
    VERMELHO        = 0xFF0000,
    VERDE           = 0x00FF00,
    AZUL            = 0x0000FF,
    AMARELO         = 0xFFFF00,
    MANGETA         = 0xFF00FF,
    CIANO           = 0x00FFFF,
    BRANCO          = 0xFFFFFF,
    PRETO           = 0x000000,
    CINZA           = 0x808080,
    LARANJA         = 0xFFA500,
    ROSA            = 0xFFC0CB,
    ROXO            = 0x800080,
    MARROM          = 0x8B4513,
    VERDE_CLARO     = 0x00FF7F,
    AZUL_CLARO      = 0xADD8E6,
    AZUL_ESCURO     = 0x00008B,
    VERDE_ESCURO    = 0x006400,
    VERMELHO_ESCURO = 0x8B0000,
    CINZA_ESCURO    = 0x494949,
    CINZA_CLARO     = 0xD3D3D3,
    LARANJA_ESCURO  = 0xCB2701,
    LARANJA_CLARO   = 0xF0A150,
    ROSA_ESCURO     = 0xFF1493,
    ROSA_CLARO      = 0xFFC0CB,
    ROXO_ESCURO     = 0x9400D3,
    PURPURA         = 0x9370DB,
    VINHO           = 0x800000,
    MARROM_ESCURO   = 0x452209,
    MARROM_CLARO    = 0x854112,
};

/* -----------------------------------------------------------------------------
    PUBLIC FUNCTIONS
----------------------------------------------------------------------------- */

#define DISPLAY_FUNC(func, ...) void func(int x, int y, ##__VA_ARGS__)

// Functions -----------------------------------------------

/*!
 * @def cor(int r, int g, int b, [a])
 * @brief Define a cor para os próximos desenhos de figuras geométricas, textos
 *          e imagens.
 * @details Essa função pode operar de duas maneiras:
 *             - Passando 1 parâmetro: esse parâmetro deve ser uma das
 *                  constantes definidas em @ref Cores;
 *             - Passando 3 ou 4 parâmetros: esses parâmetros devem ser
 *                 inteiros entre 0 e 255, representando os valores de vermelho,
 *                 verde e azul, respectivamente. O quarto parâmetro é opcional
 *                 e representa o valor de transparência.
 * 
 * @param vermelho_ou_constante  Valor de vermelho (0-255) @b OU uma das
 *                               constantes definidas em @ref Cores.
 * @param verde                  Valor de verde (0-255). @b Opcional
 * @param azul                   Valor de azul (0-255). @b Opcional
 * @param alfa                   Valor de transparência (0-255). @b Opcional
 */
#define cor(...) _cor(NARGS(__VA_ARGS__), __VA_ARGS__)
/*!
 * @example cor
 * @code
 * #include <jogo.h>
 * 
 * int main() {
 *     abre_janela(800, 600);
 * 
 *     while (janela_esta_aberta()) {
 *         cor(AMARELO); // <--- Define que as próximas figuras serão amarelas
 *         desenha_retangulo(100, 100, 200, 200);
 * 
 *         cor(255, 0, 255, 127); // <--- Define que as próximas figuras serão roxas semi-transparentes
 *         desenha_retangulo(300, 100, 200, 200);
 *     }
 * 
 *    fecha_janela();
 * }
 * @endcode
 */

/*!
 * @brief Desenha um retângulo.
 * @param x,y       Posição do retângulo na tela.
 * @param largura,altura       Dimensões do retângulo.
 * @returns @c void
 */
DISPLAY_FUNC(desenha_retangulo, int largura, int altura);

/* -----------------------------------------------------------------------------
    PRIVATE FUNCTIONS
----------------------------------------------------------------------------- */

// If removed, triggers a "-Wimplicit-function-declaration", but works
// nonetheless
void _cor(int n, ...);

#endif
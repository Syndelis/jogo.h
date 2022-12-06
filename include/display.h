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
    VERMELHO        = 0xFF0000, ///< @color{#FF0000}
    VERDE           = 0x00FF00, ///< @color{#00FF00}
    AZUL            = 0x0000FF, ///< @color{#0000FF}
    AMARELO         = 0xFFFF00, ///< @color{#FFFF00}
    MANGETA         = 0xFF00FF, ///< @color{#FF00FF}
    CIANO           = 0x00FFFF, ///< @color{#00FFFF}
    BRANCO          = 0xFFFFFF, ///< @color{#FFFFFF}
    PRETO           = 0x000000, ///< @color{#000000}
    CINZA           = 0x808080, ///< @color{#808080}
    LARANJA         = 0xFFA500, ///< @color{#FFA500}
    ROSA            = 0xFFC0CB, ///< @color{#FFC0CB}
    ROXO            = 0x800080, ///< @color{#800080}
    MARROM          = 0x8B4513, ///< @color{#8B4513}
    VERDE_CLARO     = 0x00FF7F, ///< @color{#00FF7F}
    AZUL_CLARO      = 0xADD8E6, ///< @color{#ADD8E6}
    AZUL_ESCURO     = 0x00008B, ///< @color{#00008B}
    VERDE_ESCURO    = 0x006400, ///< @color{#006400}
    VERMELHO_ESCURO = 0x8B0000, ///< @color{#8B0000}
    CINZA_ESCURO    = 0x494949, ///< @color{#494949}
    CINZA_CLARO     = 0xD3D3D3, ///< @color{#D3D3D3}
    LARANJA_ESCURO  = 0xCB2701, ///< @color{#CB2701}
    LARANJA_CLARO   = 0xF0A150, ///< @color{#F0A150}
    ROSA_ESCURO     = 0xFF1493, ///< @color{#FF1493}
    ROSA_CLARO      = 0xFFC0CB, ///< @color{#FFC0CB}
    ROXO_ESCURO     = 0x9400D3, ///< @color{#9400D3}
    PURPURA         = 0x9370DB, ///< @color{#9370DB}
    VINHO           = 0x800000, ///< @color{#800000}
    MARROM_ESCURO   = 0x452209, ///< @color{#452209}
    MARROM_CLARO    = 0x854112, ///< @color{#854112}
};

/*!
 * @typedef ponto
 * @brief Um ponto no plano cartesiano. É um vetor de 2 inteiros {x, y}.
 */
typedef int ponto[2];

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

#define cor(...) \
    _Generic( \
            &(int[]){__VA_ARGS__}, \
            int(*)[1]: _cor_1, \
            int(*)[2]: _cor_2, \
            int(*)[3]: _cor_3, \
            int(*)[4]: _cor_4, \
            default: _cor_gt_4 \
        )(__VA_ARGS__)

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

/*!
 * @brief Desenha um círculo.
 * @param x,y       Posição do círculo na tela.
 * @param raio      Raio do círculo.
 * @returns @c void
 */
DISPLAY_FUNC(desenha_circulo, int raio);

/*!
 * @brief Desenha uma linha.
 * @param x,y     Posição inicial da linha.
 * @param x2,y2   Posição final da linha.
 * @returns @c void
 */
DISPLAY_FUNC(desenha_linha, int x2, int y2);

/*!
 * @brief Determina se as próximas figuras devem ser preenchidas ou somente deve
            ser desenhado o contorno.
 * @param ativo     @c true para preenchimento, @c false para contorno.
 * @returns @c void
 */
void preenchimento(bool ativo);

/*!
 * @brief Define a espessura da linha usada para desenhar todas as formas
 *         geométricas.
 * @param espessura     Espessura da linha. @c 1.0 é o valor padrão.
 * @returns @c void
 */
void espessura(float espessura);

/*!
 * @brief Desenha um polígono arbitrário.
 * @param pontos    Vetor de @ref ponto's que formam o polígono.
 * @param qtd_pontos    Número de pontos no vetor.
 * @returns @c void
 */
void desenha_poligono(ponto *pontos, int qtd_pontos);

/*!
 * @brief Rotaciona os próximos desenhos em um @ref angulo específico em torno
 *          do ponto @ref x, @ref y
 * @param x,y       Âncora de rotação.
 * @param angulo      Ângulo de rotação em radianos.
 * @returns @c void
 */
DISPLAY_FUNC(rotacao, double angulo);

/*!
 * @brief Reinicia a rotação para o valor padrão.
 * @returns @c void
 */
void reseta_rotacao();

/*!
 * @brief Aplica zoom para os próximos desenhos de @ref fator no ponto
 *          @ref x, @ref y
 * @param x,y       Âncora de zoom.
 * @param fator     Fator de zoom a ser aplicado (1.0 = 100%, zoom normal).
 * @returns @c void
 */
DISPLAY_FUNC(zoom, double fator);

/*!
 * @brief Reinicia o zoom para o valor padrão.
 * @returns @c void
 */
void reseta_zoom();

/* -----------------------------------------------------------------------------
    PRIVATE FUNCTIONS
----------------------------------------------------------------------------- */

// If removed, triggers a "-Wimplicit-function-declaration", but works
// nonetheless

void _cor_1(int rgb_hex);
void _cor_3(int r, int g, int b);
void _cor_4(int r, int g, int b, int a);


__attribute__((error("\033[1;41mERRO! Função `cor` chamada com \033[1;4;43m2\033[1;24;41m parâmetros!\033[m"))) void _cor_2(int _x, ...);
__attribute__((error("\033[1;41mERRO! Função `cor` chamada com \033[1;4;43mmais de 4\033[1;24;41m parâmetros!\033[m"))) void _cor_gt_4(int _x, ...);

void _update_drawing_system();

#endif
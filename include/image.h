/// @file
#ifndef IMAGE_H
#define IMAGE_H

/* -----------------------------------------------------------------------------
    INCLUDES
----------------------------------------------------------------------------- */

#include "display.h"

/* -----------------------------------------------------------------------------
    PUBLIC FUNCTIONS
----------------------------------------------------------------------------- */

/*!
 * @brief Desenha uma imagem na tela.
 * @details O arquivo pode ser uma imagem estática (.PNG, .JPG, etc.) ou animada
 *         (.GIF).
 * @param x,y       Posição da imagem na tela.
 * @param arquivo   Nome do arquivo da imagem.
 * @returns @c void
 */
DISPLAY_FUNC(desenha_imagem, char *arquivo);
/*!
 * @example desenha_imagem
 * @code
 * #include <jogo.h>
 * 
 * int main() {
 *     abre_janela(TELA_CHEIA);
 * 
 *     while (janela_esta_aberta())
 *         desenha_imagem(50, 50, "assets/tux.png"); // <--- Desenha a imagem na posição (x, y) = (50, 50)
 * 
 *    fecha_janela();
 * }
 * @endcode
 */

/*!
 * @brief Escolhe uma fonte para ser usada para as subsequentes chamadas de 
 *        @ref desenha_texto().
 * @param arquivo   Nome do arquivo da fonte.
 * @param tamanho   Tamanho da fonte.
 * @returns @c void
 */
void fonte(char *arquivo, float tamanho);

/*!
 * @brief Desenha um texto na tela usando a última fonte carregada.
 * @param x,y       Posição do texto na tela.
 * @param texto     Texto a ser desenhado.
 * @param[in] ...   Argumentos opcionais para formatação do texto.
 * @returns @c void
 */
DISPLAY_FUNC(desenha_texto, char *texto, ...);
/*!
 * @example desenha_texto
 * @code
 * #include <jogo.h>
 * 
 * int main() {
 *     abre_janela(TELA_CHEIA);
 * 
 *     while (janela_esta_aberta()) {
 *         fonte("assets/UbuntuMono-R.ttf", 20); // <--- Escolhe a fonte
 *         desenha_texto(50, 50, "Olá, mundo!"); // <--- Desenha o texto na posição (x, y) = (50, 50)
 *     }
 * 
 *    fecha_janela();
 * }
 * @endcode
 */

/* -----------------------------------------------------------------------------
    PRIVATE FUNCTIONS
----------------------------------------------------------------------------- */

void _free_sprite_hashmap();
void _free_font_hashmap();

void _initialize_text_system();
void _terminate_text_system();

#endif
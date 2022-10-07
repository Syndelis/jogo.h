/// @file
#ifndef INPUT_H
#define INPUT_H

/* -----------------------------------------------------------------------------
    INCLUDES & GLOBALS
----------------------------------------------------------------------------- */

#include <GLFW/glfw3.h>

/*!
 * @var mouse_x
 * @brief Posição X do mouse.
 */

/*!
 * @var mouse_y
 * @brief Posição Y do mouse.
 */
extern int mouse_x, mouse_y;

/*!
 * @var roda_do_mouse
 * @brief Quantidade que a roda do mouse foi girada no último passo de tempo.
 * @details Positivo significa rolagem para cima, enquanto negativo significa
 *          rolagem para baixo. Note que este valor é resetado a cada passo de
 *         tempo.
 */
extern double roda_do_mouse;

/*!
 * @enum TeclasEspeciais
 * @brief Teclas que não são letras ou números.
 * @details Podem ser usadas com todas as funções da família @c tecla_*(), como
 *          @ref tecla_acabou_de_ser_apertada(), @ref tecla_pressionada() e
 *          @ref tecla_acabou_de_ser_solta().
 */
enum TeclasEspeciais {
    ESC             = GLFW_KEY_ESCAPE,          ///< Tecla @b Esc
    ENTER           = GLFW_KEY_ENTER,           ///< Tecla @b Enter
    ESPACO          = GLFW_KEY_SPACE,           ///< Tecla @b Espaço
    TAB             = GLFW_KEY_TAB,             ///< Tecla @b Tab
    BACKSPACE       = GLFW_KEY_BACKSPACE,       ///< Tecla @b Backspace
    INSERT          = GLFW_KEY_INSERT,          ///< Tecla @b Insert
    DELETE          = GLFW_KEY_DELETE,          ///< Tecla @b Delete
    DIREITA         = GLFW_KEY_RIGHT,           ///< Setinha para a @b direita
    ESQUERDA        = GLFW_KEY_LEFT,            ///< Setinha para a @b esquerda
    CIMA            = GLFW_KEY_UP,              ///< Setinha para @b cima
    BAIXO           = GLFW_KEY_DOWN,            ///< Setinha para @b baixo
    PAGE_UP         = GLFW_KEY_PAGE_UP,         ///< Tecla <b>Page Up</b>
    PAGE_DOWN       = GLFW_KEY_PAGE_DOWN,       ///< Tecla <b>Page Down</b>
    HOME            = GLFW_KEY_HOME,            ///< Tecla <b>Home</b>
    END             = GLFW_KEY_END,             ///< Tecla <b>End</b>
    CAPS_LOCK       = GLFW_KEY_CAPS_LOCK,       ///< Tecla <b>Caps Lock</b>
    SCROLL_LOCK     = GLFW_KEY_SCROLL_LOCK,     ///< Tecla <b>Scroll Lock</b>
    NUM_LOCK        = GLFW_KEY_NUM_LOCK,        ///< Tecla <b>Num Lock</b>
    PRINT_SCREEN    = GLFW_KEY_PRINT_SCREEN,    ///< Tecla <b>Print Screen</b>
    PAUSE           = GLFW_KEY_PAUSE,           ///< Tecla @b Pause
    F1              = GLFW_KEY_F1,              ///< Tecla @b F1
    F2              = GLFW_KEY_F2,              ///< Tecla @b F2
    F3              = GLFW_KEY_F3,              ///< Tecla @b F3
    F4              = GLFW_KEY_F4,              ///< Tecla @b F4
    F5              = GLFW_KEY_F5,              ///< Tecla @b F5
    F6              = GLFW_KEY_F6,              ///< Tecla @b F6
    F7              = GLFW_KEY_F7,              ///< Tecla @b F7
    F8              = GLFW_KEY_F8,              ///< Tecla @b F8
    F9              = GLFW_KEY_F9,              ///< Tecla @b F9
    F10             = GLFW_KEY_F10,             ///< Tecla @b F10
    F11             = GLFW_KEY_F11,             ///< Tecla @b F11
    F12             = GLFW_KEY_F12,             ///< Tecla @b F12
};

/*!
 * @enum BotoesMouse
 * @brief Botões do mouse.
 * @details Podem ser usados com todas as funções da família @c botao_mouse_*(),
 *          como @ref botao_mouse_acabou_de_ser_apertado(),
 *          @ref botao_mouse_acabou_de_ser_solto() e
 *          @ref botao_mouse_pressionado().
 */
enum BotoesMouse {
    BOTAO_ESQUERDO  =GLFW_MOUSE_BUTTON_LEFT,    ///< Botão @b esquerdo do mouse
    BOTAO_DIREITO   =GLFW_MOUSE_BUTTON_RIGHT,   ///< Botão @b direito do mouse
    BOTAO_MEIO      =GLFW_MOUSE_BUTTON_MIDDLE,  ///< Botão do @b meio do mouse (Pressionar a rodinha)
};

/* -----------------------------------------------------------------------------
    PUBLIC FUNCTIONS
----------------------------------------------------------------------------- */

// Teclado -------------------------------------------------

/*!
 * @brief Verifica se a tecla está apertada.
 * @param tecla Tecla a ser verificada.
 * @return Retorna @c true se a tecla está sendo pressionada
 */
bool tecla_pressionada(int tecla);

/*!
 * @brief Verifica se a tecla foi apertada.
 * @param tecla Tecla a ser verificada.
 * @return Retorna @c true se a tecla acabou de ser apertada.
 */
bool tecla_acabou_de_ser_apertada(int tecla);

/*!
 * @brief Verifica se a tecla foi solta.
 * @param tecla Tecla a ser verificada.
 * @return Retorna @c true se a tecla acabou de ser solta.
 */
bool tecla_acabou_de_ser_solta(int tecla);

// Mouse ---------------------------------------------------

/*!
 * @brief Verifica se o botão do mouse está apertado.
 * @param botao Botão a ser verificado.
 * @return Retorna @c true se o botão do mouse está sendo pressionado.
 */
bool botao_mouse_pressionado(int botao);

/*!
 * @brief Verifica se o botão do mouse foi apertado.
 * @param botao Botão a ser verificado.
 * @return Retorna @c true se o botão do mouse acabou de ser apertado.
 */
bool botao_mouse_acabou_de_ser_apertado(int botao);

/*!
 * @brief Verifica se o botão do mouse foi solto.
 * @param botao Botão a ser verificado.
 * @return Retorna @c true se o botão do mouse acabou de ser solto.
 */
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

void _mouse_scroll_wheel_callback(
    GLFWwindow* window, double xoffset, double yoffset
);

void _update_input_system();

#endif
/* -----------------------------------------------------------------------------
    INCLUDES & GLOBALS
----------------------------------------------------------------------------- */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdarg.h>

#define GLEW_STATIC
#include <GL/glew.h>

#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include "../include/jogo.h"
#include "../include/image.h"
#include "../include/input.h"
#include "../include/audio.h"
#include "../include/common.h"

GLFWwindow *window = NULL;
int largura_janela = 0, altura_janela = 0;

#define BASE_GLEW_ERROR JOGO_ERROR " Não foi possível inicializar o GLEW:\n"

/* -----------------------------------------------------------------------------
    FORWARD DECLARATIONS
----------------------------------------------------------------------------- */

void _window_framebuffer_size_callback(GLFWwindow* window, int width, int height);

/* -----------------------------------------------------------------------------
    PUBLIC FUNCTIONS
----------------------------------------------------------------------------- */

void _abre_janela(int n, ...) {

    if (n > 2) {
        fprintf(
            stderr,
            JOGO_ERROR "abre_janela() recebe 1 ou 2 parâmetros. "
            "%d foram passados!\n"
            "Exemplos de chamadas corretas para essa função:\n"
            "\tabre_janela(TELA_CHEIA); Abre o jogo em tela cheia\n"
            "\tabre_janela(800, 600); Abre o jogo nas dimensões 800x600\n",
            n
        );
        exit(EXIT_FAILURE);
    }

    if (!glfwInit())
        exit(EXIT_FAILURE);

    va_list args;
    va_start(args, n);

    int largura = va_arg(args, int);

    // A NULL monitor will create a "windowed window", while an actual monitor
    // pointer will create a fullscreen window
    GLFWmonitor *monitor = NULL;

    if (n == 1 && largura == TELA_CHEIA) {
        monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode *mode = glfwGetVideoMode(monitor);

        largura_janela = mode->width;
        altura_janela = mode->height;
    }

    else {
        int altura = va_arg(args, int);

        largura_janela = largura;
        altura_janela = altura;
    }

    va_end(args);

    // GLFW Setup ----------------------

    window = glfwCreateWindow(
        largura_janela, altura_janela,
        "Jogo em C", monitor, NULL
    );

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    glfwSetCursorPosCallback(window, _mouse_position_callback);
    glfwSetKeyCallback(window, _keyboard_key_callback);
    glfwSetMouseButtonCallback(window, _mouse_button_callback);
    glfwSetScrollCallback(window, _mouse_scroll_wheel_callback);
    glfwSetFramebufferSizeCallback(window, _window_framebuffer_size_callback);

    // GL Setup ------------------------

    glClearColor(.0f, .0f, .0f, 1.f);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_MULTISAMPLE);

    _window_framebuffer_size_callback(window, largura_janela, altura_janela);

    // GLEW Setup ----------------------

    GLenum err = glewInit();
    switch (err) {

        case GLEW_OK:
            break;

        case GLEW_ERROR_NO_GL_VERSION:
            fprintf(
                stderr,
                BASE_GLEW_ERROR
                "\tNão foi possível detectar a versão do OpenGL.\n"
            );
            exit(EXIT_FAILURE);
            break;

        case GLEW_ERROR_GL_VERSION_10_ONLY:
            fprintf(
                stderr,
                BASE_GLEW_ERROR
                "\tA versão do OpenGL não atende o requisito mínimo >= 1.1.\n"
            );
            exit(EXIT_FAILURE);
            break;

        case GLEW_ERROR_GLX_VERSION_11_ONLY:
            fprintf(
                stderr,
                BASE_GLEW_ERROR
                "\tA versão do OpenGL não atende o requisito mínimo >= 1.2.\n"
            );
            exit(EXIT_FAILURE);
            break;

        case GLEW_ERROR_NO_GLX_DISPLAY:
            fprintf(
                stderr,
                BASE_GLEW_ERROR
                "\tNão foi possível detectar o display.\n"
            );
            exit(EXIT_FAILURE);
            break;

    }

    // Additional Setups ---------------

    _initialize_text_system();
    _initialize_sound_system();

}

// ---------------------------------------------------------

void fecha_janela() {

    glfwDestroyWindow(window);
    glfwTerminate();

    _free_sprite_hashmap();
    _free_font_hashmap();

    _terminate_text_system();
    _terminate_sound_system();

}

// ---------------------------------------------------------

bool janela_esta_aberta() {

    _update_input_system();
    _update_drawing_system();

    glfwPollEvents();
    bool is_open = !glfwWindowShouldClose(window);

    if (is_open) {
        glFlush();
        glfwSwapBuffers(window);

        glClear(GL_COLOR_BUFFER_BIT);
        glColor4f(1, 1, 1, 1);
        glPointSize(1.f);
        glLineWidth(1.f);
    }

    return is_open;

}

// ---------------------------------------------------------

void janela_deve_fechar() {
    glfwSetWindowShouldClose(window, true);
}

/* -----------------------------------------------------------------------------
    PRIVATE FUNCTIONS
----------------------------------------------------------------------------- */

void _window_framebuffer_size_callback(GLFWwindow *window, int width, int height) {

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, width, height, 0, 1, -1);

    largura_janela = width;
    altura_janela = height;

}
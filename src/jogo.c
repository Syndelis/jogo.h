/* -----------------------------------------------------------------------------
    INCLUDES & GLOBALS
----------------------------------------------------------------------------- */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include "../include/jogo.h"
#include "../include/image.h"
#include "../include/input.h"
#include "../include/audio.h"

GLFWwindow *window = NULL;
int window_width = 0, window_height = 0;
bool fullscreen = false;

#define BASE_GLEW_ERROR "\033[32;41mERRO!\033[m Não foi possível inicializar o GLEW:\n"

/* -----------------------------------------------------------------------------
    PUBLIC FUNCTIONS
----------------------------------------------------------------------------- */

void tela_cheia() {
    fullscreen = true;
}

void abre_janela(int largura, int altura) {

    // A NULL monitor will create a "windowed window", while an actual monitor
    // pointer will create a fullscreen window
    GLFWmonitor *monitor = NULL;

    if (fullscreen) {
        monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode *mode = glfwGetVideoMode(monitor);

        window_width = mode->width;
        window_height = mode->height;
    }

    else {
        window_width = largura;
        window_height = altura;
    }

    // GLFW Setup ----------------------

    if (!glfwInit())
        exit(EXIT_FAILURE);

    window = glfwCreateWindow(
        window_width, window_height,
        "Jogo em C", monitor, NULL
    );

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    glfwSetCursorPosCallback(window, _mouse_position_callback);
    glfwSetKeyCallback(window, _keyboard_key_callback);

    // GL Setup ------------------------

    glClearColor(.0f, .0f, .0f, 1.f);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_MULTISAMPLE);

    glViewport(0, 0, window_width, window_height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, window_width, window_height, 0, 1, -1);

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

    // Sound Setup ---------------------

    _initialize_sound_system();

}

// ---------------------------------------------------------

void fecha_janela() {

    glfwDestroyWindow(window);
    glfwTerminate();

    _free_sprite_hashmap();
    _free_font_hashmap();

    _terminate_sound_system();

}

// ---------------------------------------------------------

bool janela_esta_aberta() {

    glfwPollEvents();
    bool is_open = !glfwWindowShouldClose(window);

    if (is_open) {
        glFlush();
        glfwSwapBuffers(window);

        glClear(GL_COLOR_BUFFER_BIT);
        glColor4f(1, 1, 1, 1);
    }

    return is_open;

}

// ---------------------------------------------------------

void janela_deve_fechar() {
    glfwSetWindowShouldClose(window, true);
}
/* -----------------------------------------------------------------------------
    INCLUDES & GLOBALS
----------------------------------------------------------------------------- */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../include/jogo.h"

GLFWwindow *window = NULL;
int window_width = 0, window_height = 0;
bool fullscreen = false;

/* -----------------------------------------------------------------------------
    PUBLIC FUNCTIONS
----------------------------------------------------------------------------- */

void tela_cheia() {
    fullscreen = true;
}

void abre_janela(int largura, int altura) {

    // A NULL monitor will created a "windowed window", while an actual monitor
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

    // GL Setup ------------------------

    glClearColor(.0f, .0f, .0f, 1.f);
    glEnable(GL_MULTISAMPLE);

    glViewport(0, 0, window_width, window_height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, window_width, window_height, 0, 1, -1);

}

// ---------------------------------------------------------

void fecha_janela() {

    glfwDestroyWindow(window);
    glfwTerminate();

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
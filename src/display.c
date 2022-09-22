/* -----------------------------------------------------------------------------
    INCLUDES & MACRO HELPERS
----------------------------------------------------------------------------- */

#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "../include/display.h"

/* -----------------------------------------------------------------------------
    PUBLIC FUNCTIONS
----------------------------------------------------------------------------- */

DISPLAY_FUNC(desenha_retangulo, int largura, int altura) {
    glBegin(GL_QUADS);
        glVertex2f(x, y);
        glVertex2f(x, y + altura);
        glVertex2f(x + largura, y + altura);
        glVertex2f(x + largura, y);
    glEnd();
}

/* -----------------------------------------------------------------------------
    PRIVATE FUNCTIONS
----------------------------------------------------------------------------- */

void _cor(int n, ...) {
    
    if (n == 2 || n > 4) {
        fprintf(
            stderr,
            "\033[32;41mERRO!\033[m cor() recebe 1, 3 ou 4 parâmetros. "
            "%d foram passados!\n"
            "Exemplos de chamadas corretas para essa função:\n"
            "\tcor(VERMELHO); Produz a cor \033[31mvermelha\033[m\n"
            "\tcor(0, 255, 255); Produz a cor \033[34mciano\033[m\n",
            n
        );
        exit(EXIT_FAILURE);
    }

    va_list args;
    va_start(args, n);

    int arg0 = va_arg(args, int);

    // if only a single parameter was passed, then arg0 is a color constant in
    // the format 0xRRGGBB
    if (n == 1)
        glColor4ub(
            (arg0 >> 16) & 0xFF,
            (arg0 >> 8) & 0xFF,
            arg0 & 0xFF,
            255
        );

    else {
        GLubyte rgba[] = {255, 255, 255, 255};
        for (int i = 0, val = arg0; i < n; i++, val = va_arg(args, int))
            rgba[i] = val;

        glColor4ubv(rgba);
    }

    va_end(args);

}
/* -----------------------------------------------------------------------------
    INCLUDES & MACRO HELPERS
----------------------------------------------------------------------------- */

#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>

#define _USE_MATH_DEFINES
#define __USE_MISC
#include <math.h>

#include "../include/display.h"
#include "../include/common.h"

int _glmode = GL_POLYGON;

/* -----------------------------------------------------------------------------
    PRIVATE STRUCTURES
----------------------------------------------------------------------------- */

typedef struct {
    int x, y;
} Point;

typedef struct {
    Point center;
    double value;
} Effect;

Effect _rotation = {
    .center = (Point) {
        .x = 0,
        .y = 0
    },
    .value = 0
};

Effect _zoom = {
    .center = (Point) {
        .x = 0,
        .y = 0
    },
    .value = 1
};

/* -----------------------------------------------------------------------------
    FORWARD DECLARATIONS
----------------------------------------------------------------------------- */

void _modify_effect(Effect *e, int x, int y, double value);
void _apply_rotation_and_zoom();

/* -----------------------------------------------------------------------------
    PUBLIC FUNCTIONS
----------------------------------------------------------------------------- */

void preenchimento(bool ativo) {
    _glmode = ativo ? GL_POLYGON : GL_LINE_LOOP;
}

// -----------------------------------------------------------------------------

DISPLAY_FUNC(desenha_retangulo, int largura, int altura) {
    glBegin(_glmode);
        glVertex2f(x, y);
        glVertex2f(x, y + altura);
        glVertex2f(x + largura, y + altura);
        glVertex2f(x + largura, y);
    glEnd();
}

// -----------------------------------------------------------------------------

void desenha_poligono(ponto *pontos, int qtd_pontos) {
    glBegin(_glmode);
        for (int i = 0; i < qtd_pontos; i++)
            glVertex2iv(pontos[i]);

    glEnd();
}

// -----------------------------------------------------------------------------

DISPLAY_FUNC(desenha_circulo, int raio) {

    const double DEG2RAD = M_PI * 2 / raio;

    glBegin(_glmode);
        for (int i = 0; i < raio; i++) {
            float rad = i * DEG2RAD;
            glVertex2f(cos(rad) * raio + x, sin(rad) * raio + y);
        }
    glEnd();

}

// -----------------------------------------------------------------------------

DISPLAY_FUNC(rotacao, double angulo) {

    _modify_effect(&_rotation, x, y, angulo);

    glPopMatrix();
    glPushMatrix();

    _apply_rotation_and_zoom();

}

// -----------------------------------------------------------------------------

DISPLAY_FUNC(zoom, double fator) {

    _modify_effect(&_zoom, x, y, fator);

    glPopMatrix();
    glPushMatrix();

    _apply_rotation_and_zoom();

}

// -----------------------------------------------------------------------------

void reseta_zoom() {
    zoom(0, 0, 1);
}

// -----------------------------------------------------------------------------

void reseta_rotacao() {
    rotacao(0, 0, 0);
}

/* -----------------------------------------------------------------------------
    PRIVATE FUNCTIONS
----------------------------------------------------------------------------- */

void _cor(int n, ...) {
    
    if (n == 2 || n > 4) {
        fprintf(
            stderr,
            JOGO_ERROR "cor() recebe 1, 3 ou 4 parâmetros. "
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

// -----------------------------------------------------------------------------

void _update_drawing_system() {
    preenchimento(true);

    GLint stack_depth;
    glGetIntegerv(GL_PROJECTION_STACK_DEPTH, &stack_depth);

    while (stack_depth --> 1)
        glPopMatrix();

}

// -----------------------------------------------------------------------------

void _modify_effect(Effect *e, int x, int y, double value) {
    e->center.x = x;
    e->center.y = y;
    e->value = value;
}

// -----------------------------------------------------------------------------

void _apply_rotation_and_zoom() {

    glTranslatef(_rotation.center.x, _rotation.center.y, 0);
    glRotatef(_rotation.value, 0, 0, 1);

    glTranslatef(
        _zoom.center.x - _rotation.center.x,
        _zoom.center.y - _rotation.center.y,
        0
    );

    glScalef(_zoom.value, _zoom.value, 1);
    glTranslatef(-_zoom.center.x, -_zoom.center.y, 0);

}
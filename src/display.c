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

void espessura(float espessura) {
    glPointSize(espessura);
    glLineWidth(espessura);
}

// -----------------------------------------------------------------------------

DISPLAY_FUNC(desenha_linha, int x2, int y2) {
    glBegin(GL_LINES);
        glVertex2i(x , y );
        glVertex2i(x2, y2);
    glEnd();
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

void _cor_1(int rgb_hex) {
    glColor4ub(
        (rgb_hex >> 16) & 0xFF,
        (rgb_hex >> 8) & 0xFF,
        rgb_hex & 0xFF,
        255
    );
}

void _cor_3(int r, int g, int b) {
    glColor4ub(r, g, b, 255);
}

void _cor_4(int r, int g, int b, int a) {
    glColor4ub(r, g, b, a);
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
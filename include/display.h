#ifndef DISPLAY_H
#define DISPLAY_H

/* -----------------------------------------------------------------------------
    INCLUDES & CONSTANTS
----------------------------------------------------------------------------- */

#include <stdarg.h>

// Color Constants -----------------------------------------
#define VERMELHO        0xFF0000
#define VERDE           0x00FF00
#define AZUL            0x0000FF
#define AMARELO         0xFFFF00
#define MANGETA         0xFF00FF
#define CIANO           0x00FFFF
#define BRANCO          0xFFFFFF
#define PRETO           0x000000
#define CINZA           0x808080
#define LARANJA         0xFFA500
#define ROSA            0xFFC0CB
#define ROXO            0x800080
#define MARROM          0x8B4513
#define VERDE_CLARO     0x00FF7F
#define AZUL_CLARO      0xADD8E6
#define AZUL_ESCURO     0x00008B
#define VERDE_ESCURO    0x006400
#define VERMELHO_ESCURO 0x8B0000
#define CINZA_ESCURO    0x494949
#define CINZA_CLARO     0xD3D3D3
#define LARANJA_ESCURO  0xCB2701
#define LARANJA_CLARO   0xF0A150
#define ROSA_ESCURO     0xFF1493
#define ROSA_CLARO      0xFFC0CB
#define ROXO_ESCURO     0x9400D3
#define PURPURA         0x9370DB
#define VINHO           0x800000
#define MARROM_ESCURO   0x452209
#define MARROM_CLARO    0x854112

/* -----------------------------------------------------------------------------
    PUBLIC FUNCTIONS
----------------------------------------------------------------------------- */

// Macro Helpers -------------------------------------------
#define NARGS_SEQ(_1,_2,_3,_4,_5,_6,_7,_8,_9,N,...) N
#define NARGS(...) NARGS_SEQ(__VA_ARGS__, 9, 8, 7, 6, 5, 4, 3, 2, 1)
#define DISPLAY_FUNC(func, ...) void func(int x, int y, ##__VA_ARGS__)

// Functions -----------------------------------------------
#define cor(...) _cor(NARGS(__VA_ARGS__), __VA_ARGS__)
DISPLAY_FUNC(desenha_retangulo, int largura, int altura);

/* -----------------------------------------------------------------------------
    PRIVATE FUNCTIONS
----------------------------------------------------------------------------- */

// If removed, triggers a "-Wimplicit-function-declaration", but works
// nonetheless
void _cor(int n, ...);

#endif
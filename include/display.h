#ifndef DISPLAY_H
#define DISPLAY_H

/* -----------------------------------------------------------------------------
    INCLUDES
----------------------------------------------------------------------------- */

#include <stdarg.h>

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
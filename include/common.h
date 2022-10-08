#ifndef COMMON_H
#define COMMON_H

/* -----------------------------------------------------------------------------
    MACROS
----------------------------------------------------------------------------- */

#define JOGO_ERROR "\033[32mERRO\033[0m "
#define JOGO_WARN "\033[33mAVISO\033[0m "

#define NARGS_SEQ(_1,_2,_3,_4,_5,_6,_7,_8,_9,N,...) N
#define NARGS(...) NARGS_SEQ(__VA_ARGS__, 9, 8, 7, 6, 5, 4, 3, 2, 1)

#endif
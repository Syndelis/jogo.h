// https://www.chiark.greenend.org.uk/~sgtatham/mp/

#define _TOKPASTEINNER(x, y) x ## y
#define _TOKPASTE(x, y) _TOKPASTEINNER(x, y)
#define _LABEL(id) _TOKPASTE(_LABEL_ ## id ## _, __LINE__)

#define _BEFORE_AFTER(labid, before, after)                             \
    if (1) {                                                            \
        before;                                                         \
        goto _LABEL(labid);                                             \
    }                                                                   \
    else                                                                \
        while (1)                                                       \
            if (1) {                                                    \
                after;                                                  \
                break;                                                  \
            } else                                                      \
            _LABEL(labid):

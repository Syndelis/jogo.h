/* -----------------------------------------------------------------------------
    INCLUDES & GLOBALS
----------------------------------------------------------------------------- */

#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>
#include <string.h>

#define GLEW_STATIC
#include <GL/glew.h>

#include <GLFW/glfw3.h>
#include <GL/gl.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#define STB_DS_IMPLEMENTATION
#include <stb/stb_ds.h>

#define STB_TRUETYPE_IMPLEMENTATION
#include <stb/stb_truetype.h>

#define STB_SPRINTF_IMPLEMENTATION
#include <stb/stb_sprintf.h>

#include "../include/display.h"
#include "../include/image.h"
#include "../include/common.h"

#define ASCII_CHR_START 32
#define ASCII_CHR_END   252
#define ASCII_CHR_COUNT (ASCII_CHR_END - ASCII_CHR_START + 1)
#define GIF_LAYERS 4
#define SECONDS_TO_MSECONDS 1000

#define TEXT_BUFFER_ADVANTAGE 256

int _text_buffer_size = 0; // Allocated at startup
char *_text_buffer = NULL;

/* -----------------------------------------------------------------------------
    PRIVATE STRUCTURES
----------------------------------------------------------------------------- */

typedef struct {
    GLuint texture_id;
} StaticSprite;

typedef struct {
    GLuint *texture_ids;
    int *frame_delays;
    int frame_count;
    int current_frame;
    double last_frame_seconds;
} AnimatedSprite;

typedef struct {
    enum { Static, Animated } type;
    int width, height;
    union {
        StaticSprite static_spr;
        AnimatedSprite animated_spr;
    };
} Sprite;

typedef struct {
    char *key;
    Sprite value;
} FileToSprite;

FileToSprite *sprite_hashmap = NULL;

// -----------------------------------------------------------------------------

typedef struct {
    GLuint texture_id;
    stbtt_bakedchar char_data[ASCII_CHR_COUNT];
} FontData;

typedef struct {
    char *key;
    FontData value;
} FileToFont;

FileToFont *font_hashmap = NULL;
ptrdiff_t active_font_index = -1;

#define FONT_HASH_FORMAT "%s_%f"

/* -----------------------------------------------------------------------------
    FORWARD DECLARATIONS
----------------------------------------------------------------------------- */

ptrdiff_t _insert_font(char *filename, float point_size, char *font_hash_name);

GLuint _insert_sprite(char *in_filename, int *out_width, int *out_height);
GLuint _load_image(char *in_filename, int *out_width, int *out_height);
GLuint *_load_gif(char *in_filename, int *out_width, int *out_height, int *out_frame_count, int **out_delays);

inline void _draw_static_sprite(Sprite *sprite, int x, int y);
void _draw_animated_sprite(Sprite *sprite, int x, int y);
void _draw_texture(GLuint texture_id, int x, int y, int width, int height);

unsigned char *_read_gif_into_data(
    char *in_filename, int *out_width, int *out_height,
    int *out_frame_count, int **out_frame_delays
);

/* -----------------------------------------------------------------------------
    PUBLIC FUNCTIONS
----------------------------------------------------------------------------- */

DISPLAY_FUNC(desenha_imagem, char *arquivo) {

    GLuint texture_id;
    int width, height;

    // Try to obtain the image from the hashmap "cache"
    FileToSprite *fts = shgetp_null(sprite_hashmap, arquivo);
    if (fts) {
        switch (fts->value.type) {
            case Static:
                _draw_static_sprite(&fts->value, x, y);
                break;

            case Animated:
                _draw_animated_sprite(&fts->value, x, y);
                break;
        }
    }

    else { // If it can't be found, load it in and use it
        texture_id = _insert_sprite(arquivo, &width, &height);
        _draw_texture(texture_id, x, y, width, height);
    }

}

// -----------------------------------------------------------------------------

#define _FONT_BUFFER_SIZE 1<<20
#define _BITMAP_BUFFER_DIMENSION 512
char font_hash_buffer[256];
unsigned char ttf_buffer[_FONT_BUFFER_SIZE];
unsigned char temp_bitmap[_BITMAP_BUFFER_DIMENSION*_BITMAP_BUFFER_DIMENSION];

// -----------------------------------------------------------------------------

void fonte(char *arquivo, float tamanho) {

    stbsp_sprintf(font_hash_buffer, FONT_HASH_FORMAT, arquivo, tamanho);
    active_font_index = shgeti(font_hashmap, font_hash_buffer);

    if (active_font_index < 0)
        active_font_index = _insert_font(arquivo, tamanho, font_hash_buffer);

}

// -----------------------------------------------------------------------------

DISPLAY_FUNC(desenha_texto, char *texto, ...) {

    if (active_font_index < 0) {
        fprintf(
            stderr,
            JOGO_ERROR "Nenhuma fonte foi definida com a função "
            "`\033[32mfonte()\033[m` antes da chamada "
            "`\033[32mdesenha_texto()\033[m`\n"
        );

        exit(EXIT_FAILURE);
    }

    if (strlen(texto) > _text_buffer_size) {
        _text_buffer_size = strlen(texto) + TEXT_BUFFER_ADVANTAGE;
        _text_buffer = realloc(_text_buffer, _text_buffer_size);
    }

    va_list args;
    va_start(args, texto);

    stbsp_vsprintf(_text_buffer, texto, args);

    va_end(args);

    FontData font_data = font_hashmap[active_font_index].value;

    glBindTexture(GL_TEXTURE_2D, font_data.texture_id);
    glBegin(GL_QUADS);

    float fx = x, fy = y;
    char current_char, *stream = _text_buffer;

    while ((current_char = *stream)) {
        if (current_char >= ASCII_CHR_START && current_char < ASCII_CHR_END) {
            stbtt_aligned_quad q;
    
            stbtt_GetBakedQuad(
                font_data.char_data,
                _BITMAP_BUFFER_DIMENSION, _BITMAP_BUFFER_DIMENSION,
                current_char - ASCII_CHR_START, &fx, &fy, &q, 1
            );

            glTexCoord2f(q.s0,q.t0); glVertex2f(q.x0,q.y0);
            glTexCoord2f(q.s1,q.t0); glVertex2f(q.x1,q.y0);
            glTexCoord2f(q.s1,q.t1); glVertex2f(q.x1,q.y1);
            glTexCoord2f(q.s0,q.t1); glVertex2f(q.x0,q.y1);

        }
        ++stream;
    }

    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);

}

/* -----------------------------------------------------------------------------
    PRIVATE FUNCTIONS
----------------------------------------------------------------------------- */

void _free_sprite_hashmap() {
    shfree(sprite_hashmap);
}

// -----------------------------------------------------------------------------

void _draw_texture(GLuint texture_id, int x, int y, int width, int height) {

    glColor4f(1., 1., 1., 1.);
    glBindTexture(GL_TEXTURE_2D, texture_id);

    float w = width  / 2.;
    float h = height / 2.;

    glBegin(GL_QUADS);
        glTexCoord2f(    1,     1);
        glVertex2f  (x + w, y + h);
        glTexCoord2f(    1,     0);
        glVertex2f  (x + w, y - h);
        glTexCoord2f(    0,     0);
        glVertex2f  (x - w, y - h);
        glTexCoord2f(    0,     1);
        glVertex2f  (x - w, y + h);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, 0);

}

// -----------------------------------------------------------------------------

void _draw_static_sprite(Sprite *sprite, int x, int y) {
    _draw_texture(sprite->static_spr.texture_id, x, y, sprite->width, sprite->height);
}

// -----------------------------------------------------------------------------

void _draw_animated_sprite(Sprite *sprite, int x, int y) {

    GLuint *texture_ids = sprite->animated_spr.texture_ids;
    double current_time_seconds = glfwGetTime();

    AnimatedSprite *aspr = &sprite->animated_spr;

    if (
        (current_time_seconds - aspr->last_frame_seconds) * SECONDS_TO_MSECONDS
        >= aspr->frame_delays[aspr->current_frame]
    ) {
        aspr->last_frame_seconds = current_time_seconds;
        aspr->current_frame = (aspr->current_frame + 1) % aspr->frame_count;
    }

    GLuint current_texture_id = texture_ids[aspr->current_frame];

    _draw_texture(current_texture_id, x, y, sprite->width, sprite->height);

}

// -----------------------------------------------------------------------------

GLuint _insert_sprite(char *in_filename, int *out_width, int *out_height) {

    bool is_animated = false;
    char *file_extension = strrchr(in_filename, '.');

    if (file_extension && *file_extension) {
        if (strcmp(file_extension, ".gif") == 0)
            is_animated = true;
    }

    if (is_animated) {
        int frame_count;
        int *frame_delays;

        GLuint *sprite_textures = _load_gif(
            in_filename, out_width, out_height, &frame_count, &frame_delays
        );

        if (sprite_textures) {

            Sprite sprite = {
                .type = Animated,
                .width = *out_width,
                .height = *out_height,
                .animated_spr = (AnimatedSprite) {
                    .texture_ids = sprite_textures,
                    .frame_delays = frame_delays,
                    .frame_count = frame_count,
                    .current_frame = 0,
                    .last_frame_seconds = glfwGetTime(),
                }
            };

            shput(sprite_hashmap, in_filename, sprite);

            // We return the first frame because since this function is only called
            // the first time a sprite is drawn, we want to draw the first frame
            return sprite_textures[0];

        }

        // File turned out to be a static image after all
        else is_animated = false;
    }

    if (!is_animated) {
        GLuint sprite_texture = _load_image(in_filename, out_width, out_height);

        Sprite sprite = {
            .type = Static,
            .width = *out_width,
            .height = *out_height,
            .static_spr = (StaticSprite) {
                .texture_id = sprite_texture,
            }
        };

        shput(sprite_hashmap, in_filename, sprite);
        return sprite_texture;
    }

}

// -----------------------------------------------------------------------------

GLuint _load_texture(unsigned char *image_data, int image_width, int image_height) {

    GLuint image_texture;
    glGenTextures(1, &image_texture);
    glBindTexture(GL_TEXTURE_2D, image_texture);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

#if defined(GL_UNPACK_ROW_LENGTH) && !defined(__EMSCRIPTEN__)
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
#endif

    glTexImage2D(
        GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height,
        0, GL_RGBA, GL_UNSIGNED_BYTE, image_data
    );

    return image_texture;

}

// -----------------------------------------------------------------------------

GLuint _load_image(char *in_filename, int *out_width, int *out_height) {

    int image_width = 0;
    int image_height = 0;
    unsigned char *image_data = stbi_load(
        in_filename, &image_width, &image_height, NULL, 4
    );

    if (image_data == NULL)
        return -1;

    GLuint image_texture = _load_texture(image_data, image_width, image_height);

    stbi_image_free(image_data);

    *out_width = image_width;
    *out_height = image_height;

    return image_texture;

}

// -----------------------------------------------------------------------------

GLuint *_load_gif(char *in_filename, int *out_width, int *out_height, int *out_frame_count, int **out_delays) {

    unsigned char *animated_data = _read_gif_into_data(
        in_filename, out_width, out_height, out_frame_count, out_delays
    );

    if (animated_data == NULL)
        return NULL;

    GLuint *animated_textures = malloc(*out_frame_count * sizeof(GLuint));
    int stride = *out_width * *out_height * GIF_LAYERS;

    for (int i = 0; i < *out_frame_count; i++)
        animated_textures[i] = _load_texture(
            animated_data + i * stride, *out_width, *out_height
        );

    return animated_textures;

}

// -----------------------------------------------------------------------------

void _free_font_hashmap() {
    shfree(font_hashmap);
}

// -----------------------------------------------------------------------------

ptrdiff_t _insert_font(char *filename, float point_size, char *font_hash_name) {

    FontData font_data;
    FILE *font_file = fopen(filename, "rb");

    fread(ttf_buffer, sizeof(char), _FONT_BUFFER_SIZE, font_file);

    stbtt_BakeFontBitmap(
        ttf_buffer, 0, point_size, temp_bitmap,
        _BITMAP_BUFFER_DIMENSION, _BITMAP_BUFFER_DIMENSION,
        ASCII_CHR_START, ASCII_CHR_COUNT, font_data.char_data
    );

    glGenTextures(1, &font_data.texture_id);
    glBindTexture(GL_TEXTURE_2D, font_data.texture_id);

    glTexImage2D(
        GL_TEXTURE_2D, 0, GL_ALPHA,
        _BITMAP_BUFFER_DIMENSION, _BITMAP_BUFFER_DIMENSION,
        0, GL_ALPHA, GL_UNSIGNED_BYTE, temp_bitmap
    );

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    fclose(font_file);

    glBindTexture(GL_TEXTURE_2D, 0);

    shput(font_hashmap, font_hash_name, font_data);
    return shgeti(font_hashmap, font_hash_name);

}

// -----------------------------------------------------------------------------

unsigned char *_read_gif_into_data(
    char *in_filename, int *out_width, int *out_height,
    int *out_frame_count, int **out_frame_delays
) {

    stbi__context s;
    FILE *gif_file;

    if (!(gif_file = stbi__fopen(in_filename, "rb"))) {
        fprintf(
            stderr,
            JOGO_ERROR "Não foi possível abrir o arquivo \"%s\".\n",
            in_filename
        );
        exit(EXIT_FAILURE);
    }

    stbi__start_file(&s, gif_file);
    
    if (!stbi__gif_test(&s)) {
        fclose(gif_file);
        return NULL;
    }

    unsigned char *gif_data = stbi__load_gif_main(
        &s, out_frame_delays, out_width, out_height,
        out_frame_count, NULL, GIF_LAYERS
    );

    fclose(gif_file);
    return gif_data;

}

// -----------------------------------------------------------------------------

void _initialize_text_system() {
    _text_buffer_size = TEXT_BUFFER_ADVANTAGE;
    _text_buffer = (char *) malloc(_text_buffer_size * sizeof(char));
}

// -----------------------------------------------------------------------------

void _terminate_text_system() {
    free(_text_buffer);
}
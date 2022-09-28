/* -----------------------------------------------------------------------------
    INCLUDES & GLOBALS
----------------------------------------------------------------------------- */

#include <GL/glew.h>
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

#define ASCII_CHR_START 32
#define ASCII_CHR_END   252
#define ASCII_CHR_COUNT (ASCII_CHR_END - ASCII_CHR_START + 1)

// Forward declarations --------------------------------------------------------
GLuint _insert_sprite(char *in_filename, int *out_width, int *out_height);
GLuint _load_image(char *in_filename, int *out_width, int *out_height);
ptrdiff_t _insert_font(char *filename, float point_size, char *font_hash_name);

/* -----------------------------------------------------------------------------
    PRIVATE STRUCTURES
----------------------------------------------------------------------------- */

typedef struct {
    GLuint texture_id;
    int width, height;
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
    PUBLIC FUNCTIONS
----------------------------------------------------------------------------- */

DISPLAY_FUNC(desenha_imagem, char *arquivo) {

    GLuint texture_id;
    int width, height;

    // Try to obtain the image from the hashmap "cache"
    FileToSprite *sprite = shgetp_null(sprite_hashmap, arquivo);
    if (sprite) {
        texture_id = sprite->value.texture_id;
        width = sprite->value.width;
        height = sprite->value.height;
    }

    else // If it can't be found, load it in and use it
        texture_id = _insert_sprite(arquivo, &width, &height);

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

#define _FONT_BUFFER_SIZE 1<<20
#define _BITMAP_BUFFER_DIMENSION 512
char font_hash_buffer[256];
unsigned char ttf_buffer[_FONT_BUFFER_SIZE];
unsigned char temp_bitmap[_BITMAP_BUFFER_DIMENSION*_BITMAP_BUFFER_DIMENSION];

// // -----------------------------------------------------------------------------

void fonte(char *arquivo, float tamanho) {

    stbsp_sprintf(font_hash_buffer, FONT_HASH_FORMAT, arquivo, tamanho);
    active_font_index = shgeti(font_hashmap, font_hash_buffer);

    if (active_font_index < 0)
        active_font_index = _insert_font(arquivo, tamanho, font_hash_buffer);

}

// // -----------------------------------------------------------------------------

DISPLAY_FUNC(desenha_texto, char *texto) {

    if (active_font_index < 0) {
        fprintf(
            stderr,
            "\033[32;41mERRO!\033[m Nenhuma fonte foi definida com a função "
            "`\033[32mfonte()\033[m` antes da chamada "
            "`\033[32mdesenha_texto()\033[m`\n"
        );

        exit(EXIT_FAILURE);
    }

    FontData font_data = font_hashmap[active_font_index].value;

    glBindTexture(GL_TEXTURE_2D, font_data.texture_id);
    glBegin(GL_QUADS);

    float fx = x, fy = y;
    char current_char;

    while ((current_char = *texto)) {
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
        ++texto;
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

GLuint _insert_sprite(char *in_filename, int *out_width, int *out_height) {

    GLuint sprite_texture = _load_image(in_filename, out_width, out_height);

    Sprite sprite = {
        .texture_id = sprite_texture,
        .width = *out_width,
        .height = *out_height
    };

    shput(sprite_hashmap, in_filename, sprite);
    return sprite_texture;

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

    stbi_image_free(image_data);

    *out_width = image_width;
    *out_height = image_height;

    return image_texture;

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

    shput(font_hashmap, font_hash_name, font_data);
    return shgeti(font_hashmap, font_hash_name);

}
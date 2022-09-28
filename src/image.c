/* -----------------------------------------------------------------------------
    INCLUDES & GLOBALS
----------------------------------------------------------------------------- */

#include <GL/glew.h>
#include <GL/gl.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#define STB_DS_IMPLEMENTATION
#include <stb/stb_ds.h>

#include "../include/display.h"
#include "../include/image.h"


// Forward declarations --------------------------------------------------------
GLuint _insert_sprite(char *in_filename, int *out_width, int *out_height);
GLuint _load_image(char *in_filename, int *out_width, int *out_height);

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
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
    stbi_image_free(image_data);

    *out_width = image_width;
    *out_height = image_height;

    return image_texture;

}
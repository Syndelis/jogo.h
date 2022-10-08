/* -----------------------------------------------------------------------------
    INCLUDES & GLOBALS
----------------------------------------------------------------------------- */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MINIAUDIO_IMPLEMENTATION
#include <miniaudio.h>

#include <stb/stb_ds.h>

#include "../include/audio.h"
#include "../include/common.h"

#define SAMPLE_FORMAT   ma_format_f32
#define CHANNEL_COUNT   2
#define SAMPLE_RATE     48000

ma_engine _engine;

/* -----------------------------------------------------------------------------
    PRIVATE STRUCTURES
----------------------------------------------------------------------------- */

typedef struct _Sound {
    ma_sound _sound;
    struct _Sound *next; // For multiple of the same sound playing at the same time
} Sound;

typedef struct {
    char *key;
    Sound value;
} FileToSound;

FileToSound *sound_hashmap = NULL;

/* -----------------------------------------------------------------------------
    FORWARD DECLARATIONS
----------------------------------------------------------------------------- */

Sound *_insert_sound(char *filename);
void _rewind_sound(Sound *sound);
Sound *_duplicate_sound(Sound *last);
void _play_sound(char *filename, bool loop);
void _loop_sound(ma_sound *sound, bool loop);

/* -----------------------------------------------------------------------------
    PUBLIC FUNCTIONS
----------------------------------------------------------------------------- */

void toca_som_em_loop(char *arquivo) {
    _play_sound(arquivo, true);
}

void toca_som(char *arquivo) {
    _play_sound(arquivo, false);
}

void para_som(char *arquivo) {

    FileToSound *fts = shgetp_null(sound_hashmap, arquivo);

    if (!fts) return;

    for (
        Sound *current = &(fts->value);
        current;
        current = current->next
    )
        if (ma_sound_is_playing(&current->_sound))
            ma_sound_stop(&current->_sound);

}

bool som_esta_tocando(char *arquivo) {

    FileToSound *fts = shgetp_null(sound_hashmap, arquivo);

    if (!fts) return false;

    for (
        Sound *current = &(fts->value);
        current;
        current = current->next
    )
        if (ma_sound_is_playing(&current->_sound))
            return true;

    return false;

}

void recomeca_som(char *arquivo) {

    FileToSound *fts = shgetp_null(sound_hashmap, arquivo);

    if (!fts)
        return toca_som(arquivo);

    bool one_is_playing = false;

    for (
        Sound *current = &(fts->value);
        current && !one_is_playing;
        current = current->next
    )
        if (ma_sound_is_playing(&current->_sound)) {
            ma_sound_stop(&current->_sound);
            _rewind_sound(current);
            ma_sound_start(&current->_sound);
            one_is_playing = true;
        }

    if (!one_is_playing) {
        Sound *first = &(fts->value);

        ma_sound_stop(&first->_sound);
        _rewind_sound(first);
        ma_sound_start(&first->_sound);
    }

}

/* -----------------------------------------------------------------------------
    PRIVATE FUNCTIONS
----------------------------------------------------------------------------- */

void _play_sound(char *filename, bool loop) {

    Sound *new_entry;
    FileToSound *fts = shgetp_null(sound_hashmap, filename);
    
    // If it exists we need to check whether it's being played or not
    if (fts) {

        bool all_playing = true;
        Sound *current, *last;

        // That means we need to look for the whole linked list
        for (
            current = &(fts->value);
            current && all_playing;
            current = current->next
        ) {
            last = current;
            all_playing &= ma_sound_is_playing(&current->_sound);
        }

        // If it is: duplicate it
        if (all_playing) {
            new_entry = _duplicate_sound(last);
            _loop_sound(&new_entry->_sound, loop);
            ma_sound_start(&new_entry->_sound);
        }

        // If it's not: reuse it
        else {
            ma_sound_stop(&last->_sound);
            _rewind_sound(last);
            _loop_sound(&new_entry->_sound, loop);
            ma_sound_start(&last->_sound);
        }

    }

    else {
        new_entry = _insert_sound(filename);
        _loop_sound(&new_entry->_sound, loop);
        ma_sound_start(&new_entry->_sound);
    }

}

// -----------------------------------------------------------------------------

void _initialize_sound_system() {

    ma_result result = ma_engine_init(NULL, &_engine);
    if (result != MA_SUCCESS) {
        fprintf(
            stderr,
            JOGO_ERROR "Não foi possível inicializar o sistema de "
            "áudio.\n"
        );
        exit(EXIT_FAILURE);
    }

}

// -----------------------------------------------------------------------------

void _terminate_sound_system() {

    ma_engine_uninit(&_engine);
    Sound *a, *b;

    for (int entry_idx = 0; entry_idx < shlenu(sound_hashmap); entry_idx++) {

        // We don't own the data for the head, so it needs to be unititialized
        // separately

        // This line causes a double free for some unknown reason, even though
        // `ma_sound_uninit(&a->_sound);` does not

        // ma_sound_uninit(&(sound_hashmap[entry_idx].value._sound));

        a = sound_hashmap[entry_idx].value.next;
        while (a) {
            b = a->next;
            ma_sound_uninit(&a->_sound);
            free(a);
            a = b;
        }
    }
    
    shfree(sound_hashmap);

}

// -----------------------------------------------------------------------------

Sound *_insert_sound(char *filename) {

    // For whatever reason, miniaudio can't work when `ma_sound` is copied
    // around. Therefore, the solution is to insert an empty `ma_sound` struct
    // first and only then initialize it using its pointer.

    shput(
        sound_hashmap, filename,
        ((Sound) {
            ._sound = {0},
            .next = NULL,
        })
    );

    FileToSound *new_entry = shgetp_null(sound_hashmap, filename);

    ma_result result = ma_sound_init_from_file(
        &_engine, filename, 0, NULL, NULL, &new_entry->value._sound
    );

    if (result != MA_SUCCESS) {
        fprintf(
            stderr,
            JOGO_WARN "Não foi possível carregar o arquivo de áudio `%s`\n",
            filename
        );

        shdel(sound_hashmap, filename);
    }

    return &new_entry->value;

}

// -----------------------------------------------------------------------------

void _rewind_sound(Sound *sound) {

    ma_sound_seek_to_pcm_frame(&sound->_sound, 0);

}

// -----------------------------------------------------------------------------

void _loop_sound(ma_sound *sound, bool loop) {

    // For some reason, setting `ma_sound_set_looping(_, MA_FALSE)` can result
    // in a segmentation fault, hence the `if` statement

    if (loop)
        ma_sound_set_looping(sound, MA_TRUE);
}

// -----------------------------------------------------------------------------

Sound *_duplicate_sound(Sound *last) {

    Sound *new_sound = (Sound *) malloc(sizeof(Sound));
    new_sound->next = NULL;
    last->next = new_sound;

    ma_sound_init_copy(&_engine, &last->_sound, 0, NULL, &new_sound->_sound);
    _rewind_sound(new_sound);

    return new_sound;

}
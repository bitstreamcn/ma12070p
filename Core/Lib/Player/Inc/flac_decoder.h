#ifndef STM32_FLAC_PLAYER_FLAC_DECODER_H
#define STM32_FLAC_PLAYER_FLAC_DECODER_H

#include <stdint.h>
#include "FLAC/stream_decoder.h"
#include "ff.h"
#include <stdlib.h>
#include "logger.h"
#include <stdbool.h>

typedef struct {
    uint64_t total_samples;
    unsigned bits_per_sample;
    unsigned sample_rate;
    unsigned channels;
} FlacMetaData;

typedef struct {
    unsigned samples;
    unsigned size;
    uint8_t *buffer;
} FlacFrame;

typedef struct {
    FLAC__StreamDecoder *decoder;
    FlacMetaData metadata;
    FlacFrame *frame;
    FIL *file;
} Flac;

Flac *create_flac(FIL *input);

void destroy_flac(Flac *flac);

int read_metadata(Flac *flac, FlacMetaData *metadata);

int read_frame(Flac *flac, FlacFrame *frame);

void free_frame(FlacFrame *frame);

void free_metadata(FlacMetaData *metadata);

#endif //STM32_FLAC_PLAYER_FLAC_DECODER_H

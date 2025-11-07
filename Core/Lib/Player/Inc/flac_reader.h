#ifndef STM32_FLAC_PLAYER_FLAC_READER_H
#define STM32_FLAC_PLAYER_FLAC_READER_H

#include "flac_decoder.h"

typedef struct {
    Flac* flac;
    FlacFrame *frame;
    unsigned buffer_index;
} FlacReader;

FlacReader *create_flac_reader(Flac *flac);
void free_flac_reader(FlacReader *reader);
unsigned read_flac(FlacReader *reader, uint8_t  *buffer, unsigned size);

#endif //STM32_FLAC_PLAYER_FLAC_READER_H

#include <string.h>
#include "flac_reader.h"


FlacReader *create_flac_reader(Flac *flac) {
    log_debug("Creating flac reader");
    FlacReader *reader = malloc(sizeof(FlacReader));
    reader->flac = flac;
    reader->frame = NULL;
    reader->buffer_index = 0;
    return reader;
}

void free_flac_reader(FlacReader *reader) {
    free(reader->frame);
    free(reader);
}

unsigned read_flac(FlacReader *reader, uint8_t *buffer, unsigned size) {
    log_debug("Reading %d bytes from flac", size);
    unsigned bytes_read = 0;

    while (bytes_read < size) {
        log_debug("Bytes read: %d", bytes_read);

        if (reader->frame == NULL) {
            log_debug("Reading frame");
            if (read_frame(reader->flac, reader->frame)) {
                return bytes_read;
            }
            reader->buffer_index = 0;
        }

        unsigned bytes_to_read = reader->frame->size - reader->buffer_index;
        unsigned bytes_left = size - bytes_read;

        log_debug("Bytes read: %d, Frame size: %d, Buffer index: %d, Bytes to read: %d, Bytes left: %d",
                  bytes_read, reader->frame->size, reader->buffer_index, bytes_to_read, bytes_left);

        if (bytes_to_read > bytes_left) {
            memcpy(buffer + bytes_read, reader->frame->buffer + reader->buffer_index, bytes_left);
            reader->buffer_index += bytes_left;
            bytes_read += bytes_left;
        } else {
            memcpy(buffer + bytes_read, reader->frame->buffer + reader->buffer_index, bytes_to_read);
            free_frame(reader->frame);
            reader->frame = NULL;
            reader->buffer_index = 0;
            bytes_read += bytes_to_read;
        }
    }

    return bytes_read;
}

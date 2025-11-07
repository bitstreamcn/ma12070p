#include <flac_decoder.h>



static FLAC__StreamDecoderReadStatus decoder_read_callback(
        const FLAC__StreamDecoder *decoder,
        FLAC__byte buffer[],
        size_t *bytes,
        void *client_data) {
    log_debug("decoder_read_callback");

    if (*bytes <= 0) {
        return FLAC__STREAM_DECODER_READ_STATUS_ABORT;
    }

    Flac *flac = (Flac *) client_data;

    UINT bytes_read;
    if (f_read(flac->file, buffer, *bytes, &bytes_read) != FR_OK) {
        log_error("Could not read from file");
        return FLAC__STREAM_DECODER_READ_STATUS_ABORT;
    }

    *bytes = bytes_read;

    if (bytes_read == 0) {
        return FLAC__STREAM_DECODER_READ_STATUS_END_OF_STREAM;
    }
    if (bytes_read > 0) {
        return FLAC__STREAM_DECODER_READ_STATUS_CONTINUE;
    }
    return FLAC__STREAM_DECODER_READ_STATUS_ABORT;
}

static FLAC__StreamDecoderWriteStatus decoder_write_callback(
        const FLAC__StreamDecoder *decoder,
        const FLAC__Frame *frame,
        const FLAC__int32 *const buffer[],
        void *client_data) {
    log_debug("decoder_write_callback");

    Flac *flac = (Flac *) client_data;

    for (int i = 0; i < frame->header.channels; i++) {
        if (buffer[i] == NULL) {
            return FLAC__STREAM_DECODER_WRITE_STATUS_ABORT;
        }
    }

    unsigned samples = frame->header.blocksize;
    unsigned channels = frame->header.channels;
    unsigned bytes_per_sample = frame->header.bits_per_sample / 8;
    //unsigned size = samples * channels * bytes_per_sample;
    unsigned size = samples * channels * 4;

    flac->frame = (FlacFrame*)malloc(sizeof(FlacFrame));
    flac->frame->buffer = malloc(size);
    flac->frame->size = size;

    log_debug("samples: %d, channels: %d, bytes_per_sample: %d, buffer_size: %d\n",
              samples, channels, bytes_per_sample, size);

    /*
    for (int sample = 0; sample < samples; sample++) {
        for (int channel = 0; channel < channels; channel++) {
            for (int byte = 0; byte < bytes_per_sample; byte++) {
                flac->frame->buffer[(sample * channels + channel) * bytes_per_sample + byte] =
                        (uint8_t)((buffer[channel][sample] >> (byte * 8)) & 0xFF);
            }
        }
    }
    */
    int32_t* pNextSampleInNewFrame = (int32_t*)flac->frame->buffer;
    for (int i = 0; i < samples; i += 1) {
        for (int j = 0; j < channels; ++j) {
            *pNextSampleInNewFrame++ = buffer[j][i] << (32 - frame->header.bits_per_sample);
        }
    }

    return FLAC__STREAM_DECODER_WRITE_STATUS_CONTINUE;
}

static void decoder_metadata_callback(
        const FLAC__StreamDecoder *decoder,
        const FLAC__StreamMetadata *metadata,
        void *client_data) {
    log_debug("decoder_metadata_callback");

    Flac *flac = (Flac *) client_data;

    if (metadata->type == FLAC__METADATA_TYPE_STREAMINFO) {
        flac->metadata = (FlacMetaData) {
                .total_samples = metadata->data.stream_info.total_samples,
                .bits_per_sample = metadata->data.stream_info.bits_per_sample,
                .sample_rate = metadata->data.stream_info.sample_rate,
                .channels = metadata->data.stream_info.channels,

        };

        log_debug("total_samples: %d", flac->metadata.total_samples);
        log_debug("bits_per_sample: %d", flac->metadata.bits_per_sample);
        log_debug("sample_rate: %d", flac->metadata.sample_rate);
        log_debug("channels: %d", flac->metadata.channels);
    }
}

static void decoder_error_callback(
        const FLAC__StreamDecoder *decoder,
        FLAC__StreamDecoderErrorStatus status,
        void *client_data) {
    log_error("Error decoding FLAC: %s", FLAC__StreamDecoderErrorStatusString[status]);
}

Flac *create_flac(FIL *input) {
    Flac *flac = (Flac *) calloc(1, sizeof(Flac));
    *flac = (Flac) {
            .file = input
    };

    flac->decoder = FLAC__stream_decoder_new();
    if (flac->decoder == NULL) {
        log_error("Could not allocate FLAC decoder");
        destroy_flac(flac);
        return NULL;
    }

    // TODO - check if these & are required
    FLAC__StreamDecoderInitStatus init_status = FLAC__stream_decoder_init_stream(
            flac->decoder,
            &decoder_read_callback,
            NULL,
            NULL,
            NULL,
            NULL,
            &decoder_write_callback,
            &decoder_metadata_callback,
            &decoder_error_callback,
            flac
    );

    if (init_status != FLAC__STREAM_DECODER_INIT_STATUS_OK) {
        log_error("Could not initialize decoder: %s", FLAC__StreamDecoderInitStatusString[init_status]);
        destroy_flac(flac);
        return NULL;
    }

    return flac;
}

void destroy_flac(Flac *flac) {
    if (flac != NULL) {
        if (flac->decoder != NULL) {
            FLAC__stream_decoder_delete(flac->decoder);
        }
        free(flac);
    }
}

int read_metadata(Flac *flac, FlacMetaData *metadata) {
    if (FLAC__stream_decoder_process_until_end_of_metadata(flac->decoder)) {
        *metadata = flac->metadata;
        return 0;
    } else {
        log_error("Could not read metadata %s",
                  FLAC__StreamDecoderStateString[FLAC__stream_decoder_get_state(flac->decoder)]);
        return 1;
    }
}

int read_frame(Flac *flac, FlacFrame *frame) {
    unsigned int t = xTaskGetTickCount();

    if (FLAC__stream_decoder_process_single(flac->decoder)) {
        if (flac->frame == NULL) {
            log_error("No frame read");
            return 1;
        }
        *frame = *flac->frame;
        flac->frame = NULL;

        t = xTaskGetTickCount() - t;
        log_debug("read_frame read frame with size %d in %d ms", frame->size, t);

        return 0;
    } else {
        log_error("Could not read frame %s",
                  FLAC__StreamDecoderStateString[FLAC__stream_decoder_get_state(flac->decoder)]);
        return 1;
    }
}

void free_frame(FlacFrame *frame) {
    if (frame != NULL) {
        free(frame->buffer);
        free(frame);
    }
}

void free_metadata(FlacMetaData *metadata) {
    if (metadata != NULL) {
        free(metadata);
    }
}

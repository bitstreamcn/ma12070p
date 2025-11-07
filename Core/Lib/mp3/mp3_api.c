/*
 * mp3_api.c
 *
 *  Created on: Nov 4, 2025
 *      Author: bitstream
 */
#include "main.h"
#include "mp3_api.h"
#include "ff.h"


static FIL					mp3_file;
static FIL                  pcm_file;

/* MP3 file read, provided to MP3 decoder */
static uint32_t fd_fetch(void *parameter, uint8_t *buffer, uint32_t length) {
    uint32_t read_bytes = 0;

	f_read((FIL *)parameter, (void *)buffer, length, &read_bytes);
    if (read_bytes <= 0) return 0;

    return read_bytes;
}















#include "mp3_common.inc"

/* Define OPEN_MEMORY to use drmp3_init_memory() instead of drmp3_init_file(). */
#define OPEN_MEMORY

/* Define WITH_METADATA to open with a metadata callback. */
#define WITH_METADATA

#define FORMAT_S16 0
#define FORMAT_F32 1

#if defined(WITH_METADATA)
void on_meta(void* pUserData, const drmp3_metadata* pMetadata)
{
    const char* pMetaName = "Unknown";
    if (pMetadata->type == DRMP3_METADATA_TYPE_ID3V1) {
        pMetaName = "ID3v1";
    } else if (pMetadata->type == DRMP3_METADATA_TYPE_ID3V2) {
        pMetaName = "ID3v2";
    } else if (pMetadata->type == DRMP3_METADATA_TYPE_APE) {
        pMetaName = "APE";
    } else if (pMetadata->type == DRMP3_METADATA_TYPE_XING) {
        pMetaName = "Xing";
    } else if (pMetadata->type == DRMP3_METADATA_TYPE_VBRI) {
        pMetaName = "Info";
    }

    printf("Metadata: %s (%d bytes)\n", pMetaName, (int)pMetadata->rawDataSize);

    (void)pUserData;
}
#endif

int mp3_main(const char* file_path)
{
    drmp3 mp3;
    const char* pOutputFilePath = NULL;
    int format = FORMAT_S16;
    int iarg;
    FIL pFileOut;
    drmp3_uint64 framesRead;
    drmp3_uint64 totalFramesRead = 0;
    drmp3_uint64 queriedFrameCount = 0;
    drmp3_bool32 hasError = DRMP3_FALSE;
#if defined(OPEN_MEMORY)
    size_t dataSize;
    void* pData;
#endif
#if defined(WITH_METADATA)
    drmp3_meta_proc onMeta = on_meta;
#else
    drmp3_meta_proc onMeta = NULL;
#endif

    /*
    if (argc < 2) {
        printf("Usage: mp3_extract <input filename> -o <output filename> -f [s16|f32]\n");
        return 1;
    }
    */

    /* Parse parameters. */
    /*
    for (iarg = 2; iarg < argc; iarg += 1) {
        if (strcmp(argv[iarg], "-o") == 0) {
            if (iarg+1 < argc) {
                iarg += 1;
                pOutputFilePath = argv[iarg];
            }
        } else if (strcmp(argv[iarg], "-f") == 0) {
            if (iarg+1 < argc) {
                iarg += 1;
                if (strcmp(argv[iarg], "s16") == 0) {
                    format = FORMAT_S16;
                } else if (strcmp(argv[iarg], "f32") == 0) {
                    format = FORMAT_F32;
                }
            }
        }
    }
    */
    format = FORMAT_S16;
    pOutputFilePath = "/mp3_dec.pcm";

    if (pOutputFilePath == NULL) {
        printf("No output file specified.\n");
        return 1;
    }

    #if defined(OPEN_MEMORY)
    {
        pData = dr_open_and_read_file(file_path, &dataSize);
        if (pData == NULL) {
            printf("Failed to open file: %s\n", file_path);
            return 1;
        }

        if (drmp3_init_memory_with_metadata(&mp3, pData, dataSize, onMeta, NULL, NULL) == DRMP3_FALSE) {
            free(pData);
            printf("Failed to init MP3 decoder: %s\n", file_path);
            return 1;
        }
    }
    #else
    {
        if (drmp3_init_file_with_metadata(&mp3, file_path, onMeta, NULL, NULL) == DRMP3_FALSE) {
            printf("Failed to open file: %s\n", file_path);
            return 1;
        }
    }
    #endif

    /*
    There was a bug once where seeking would result in the decoder not properly skipping the Xing/Info
    header if present. We'll do a seek here to ensure that code path is hit.
    */
    if (0)
    {
        drmp3_uint64 totalFrameCount = drmp3_get_pcm_frame_count(&mp3);
        drmp3_seek_to_pcm_frame(&mp3, totalFrameCount / 2);
        drmp3_seek_to_pcm_frame(&mp3, 0);
    }

    if (FR_OK != f_open(&pFileOut, pOutputFilePath, FA_CREATE_ALWAYS | FA_WRITE))
    {
    	return 1;
    }

    /*
    if (drmp3_fopen(&pFileOut, pOutputFilePath, "wb") != DRMP3_SUCCESS) {
        printf("Failed to open output file: %s\n", pOutputFilePath);
        return 1;
    }
    */

    /* This will be compared against the total frames read below. */
    queriedFrameCount = drmp3_get_pcm_frame_count(&mp3);
    totalFramesRead   = 0;

    if (format == FORMAT_S16) {
        drmp3_int16 pcm[4096];

        for (;;) {
            framesRead = drmp3_read_pcm_frames_s16(&mp3, sizeof(pcm)/sizeof(pcm[0]) / mp3.channels, pcm);
            if (framesRead == 0) {
                break;
            }

            //fwrite(pcm, 1, (size_t)(framesRead * mp3.channels * sizeof(pcm[0])), pFileOut);
            UINT br;
            f_write(&pFileOut, pcm, (size_t)(framesRead * mp3.channels * sizeof(pcm[0])), &br);
            totalFramesRead += framesRead;
        }
    } else {
        float pcm[4096];

        for (;;) {
            framesRead = drmp3_read_pcm_frames_f32(&mp3, sizeof(pcm)/sizeof(pcm[0]) / mp3.channels, pcm);
            if (framesRead == 0) {
                break;
            }

            //fwrite(pcm, 1, (size_t)(framesRead * mp3.channels * sizeof(pcm[0])), pFileOut);
            UINT br;
            f_write(&pFileOut, pcm, (size_t)(framesRead * mp3.channels * sizeof(pcm[0])), &br);
            totalFramesRead += framesRead;
        }
    }

    if (totalFramesRead != queriedFrameCount) {
        printf("Frame count mismatch: %d (queried) != %d (read)\n", (int)queriedFrameCount, (int)totalFramesRead);
        hasError = DRMP3_TRUE;
    }

    f_close(&pFileOut);
    drmp3_uninit(&mp3);

    #if defined(OPEN_MEMORY)
    {
        //free(pData);
    }
    #endif

    if (hasError) {
        return 1;
    } else {
        return 0;
    }
}


drmp3 mp3;
const char* pOutputFilePath = NULL;
int format = FORMAT_S16;
int iarg;

drmp3_uint64 framesRead;
drmp3_uint64 totalFramesRead = 0;
drmp3_uint64 queriedFrameCount = 0;
drmp3_bool32 hasError = DRMP3_FALSE;
#if defined(OPEN_MEMORY)
size_t dataSize;
void* pData;
#endif

int open_mp3(char* file_path)
{
#if defined(WITH_METADATA)
    drmp3_meta_proc onMeta = on_meta;
#else
    drmp3_meta_proc onMeta = NULL;
#endif

    format = FORMAT_S16;

    #if defined(OPEN_MEMORY)
    {
        pData = dr_open_and_read_file(file_path, &dataSize);
        if (pData == NULL) {
            printf("Failed to open file: %s\n", file_path);
            return 1;
        }

        if (drmp3_init_memory_with_metadata(&mp3, pData, dataSize, onMeta, NULL, NULL) == DRMP3_FALSE) {
            free(pData);
            printf("Failed to init MP3 decoder: %s\n", file_path);
            return 1;
        }
    }
    #else
    {
        if (drmp3_init_file_with_metadata(&mp3, file_path, onMeta, NULL, NULL) == DRMP3_FALSE) {
            printf("Failed to open file: %s\n", file_path);
            return 1;
        }
    }
    #endif

    /*
    There was a bug once where seeking would result in the decoder not properly skipping the Xing/Info
    header if present. We'll do a seek here to ensure that code path is hit.
    */
    if (0)
    {
        drmp3_uint64 totalFrameCount = drmp3_get_pcm_frame_count(&mp3);
        drmp3_seek_to_pcm_frame(&mp3, totalFrameCount / 2);
        drmp3_seek_to_pcm_frame(&mp3, 0);
    }

    /* This will be compared against the total frames read below. */
    queriedFrameCount = drmp3_get_pcm_frame_count(&mp3);
    totalFramesRead   = 0;

    if (hasError) {
        return 1;
    } else {
        return 0;
    }
}

int decode_mp3(uint8_t * buff, int size)
{
    drmp3_int16 pcm[4096];
    int index = 0;
    static uint8_t prev_pcm[4096 * sizeof(pcm[0])];
    static int 	   prev_pcm_size = 0;
    if (prev_pcm_size >= size)
    {
    	memcpy(buff, prev_pcm, size);
    	prev_pcm_size -= size;
    	memmove(prev_pcm, &prev_pcm[size], prev_pcm_size);
    	return 0;
    }
    else
    {
    	memcpy(buff, prev_pcm, prev_pcm_size);
    	index += prev_pcm_size;
    	prev_pcm_size = 0;
    }
    while (index < size)
    {
		framesRead = drmp3_read_pcm_frames_s16(&mp3, sizeof(pcm)/sizeof(pcm[0]) / mp3.channels, pcm);
		if (framesRead == 0) {
			return -1;
		}

		//fwrite(pcm, 1, (size_t)(framesRead * mp3.channels * sizeof(pcm[0])), pFileOut);
		//UINT br;
		//f_write(&pFileOut, pcm, (size_t)(framesRead * mp3.channels * sizeof(pcm[0])), &br);

		size_t pcm_size = (size_t)(framesRead * mp3.channels * sizeof(pcm[0]));

		int needsize = size - index;
		if (pcm_size >= needsize)
		{
	    	memcpy(&buff[index], pcm, needsize);
	    	index += needsize;
	    	prev_pcm_size = pcm_size - needsize;
	    	memcpy(prev_pcm, &pcm[needsize / sizeof(pcm[0])], prev_pcm_size);
		}
		else
		{
	    	memcpy(&buff[index], pcm, pcm_size);
	    	index += pcm_size;
		}
		totalFramesRead += framesRead;
    }
    return 0;
}

void close_mp3()
{
    if (totalFramesRead != queriedFrameCount) {
        printf("Frame count mismatch: %d (queried) != %d (read)\n", (int)queriedFrameCount, (int)totalFramesRead);
        hasError = DRMP3_TRUE;
    }

    drmp3_uninit(&mp3);

    #if defined(OPEN_MEMORY)
    {
        //free(pData);
    }
    #endif
}





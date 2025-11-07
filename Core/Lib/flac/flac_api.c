/*
 * flac_api.c
 *
 *  Created on: Nov 6, 2025
 *      Author: bitstream
 */
#include "flac_common.inc"
#include "flac_api.h"
#include "ff.h"

static void on_meta(void* pUserData, drflac_metadata* pMetadata)
{
    (void)pUserData;

    switch (pMetadata->type)
    {
        case DRFLAC_METADATA_BLOCK_TYPE_CUESHEET:
        {
            /*
            This section is here just to make it easy to test cuesheets. It's not designed to integrate cleanly with the output of this program
            so I'm just selectively enabling and disabling this section as required.
            */
        #if 0
            drflac_cuesheet_track_iterator i;
            drflac_cuesheet_track track;

            printf("Cuesheet Found. Track Count = %d\n", (int)pMetadata->data.cuesheet.trackCount);

            drflac_init_cuesheet_track_iterator(&i, pMetadata->data.cuesheet.trackCount, pMetadata->data.cuesheet.pTrackData);
            while (drflac_next_cuesheet_track(&i, &track)) {
                drflac_uint32 iTrackIndex;

                printf("Cuesheet Track %d. Index Count = %d:\n", track.trackNumber, track.indexCount);
                for (iTrackIndex = 0; iTrackIndex < track.indexCount; iTrackIndex += 1) {
                    printf("    Index %d - Offset %llu\n", iTrackIndex, track.pIndexPoints[iTrackIndex].offset);
                }
            }
        #endif
        } break;
    }
}

#pragma pack(1)
typedef struct
{
    char rld[4];    //riff 标志符号
    int  rLen;      //
    char wld[4];    //格式类型（wave）
    char fld[4];    //"fmt"

    int fLen;   //sizeof(wave format matex)

    short wFormatTag;   //编码格式
    short wChannels;    //声道数
    int   nSamplesPersec;  //采样频率
    int   nAvgBitsPerSample;//WAVE文件采样大小
    short wBlockAlign; //块对齐
    short wBitsPerSample;   //WAVE文件采样大小

    char dld[4];        //”data“
    int  wSampleLength; //音频数据的大小
 }WAV_HEADER;

 static WAV_HEADER wav_header;

int flac_main(char* filename)
{
	drflac_result result;
	drflac* pFlac;
	FIL pFileOut;
	const char* pOutputFilePath = "flac_dec.pcm";
	const char* pFilePath = filename;

    if (FR_OK != f_open(&pFileOut, pOutputFilePath, FA_CREATE_ALWAYS | FA_WRITE))
    {
    	return 1;
    }

    /* Now load from dr_flac. */
    pFlac = drflac_open_file_with_metadata(pFilePath, on_meta, NULL, NULL);
    if (pFlac == NULL) {
        printf("  Failed to open via dr_flac.");
        return DRFLAC_ERROR;    /* Failed to load dr_flac decoder. */
    }

	wav_header.wChannels = 2;
	wav_header.nSamplesPersec = pFlac->sampleRate;
	wav_header.wBitsPerSample = 16;
	//f_write(&pFileOut, &wav_header, sizeof(wav_header), NULL);

	size_t pcmFrameChunkSize = (pFlac->maxBlockSizeInPCMFrames > 0) ? pFlac->maxBlockSizeInPCMFrames : 4096;
    drflac_int16* pPCMFrames_drflac;
    size_t frameSize = (size_t)(pcmFrameChunkSize * pFlac->channels * sizeof(drflac_int16));
    pPCMFrames_drflac = (drflac_int16*)malloc(frameSize);
    if (pPCMFrames_drflac == NULL) {
        printf("  [dr_flac] Out of memory");
        return DRFLAC_ERROR;
    }

    while (1)
    {
    	drflac_uint64 pcmFrameCount_drflac;
    	pcmFrameCount_drflac = drflac_read_pcm_frames_s16(pFlac, 1024, pPCMFrames_drflac);
    	if (pcmFrameCount_drflac <= 0)
    	{
    		break;
    	}
    	UINT br;
    	f_write(&pFileOut, pPCMFrames_drflac, pcmFrameCount_drflac * pFlac->channels * sizeof(drflac_int16), &br);
    }

    free(pPCMFrames_drflac);
    f_close(&pFileOut);
    drflac_close(pFlac);
    return 0;
}





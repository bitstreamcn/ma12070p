#include <assert.h>
#include "player.h"
#include "files.h"
#include "sai.h"
#include "ring_buffer.h"
#include "../Lib/mp3/mp3_api.h"
#include "../Lib/flac/flac_api.h"

#if defined USE_MA12070P
#include "ma12070p.h"
#elif defined USE_TAS5825
#include "TAS5825.h"
#else
#error "please define USE_MA12070P or USE_TAS5825"
#endif

#define SDRAM_BASE_ADDR 0xC0000000

//static uint8_t pcm_buffer[AUDIO_BUFFER_SIZE];

//static uint8_t audio_buffer[AUDIO_BUFFER_SIZE];
static uint8_t *audio_buffer = (uint8_t *)SDRAM_BASE_ADDR;
static uint8_t audio_buffer_state = BUFFER_OFFSET_NONE;
static unsigned last_audio_buffer_state_change_time = 0;

static uint8_t *pcm_buffer = (uint8_t *)(SDRAM_BASE_ADDR + AUDIO_BUFFER_SIZE);
static uint8_t *file_buffer = (uint8_t *)(SDRAM_BASE_ADDR + AUDIO_BUFFER_SIZE + AUDIO_BUFFER_SIZE);
static RingBuffer_t * ring_buffer = (RingBuffer_t *)(SDRAM_BASE_ADDR + AUDIO_BUFFER_SIZE + AUDIO_BUFFER_SIZE + AUDIO_BUFFER_SIZE);

uint16_t *I2STxBuf = (uint16_t *)(SDRAM_BASE_ADDR + AUDIO_BUFFER_SIZE + AUDIO_BUFFER_SIZE + AUDIO_BUFFER_SIZE + sizeof(RingBuffer_t));

static PlayerState player_state = STOPPED;
static uint64_t samples_played = 0;

static FIL current_audio_file;
static Flac *flac;
static FlacReader *flac_reader;
static FlacMetaData flac_metadata;

bool is_flac = false;
bool is_mp3 = false;
bool is_wav = false;



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


void BSP_AUDIO_OUT_HalfTransfer_CallBack(void) {
    audio_buffer_state = BUFFER_OFFSET_HALF;
    unsigned t = osKernelSysTick();
    //log_debug("[%u] TransferredFirstHalf (%u)\n", t, t - last_audio_buffer_state_change_time);
    last_audio_buffer_state_change_time = t;
}

void BSP_AUDIO_OUT_TransferComplete_CallBack(void) {
    audio_buffer_state = BUFFER_OFFSET_FULL;
    unsigned t = osKernelSysTick();
    //log_debug("[%u] TransferredSecondHalf (%u)\n", t, t - last_audio_buffer_state_change_time);
    last_audio_buffer_state_change_time = t;
}

void initialize_codec(void) {
    log_info("Initializing audio codec");
    //if (BSP_AUDIO_OUT_Init(OUTPUT_DEVICE_HEADPHONE1, 10, AUDIO_FREQUENCY_44K, 24) == 0)
    if (is_flac)
    {
    	if (BSP_AUDIO_OUT_Init(OUTPUT_DEVICE_HEADPHONE1, 10, flac_metadata.sample_rate, 32) == 0)
		{
			log_success("Audio codec was successfully initialized");
		} else {
			log_error("Failed to initialize audio codec");
		}
    }
    else if (is_mp3)
    {
		if (BSP_AUDIO_OUT_Init(OUTPUT_DEVICE_HEADPHONE1, 10, mp3.sampleRate, 32) == 0)
		{
			log_success("Audio codec was successfully initialized");
		} else {
			log_error("Failed to initialize audio codec");
		}
    }
    else if (is_wav)
    {
		if (BSP_AUDIO_OUT_Init(OUTPUT_DEVICE_HEADPHONE1, 10, wav_header.nSamplesPersec, 32) == 0)
		{
			log_success("Audio codec was successfully initialized");
		} else {
			log_error("Failed to initialize audio codec");
		}
    }
    else
    {
		if (BSP_AUDIO_OUT_Init(OUTPUT_DEVICE_HEADPHONE1, 10, 44100, 32) == 0)
		{
			log_success("Audio codec was successfully initialized");
		} else {
			log_error("Failed to initialize audio codec");
		}
    }
    //BSP_AUDIO_OUT_SetAudioFrameSlot(SAI_SLOTACTIVE_0 | SAI_SLOTACTIVE_1);
}

int32_t pcm16_to_pcm32_basic(int16_t pcm16) {
    // 符号位扩展：将16位有符号整数转换为32位有符号整数
    return (int32_t)pcm16 * 65536 / 20;  // 编译器自动完成符号位扩展
}

void read_file()
{
	if (player_state == PLAYING)
	{
		if (RingBuffer_GetDataAvailable(ring_buffer) < AUDIO_BUFFER_SIZE * 5)
		{
			unsigned bytes_read = 0;
			if (FR_OK != f_read(&current_audio_file, file_buffer, AUDIO_BUFFER_SIZE, &bytes_read))
			{
				log_error("f_read fail.");
			}
			else
			{
				RingBuffer_Write(ring_buffer, file_buffer, bytes_read);
			}
		}
	}
}

int mp3_main(const char* file_path);

void flac_dec(const char* file_path, const char* wav_path)
{

	open_file(file_path, &current_audio_file);

	log_info("Creating FLAC reader");
	flac = create_flac(&current_audio_file);
	flac_reader = create_flac_reader(flac);

	log_info("Reading FLAC metadata");
	// TODO - handle errors instead of returning from a function
	if (read_metadata(flac, &flac_metadata) == 1) return;

	FIL wavfile;
	if (f_open(&wavfile, wav_path, FA_CREATE_ALWAYS | FA_WRITE) != FR_OK)
	{
		log_error("create wav file error");
		return;
	}
	wav_header.wChannels = 2;
	wav_header.nSamplesPersec = flac_metadata.sample_rate;
	wav_header.wBitsPerSample = 32;
	f_write(&wavfile, &wav_header, sizeof(wav_header), NULL);

	while(1)
	{
		unsigned bytes_to_read = AUDIO_BUFFER_SIZE;
		unsigned bytes_read = read_flac(flac_reader, pcm_buffer, bytes_to_read);
		if (bytes_read < bytes_to_read) {
			log_info("Reached end of file");
			break;
		}

		f_write(&wavfile, pcm_buffer, bytes_to_read, NULL);
	}

	f_close(&wavfile);

	log_info("Destroying flac reader");
	if (NULL != flac_reader)
	{
		free_flac_reader(flac_reader);
		flac_reader = NULL;
		destroy_flac(flac);
	}
	log_info("Closing file");
	f_close(&current_audio_file);
}

void start_player(const char* file_path) {
    log_info("Playing file %s", file_path);

    assert(player_state == STOPPED);

    //file_path = "/hifi.pcm";

    RingBuffer_Init(ring_buffer);

    FILINFO fno;
    if (FR_OK != f_stat(file_path, &fno))
    {
    	return;
    }

    char *extension = strrchr(file_path, '.');
    if (extension == NULL) {
        return;
    }
    is_flac = false;
    is_mp3 = false;
    is_wav = false;
    if (strcasecmp(extension, ".flac") == 0)
    {
    	/*
    	is_flac = false;
    	is_mp3 = false;
    	is_wav = true;
    	static char wavfile[255];
    	strcpy(wavfile, file_path);
    	char *pext = strrchr(wavfile, '.');
    	strcpy(pext, ".wav");
    	if (FR_OK != f_stat(wavfile, &fno))
    	{
    		flac_dec(file_path, wavfile);
    	}
    	file_path = wavfile;
    	*/
    	is_flac = true;
    }
    if (strcasecmp(extension, ".mp3") == 0)
    {
    	is_mp3 = true;
    	is_flac = false;
    }
    if (strcasecmp(extension, ".wav") == 0)
    {
    	is_wav = true;
    }

    player_state = PLAYING;

    int bits_per_sample = 0;

    if (is_mp3)
    {
    	//mp3_main(file_path);
    	//helix_main_mp3(file_path);
    	open_mp3(file_path);
		unsigned bytes_to_read = AUDIO_BUFFER_SIZE / 2;
    	decode_mp3(pcm_buffer, bytes_to_read);
    	bits_per_sample = 16;
    }
    else if(is_flac)
    {

    	//flac_main(file_path);

    	open_file(file_path, &current_audio_file);

		log_info("Creating FLAC reader");
		flac = create_flac(&current_audio_file);
		flac_reader = create_flac_reader(flac);

		log_info("Reading FLAC metadata");
		// TODO - handle errors instead of returning from a function
		if (read_metadata(flac, &flac_metadata) == 1) return;
		unsigned bytes_to_read = AUDIO_BUFFER_SIZE;
		unsigned bytes_read = read_flac(flac_reader, pcm_buffer, bytes_to_read);
		if (bytes_read < bytes_to_read) {
			log_info("Reached end of file");
			stop_player();
			return;
		}

		bits_per_sample = 32;
    }
    else if (is_wav)
    {
		if (0 != open_file(file_path, &current_audio_file))
		{
			return;
		}
		int rlen=0;
		if (FR_OK != f_read(&current_audio_file, &wav_header, sizeof(wav_header), &rlen))
		{
			log_error("f_read fail.");
		}
		if(rlen!=sizeof(wav_header)){
			log_error("read faliled");
			return;
		}
		unsigned bytes_read = 0;
		unsigned bytes_to_read = AUDIO_BUFFER_SIZE * (wav_header.wBitsPerSample / 8 / 4.0f);
		if (FR_OK != f_read(&current_audio_file, pcm_buffer, bytes_to_read, &bytes_read))
		{
			log_error("f_read fail.");
		}
		bits_per_sample = wav_header.wBitsPerSample;
    }
    else
    {
		if (0 != open_file(file_path, &current_audio_file))
		{
			return;
		}

		unsigned bytes_read = 0;
		unsigned bytes_to_read = AUDIO_BUFFER_SIZE / 2;
		if (FR_OK != f_read(&current_audio_file, pcm_buffer, bytes_to_read, &bytes_read))
		{
			log_error("f_read fail.");
		}
		bits_per_sample = 16;
    }

/*
    if (RingBuffer_GetDataAvailable(ring_buffer) >= bytes_to_read)
    {
    	RingBuffer_Read(ring_buffer, pcm_buffer, bytes_to_read);
    }
*/
    //audio_buffer_state = BUFFER_OFFSET_HALF;
    last_audio_buffer_state_change_time = HAL_GetTick();

    log_info("Starting playing audio file");

    //转换到32位
    //*
    if (bits_per_sample == 16)
    {
        for (int i=0; i<AUDIO_BUFFER_SIZE / 4; i++)
        {
        	int16_t data16 = ((int16_t*)pcm_buffer)[i];
        	int32_t data32 = pcm16_to_pcm32_basic(data16);
        	((int32_t*)audio_buffer)[i] = data32;
        }
    }
    else if (bits_per_sample == 24)
    {
        for (int i=0; i<AUDIO_BUFFER_SIZE / 4; i++)
        {
        	int32_t data32 = (int32_t)pcm_buffer[i + 2] << 16 | pcm_buffer[i + 1] << 8 | pcm_buffer[i + 0];
        	data32 = data32 * 128 / 20;
        	((int32_t*)audio_buffer)[i] = data32;
        }
    }
    else if (bits_per_sample == 32)
    {
        for (int i=0; i<AUDIO_BUFFER_SIZE / 4; i++)
        {
        	((int32_t*)audio_buffer)[i] = ((int32_t*)pcm_buffer)[i] / 20;
        }
    }
    //*/
/*
    for (int i=0; i<AUDIO_BUFFER_SIZE / 4; i++)
    {
    	audio_buffer[i * 4 + 1] = MusicData[i * 3 + 0];
    	audio_buffer[i * 4 + 2] = MusicData[i * 3 + 1];
    	audio_buffer[i * 4 + 3] = MusicData[i * 3 + 2];
    	audio_buffer[i * 4 + 0] = 0;
    }
*/
    /*
    uint32_t pcmIndex = 0;
    uint32_t i2sIndex = 0;

    for (pcmIndex = 0; pcmIndex < AUDIO_BUFFER_SIZE - 6; pcmIndex += 6) {
        // 24 位左声道数据（3 字节）
        uint32_t leftChannel = (MusicData[pcmIndex] << 0) | (MusicData[pcmIndex + 1] << 8) | (MusicData[pcmIndex + 2] << 16);

        // 24 位右声道数据（3 字节）
        uint32_t rightChannel = (MusicData[pcmIndex + 3] << 0) | (MusicData[pcmIndex + 4] << 8) | (MusicData[pcmIndex + 5] << 16);

        // 处理 I2S 标准格式：将 24 位 PCM 转换为 32 位 I2S 格式
        // 由于是 32 位传输，我们填充零并将左、右声道的 PCM 数据转换成 32 位格式
        i2sData[i2sIndex++] = (leftChannel << 8);
        i2sData[i2sIndex++] = (rightChannel << 8);
    }
    */

    //BSP_AUDIO_OUT_Play(i2sData, AUDIO_BUFFER_SIZE);
    //BSP_AUDIO_OUT_Play(MusicData, AUDIO_BUFFER_SIZE);

    osDelay(2000);
    initialize_codec();

    BSP_AUDIO_OUT_Play(audio_buffer, AUDIO_BUFFER_SIZE);

    BSP_AUDIO_OUT_Resume();

    log_info("Started playing");
}

void pause_player(void) {
    log_info("Pausing player");

    assert(player_state == PLAYING);

    player_state = PAUSED;
}

void resume_player(void) {
    log_info("Resuming player");

    assert(player_state == PAUSED);
    BSP_AUDIO_OUT_Resume();
    player_state = PLAYING;
}

void stop_player(void) {
    log_info("Stopping player");

    assert(player_state == PLAYING || player_state == PAUSED);

    log_info("Stopping audio codec");
    BSP_AUDIO_OUT_Stop(CODEC_PDWN_SW);
    player_state = STOPPED;

    if (is_mp3)
	{
    	//mp3_play_stop();
    	close_mp3();
	}
    else
    {
		log_info("Destroying flac reader");
		if (NULL != flac_reader)
		{
			free_flac_reader(flac_reader);
			flac_reader = NULL;
			destroy_flac(flac);
		}
		log_info("Closing file");
		f_close(&current_audio_file);
    }
    samples_played = 0;

    log_info("Stopped playing");
}

static BufferState get_buffer_state() {
    BufferState buffer_state = audio_buffer_state;
    audio_buffer_state = BUFFER_OFFSET_NONE;
    return buffer_state;
}


void update_player(void) {
    if (player_state == PLAYING) {
    	BufferState buffer_state = get_buffer_state();
        uint16_t bufsize = AUDIO_BUFFER_SIZE / 4;
        static BufferState prev_state = BUFFER_OFFSET_NONE;
        static int bits_per_sample = 0;

    	static bool have_data = false;
    	if (!have_data)
    	{
			//unsigned bytes_read = read_flac(flac_reader, pcm_buffer, bufsize);
            unsigned bytes_read = 0;

            if (is_mp3)
            {
            	//bufsize = AUDIO_BUFFER_SIZE / 2;
            	/*
            	if (0 == mp3_play_get_buffer(pcm_buffer, bufsize))
            	{
            		bytes_read = bufsize;
            	}
            	bits_per_sample = mp3ctrl->bitsPerSample;
            	*/
            	if (0 == decode_mp3(pcm_buffer, bufsize))
            	{
            		bytes_read = bufsize;
            	}
				bits_per_sample = 16;

            }
            else if(is_flac)
            {
            	bits_per_sample = 32;
            	bufsize = AUDIO_BUFFER_SIZE / 2;
            	bytes_read = read_flac(flac_reader, pcm_buffer, bufsize);
				/*
				//bufsize = AUDIO_BUFFER_SIZE / 2;
				if (FR_OK != f_read(&current_audio_file, pcm_buffer, bufsize, &bytes_read))
				{
					log_error("f_read fail.");
				}
				//*/
				//read_file();
				/*
				unsigned buffer_data_size = RingBuffer_GetDataAvailable(ring_buffer);
				if (buffer_data_size >= bufsize)
				{
					RingBuffer_Read(ring_buffer, pcm_buffer, bufsize);
					bytes_read = bufsize;
				}
				//*/
            }
            else if (is_wav)
            {
				if (FR_OK != f_read(&current_audio_file, pcm_buffer, bufsize, &bytes_read))
				{
					log_error("f_read fail.");
				}
				bits_per_sample = wav_header.wBitsPerSample;
            }
            else
            {
				if (FR_OK != f_read(&current_audio_file, pcm_buffer, bufsize, &bytes_read))
				{
					log_error("f_read fail.");
				}
				bits_per_sample = 16;
            }
            if (bytes_read < bufsize) {
                log_info("Stop at EOF");
                stop_player();
            }
            have_data = true;
    	}

        if (buffer_state && prev_state != buffer_state && have_data) {
        	prev_state = buffer_state;
            uint32_t offset;
            if (buffer_state == BUFFER_OFFSET_HALF) {
            	//log_info("BUFFER_OFFSET_HALF");
            	offset = 0;
            } else {
            	//log_info("BUFFER_OFFSET_FULL");
            	offset = AUDIO_BUFFER_SIZE / 2;
            }

		    //转换到32位
            //*
		    if (bits_per_sample == 16)
		    {
		        for (int i=0; i<AUDIO_BUFFER_SIZE / 4 / 2; i++)
		        {
		        	//int16_t data16 = ((int16_t*)pcm_buffer)[i];
		        	//int32_t data32 = pcm16_to_pcm32_basic(data16);
		        	//((int32_t*)(&audio_buffer[offset]))[i] = data32;
		        	int16_t data16 = ((int16_t)pcm_buffer[i*2 + 1]) << 8 | pcm_buffer[i*2 + 0];
		        	int32_t data32 = pcm16_to_pcm32_basic(data16);
		        	audio_buffer[offset + i * 4 + 3] = data32 >> 24 & 0xff;
		        	audio_buffer[offset + i * 4 + 2] = data32 >> 16 & 0xff;
		        	audio_buffer[offset + i * 4 + 1] = data32 >> 8 & 0xff;
		        	audio_buffer[offset + i * 4 + 0] = data32 >> 0 & 0xff;
		        }
		    }
		    else if (bits_per_sample == 24)
		    {
		        for (int i=0; i<AUDIO_BUFFER_SIZE / 4; i++)
		        {
		        	int32_t data32 = (int32_t)pcm_buffer[i + 2] << 16 | pcm_buffer[i + 1] << 8 | pcm_buffer[i + 0];
		        	data32 = data32 * 128 / 20;
		        	((int32_t*)audio_buffer)[offset + i] = data32;
		        }
		    }
		    else if (bits_per_sample == 32)
		    {
		        for (int i=0; i<AUDIO_BUFFER_SIZE / 4; i++)
		        {
		        	((int32_t*)audio_buffer)[offset + i] = ((int32_t*)pcm_buffer)[i] / 20;
		        }
		    }
		    //*/
            //unsigned bytes_read = read_flac(flac_reader, &audio_buffer[offset], AUDIO_BUFFER_SIZE / 2);
            //samples_played += bytes_read / flac_metadata.channels / (flac_metadata.bits_per_sample / 8);

            //memcpy(&audio_buffer[offset], &MusicData[offset], AUDIO_BUFFER_SIZE / 2);
            //unsigned bytes_read = AUDIO_BUFFER_SIZE / 2;
            //samples_played += bytes_read / flac_metadata.channels / (flac_metadata.bits_per_sample / 8);
            //测试数据
            /*
            unsigned bytes_read;
            for (bytes_read = 0; bytes_read < bufsize / 2; bytes_read += SAMPLE_NUM)
            {
            	if (bufsize / 2 - bytes_read < SAMPLE_NUM)
            	{
            		memcpy(&audio_buffer[offset + bytes_read], SineTable, bufsize / 2 - bytes_read);
            		break;
            	}
            	else{
            		memcpy(&audio_buffer[offset + bytes_read], SineTable, SAMPLE_NUM);
            	}
            }
            bytes_read = bufsize / 2;
            samples_played += bytes_read / flac_metadata.channels / (flac_metadata.bits_per_sample / 8);
            //*/
		    have_data = false;
        }
    }
}

double get_playing_progress(void) {
    if (flac_metadata.total_samples == 0) {
        return 0;
    }
    double progress = (double) samples_played / flac_metadata.total_samples;
    if (progress > 1) {
        progress = 1;
    }
    return progress;
}

PlayerState get_player_state() {
    return player_state;
}

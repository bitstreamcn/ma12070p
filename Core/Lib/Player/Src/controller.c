#include "controller.h"
#include "files.h"
#include "flac_reader.h"
#include "player.h"
#include "utils.h"
#include "../../../Drivers/BSP/ma12070p/include/ma120x0.h"

#include "stm32h7xx_hal.h"

#include "fatfs.h"

static FileList file_list;
static uint8_t current_file_index = 0;

static char track_author[64];
static char track_name[128];

extern char SDPath[4];   /* SD logical drive path */
extern FATFS SDFatFS;    /* File system object for SD logical drive */

static const char *get_current_file_path(void) {
    static char path[MAX_FILE_PATH_LENGTH + 1];
    snprintf(path, MAX_FILE_PATH_LENGTH + 1, "%s", file_list.files[current_file_index].path);
    return path;
}

static void play_next() {
    PlayerState prev_state = get_player_state();
    if (prev_state != STOPPED) {
        stop_player();
    }

    current_file_index = (current_file_index + 1) % file_list.count;
    if (prev_state == PLAYING) {
        start_player(get_current_file_path());
    }
    log_debug("Current file index: %d", current_file_index);
}

static void play_previous() {
    PlayerState prev_state = get_player_state();
    if (prev_state != STOPPED) {
        stop_player();
    }

    if (get_playing_progress() <= 0.1) {
        current_file_index = (file_list.count + current_file_index - 1) % file_list.count;
        log_debug("Current file index: %d", current_file_index);
    }
    if (prev_state == PLAYING) {
        start_player(get_current_file_path());
    }
}

static void start() {
    PlayerState state = get_player_state();
    if (state == STOPPED) {
        start_player(get_current_file_path());
    } else if (state == PAUSED) {
        resume_player();
    }
}

static void pause() {
    pause_player();
}

void update_track_info(void) {
    get_author_and_track_name(get_current_file_path(), track_author, track_name);
}

void controller_task(void) {
    //set_debug_mode(true);
    log_info("FLAC player starts");


    int fat_mount = 0;

    uint32_t timer = HAL_GetTick();
    int isUsbConnect = 0;
    while (true) {
    	uint32_t now = HAL_GetTick();

    	if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_9) == GPIO_PIN_SET && isUsbConnect == 0) {
    	  osDelay(20);  // 确认低电平稳定
    	  if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_9) == GPIO_PIN_SET)
    	  {
    		  isUsbConnect = 1;
    	  }
    	}
    	else{
    		isUsbConnect = 0;
    	}

    	if (isUsbConnect) {
    	  if (fat_mount)
    	  {
    		  if (get_player_state() != STOPPED)
    		  {
    			  stop_player();
    		  }
			  // USB已连接成功，unmount SD卡
			  FRESULT rst = f_mount(NULL, SDPath, 0);
			  if (rst != FR_OK){
				  (void)(rst);
			  }
			  fat_mount = 0;
    	  }
    	}
    	else
    	{
    		if (!fat_mount)
    		{
			  // USB断开或未连接
    		  //*
			  FRESULT rst = f_mount(&SDFatFS, SDPath, 0);
			  if (rst != FR_OK){
				  (void)(rst);
			  }
			  fat_mount = 1;


			    wait_for_sd_card();

			    if (find_music_files("", &file_list) == 1) {
			        //return;
			    }
			    //initialize_codec();
			    update_track_info();
			    start();
			    //*/
    		}
    		else{
    			if (get_player_state() == STOPPED && file_list.count > 0)
    			{
    				current_file_index = (current_file_index + 1) % file_list.count;
    			}
    			start();
    		}

    	}

    	//read_file();

        //render_track_screen(track_name, track_author, 3, 0, get_playing_progress(), 182.42, get_player_state() == PLAYING);
    	/*
        if (is_next_button_active()) {
            play_next();
            update_track_info();
        } else if (is_back_button_active()) {
            play_previous();
            update_track_info();
        } else if (is_play_button_active()) {
            start();
        } else if (is_pause_button_active()) {
            pause();
        }
        */
        update_player();

        if ((timer - now) > 5000)
        {
        	//HAL_GPIO_WritePin(MUTE_GPIO_Port, MUTE_Pin, GPIO_PIN_SET);
        }

        osDelay(1);
    }
}

void check_task()
{
	while (1)
	{
		if (HAL_GPIO_ReadPin(ERROR_GPIO_Port, ERROR_Pin) == GPIO_PIN_RESET)
		{
			//log_error("ERROR!");
		}
		if (HAL_GPIO_ReadPin(CLIP_GPIO_Port, CLIP_Pin) == GPIO_PIN_RESET)
		{
			//log_error("CLIP!");
			set_MA_ocp_latch_clear(MA_ocp_latch_clear__shift);
		}
		uint8_t accumulated = get_MA_error_acc();
		uint8_t error = get_MA_error();

		if (error > 0)
		{
			//log_error("ERROR!");
			if (error & 0b10000000)
			{
				log_error("Bit 7: DC protection");
			}
			if (error & 0b1000000)
			{
				log_error("Bit 6: pin-to-pin low impedance protection ");
			}
			if (error & 0b100000)
			{
				log_error("Bit 5: over-temperature error");
			}
			if (error & 0b10000)
			{
				log_error("Bit 4: over-temperature warning");
			}
			if (error & 0b1000)
			{
				log_error("Bit 3: PVDD under-voltage protection");
			}
			if (error & 0b100)
			{
				log_error("Bit 2: pll error");
			}
			if (error & 0b10)
			{
				log_error("Bit 1: over-current protection");
			}
			if (error & 0b1)
			{
				log_error("Bit 0: flying capacitor over-voltage error");
			}
		}
		if (accumulated > 0)
		{
			set_MA_eh_clear(0);
			set_MA_eh_clear(1);
			set_MA_eh_clear(0);
			//log_error("ERROR!");
		}
		osDelay(100);
	}
}

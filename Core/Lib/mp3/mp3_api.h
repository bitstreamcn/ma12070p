/*
 * mp3_api.h
 *
 *  Created on: Nov 4, 2025
 *      Author: bitstream
 */

#ifndef LIB_MP3_MP3_API_H_
#define LIB_MP3_MP3_API_H_

#include "../dr_lib/dr_mp3.h"

void play_mp3(char* filename);

int open_mp3(char* file_path);
int decode_mp3(uint8_t * buff, int size);
void close_mp3();

extern drmp3 mp3;

#endif /* LIB_MP3_MP3_API_H_ */

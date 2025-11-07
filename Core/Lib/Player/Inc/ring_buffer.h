/*
 * ring_buffer.h
 *
 *  Created on: Nov 1, 2025
 *      Author: bitstream
 */

#ifndef LIB_PLAYER_INC_RING_BUFFER_H_
#define LIB_PLAYER_INC_RING_BUFFER_H_

#include <stdlib.h>
#include <stdint.h>

#define RING_BUFFER_SIZE 1024 * 1024 * 2 + 1   // 缓冲区大小

// 环形缓冲区结构体
typedef struct {
    uint8_t buffer[RING_BUFFER_SIZE];    // 存储数据的缓冲区
    volatile uint32_t read_head;    // 读指针
    volatile uint32_t write_head;   // 写指针
    volatile uint32_t lock;         // 锁，防止多线程访问时出错
} RingBuffer_t;

void RingBuffer_Init(RingBuffer_t *rb);
int RingBuffer_Read(RingBuffer_t *rb, uint8_t *data, uint32_t length);
int RingBuffer_Write(RingBuffer_t *rb, uint8_t *data, uint32_t length);
int RingBuffer_IsEmpty(RingBuffer_t *rb);
int RingBuffer_IsFull(RingBuffer_t *rb);
uint32_t RingBuffer_GetDataAvailable(RingBuffer_t *rb);
uint32_t RingBuffer_GetSpaceAvailable(RingBuffer_t *rb);

#endif /* LIB_PLAYER_INC_RING_BUFFER_H_ */

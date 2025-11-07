/*
 * ring_buffer.c
 *
 *  Created on: Nov 1, 2025
 *      Author: bitstream
 */
#include "ring_buffer.h"

// 初始化环形缓冲区
void RingBuffer_Init(RingBuffer_t *rb) {
    rb->read_head = 0;
    rb->write_head = 0;
    rb->lock = 0;  // 假设 0 为未锁定状态
}

// 锁定环形缓冲区（临界区保护）
int Lock(RingBuffer_t *rb) {
    if (__atomic_test_and_set(&rb->lock, __ATOMIC_ACQUIRE)) {
        return -1;  // 如果已经被锁定，则返回失败
    }
    return 0;  // 成功锁定
}

// 解锁环形缓冲区
void Unlock(RingBuffer_t *rb) {
    __atomic_clear(&rb->lock, __ATOMIC_RELEASE);  // 释放锁
}

// 写多个字节数据到环形缓冲区
int RingBuffer_Write(RingBuffer_t *rb, uint8_t *data, uint32_t length) {
    if (Lock(rb) != 0) {
        return -1;  // 锁定失败，可能是多线程竞争
    }

    uint32_t space_available = RingBuffer_GetSpaceAvailable(rb);

    if (length >= space_available) {
        Unlock(rb);
        return -1;  // 缓冲区没有足够空间写入数据
    }

    // 写入数据
    for (uint32_t i = 0; i < length; i++) {
        rb->buffer[rb->write_head] = data[i];
        uint32_t index = rb->write_head + 1;
        index %= RING_BUFFER_SIZE;
        rb->write_head = index;  // 更新写指针，并处理环绕
    }

    Unlock(rb);
    return 0;
}

// 从环形缓冲区读取多个字节数据
int RingBuffer_Read(RingBuffer_t *rb, uint8_t *data, uint32_t length) {
    if (Lock(rb) != 0) {
        return -1;  // 锁定失败，可能是多线程竞争
    }

    uint32_t data_available = RingBuffer_GetDataAvailable(rb);

    if (length > data_available) {
        Unlock(rb);
        return -1;  // 缓冲区没有足够的数据可供读取
    }

    // 读取数据
    for (uint32_t i = 0; i < length; i++) {
        data[i] = rb->buffer[rb->read_head];
        uint32_t index = rb->read_head + 1;
        index %= RING_BUFFER_SIZE;
        rb->read_head = index;  // 更新读指针，并处理环绕
    }

    Unlock(rb);
    return 0;
}

int RingBuffer_IsEmpty(RingBuffer_t *rb)
{
	return rb->read_head == rb->write_head;
}
int RingBuffer_IsFull(RingBuffer_t *rb)
{
	uint32_t index = rb->write_head + 1;
	index %= RING_BUFFER_SIZE;
	return index  == rb->read_head;
}

// 获取缓冲区的有效数据数量
uint32_t RingBuffer_GetDataAvailable(RingBuffer_t *rb) {
	uint32_t size = (RING_BUFFER_SIZE + rb->write_head - rb->read_head);
	size %= RING_BUFFER_SIZE;
    return size;
}

// 获取缓冲区的剩余空间
uint32_t RingBuffer_GetSpaceAvailable(RingBuffer_t *rb) {
    return (RING_BUFFER_SIZE - RingBuffer_GetDataAvailable(rb)) - 1;
}

// 示例使用：读写多个字节
void TestRingBuffer() {
    RingBuffer_t rb;
    uint8_t writeData[5] = {0x11, 0x22, 0x33, 0x44, 0x55};  // 要写入的数据
    uint8_t readData[5];  // 用于读取的数据
    uint32_t length = 5;  // 数据长度

    // 初始化环形缓冲区
    RingBuffer_Init(&rb);

    // 写入多个字节数据
    if (RingBuffer_Write(&rb, writeData, length) == 0) {
        // 写入成功
    } else {
        // 写入失败，缓冲区已满
    }

    // 从缓冲区读取多个字节数据
    if (RingBuffer_Read(&rb, readData, length) == 0) {
        // 读取成功
    } else {
        // 读取失败，缓冲区为空
    }
}


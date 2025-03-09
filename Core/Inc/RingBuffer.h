/*
 * RingBuffer.h
 *
 *  Created on: Mar 16, 2024
 *      Author: jakub
 */

#ifndef INC_RINGBUFFER_H_
#define INC_RINGBUFFER_H_

#define RING_BUFFER_SIZE 64

// Success status
typedef enum
{
	RB_OK       = 0,
	RB_ERROR	= 1
} RB_Status;

// Object Ring Buffer
typedef struct
{
	uint16_t Head; // Pointer to write
	uint16_t Tail; // Pointer to read
	uint8_t Buffer[RING_BUFFER_SIZE]; // Array to store data
} RingBuffer_t;

RingBuffer_t ReceiveBuffer;
uint8_t ReceivedLines;
uint8_t ReceivedData[32]; // A buffer for parsing

//Functions
// Write
RB_Status RB_Write(RingBuffer_t *Buf, uint8_t Value);
// Read
RB_Status RB_Read(RingBuffer_t *Buf, uint8_t *Value);
// Flush
void RB_Flush(RingBuffer_t *Buf);



#endif /* INC_RINGBUFFER_H_ */

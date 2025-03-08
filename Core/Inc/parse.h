/*
 * parse.h
 *
 *  Created on: Feb 24, 2025
 *      Author: jakub
 */

#ifndef INC_PARSE_H_
#define INC_PARSE_H_

#define ENDLINE '\n'

#include "RingBuffer.h"
//#include "usb_device.h"
#include "usbd_cdc_if.h"


void Parser_TakeLine(RingBuffer_t *Buf, uint8_t *Destination);
void Parser_Parse(uint8_t *DataToParse);
void Parser_ParseVelocity(uint8_t *DataToParse);


#endif /* INC_PARSE_H_ */

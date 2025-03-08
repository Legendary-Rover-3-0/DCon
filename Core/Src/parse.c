/*
 * parse.c
 *
 *  Created on: Feb 24, 2025
 *      Author: jakub
 */

#include "main.h"
#include "parse.h"
#include "RingBuffer.h"
#include "string.h"
#include "stdio.h"
#include "stdlib.h"

extern char rx_buffer[100];

//
// Get a one complete line from Ring Buffer
//
void Parser_TakeLine(RingBuffer_t *Buf, uint8_t *Destination)
{
	uint8_t Tmp;
	uint8_t i = 0;

	// Loop for every char in Ring Buffer
	do
	{
		// Read a one byte from Ring Buffer
		RB_Read(Buf, &Tmp);

		// Check if we take the endline char
		if(Tmp == ENDLINE)
		{
			// If endline - replace it with c-string end sign - 0x00
			Destination[i] = 0;
		}
		else
		{
			// If not endline - just write to work-buffer
			Destination[i] = Tmp;
		}

		i++; // increment array iterator
	}while(Tmp != ENDLINE); // If we hit the endline char - end the loop
}

void Parser_ParseVelocity(uint8_t *DataToParse)
{

	uint8_t cos_robimy_tu =0;
	cos_robimy_tu++;
	uint16_t Length = strlen((char*)DataToParse);


	CDC_Transmit_FS(DataToParse, Length);


}


void Parser_Parse(uint8_t *DataToParse)
{
	// Pointer to sub-string
	char *ParsePointer = strtok((char*)DataToParse, "="); // Create a sub-string
	char *ValuePointer = strtok(NULL, "="); // Get the value after '='


	if(strcmp("Velocity", ParsePointer) == 0)
	{
		Parser_ParseVelocity((uint8_t*)ValuePointer); // Pass the value to the function
	}
	// Compare provided array with line to parse with command template
//	if(strcmp("LED", ParsePointer) == 0)
//	{
//		Parser_ParseLED(); // Call a parsing function for the LED command
//	}
//	else if(strcmp("ENV", ParsePointer) == 0)
//	{
//		Parser_ParseENV(); // Call a parsing function for the ENV command
//	}
//	else if(strcmp("NAME", ParsePointer) == 0)
//	{
//		Parser_ParseNAME(); // Call a parsing function for the NAME command
//	}
}


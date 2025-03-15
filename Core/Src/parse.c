/*
 * parse.c
 *
 *  Created on: Feb 24, 2025
 *      Author: jakub
 */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#include "main.h"
#include "parse.h"
#include "RingBuffer.h"
#include "string.h"
#include "StepperMotor_Driver.h"

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

/**
 * @brief   Parsuje komendy sterujące silnikiem krokowym.
 * @param   pStepperCommand Wskaźnik na string z komendą (np. "STEPPER=EN.1").
 * @return  HAL_OK jeśli komenda została przetworzona poprawnie, HAL_ERROR w przypadku błędu.
 *
 * Dozwolone parametry:
 * - `EN.x`   - Włącza (`x=1`) lub wyłącza (`x=0`) silnik.
 * - `DR.x`   - Kierunek (`x=0` - do przodu, `x=1` - do tyłu).
 * - `RS.x`   - Rozdzielczość kroku (`x=0` - pełny, `1` - 1/2, `2` - 1/4, `3` - 1/8, `4` - 1/16).
 * - `ST.x`   - Wykonuje `x` kroków, max 254 kroki.
 * - Przykładowa komenda: STEPPER=EN.1
 */
HAL_StatusTypeDef Parser_StepperMotorCommand(uint8_t *pStepperCommand)
{
    HAL_StatusTypeDef retStatus = HAL_OK;

	int value = 0;
	StepperMotorEnable_Type enable = STEPPER_MOTOR_DISABLE;
	StepperMotorDirection_Type direction = STEPPER_MOTOR_FORWARD;
	StepperMotorResolution_Type resolution = STEP_FULL;

    char *pCommand = NULL;
    char *pParam = NULL;
    char *pValue = NULL;

    if (pStepperCommand == NULL) 
    {
        return HAL_ERROR;
    }

    pCommand = strtok((char*)pStepperCommand, "=");
    pParam = strtok(NULL, ".");
    pValue = strtok(NULL, ".");

    if (!pCommand || !pParam || !pValue)
    {
        retStatus = HAL_ERROR;
    }

    for (char *p = pValue; p && *p; ++p)
    {
        if (!isdigit((unsigned char)*p))
        {
            retStatus = HAL_ERROR;
        }
    }

    if (retStatus != HAL_ERROR)
    {
        value = atoi(pValue);

        if (strcmp("EN", pParam) == 0) // ENABLE
        {
            enable = (value == 1) ? STEPPER_MOTOR_ENABLE : STEPPER_MOTOR_DISABLE;
            StepperMotor_switchEnable(enable);
        }
        else if (strcmp("DR", pParam) == 0) // DIRECTION
        {
            direction = (value == 1) ? STEPPER_MOTOR_BACKWARD : STEPPER_MOTOR_FORWARD;
            StepperMotor_switchDirection(direction);
        }
        else if (strcmp("RS", pParam) == 0) // RESOLUTION
        {
            switch (value)
            {
                case 1: resolution = STEP_HALF; break;
                case 2: resolution = STEP_QUARTER; break;
                case 3: resolution = STEP_EIGHTH; break;
                case 4: resolution = STEP_SIXTEENTH; break;
                default: resolution = STEP_FULL; break;
            }
            StepperMotor_setStepResolution(resolution);
        }
        else if (strcmp("ST", pParam) == 0) // STEP
        {
            StepperMotor_step((uint8_t)value);
        }
        else
        {
            retStatus = HAL_ERROR;
        }
    }

    return retStatus;
}

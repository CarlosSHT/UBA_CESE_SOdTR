/* Copyright 2020, Juan Manuel Cruz.
 * All rights reserved.
 *
 * This file is part of Project => freertos_book_Example4_6
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */


/*--------------------------------------------------------------------*-

    taskButton.c (Released 2022-05)

--------------------------------------------------------------------

    task file for FreeRTOS - Event Driven System (EDS) - Project for
    STM32F429ZI_NUCLEO_144.

    See readme.txt for project information.

-*--------------------------------------------------------------------*/


// ------ Includes -------------------------------------------------
/* Project includes. */
#include "main.h"
#include "cmsis_os.h"

/* Standard includes. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Demo includes. */
#include "supportingFunctions.h"

/* Application includes. */
#include "taskLed.h"
#include "taskButton.h"

#if( TASKS_SCOPE == TASKS_OUTSIDE_MAIN)
// ------ Private constants ----------------------------------------

/* Define the strings that will be passed in as the Supporting Functions parameters.
 * These are defined const and off the stack to ensure they remain valid when the
 * tasks are executing. */
const char *pcTextForTask_BlinkingOn	= " - Blinking turn On \r\n";
const char *pcTextForTask_BlinkingOff	= " - Blinking turn Off\r\n";

#define 		buttonTickCntMAX	50

// ------ Private variables ----------------------------------------
extern QueueHandle_t QueueHandle;

// ------ Public functions prototypes ------------------------------

/* Task Button thread */
void vTaskButton( void const * argument );

// ------ Public functions -----------------------------------------

/*------------------------------------------------------------------*/
/* Task Button thread */
void vTaskButton( void const * argument )
{
	/*  Declare & Initialize Task Function variables for argument, led, button and task */
//	LDX_Config_t * ptr = (LDX_Config_t *) argument;

	char *pcTaskName = ( char * ) pcTaskGetName( NULL );

	/* Print out the name of this task. */
	vPrintTwoStrings(pcTaskName, "- is running\r\n" );

	BaseType_t xStatus;
	ledFlag_t privLedFlag=NotBlinking;
	xStatus = xQueueSend( QueueHandle, &privLedFlag, 10 );
	/* As per most tasks, this task is implemented in an infinite loop. */
	for( ;; )
	{
		/* Check HW Button State */
		if( HAL_GPIO_ReadPin(USER_Btn_GPIO_Port, USER_Btn_Pin) == GPIO_PIN_SET )
		{
        	/* Check, Update and Print Led Flag */
			if( privLedFlag == NotBlinking )
			{
				privLedFlag = Blinking;
				vPrintTwoStrings(pcTaskName, pcTextForTask_BlinkingOn );
			}
			else
			{
				privLedFlag = NotBlinking;
            	vPrintTwoStrings(pcTaskName, pcTextForTask_BlinkingOff );
			}

			xStatus = xQueueSend( QueueHandle, &privLedFlag, 10 );

			if( xStatus != pdPASS )
			{
				/* We could not write to the queue because it was full – this must
				be an error as the queue should never contain more than one item! */
				vPrintString( "State led updated\r\n" );
			}
		}

		/* We want this task to execute every 50 milliseconds. */
		osDelay( buttonTickCntMAX );
	}
}
#endif

/*------------------------------------------------------------------*-
  ---- END OF FILE -------------------------------------------------
-*------------------------------------------------------------------*/

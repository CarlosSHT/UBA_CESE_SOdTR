/* Copyright 2020, Juan Manuel Cruz.
 * All rights reserved.
 *
 * This file is part of Project => freertos_book_Example2_6
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

 taskA.c (Released 2022-05)

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
#include "taskAll.h"

#if( TASKS_SCOPE == TASKS_OUTSIDE_MAIN)
// ------ Private constants ----------------------------------------

/* Define the strings that will be passed in as the Supporting Functions parameters.
 * These are defined const and off the stack to ensure they remain valid when the
 * tasks are executing. */
const char *pcTextForTask_IsRunning =     " -        is running\r\n";

const char *pcTextForTask_LDXTOn[]	=   { " -      LD1 turn On \r\n", \
										  " -      LD2 turn On \r\n", \
										  " -      LD3 turn On \r\n" };

const char *pcTextForTask_LDXTOff[]	=   { " -      LD1 turn Off\r\n", \
										  " -      LD2 turn Off\r\n", \
										  " -      LD3 turn Off\r\n" };

const char *pcTextForTask_BlinkingOn	= " - Blinking turn On \r\n";
const char *pcTextForTask_BlinkingOff	= " - Blinking turn Off\r\n";

#define 		buttonTickCntMAX	500
#define			ledTickCntMAX		500
typedef enum	ledFlag_e{ Blinking, NotBlinking } ledFlag_t;

uint16_t LDX_Pin[]			 	= { LD1_Pin,       LD2_Pin,       LD3_Pin };
GPIO_TypeDef* LDX_GPIO_Port[]	= { LD1_GPIO_Port, LD3_GPIO_Port, LD3_GPIO_Port };

// ------ Private variables ----------------------------------------

// ------ Public functions prototypes ------------------------------

/* Task Function thread */
void vTaskFunction( void const * argument );

// ------ Public functions -----------------------------------------

/*------------------------------------------------------------------*/
/* Task Function thread */
void vTaskFunction( void const * argument )
{
	/*  Declare & Initialize Task Function variables for argument, led, button and task */
	uint32_t index = (uint32_t) argument;

	ledFlag_t ledFlag = NotBlinking;
	GPIO_PinState ledState = GPIO_PIN_RESET;
	uint32_t ledTickCnt = osKernelSysTick();

	uint32_t buttonTickCnt = osKernelSysTick();

	char *pcTaskName = ( char * ) pcTaskGetName( NULL );

	/* Print out the name of this task. */
	vPrintTwoStrings(pcTaskName, pcTextForTask_IsRunning );

	/* As per most tasks, this task is implemented in an infinite loop. */
	for( ;; )
	{
		/* Check HW Button State */
		if( HAL_GPIO_ReadPin(USER_Btn_GPIO_Port, USER_Btn_Pin) == GPIO_PIN_SET )
		{
			/* Delay for a period using Tick Count */
			if( (osKernelSysTick() - buttonTickCnt) >= buttonTickCntMAX )
			{
        		/* Check, Update and Print Led Flag */
				if( ledFlag == NotBlinking )
				{
					ledFlag = Blinking;
                	vPrintTwoStrings(pcTaskName, pcTextForTask_BlinkingOn );
				}
				else
				{
					ledFlag = NotBlinking;
                	vPrintTwoStrings(pcTaskName, pcTextForTask_BlinkingOff );
				}
				/* Update and Button Tick Counter */
        		buttonTickCnt = osKernelSysTick();
			}
		}

		/* Check Led Flag */
		if( ledFlag == Blinking )
		{
			/* Delay for a period using Tick Count. */
			if( (osKernelSysTick() - ledTickCnt) >= ledTickCntMAX )
			{
				/* Check, Update and Print Led State */
		    	if( ledState == GPIO_PIN_RESET)
		    	{
		    		ledState = GPIO_PIN_SET;
                	vPrintTwoStrings(pcTaskName, pcTextForTask_LDXTOn[ index ] );
		    	}
		    	else
		    	{
		    		ledState = GPIO_PIN_RESET;
                	vPrintTwoStrings(pcTaskName, pcTextForTask_LDXTOff[ index ] );
		    	}
				/* Update HW Led State */
		    	HAL_GPIO_WritePin( LDX_GPIO_Port[ index ], LDX_Pin[ index ], ledState );

				/* Update and Led Tick Counter */
				ledTickCnt = osKernelSysTick();
			}
		}
	}
}
#endif

/* Separate code to keep the file clean */
#if( TASKS_SCOPE == TASKS_OUTSIDE_MAIN2)
// ------ Private constants ----------------------------------------

/* Define the strings that will be passed in as the Supporting Functions parameters.
 * These are defined const and off the stack to ensure they remain valid when the
 * tasks are executing. */
const char *pcTextForTask_IsRunning = " -        is running\r\n";

const char *pcTextForTask_LDXTOn[] = { " -      LD1 turn On \r\n",
		" -      LD2 turn On \r\n", " -      LD3 turn On \r\n" };

const char *pcTextForTask_LDXTOff[] = { " -      LD1 turn Off\r\n",
		" -      LD2 turn Off\r\n", " -      LD3 turn Off\r\n" };

const char *pcTextForTask_BlinkingOn = " - Blinking turn On \r\n";
const char *pcTextForTask_BlinkingOff = " - Blinking turn Off\r\n";

#define 		buttonTickCntMAX	500
#define			ledTickCntMAX		500

// ------ Private variables ----------------------------------------

// ------ Public functions prototypes ------------------------------

/* Task Function thread */
void vTaskFunction(void const *argument);

// ------ Public functions -----------------------------------------

/*------------------------------------------------------------------*/
/* Task Function thread */
void vTaskFunction(void const *argument) {
	/*  Convert argument to struct stmLeds with initialized variables (led, port pin, etc) */
	stmLeds *structLED = (stmLeds*) argument;

	ledFlag_t ledFlag = NotBlinking;
	GPIO_PinState ledState = GPIO_PIN_RESET;
	uint32_t ledTickCnt = osKernelSysTick();

	uint32_t buttonTickCnt = osKernelSysTick();

	char *pcTaskName = (char*) pcTaskGetName( NULL);

	/* Print out the name of this task. */
	vPrintTwoStrings(pcTaskName, pcTextForTask_IsRunning);

	/* As per most tasks, this task is implemented in an infinite loop. */
	for (;;) {
		/* Check HW Button State */
		if (HAL_GPIO_ReadPin(USER_Btn_GPIO_Port, USER_Btn_Pin)
				== GPIO_PIN_SET) {
			/* Delay for a period using Tick Count */
			if ((osKernelSysTick() - buttonTickCnt) >= buttonTickCntMAX) {
				/* Check, Update and Print Led Flag */
				if (ledFlag == NotBlinking) {
					ledFlag = Blinking;
					vPrintTwoStrings(pcTaskName, pcTextForTask_BlinkingOn);
				} else {
					ledFlag = NotBlinking;
					vPrintTwoStrings(pcTaskName, pcTextForTask_BlinkingOff);
				}

		    	structLED->statusBlink=ledFlag;

				/* Update and Button Tick Counter */
				buttonTickCnt = osKernelSysTick();
			}
		}

		/* Check Led Flag */
		if (ledFlag == Blinking) {
			/* Delay for a period using Tick Count. */
			if ((osKernelSysTick() - ledTickCnt) >= ledTickCntMAX) {
				/* Check, Update and Print Led State */
				if (ledState == GPIO_PIN_RESET) {
					ledState = GPIO_PIN_SET;
					vPrintTwoStrings(pcTaskName,
							pcTextForTask_LDXTOn[structLED->indexLed]);
				} else {
					ledState = GPIO_PIN_RESET;
					vPrintTwoStrings(pcTaskName,
							pcTextForTask_LDXTOff[structLED->indexLed]);
				}

		    	structLED->stateLed=ledState;

				/* Update HW Led State */
				HAL_GPIO_WritePin(structLED->LDX_GPIO_Port, structLED->LDX_Pin,
						ledState);

				/* Update and Led Tick Counter */
				ledTickCnt = osKernelSysTick();
			}
		}
	}
}
#endif
/*------------------------------------------------------------------*-
 ---- END OF FILE -------------------------------------------------
 -*------------------------------------------------------------------*/

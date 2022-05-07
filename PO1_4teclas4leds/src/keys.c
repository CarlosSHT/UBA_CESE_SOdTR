/*=============================================================================
 * Copyright (c) 2021, Franco Bucafusco <franco_bucafusco@yahoo.com.ar>
 * 					   Martin N. Menendez <mmenendez@fi.uba.ar>
 * All rights reserved.
 * License: Free
 * Date: 2021/10/03
 * Version: v1.0
 *===========================================================================*/

/*==================[ Inclusions ]============================================*/
#include "keys.h"

/*=====[ Definitions of private data types ]===================================*/


/*=====[Definition macros of private constants]==============================*/

/*=====[Prototypes (declarations) of private functions]======================*/


/*=====[Definitions of private global variables]=============================*/

/*=====[Definitions of public global variables]==============================*/
keys_ButtonState_t state[4];   //state of the key
tick_t time_down[4];		//timestamp of the last High to Low transition of the key
tick_t time_up[4];		    //timestamp of the last Low to High transition of the key
tick_t time_diff[4];	    //las time difference

// const gpioMap_t tecla = TEC1;

/*=====[prototype of private functions]=================================*/


/*=====[Implementations of public functions]=================================*/
/**
   @brief Obtiene el tiempo de la ultima pulsacion de la tecla indicada.

   @param index
   @return TickType_t
 */
tick_t keys_get_diff( int32_t idx )
{
    return time_diff[idx];
}

/**
   @brief Borra el tiempo de la ultima pulsacion de la tecla indicada.

   @param index
 */
void keys_clear_diff( int32_t idx )
{
    time_diff[idx] = KEYS_INVALID_TIME;
}

/**
   @brief Inicializa el driver

 */
void keys_init( void )
{
    for (int var = 0; var < 4; ++var) {        
        state[var]          = BUTTON_UP;  // Set initial state
        time_down[var]      = KEYS_INVALID_TIME;
        time_up[var]        = KEYS_INVALID_TIME;
        time_diff[var]      = KEYS_INVALID_TIME;
	}
}

// keys_ Update State Function
/**
   @brief Keys SM for polled operation

 */
uint32_t  keys_update( gpioMap_t tecla )
{
    uint32_t event = KEYS_EVENT_NONE ;

    switch( state[tecla-TEC1] )
    {
        case STATE_BUTTON_UP:
            /* CHECK TRANSITION CONDITIONS */
            if( !gpioRead( tecla ) )
            {
                state[tecla-TEC1] = STATE_BUTTON_FALLING;
            }
            break;

        case STATE_BUTTON_FALLING:
            /* ENTRY */

            /* CHECK TRANSITION CONDITIONS */
            if( !gpioRead( tecla ) )
            {
                state[tecla-TEC1] = STATE_BUTTON_DOWN;

                /* ACCION DEL EVENTO !*/
                event = KEYS_EVENT_KEY_DOWN ;

                time_down[tecla-TEC1] = tickRead();
            }
            else
            {
                state[tecla-TEC1] = STATE_BUTTON_UP;
            }

            /* LEAVE */
            break;

        case STATE_BUTTON_DOWN:
            /* CHECK TRANSITION CONDITIONS */
            if( gpioRead( tecla ) )
            {
                state[tecla-TEC1] = STATE_BUTTON_RISING;
            }
            break;

        case STATE_BUTTON_RISING:
            /* ENTRY */

            /* CHECK TRANSITION CONDITIONS */

            if( gpioRead( tecla ) )
            {
                state[tecla-TEC1] = STATE_BUTTON_UP;

                /* ACCION DEL EVENTO ! */
                event = KEYS_EVENT_KEY_UP ;

                tick_t current_tick_count = tickRead();

                /* calculo el tiempo de pulsacion */
                if( time_down[tecla-TEC1] < current_tick_count )
                {
                    time_diff[tecla-TEC1]  = current_tick_count - time_down[tecla-TEC1];
                }
                else
                {
                    time_diff[tecla-TEC1]  = current_tick_count - time_down[tecla-TEC1] + 0xFFFFFFFF;
                }
            }
            else
            {
                state[tecla-TEC1] = STATE_BUTTON_DOWN;
            }

            /* LEAVE */
            break;
    }

    return event;
}

/*=====[Implementations of private functions]================================*/

/*=====[Implementations of private functions]=================================*/


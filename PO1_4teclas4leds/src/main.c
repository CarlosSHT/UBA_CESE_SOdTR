/*=============================================================================
 * Copyright (c) 2021, Franco Bucafusco <franco_bucafusco@yahoo.com.ar>
 * 					   Martin N. Menendez <mmenendez@fi.uba.ar>
 * All rights reserved.
 * License: Free
 * Date: 2021/10/03
 * Version: v1.0
 *===========================================================================*/

#include "keys.h"

// The maximum number of tasks required at any one time during the execution
// of the program. MUST BE ADJUSTED FOR EACH NEW PROJECT
#define SCHEDULER_MAX_TASKS   (9)

/*==================[inlcusiones]============================================*/

//#include "cooperative_os_scheduler_03_seos_Pont2014.h"   // <= own header (optional)
#include "sapi.h"        // <= sAPI header

#include "seos_pont_2014_isr.h"       // <= dispatcher and task management header
#include "seos_pont_2014_scheduler.h" // <= scheduler and system initialization header

/*==================[definiciones y macros]==================================*/
typedef enum
{
    STATE_OFF,
    STATE_ON
} led_state_t;

/*==================[definiciones de datos internos]=========================*/

/*==================[definiciones de datos externos]=========================*/

/*==================[declaraciones de funciones internas]====================*/
uint32_t led_state[4];

/*==================[declaraciones de funciones externas]====================*/
void keys_service_task( void* param );

/*==================[funcion principal]======================================*/

// FUNCION PRINCIPAL, PUNTO DE ENTRADA AL PROGRAMA LUEGO DE ENCENDIDO O RESET.
int main( void )
{
    // ---------- CONFIGURACIONES ------------------------------
    // Inicializar y configurar la plataforma
    boardConfig();

    /* inicializo el planificador */
    schedulerInit();

    /* inicializo el modulo de tecla */
    keys_init();
    for (int var = 0; var < 4; ++var) {
        led_state[var] = STATE_OFF;
	}
    /* agregamos al planificador una tarea que "consulte" el estado de la tecla TEC1,
       de manera periodica y controle LEDR */
    for (int var = TEC1; var < TEC1+4; ++var) {        
        schedulerAddTask( keys_service_task,   // funcion de tarea a agregar
                        (void*)var,            // parametro de la tarea
                        0,            // offset de ejecucion en ticks
                        DEBOUNCE_TIME // periodicidad de ejecucion en ticks
                        );
	}

    // FUNCION que inicializa la interrupcion que ejecuta el planificador de
    // tareas con tick cada 1ms.
    schedulerStart( 1 );

    // ---------- REPETIR POR SIEMPRE --------------------------
    while( TRUE )
    {
        // Se despachan (ejecutan) las tareas marcadas para su ejecucion.
        // Luego se pone el sistema en bajo consumo hasta que ocurra la
        // proxima interrupcion, en este caso la de Tick.
        // Al ocurrir la interrupcion de Tick se ejecutara el planificador
        // que revisa cuales son las tareas a marcar para su ejecucion.
        schedulerDispatchTasks();
    }

    // NO DEBE LLEGAR NUNCA AQUI, debido a que a este programa se ejecuta
    // directamenteno sobre un microcontroladore y no es llamado por ningun
    // Sistema Operativo, como en el caso de un programa para PC.
    return 0;
}

/*==================[definiciones de funciones internas]=====================*/

void task_led( void* param )
{
    if( led_state[(int32_t)param-TEC1] == STATE_OFF )
    {
        tick_t key_time_diff = keys_get_diff((int32_t)param-TEC1);

        /* toggle del led */
        gpioToggle( (int32_t)param );

        /* cambio de estado al led */
        led_state[(int32_t)param-TEC1] = STATE_ON;

        /* planifico el apagado del led */
        schedulerAddTask( task_led,               // funcion de tarea a agregar
                          (void*)param,                    // parametro de la tarea
                          key_time_diff,         // offset de ejecucion en ticks
                          0                     // periodicidad de ejecucion en ticks
                        );
    }
    else if( led_state[(int32_t)param-TEC1] == STATE_ON )
    {
        /* toggle del led */
        gpioToggle( (int32_t)param );

        /* cambio de estado al led */
        led_state[(int32_t)param-TEC1] = STATE_OFF;

        keys_clear_diff((int32_t)param-TEC1);
    }
}

/**
   @brief Funcion que se ejecuta cada DEBOUNCE_TIME ticks.

   @param param
 */
void keys_service_task( void* param )
{
    uint32_t event = keys_update((int32_t)param);

    if( event == KEYS_EVENT_KEY_DOWN )
    {
        /* no hago nada */
    }
    else if( event == KEYS_EVENT_KEY_UP )
    {
        /* planifico que la tarea de LED1 se ejecute en 0 ticks */
        schedulerAddTask(  task_led,      // funcion de tarea a agregar
                           (void*)(param+(LEDR-TEC1)),  // parametro de la tarea 
                                          // LEDR(40), LEDG(41), LEDB(42), LED1(43), TEC1(36)
                                          // LEDR(40) = param(36) + (LEDR(40) - TEC1(36))
                           0,             // offset -> 0 = "ejecutate inmediatamente"
                           0              // periodicidad de ejecucion en ticks
                        );
    }
}
/*==================[definiciones de funciones externas]=====================*/


/*==================[fin del archivo]========================================*/

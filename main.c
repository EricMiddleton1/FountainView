/*
 * Stickman V2
 *
 * Written by Eric Middleton
 */

#define LED_COUNT	94

#include <stdlib.h>
#include <string.h>

// Simplelink includes
#include "simplelink.h"

// driverlib includes
#include "hw_types.h"
#include "hw_ints.h"
#include "rom.h"
#include "rom_map.h"
#include "interrupt.h"
#include "prcm.h"
#include "utils.h"
#include "gpio.h"

// free_rtos/ti-rtos includes
#include "osi.h"

// common interface includes
#include "common.h"
#ifndef NOTERM
#include "uart_if.h"
#endif

#define APP_NAME                "Fountain Controller"
#define APPLICATION_VERSION     "0.1"
#define OSI_STACK_SIZE          2048

#include "pin_mux_config.h"


#include "Valve.h"

//*****************************************************************************
//
//! Board Initialization & Configuration
//! Note: Taken from wlan_ap example program
//!
//! \param  None
//!
//! \return None
//
//*****************************************************************************
static void
BoardInit(void)
{
/* In case of TI-RTOS vector table is initialize by OS itself */
#ifndef USE_TIRTOS
    //
    // Set vector table base
    //
#if defined(ccs) || defined(gcc)
    MAP_IntVTableBaseSet((unsigned long)&g_pfnVectors[0]);
#endif
#if defined(ewarm)
    MAP_IntVTableBaseSet((unsigned long)&__vector_table);
#endif
#endif
    //
    // Enable Processor
    //
    MAP_IntMasterEnable();
    MAP_IntEnable(FAULT_SYSTICK);

    PRCMCC3200MCUInit();
}

/*
 *  ======== main ========
 */
int main(void)
{
	//Board initialization
	BoardInit();

    //Initialize the pin configuration
    PinMuxConfig();

    //Clear the LEDs
    GPIOPinWrite(LED_PORT, LED_RED_PIN | LED_ORANGE_PIN | LED_GREEN_PIN, 0);

    //Set the green LED
    GPIOPinWrite(LED_PORT, LED_GREEN_PIN, LED_GREEN_PIN);

    //For debugging purposes
    //Perform unending self-tests
    while(1) {
    	static uint8_t toggle = 0xFF;

    	//Perform self test
    	//With solenoid switch frequency 6hz, duty cycle 20%
    	Valve_selfTest(33, 133);

    	GPIOPinWrite(LED_PORT, LED_ORANGE_PIN, LED_ORANGE_PIN & toggle);

    	toggle = ~toggle;
    }




/*
    int retval;

    //Start simplelink host
    retval = VStartSimpleLinkSpawnTask(SPAWN_TASK_PRIORITY);
    if(retval < 0) {
    	for(;;);
    }

    if(retval < 0) {
    	for(;;);
    }

    //Start task scheduler
    osi_start();
*/
    return 0;
}

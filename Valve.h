/*
 * Valve.h
 *
 *  Created on: Sep 17, 2016
 *      Author: eric
 *
 *  Note: This system is for generating animations using blocking routines. This is now deprecated.
 */

#ifndef VALVE_H_
#define VALVE_H_

#include <stdint.h>

#include "hw_types.h"
#include "gpio.h"
#include "hw_memmap.h"

//Port, pin definitions
#define	SOL0_PORT		GPIOA0_BASE
#define SOL0_PIN		0x08

#define	SOL1_PORT		GPIOA1_BASE
#define SOL1_PIN		0x10

#define	SOL2_PORT		GPIOA0_BASE
#define SOL2_PIN		0x40

#define	SOL3_PORT		GPIOA0_BASE
#define SOL3_PIN		0x10

#define	SOL4_PORT		GPIOA1_BASE
#define SOL4_PIN		0x40

#define	SOL5_PORT		GPIOA0_BASE
#define SOL5_PIN		0x80

#define	SOL6_PORT		GPIOA0_BASE
#define SOL6_PIN		0x04

#define	SOL7_PORT		GPIOA0_BASE
#define SOL7_PIN		0x20

#define	SOL8_PORT		GPIOA1_BASE
#define SOL8_PIN		0x01

#define	SOL9_PORT		GPIOA3_BASE
#define SOL9_PIN		0x40

#define SOL_COUNT		10

//ValveLine
//Bitfield representation of the on/off settings of each valve
typedef uint16_t ValveLine;

//ValveAnimation
//Object containing an array of ValveLines
//and period between each line change (in ms)
typedef struct {
	//Array of valve lines
	ValveLine* lines;
	uint32_t lineCount;

	//Time between line changes (ms)
	uint16_t period;
} ValveAnimation;


//Valve_selfTest
//Performs a self-test of the valve system
//This function will activate each valve individually from
//0 to SOL_COUNT-1 and back to 0
//This function will block until the self-test is complete
//param uint16_t: on time for each solenoid in milliseconds
//param uint16_t: off time for each solenoid in milliseconds
//return: void
void Valve_selfTest(uint16_t, uint16_t);


//ValveLine_output
//Outputs the given ValveLine values on GPIO pins
//param ValveLine: line to output
void ValveLine_output(ValveLine);


//ValveAnimation_init
//Initializes a ValveAnimation object
//param ValueAnimation*: object to initialize
//param uint32_t: number of lines in animation
//param uint16_t: time between lines in animation (ms)
//return: void
void ValveAnimation_init(ValveAnimation*, uint32_t, uint16_t);

//ValveAnimation_getState
//Gets an individual line from the ValueAnimation object
//param ValveAnimation*: object to get the line from
//param uint32_t: index of desired line
//return: ValveLine, the line at the given index
ValveLine ValveAnimation_getState(ValveAnimation*, uint32_t);

//ValveAnimation_setState
//Sets an individual line in the ValueAnimation object
//param ValveAnimation*: object to set the line in
//param uint32_t: index of line to set
//param ValveLine: ValveLine value to set
//return: void
void ValveAnimation_setState(ValveAnimation*, uint32_t, ValveLine);

//ValveAnimation_render
//Renders a ValveAnimation object
//This function will block until the animation has been fully displayed
//param ValveAnimation*: object to render
//return: void
void ValveAnimation_render(ValveAnimation*);


#endif /* VALVE_H_ */

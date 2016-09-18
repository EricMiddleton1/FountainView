/*
 * Foutain.c
 *
 *  Created on: Sep 17, 2016
 *      Author: eric
 */

#include "Fountain.h"

#include "hw_types.h"
#include "hw_memmap.h"
#include "timer.h"
#include "pin_mux_config.h"

//GPTM definitions
#define TIMER_PRESCALAR		80			//Gives tick=1us
#define TIMER_LOAD_VALUE	999			//Gives period=1ms

#define SOL_ON_TIME		33				//On time 33ms
#define SOL_OFF_TIME	133				//Off time >=133ms

#define STROBE_ON_TIME	1				//On time 1ms

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


#define SOL_COUNT		8


//const arrays for solenoid port/pin values, plus strobe output
const uint32_t _portArray[SOL_COUNT + 1] =
	{SOL0_PORT, SOL1_PORT, SOL2_PORT, SOL3_PORT, SOL4_PORT, SOL5_PORT, SOL6_PORT, SOL7_PORT, STROBE_PORT};
const uint8_t _pinArray[SOL_COUNT + 1] =
	{SOL0_PIN, SOL1_PIN, SOL2_PIN, SOL3_PIN, SOL4_PIN, SOL5_PIN, SOL6_PIN, SOL7_PIN, STROBE_PIN};

//Struct for valve state
typedef struct {
	enum {
		OFF, ON
	} position;
	uint32_t lastActuationTime;
} ValveState;

//Array of valve states
volatile ValveState _valveStates[SOL_COUNT + 1];
volatile uint32_t _timerTick;

//Local function prototypes
void _fountainTimerISR();

void _fountainSetValves(uint16_t);


void FountainSequence_init(FountainSequence* _sequence, uint32_t _maxEvents) {
	_sequence->events = (FountainEvent*)malloc(sizeof(FountainEvent) * _maxEvents);
	_sequence->eventCount = 0;
	_sequence->maxEvents = _maxEvents;
}

uint32_t FountainSequence_getEventCount(FountainSequence* _sequence) {
	return _sequence->eventCount;
}

uint32_t FountainSequence_getMaxEvents(FountainSequence* _sequence) {
	return _sequence->maxEvents;
}

void FountainSequence_insertEvent(FountainSequence* _sequence, FountainEvent _event) {
	//Do a linear search through the array to find where to insert this event
	//TODO: replace this with a binary search
	FountainEvent* ePtr = _sequence->events;
	FountainEvent* eEnd = _sequence->events + _sequence->eventCount;
	for(; ePtr != eEnd && _event.timepoint > ePtr->timepoint; ++ePtr);

	//TODO: add checking to make sure we don't go over the maxSize

	if(ePtr == eEnd) {
		//Place the new event at the end of the array
		*ePtr = _event;

		_sequence->eventCount++;
	}
	else if(ePtr->timepoint == _event.timepoint) {
		//Just add the valves for the new event to the existing event
		ePtr->valvesToActuate |= _event.valvesToActuate;
	}
	else {
		//Move the elements starting at ePtr over by 1
		memmove(ePtr + 1, ePtr, sizeof(FountainEvent) * (eEnd - ePtr));

		//Insert the new event
		*ePtr = _event;

		_sequence->eventCount++;
	}
}

FountainEvent FountainSequence_getEvent(FountainSequence* _sequence, uint32_t _eventIndex) {
	return _sequence->events[_eventIndex];
}

void FountainSequence_display(FountainSequence* _sequence, uint32_t _startIndex) {
	//TODO: for use in OS environment, add yield inside event loop

	FountainEvent* ePtr = _sequence->events + _startIndex;
	FountainEvent* eEnd = _sequence->events + _sequence->eventCount;

	uint32_t offsetTime = _timerTick;// - ePtr->timepoint;

	for(; ePtr != eEnd; ++ePtr) {
		//Wait for time to actuate current event
		while(_timerTick < (offsetTime + ePtr->timepoint));

		//Set the valves indicated in the event
		_fountainSetValves(ePtr->valvesToActuate);
	}
}

void _fountainSetValves(uint16_t _values) {
	uint8_t i;
	for(i = 0; i < (SOL_COUNT + 1); ++i) {
		if(_values & (1 << i)) {
			Fountain_triggerValve(i);
		}
	}
}

void Fountain_start() {
	//Initialize the valve states array
	uint8_t i;
	for(i = 0; i < SOL_COUNT; ++i) {
		_valveStates[i].position = OFF;
		_valveStates[i].lastActuationTime = 0;
	}

	//Reset the timer tick value
	_timerTick = 0;

	//Initialize the GPTM
	TimerConfigure(TIMERA0_BASE,		//16-bit timer, periodic mode
			TIMER_CFG_SPLIT_PAIR | TIMER_CFG_A_PERIODIC);
	TimerPrescaleSet(TIMERA0_BASE, TIMER_A, TIMER_PRESCALAR);
	TimerLoadSet(TIMERA0_BASE, TIMER_A, TIMER_LOAD_VALUE);

	//Enable timer interupts
	TimerIntEnable(TIMERA0_BASE, TIMER_TIMA_TIMEOUT); //Interrupt on timeout
	TimerIntRegister(TIMERA0_BASE, TIMER_A, &_fountainTimerISR);

	//Start the timer
	TimerEnable(TIMERA0_BASE, TIMER_A);
}

void Fountain_stop() {
	//Stop the timer
	TimerDisable(TIMERA0_BASE, TIMER_A);

	//TODO: turn off all valves that are still on
}

int Fountain_triggerValve(uint8_t _valveID) {

	//Check that valve ID is valid, and that the solenoid is in a state that can be switched on
	if(_valveID < (SOL_COUNT + 1) &&
			_valveStates[_valveID].position == OFF && _timerTick >= (_valveStates[_valveID].lastActuationTime + SOL_OFF_TIME)) {
		GPIOPinWrite(_portArray[_valveID], _pinArray[_valveID], _pinArray[_valveID]);

		//Update the solenoid state
		TimerIntDisable(TIMERA0_BASE, TIMER_TIMA_TIMEOUT); //Turn of interrupts while we mess with the state array
		_valveStates[_valveID].position = ON;
		_valveStates[_valveID].lastActuationTime = _timerTick;
		TimerIntEnable(TIMERA0_BASE, TIMER_TIMA_TIMEOUT); //Turn the interrupts back on

		//GPIOPinWrite(LED_PORT, LED_RED_PIN, LED_RED_PIN);

		return 0;
	}
	else {
		return -1;
	}
}

void _fountainTimerISR() {
	//Clear interrupt flag
	TimerIntClear(TIMERA0_BASE, TimerIntStatus(TIMERA0_BASE, false));

	//Update timer tick
	_timerTick++;

	//Check each valve state
	uint8_t i;
	for(i = 0; i < SOL_COUNT; ++i) {
		if(_valveStates[i].position == ON &&
				_timerTick >= (_valveStates[i].lastActuationTime + SOL_ON_TIME)) {
			//Turn off the solenoid
			GPIOPinWrite(_portArray[i], _pinArray[i], 0);

			//Update state
			_valveStates[i].position = OFF;
			_valveStates[i].lastActuationTime = _timerTick;

			//GPIOPinWrite(LED_PORT, LED_RED_PIN, 0);
		}
	}

	//Check strobe state
	if(_valveStates[SOL_COUNT].position == ON &&
			_timerTick >= (_valveStates[SOL_COUNT].lastActuationTime + STROBE_ON_TIME)) {
		//Turn off the solenoid
		GPIOPinWrite(_portArray[SOL_COUNT], _pinArray[SOL_COUNT], 0);

		//Update state
		_valveStates[SOL_COUNT].position = OFF;
		_valveStates[SOL_COUNT].lastActuationTime = _timerTick;

		//GPIOPinWrite(LED_PORT, LED_RED_PIN, 0);
	}
}

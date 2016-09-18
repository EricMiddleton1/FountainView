/*
 * Fountain.h
 *
 *  Created on: Sep 17, 2016
 *      Author: eric
 */

#ifndef FOUNTAIN_H_
#define FOUNTAIN_H_

#include <stdint.h>

//Definitions for actuation outputs
#define VALVE_0		(1 << 0)
#define VALVE_1		(1 << 1)
#define VALVE_2		(1 << 2)
#define VALVE_3		(1 << 3)
#define VALVE_4		(1 << 4)
#define VALVE_5		(1 << 5)
#define VALVE_6		(1 << 6)
#define VALVE_7		(1 << 7)
#define VALVE_8		(1 << 8)
#define VALVE_9		(1 << 9)
#define STROBE		(1 << 10)


typedef struct {
	uint16_t valvesToActuate;
	uint32_t timepoint;
} FountainEvent;

//TODO: Consider changing the FountainSequence to a linked list
typedef struct {
	FountainEvent* events;
	uint32_t eventCount, maxEvents;
} FountainSequence;

void FountainSequence_init(FountainSequence*, uint32_t);

uint32_t FountainSequence_getEventCount(FountainSequence*);
uint32_t FountainSequence_getMaxEvents(FountainSequence*);

void FountainSequence_insertEvent(FountainSequence*, FountainEvent);

FountainEvent FountainSequence_getEvent(FountainSequence*, uint32_t);

void FountainSequence_display(FountainSequence*, uint32_t startIndex);


void Fountain_start();

void Fountain_stop();

int Fountain_triggerValve(uint8_t);


#endif /* FOUNTAIN_H_ */

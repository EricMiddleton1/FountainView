/*
 * Frame.c
 *
 *  Created on: Sep 18, 2016
 *      Author: eric
 */


#include "Frame.h"
#include <math.h>

#define STROBE_DELAY	10	//ms

void Frame_addPoint(FountainSequence* _sequence, int x, float y) {
	//x is the index of the solenoid
	//y will be converted into ms using physics magic
	uint32_t timepoint = 1000 * sqrt(2*(FRAME_HEIGHT - y) / 9.8f);

	//Create event
	FountainEvent event = {1 << x, timepoint};

	//Add the event to the sequence
	FountainSequence_insertEvent(_sequence, event);
}

void Frame_finalize(FountainSequence* _sequence) {
	//Get the timepoint of the last point in the sequence
	uint32_t timepoint = _sequence->events[_sequence->eventCount - 1].timepoint;

	//Create a new event for the strobe effect
	FountainEvent event = {STROBE, timepoint + STROBE_DELAY};

	//Add the strobe event to the sequence
	FountainSequence_insertEvent(_sequence, event);
}

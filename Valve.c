/*
 * Valve.c
 *
 *  Created on: Sep 17, 2016
 *      Author: eric
 */

#include "Valve.h"

#include "pin_mux_config.h"

//Utility function to delay milliseconds
void __waitMs(uint32_t ms);

void Valve_selfTest(uint16_t _onTime, uint16_t _offTime) {
	int8_t i;

	//Test each valve in the forward direction
	for(i = 0; i < SOL_COUNT; ++i) {
		//Trigger the solenoid valve
		ValveLine line = (1 << i);
		ValveLine_output(line);

		//Keep the solenoid on for _onTime ms
		__waitMs(_onTime);

		//Stop the solenoid
		ValveLine_output((ValveLine)0);

		//Keep the solenoid off for _offTime ms
		__waitMs(_offTime);
	}

	//Test each valve in the reverse direction
	for(i = (SOL_COUNT-1); i >= 0; --i) {
		//Trigger the solenoid valve
		ValveLine line = (1 << i);
		ValveLine_output(line);

		//Keep the solenoid on for _onTime ms
		__waitMs(_onTime);

		//Stop the solenoid
		ValveLine_output((ValveLine)0);

		//Keep the solenoid off for _offTime ms
		__waitMs(_offTime);
	}
}

void ValveLine_output(ValveLine _line) {
	static const uint32_t portArray[SOL_COUNT] =
		{SOL0_PORT, SOL1_PORT, SOL2_PORT, SOL3_PORT, SOL4_PORT, SOL5_PORT, SOL6_PORT, SOL7_PORT, SOL8_PORT, SOL9_PORT};
	static const uint8_t pinArray[SOL_COUNT] =
		{SOL0_PIN, SOL1_PIN, SOL2_PIN, SOL3_PIN, SOL4_PIN, SOL5_PIN, SOL6_PIN, SOL7_PIN, SOL8_PIN, SOL9_PIN};

	uint8_t i;
	for(i = 0; i < SOL_COUNT; ++i) {
		//uint8_t pinValue = (_line & (1 << SOL_COUNT)) ? pinArray[i] : 0;
		if(_line & (1 << i)) {
			GPIOPinWrite(portArray[i], pinArray[i], pinArray[i]);
		}
		else {
			GPIOPinWrite(portArray[i], pinArray[i], 0);
		}
	}

	//For debugging purposes
	//Toggle the red LED
	GPIOPinWrite(LED_PORT, LED_RED_PIN, ~GPIOPinRead(LED_PORT, LED_RED_PIN));
}

void ValveAnimation_init(ValveAnimation* _animation, uint32_t _lineCount, uint16_t _period) {
	_animation->lines = (ValveLine*)malloc(sizeof(ValveLine) * _lineCount);
	_animation->lineCount = _lineCount;
	_animation->period = _period;
}

ValveLine ValveAnimation_getState(ValveAnimation* _animation, uint32_t _lineIndex) {
	if(_lineIndex < _animation->lineCount)
		return _animation->lines[_lineIndex];
	else
		return (ValveLine)0;
}


void ValveAnimation_setState(ValveAnimation* _animation, uint32_t _lineIndex, ValveLine _line) {
	if(_lineIndex < _animation->lineCount) {
		_animation->lines[_lineIndex] = _line;
	}
}

void ValveAnimation_render(ValveAnimation* _animation) {
	uint32_t i;
	for(i = 0; i < _animation->lineCount; ++i) {
		//Output the current line
		ValveLine_output(_animation->lines[i]);

		//Wait before outputing the next line
		__waitMs(_animation->period);
	}
}

void __waitMs(uint32_t ms) {
	//Convert the number of milliseconds
	//Into the number of loop cycles required by
	//UtilsDelay (each loop cycle takes 3 clock cycles)
	ms *= 26667;

	//Delay
	UtilsDelay(ms);
}

/*
 * Frame.h
 *
 *  Created on: Sep 18, 2016
 *      Author: eric
 */

#ifndef FRAME_H_
#define FRAME_H_

#include "Fountain.h"

#define FRAME_HEIGHT		1.	//Frame height of 1 meter
#define FRAME_WIDTH			8

void Frame_addPoint(FountainSequence*, int, float);

void Frame_finalize(FountainSequence*);




#endif /* FRAME_H_ */

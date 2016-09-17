//*****************************************************************************
// pin_mux_config.h
//
// configure the device pins for different signals
//
// Copyright (c) 2016, Texas Instruments Incorporated - http://www.ti.com/
// All rights reserved.
//
//  Redistribution and use in source and binary forms, with or without
//  modification, are permitted provided that the following conditions
//  are met:
//
//    Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
//
//    Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the
//    distribution.
//
//    Neither the name of Texas Instruments Incorporated nor the names of
//    its contributors may be used to endorse or promote products derived
//    from this software without specific prior written permission.
//
//  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
//  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
//  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
//  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
//  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
//  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
//  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
//  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
//  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
//  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
//  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
//*****************************************************************************

// This file was automatically generated on 7/30/2016 at 3:56:43 PM
// by TI PinMux version
//
//*****************************************************************************

#ifndef __PIN_MUX_CONFIG_H__
#define __PIN_MUX_CONFIG_H__

#include "hw_types.h"
#include "hw_memmap.h"
#include "hw_gpio.h"
#include "pin.h"
#include "gpio.h"

#include "prcm.h"
#include "rom.h"
#include "rom_map.h"


//---------------------------------
//------Board IO definitions-------
//---------------------------------
#define LED_PORT	GPIOA1_BASE
#define LED_RED_PIN		0x02
#define LED_ORANGE_PIN	0x04
#define LED_GREEN_PIN	0x08

#define SW2_PORT		GPIOA2_BASE
#define SW2_PIN			0x40

#define SW3_PORT		GPIOA1_BASE
#define SW3_PIN			0x20
//---------------------------------


//---------------------------------
//-----External IO definitions-----
//---------------------------------
#define	SOL1_PORT		GPIOA0_BASE
#define SOL1_PIN		0x08

#define	SOL2_PORT		GPIOA1_BASE
#define SOL2_PIN		0x10

#define	SOL3_PORT		GPIOA0_BASE
#define SOL3_PIN		0x40

#define	SOL4_PORT		GPIOA0_BASE
#define SOL4_PIN		0x10

#define	SOL5_PORT		GPIOA1_BASE
#define SOL5_PIN		0x40

#define	SOL6_PORT		GPIOA0_BASE
#define SOL6_PIN		0x80

#define	SOL7_PORT		GPIOA0_BASE
#define SOL7_PIN		0x04

#define	SOL8_PORT		GPIOA0_BASE
#define SOL8_PIN		0x20

#define	SOL9_PORT		GPIOA1_BASE
#define SOL9_PIN		0x01

#define	SOL10_PORT		GPIOA3_BASE
#define SOL10_PIN		0x40

#define STROBE_PORT		GPIOA0_BASE
#define STROBE_PIN		0x01
//---------------------------------

extern void PinMuxConfig(void);

#endif //  __PIN_MUX_CONFIG_H__

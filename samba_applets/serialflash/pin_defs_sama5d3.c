/* ----------------------------------------------------------------------------
 *         ATMEL Microcontroller Software Support
 * ----------------------------------------------------------------------------
 * Copyright (c) 2015, Atmel Corporation
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * - Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the disclaimer below.
 *
 * Atmel's name may not be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * DISCLAIMER: THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * ----------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------
 *        Headers
 *----------------------------------------------------------------------------*/

#include "pin_defs.h"
#include "compiler.h"

/*----------------------------------------------------------------------------
 *        Local constants
 *----------------------------------------------------------------------------*/

static const struct _pin spi0[] = PINS_SPI0;
static const struct _pin spi0_npcs0 = PIN_SPI0_NPCS0;
static const struct _pin spi0_npcs1 = PIN_SPI0_NPCS1;
static const struct _pin spi0_npcs2 = PIN_SPI0_NPCS2;
static const struct _pin spi0_npcs3 = PIN_SPI0_NPCS3;

static const struct _pin spi1[] = PINS_SPI1;
static const struct _pin spi1_npcs0 = PIN_SPI1_NPCS0;
static const struct _pin spi1_npcs1 = PIN_SPI1_NPCS1;
static const struct _pin spi1_npcs2 = PIN_SPI1_NPCS2;
static const struct _pin spi1_npcs3 = PIN_SPI1_NPCS3;

/*----------------------------------------------------------------------------
 *        Public constants
 *----------------------------------------------------------------------------*/

const struct serialflash_pin_definition serialflash_pin_defs[] = {
	{
		.instance = 0,
		.ioset = 1,
		.addr = SPI0,
		.num_pins = ARRAY_SIZE(spi0),
		.pins = spi0,
		.npcs_pins = {
			&spi0_npcs0, &spi0_npcs1,
			&spi0_npcs2, &spi0_npcs3
		}
	},
	{
		.instance = 1,
		.ioset = 1,
		.addr = SPI1,
		.num_pins = ARRAY_SIZE(spi1),
		.pins = spi1,
		.npcs_pins = {
			&spi1_npcs0, &spi1_npcs1,
			&spi1_npcs2, &spi1_npcs3
		}
	},
};

const int num_serialflash_pin_defs = ARRAY_SIZE(serialflash_pin_defs);

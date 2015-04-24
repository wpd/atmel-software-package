/* ---------------------------------------------------------------------------- */
/*                  Atmel Microcontroller Software Support                      */
/* ---------------------------------------------------------------------------- */
/* Copyright (c) 2015, Atmel Corporation                                        */
/*                                                                              */
/* All rights reserved.                                                         */
/*                                                                              */
/* Redistribution and use in source and binary forms, with or without           */
/* modification, are permitted provided that the following condition is met:    */
/*                                                                              */
/* - Redistributions of source code must retain the above copyright notice,     */
/* this list of conditions and the disclaimer below.                            */
/*                                                                              */
/* Atmel's name may not be used to endorse or promote products derived from     */
/* this software without specific prior written permission.                     */
/*                                                                              */
/* DISCLAIMER:  THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR   */
/* IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE   */
/* DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR ANY DIRECT, INDIRECT,      */
/* INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT */
/* LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,  */
/* OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF    */
/* LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING         */
/* NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, */
/* EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.                           */
/* ---------------------------------------------------------------------------- */
/*                                                                              */

#ifndef _CHIP_H_
#define _CHIP_H_

/*----------------------------------------------------------------------------
 *        Headers
 *----------------------------------------------------------------------------*/

#include "sama5d2.h"


#ifndef __ASSEMBLY__
typedef volatile unsigned int AT91_REG; /* Hardware register definition */
#define AT91_CAST(a) (a)
#else
#define AT91_CAST(a)
#endif

/*----------------------------------------------------------------------------
 *       PIOs Define
 *----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/**
 * This pages lists all the pio definitions contained in chip.h.
 * The constants are named using the following convention: PIN_* for a constant
 * which defines a single Pin instance (but may include several PIOs sharing the
 * same controller), and PINS_* for a list of Pin instances.
 */


#define PIO_ADD	PIOA

/* ========== Pio PIN definition for ADC peripheral ========== */

/* Not useful, managed directly by the ADC controller
#define PIN_AD0 { PIO_PD19X1_AD0, PIO_ADD, ID_PIOD, PIO_PERIPH_F, PIO_DEFAULT }
#define PIN_AD1 { PIO_PD20X1_AD1, PIO_ADD, ID_PIOD, PIO_PERIPH_F, PIO_DEFAULT }
#define PIN_AD2 { PIO_PD21X1_AD2, PIO_ADD, ID_PIOD, PIO_PERIPH_F, PIO_DEFAULT }
#define PIN_AD3 { PIO_PD22X1_AD3, PIO_ADD, ID_PIOD, PIO_PERIPH_F, PIO_DEFAULT }
#define PIN_AD4 { PIO_PD23X1_AD4, PIO_ADD, ID_PIOD, PIO_PERIPH_F, PIO_DEFAULT }
#define PIN_AD5 { PIO_PD24X1_AD5, PIO_ADD, ID_PIOD, PIO_PERIPH_F, PIO_DEFAULT }
#define PIN_AD6 { PIO_PD25X1_AD6, PIO_ADD, ID_PIOD, PIO_PERIPH_F, PIO_DEFAULT }
#define PIN_AD7 { PIO_PD26X1_AD7, PIO_ADD, ID_PIOD, PIO_PERIPH_F, PIO_DEFAULT }
#define PIN_AD8 { PIO_PD27X1_AD8, PIO_ADD, ID_PIOD, PIO_PERIPH_F, PIO_DEFAULT }
#define PIN_AD9 { PIO_PD28X1_AD9, PIO_ADD, ID_PIOD, PIO_PERIPH_F, PIO_DEFAULT }
#define PIN_AD10 { PIO_PD29X1_AD10, PIO_ADD, ID_PIOD, PIO_PERIPH_F, PIO_DEFAULT }
#define PIN_AD11 { PIO_PD30X1_AD11, PIO_ADD, ID_PIOD, PIO_PERIPH_F, PIO_DEFAULT }
*/
#define PIN_ADTRG { PIO_PD31A_ADTRG, PIO_ADD, ID_PIOD, PIO_PERIPH_A, PIO_DEFAULT }

/* ========== Pio PIN definition for AIC peripheral ========== */


/* ========== Pio PIN definition for ARM JTAG peripheral ========== */

/* JTAG IOSET 1 */
#define PINS_JTAG_IOS1 {\
	{ 0x0007c000, PIO_ADD, ID_PIOD, PIO_PERIPH_A, PIO_DEFAULT }}
/* JTAG IOSET 2 */
#define PINS_JTAG_IOS2 {\
	{ 0x000007c0, PIO_ADD, ID_PIOD, PIO_PERIPH_A, PIO_DEFAULT }}
/* JTAG IOSET 3 */
#define PINS_JTAG_IOS3 {\
	{ 0xf8000000, PIO_ADD, ID_PIOD, PIO_PERIPH_B, PIO_DEFAULT }}
/* JTAG IOSET 4 */
#define PINS_JTAG_IOS4 {\
	{ 0x07c00000, PIO_ADD, ID_PIOD, PIO_PERIPH_C, PIO_DEFAULT }}


/* ========== Pio PIN definition for CAN0 peripheral ========== */

#define PINS_CAN0_IOS0 { PIO_PC1C_CANTX0 | PIO_PC2C_CANRX0, PIO_ADD, ID_PIOC, PIO_PERIPH_C, PIO_DEFAULT }
#define PINS_CAN0_IOS1 { PIO_PC10E_CANTX0 | PIO_PC11E_CANRX0, PIO_ADD, ID_PIOC, PIO_PERIPH_E, PIO_DEFAULT }

/* ========== Pio PIN definition for CAN1 peripheral ========== */

#define PINS_CAN1_IOS0 { PIO_PC26D_CANTX1 | PIO_PC27D_CANRX1, PIO_ADD, ID_PIOC, PIO_PERIPH_D, PIO_DEFAULT }

/* ========== Pio PIN definition for CLASSD peripheral ========== */

/* To Do */

/* ========== Pio PIN definition for EBI peripheral ========== */

/* NFC IOSET 1 */
#define PINS_NFC_IOS1 {\
    { 0xffc00000, PIO_ADD, ID_PIOA, PIO_PERIPH_B, PIO_DEFAULT },\
    { 0x000007ff, PIO_ADD, ID_PIOB, PIO_PERIPH_B, PIO_DEFAULT },\
}
/* NFC IOSET 2 */
#define PINS_NFC_IOS2 {\
    { 0x003fffff, PIO_ADD, ID_PIOA, PIO_PERIPH_F, PIO_DEFAULT },\
}

/* ========== Pio PIN definition for FLEXCOM0 peripheral ========== */

/* FLEXCOM0 IOSET 1 */

// TWD = TXD = MOSI & TWCK = RXD = MISO
#define FLEXCOM0_IOS1 PIO_PB28C_FLEXCOM0_IO1 | PIO_PB29C_FLEXCOM0_IO2

// TWI
#define PINS_FLEXCOM0_TWI_IOS1 {\
	{ FLEXCOM0_IOS1, PIO_ADD, ID_PIOB, PIO_PERIPH_C, PIO_DEFAULT },\
}
// USART
#define PINS_FLEXCOM0_USART_IOS1 {\
	{ FLEXCOM0_IOS1, PIO_ADD, ID_PIOB, PIO_PERIPH_C, PIO_DEFAULT },\
	{ PIO_PC0C_FLEXCOM0_O1, PIO_ADD, ID_PIOC, PIO_PERIPH_C, PIO_DEFAULT },\
};
// USART HANDSHAKE
#define PINS_FLEXCOM0_USART_HS_IOS1 {\
	{ FLEXCOM0_IOS1, PIO_ADD, ID_PIOB, PIO_PERIPH_C, PIO_DEFAULT },\
	{ PIO_PB31C_FLEXCOM0_IO4, PIO_ADD, ID_PIOB, PIO_PERIPH_C, PIO_DEFAULT },\
	{ PIO_PC0C_FLEXCOM0_O1, PIO_ADD, ID_PIOC, PIO_PERIPH_C, PIO_DEFAULT },\
}
// SPI NPCS0
#define PINS_FLEXCOM0_SPI_NPCS0_IOS1 {\
	{ FLEXCOM0_IOS1, PIO_ADD, ID_PIOB,PIO_PERIPH_C, PIO_DEFAULT },\
	{ PIO_PB30C_FLEXCOM0_IO3, PIO_ADD, ID_PIOB, PIO_PERIPH_C, PIO_DEFAULT },\
	{ PIO_PB31C_FLEXCOM0_IO4, PIO_ADD, ID_PIOB, PIO_PERIPH_C, PIO_DEFAULT },\
}
// SPI NPCS1
#define PINS_FLEXCOM0_SPI_NPCS1_IOS1 {\
	{ FLEXCOM0_IOS1, PIO_ADD, ID_PIOB, PIO_PERIPH_C, PIO_DEFAULT },\
	{ PIO_PB30C_FLEXCOM0_IO3, PIO_ADD, ID_PIOB, PIO_PERIPH_C, PIO_DEFAULT },\
	{ PIO_PC0C_FLEXCOM0_O1, PIO_ADD, ID_PIOC, PIO_PERIPH_C, PIO_DEFAULT },\
}

/* ========== Pio PIN definition for FLEXCOM1 peripheral ========== */

/* FLEXCOM1 IOSET 1 */

// TWD = TXD = MOSI & TWCK = RXD = MISO
#define FLEXCOM1_IOS1 PIO_PA24A_FLEXCOM1_IO1 | PIO_PA23A_FLEXCOM1_IO2
// RTS and CTS
#define FLEXCOM1_IOS1_HS  PIO_PA25A_FLEXCOM1_IO4 | PIO_PA26A_FLEXCOM1_O1

// TWI
#define PINS_FLEXCOM1_TWI_IOS1 {\
	{ FLEXCOM1_IOS1, PIO_ADD, ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT },\
}
// USART
#define PINS_FLEXCOM1_USART_IOS1 {\
	{ FLEXCOM1_IOS1, PIO_ADD, ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT },\
}
// USART HANDSHAKE
#define PINS_FLEXCOM1_USART_HS_IOS1 {\
	{ FLEXCOM1_IOS1, PIO_ADD, ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT },\
	{ FLEXCOM1_IOS1_HS, PIO_ADD, ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT },\
}
// SPI NPCS0
#define PINS_FLEXCOM1_SPI_NPCS0_IOS1 {\
	{ FLEXCOM1_IOS1, PIO_ADD, ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT },\
	{ PIO_PA22A_FLEXCOM1_IO3, PIO_ADD, ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT },\
	{ PIO_PA25A_FLEXCOM1_IO4, PIO_ADD, ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT },\
}
// SPI NPCS1
#define PINS_FLEXCOM1_SPI_NPCS1_IOS1 {\
	{ FLEXCOM1_IOS1, PIO_ADD, ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT },\
	{ PIO_PA22A_FLEXCOM1_IO3, PIO_ADD, ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT },\
	{ PIO_PA26A_FLEXCOM1_O1, PIO_ADD, ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT },\
}

/* ========== Pio PIN definition for FLEXCOM2 peripheral ========== */

/* FLEXCOM2 IOSET 1 */

// TWD = TXD = MOSI & TWCK = RXD = MISO
#define FLEXCOM2_IOS1 PIO_PA6E_FLEXCOM2_IO1 | PIO_PA7E_FLEXCOM2_IO2
// RTS and CTS
#define FLEXCOM2_IOS1_USART_HS  PIO_PA9E_FLEXCOM2_IO4 | PIO_PA10E_FLEXCOM2_O1

// TWI
#define PINS_FLEXCOM2_TWI_IOS1 {\
	{ FLEXCOM2_IOS1, PIO_ADD, ID_PIOA, PIO_PERIPH_E, PIO_DEFAULT },\
}
// USART
#define PINS_FLEXCOM2_USART_IOS1 {\
	{ FLEXCOM2_IOS1, PIO_ADD, ID_PIOA, PIO_PERIPH_E, PIO_DEFAULT },\
}
// USART HANDSHAKE
#define PINS_FLEXCOM2_USART_HS_IOS1 {\
	{ FLEXCOM2_IOS1, PIO_ADD, ID_PIOA, PIO_PERIPH_E, PIO_DEFAULT },\
	{ FLEXCOM2_IOS1_USART_HS, PIO_ADD, ID_PIOA, PIO_PERIPH_E, PIO_DEFAULT },\
}
// SPI NPCS0
#define PINS_FLEXCOM2_SPI_NPCS0_IOS1 {\
	{ FLEXCOM2_IOS1, PIO_ADD, ID_PIOA, PIO_PERIPH_E, PIO_DEFAULT },\
	{ PIO_PA22A_FLEXCOM1_IO3, PIO_ADD, ID_PIOA, PIO_PERIPH_E, PIO_DEFAULT },\
	{ PIO_PA9E_FLEXCOM2_IO4, PIO_ADD, ID_PIOA, PIO_PERIPH_E, PIO_DEFAULT },\
}
// SPI NPCS1
#define PINS_FLEXCOM2_SPI_NPCS1_IOS1 {\
	{ FLEXCOM2_IOS1, PIO_ADD, ID_PIOA, PIO_PERIPH_E, PIO_DEFAULT },\
	{ PIO_PA22A_FLEXCOM1_IO3, PIO_ADD, ID_PIOA, PIO_PERIPH_E, PIO_DEFAULT },\
	{ PIO_PA10E_FLEXCOM2_O1, PIO_ADD, ID_PIOA, PIO_PERIPH_E, PIO_DEFAULT },\
}

/* FLEXCOM2 IOSET 2 */

// TWD = TXD = MOSI & TWCK = RXD = MISO
#define FLEXCOM2_IOS2 PIO_PD26C_FLEXCOM2_IO1 | PIO_PD27C_FLEXCOM2_IO2
// RTS and CTS
#define FLEXCOM2_IOS2_USART_HS PIO_PD29C_FLEXCOM2_IO4 | PIO_PD30C_FLEXCOM2_O1

// TWI
#define PINS_FLEXCOM2_TWI_IOS2 {\
	{ FLEXCOM2_IOS2, PIO_ADD, ID_PIOD, PIO_PERIPH_C, PIO_DEFAULT },\
}
// USART
#define PINS_FLEXCOM2_USART_IOS2 {\
	{ FLEXCOM2_IOS2, PIO_ADD, ID_PIOD, PIO_PERIPH_C, PIO_DEFAULT },\
}
// USART HANDSHAKE
#define PINS_FLEXCOM2_USART_HS_IOS2 {\
	{ FLEXCOM2_IOS2, PIO_ADD, ID_PIOD, PIO_PERIPH_C, PIO_DEFAULT },\
	{ FLEXCOM2_IOS2_USART_HS, PIO_ADD, ID_PIOD, PIO_PERIPH_C, PIO_DEFAULT ,\
}
// SPI NPCS0
#define PINS_FLEXCOM2_SPI_NPCS0_IOS2 {\
	{ FLEXCOM2_IOS2, PIO_ADD, ID_PIOD, PIO_PERIPH_C, PIO_DEFAULT },\
	{ PIO_PD28C_FLEXCOM2_IO3, PIO_ADD, ID_PIOD, PIO_PERIPH_C, PIO_DEFAULT },\
	{ PIO_PD29C_FLEXCOM2_IO4, PIO_ADD, ID_PIOD, PIO_PERIPH_C, PIO_DEFAULT },\
}
// SPI NPCS1
#define PINS_FLEXCOM2_SPI_NPCS1_IOS2 {\
	{ FLEXCOM2_IOS2, PIO_ADD, ID_PIOD, PIO_PERIPH_C, PIO_DEFAULT },\
	{ PIO_PD28C_FLEXCOM2_IO3, PIO_ADD, ID_PIOD, PIO_PERIPH_C, PIO_DEFAULT },\
	{ PIO_PD30C_FLEXCOM2_O1, PIO_ADD, ID_PIOD, PIO_PERIPH_C, PIO_DEFAULT },\
}

/* ========== Pio PIN definition for FLEXCOM3 peripheral ========== */

/* FLEXCOM3 IOSET 1 */

// TWD = TXD = MOSI & TWCK = RXD = MISO
#define FLEXCOM3_IOS1 PIO_PA15E_FLEXCOM3_IO1 | PIO_PA13E_FLEXCOM3_IO2
// RTS and CTS
#define FLEXCOM3_IOS1_USART_HS PIO_PA16E_FLEXCOM3_IO4 | PIO_PA17E_FLEXCOM3_O1

// TWI
#define PINS_FLEXCOM3_TWI_IOS1 {\
	{ FLEXCOM3_IOS1, PIO_ADD, ID_PIOA, PIO_PERIPH_E, PIO_DEFAULT },\
}
// USART
#define PINS_FLEXCOM3_USART_IOS1 {\
	{ FLEXCOM3_IOS1, PIO_ADD, ID_PIOA, PIO_PERIPH_E, PIO_DEFAULT },\
}
// USART HANDSHAKE
#define PINS_FLEXCOM3_USART_HS_IOS1 {\
	{ FLEXCOM3_IOS1, PIO_ADD, ID_PIOA, PIO_PERIPH_E, PIO_DEFAULT },\
	{ FLEXCOM3_IOS1_USART_HS, PIO_ADD, ID_PIOA, PIO_PERIPH_E, PIO_DEFAULT },\
}
// SPI NPCS0
#define PINS_FLEXCOM3_SPI_NPCS0_IOS1 {\
	{ FLEXCOM3_IOS1, PIO_ADD, ID_PIOA, PIO_PERIPH_E, PIO_DEFAULT },\
	{ PIO_PA14E_FLEXCOM3_IO3, PIO_ADD, ID_PIOA, PIO_PERIPH_E, PIO_DEFAULT },\
	{ PIO_PA16E_FLEXCOM3_IO4, PIO_ADD, ID_PIOA, PIO_PERIPH_E, PIO_DEFAULT },\
}
// SPI NPCS1
#define PINS_FLEXCOM3_SPI_NPCS1_IOS1 {\
	{ FLEXCOM3_IOS1, PIO_ADD, ID_PIOA, PIO_PERIPH_E, PIO_DEFAULT },\
	{ PIO_PA14E_FLEXCOM3_IO3, PIO_ADD, ID_PIOA, PIO_PERIPH_E, PIO_DEFAULT },\
	{ PIO_PA17E_FLEXCOM3_O1, PIO_ADD, ID_PIOA, PIO_PERIPH_E, PIO_DEFAULT },\
}

/* FLEXCOM3 IOSET 2 */

// TWD = TXD = MOSI & TWCK = RXD = MISO
#define FLEXCOM3_IOS2 PIO_PC20E_FLEXCOM3_IO1 | PIO_PC19E_FLEXCOM3_IO2
// RTS and CTS
#define FLEXCOM3_IOS2_USART_HS PIO_PC21E_FLEXCOM3_IO4 | PIO_PC22E_FLEXCOM3_O1

// TWI
#define PINS_FLEXCOM3_TWI_IOS2 {\
	{ FLEXCOM3_IOS2, PIO_ADD, ID_PIOC, PIO_PERIPH_E, PIO_DEFAULT },\
}
// USART
#define PINS_FLEXCOM3_USART_IOS2 {\
	{ FLEXCOM3_IOS2, PIO_ADD, ID_PIOC, PIO_PERIPH_E, PIO_DEFAULT },\
}
// USART HANDSHAKE
#define PINS_FLEXCOM3_USART_HS_IOS2 {\
	{ FLEXCOM3_IOS2, PIO_ADD, ID_PIOC, PIO_PERIPH_E, PIO_DEFAULT },\
	{ FLEXCOM3_IOS2_USART_HS, PIO_ADD, ID_PIOC, PIO_PERIPH_E, PIO_DEFAULT },\
}
// SPI NPCS0
#define PINS_FLEXCOM3_SPI_NPCS0_IOS2 {\
	{ FLEXCOM3_IOS2, PIO_ADD, ID_PIOC, PIO_PERIPH_E, PIO_DEFAULT },\
	{ PIO_PC18E_FLEXCOM3_IO3, PIO_ADD, ID_PIOC, PIO_PERIPH_E, PIO_DEFAULT },\
	{ PIO_PC21E_FLEXCOM3_IO4, PIO_ADD, ID_PIOC, PIO_PERIPH_E, PIO_DEFAULT },\
}
// SPI NPCS1
#define PINS_FLEXCOM3_SPI_NPCS1_IOS2 {\
	{ FLEXCOM3_IOS2, PIO_ADD, ID_PIOC, PIO_PERIPH_E, PIO_DEFAULT },\
	{ PIO_PC18E_FLEXCOM3_IO3, PIO_ADD, ID_PIOC, PIO_PERIPH_E, PIO_DEFAULT },\
	{ PIO_PC22E_FLEXCOM3_O1, PIO_ADD, ID_PIOC, PIO_PERIPH_E, PIO_DEFAULT },\
}

/* FLEXCOM3 IOSET 3 */

// TWD = TXD = MOSI & TWCK = RXD = MISO
#define FLEXCOM3_IOS3 PIO_PB23E_FLEXCOM3_IO1 | PIO_PB22E_FLEXCOM3_IO2
// RTS and CTS
#define FLEXCOM3_IOS3_USART_HS PIO_PB24E_FLEXCOM3_IO4 | PIO_PB25E_FLEXCOM3_O1

// TWI
#define PINS_FLEXCOM3_TWI_IOS3 {\
	{ FLEXCOM3_IOS3, PIO_ADD, ID_PIOB, PIO_PERIPH_E, PIO_DEFAULT },\
}
// USART
#define PINS_FLEXCOM3_USART_IOS3 {\
	{ FLEXCOM3_IOS3, PIO_ADD, ID_PIOB, PIO_PERIPH_E, PIO_DEFAULT },\
}
// USART HANDSHAKE
#define PINS_FLEXCOM3_IOS3_USART_HS {\
	{ FLEXCOM3_IOS3, PIO_ADD, ID_PIOB, PIO_PERIPH_E, PIO_DEFAULT },\
	{ FLEXCOM3_IOS3_USART_HS, PIO_ADD, ID_PIOB, PIO_PERIPH_E, PIO_DEFAULT },\
}
// SPI NPCS0
#define PINS_FLEXCOM3_SPI_NPCS0_IOS3 {\
	{ FLEXCOM3_IOS3, PIO_ADD, ID_PIOB, PIO_PERIPH_E, PIO_DEFAULT },\
	{ PIO_PB21E_FLEXCOM3_IO3, PIO_ADD, ID_PIOB, PIO_PERIPH_E, PIO_DEFAULT },\
	{ PIO_PB24E_FLEXCOM3_IO4, PIO_ADD, ID_PIOB, PIO_PERIPH_E, PIO_DEFAULT },\
}
// SPI NPCS1
#define PINS_FLEXCOM3_SPI_NPCS1_IOS3 {\
	{ FLEXCOM3_IOS3, PIO_ADD, ID_PIOB, PIO_PERIPH_E, PIO_DEFAULT },\
	{ PIO_PB21E_FLEXCOM3_IO3, PIO_ADD, ID_PIOB, PIO_PERIPH_E, PIO_DEFAULT },\
	{ PIO_PB25E_FLEXCOM3_O1, PIO_ADD, ID_PIOB, PIO_PERIPH_E, PIO_DEFAULT },\
}

/* ========== Pio PIN definition for FLEXCOM4 peripheral ========== */

/* FLEXCOM4 IOSET 1 */

/* TWD = TXD = MOSI & TWCK = RXD = MISO */
#define FLEXCOM4_IOS1   PIO_PC28B_FLEXCOM4_IO1 | PIO_PC29B_FLEXCOM4_IO2

// TWI
#define PINS_FLEXCOM4_TWI_IOS1 {\
	{ FLEXCOM4_IOS1, PIO_ADD, ID_PIOC, PIO_PERIPH_B, PIO_DEFAULT } \
}
// USART
#define PINS_FLEXCOM4_USART_IOS1 {\
	{ FLEXCOM4_IOS1, PIO_ADD, ID_PIOC, PIO_PERIPH_B, PIO_DEFAULT } \
}
// USART HANDSHAKE
#define PINS_FLEXCOM4_USART_HS_IOS1 {\
	{ FLEXCOM4_IOS1, PIO_ADD, ID_PIOC, PIO_PERIPH_B, PIO_DEFAULT },\
	{ PIO_PC31B_FLEXCOM4_IO4, PIO_ADD, ID_PIOC, PIO_PERIPH_B, PIO_DEFAULT },\
	{ PIO_PD0B_FLEXCOM4_O1, PIO_ADD, ID_PIOD, PIO_PERIPH_B, PIO_DEFAULT },\
}
// SPI NPCS0
#define PINS_FLEXCOM4_SPI_NPCS0_IOS1 {\
	{ FLEXCOM4_IOS1, PIO_ADD, ID_PIOC, PIO_PERIPH_B, PIO_DEFAULT },\
	{ PIO_PC30B_FLEXCOM4_IO3, PIO_ADD, ID_PIOC, PIO_PERIPH_B, PIO_DEFAULT },\
	{ PIO_PC31B_FLEXCOM4_IO4, PIO_ADD, ID_PIOC, PIO_PERIPH_B, PIO_DEFAULT },\
}
// SPI NPCS1
#define PINS_FLEXCOM4_SPI_NPCS1_IOS1 {\
	{ FLEXCOM4_IOS1, PIO_ADD, ID_PIOC, PIO_PERIPH_B, PIO_DEFAULT },\
	{ PIO_PC30B_FLEXCOM4_IO3, PIO_ADD, ID_PIOC, PIO_PERIPH_B, PIO_DEFAULT },\
	{ PIO_PD0B_FLEXCOM4_O1, PIO_ADD, ID_PIOD, PIO_PERIPH_B, PIO_DEFAULT } \
}

/* FLEXCOM4 IOSET 2 */

// TWD = TXD = MOSI & TWCK = RXD = MISO
#define FLEXCOM4_IOS2 PIO_PD12B_FLEXCOM4_IO1 | PIO_PD13B_FLEXCOM4_IO2
// RTS and CTS
#define FLEXCOM4_IOS2_USART_HS PIO_PD15B_FLEXCOM4_IO4 | PIO_PD16B_FLEXCOM4_O1

// TWI
#define PINS_FLEXCOM4_TWI_IOS2 {\
	{ FLEXCOM4_IOS2, PIO_ADD, ID_PIOD, PIO_PERIPH_B, PIO_DEFAULT },\
}
// USART
#define PINS_FLEXCOM4_USART_IOS2 {\
	{ FLEXCOM4_IOS2, PIO_ADD, ID_PIOD, PIO_PERIPH_B, PIO_DEFAULT },\
}
// USART HANDSHAKE
#define PINS_FLEXCOM4_USART_HS_IOS2 {\
	{ FLEXCOM4_IOS2, PIO_ADD, ID_PIOD, PIO_PERIPH_B, PIO_DEFAULT },\
	{ FLEXCOM4_IOS2_USART_HS, PIO_ADD, ID_PIOD, PIO_PERIPH_B, PIO_DEFAULT },\
}
// SPI NPCS0
#define PINS_FLEXCOM4_SPI_NPCS0_IOS2 {\
	{ FLEXCOM4_IOS2, PIO_ADD, ID_PIOD, PIO_PERIPH_B, PIO_DEFAULT },\
	{ PIO_PD14B_FLEXCOM4_IO3, PIO_ADD, ID_PIOD, PIO_PERIPH_B, PIO_DEFAULT },\
	{ PIO_PD15B_FLEXCOM4_IO4, PIO_ADD, ID_PIOD, PIO_PERIPH_B, PIO_DEFAULT },\
}
// SPI NPCS1
#define PINS_FLEXCOM4_SPI_NPCS1_IOS2 {\
	{ FLEXCOM4_IOS2, PIO_ADD, ID_PIOD, PIO_PERIPH_B, PIO_DEFAULT },\
	{ PIO_PD14B_FLEXCOM4_IO3, PIO_ADD, ID_PIOD, PIO_PERIPH_B, PIO_DEFAULT },\
	{ PIO_PD16B_FLEXCOM4_O1, PIO_ADD, ID_PIOD, PIO_PERIPH_B, PIO_DEFAULT },\
}

/* FLEXCOM4 IOSET 3 */

// TWD = TXD = MOSI & TWCK = RXD = MISO
#define FLEXCOM4_IOS3 PIO_PD21C_FLEXCOM4_IO1 | PIO_PD22C_FLEXCOM4_IO2
// RTS and CTS
#define FLEXCOM4_IOS3_USART_HS PIO_PD24C_FLEXCOM4_IO4 | PIO_PD25C_FLEXCOM4_O1

// TWI
#define PINS_FLEXCOM4_TWI_IOS3 {\
	{ FLEXCOM4_IOS3, PIO_ADD, ID_PIOD, PIO_PERIPH_C, PIO_DEFAULT },\
}
// USART
#define PINS_FLEXCOM4_USART_IOS3 {\
	{ FLEXCOM4_IOS3, PIO_ADD, ID_PIOD, PIO_PERIPH_C, PIO_DEFAULT },\
}
// USART HANDSHAKE
#define PINS_FLEXCOM4_USART_HS_IOS3 {\
	{ FLEXCOM4_IOS3, PIO_ADD, ID_PIOD, PIO_PERIPH_C, PIO_DEFAULT },\
	{ FLEXCOM4_IOS3_USART_HS, PIO_ADD, ID_PIOD, PIO_PERIPH_C, PIO_DEFAULT },\
}
// SPI NPCS0
#define PINS_FLEXCOM4_SPI_NPCS0_IOS3 {\
	{ FLEXCOM4_IOS3, PIO_ADD, ID_PIOD, PIO_PERIPH_C, PIO_DEFAULT },\
	{ PIO_PD23C_FLEXCOM4_IO3, PIO_ADD,ID_PIOD,  PIO_PERIPH_C, PIO_DEFAULT },\
	{ PIO_PD24C_FLEXCOM4_IO4, PIO_ADD, ID_PIOD, PIO_PERIPH_C, PIO_DEFAULT },\
}
// SPI NPCS1
#define PINS_FLEXCOM4_SPI_NPCS1_IOS3 {\
	{ FLEXCOM4_IOS3, PIO_ADD, ID_PIOD, PIO_PERIPH_C, PIO_DEFAULT },\
	{ PIO_PD23C_FLEXCOM4_IO3, PIO_ADD, ID_PIOD, PIO_PERIPH_C, PIO_DEFAULT },\
	{ PIO_PD25C_FLEXCOM4_O1, PIO_ADD, ID_PIOD, PIO_PERIPH_C, PIO_DEFAULT } \
}

/* ========== Pio PIN definition for GMAC peripheral ========== */

#define PIN_GTSUCOM_IOS1 {\
	{ PIO_PC9B_GTSUCOMP, PIO_ADD, ID_PIOC, PIO_PERIPH_B, PIO_DEFAULT },\
}
#define PINS_GMAC_MII_IOS1 {\
	{ 0x0FFFFC00, PIO_ADD, ID_PIOC, PIO_PERIPH_B, PIO_DEFAULT },\
}
#define PINS_GMAC_RMII_IOS1 {\
	{ 0x000FFC00, PIO_ADD, ID_PIOC, PIO_PERIPH_B, PIO_DEFAULT },\
}
#define PIN_GTSUCOM_IOS2 {\
	{ PIO_PD0D_GTSUCOMP, PIO_ADD, ID_PIOD, PIO_PERIPH_D, PIO_DEFAULT },\
}
#define PINS_GMAC_MII_IOS2 {\
	{ 0x0007FFFE, PIO_ADD, ID_PIOD, PIO_PERIPH_D, PIO_DEFAULT },\
}
#define PINS_GMAC_RMII_IOS2 {\
	{ 0x0007FE00, PIO_ADD, ID_PIOD, PIO_PERIPH_D, PIO_DEFAULT },\
}
#define PIN_GTSUCOM_IOS3 {\
	{ PIO_PB5F_GTSUCOMP, PIO_ADD, ID_PIOB, PIO_PERIPH_F, PIO_DEFAULT },\
}
#define PINS_GMAC_MII_IOS3 {\
	{ 0x00FFFFC0, PIO_ADD, ID_PIOB, PIO_PERIPH_F, PIO_DEFAULT },\
}
#define PINS_GMAC_RMII_IOS3 {\
	{ 0x00FFC000, PIO_ADD, ID_PIOB, PIO_PERIPH_F, PIO_DEFAULT },\
}

/* ========== Pio PIN definition for I2SC0 peripheral ========== */

#define I2S0_IOS1       PIO_PC1E_I2SCK0 | PIO_PC2E_I2SMCK0 | PIO_PC3E_I2SWS0 | PIO_PC4E_I2SDI0 | PIO_PC5E_I2SDO0
#define PINS_I2S0_IOS1 {\
	{ I2S0_IOS1, PIO_ADD, ID_PIOC, PIO_PERIPH_E, PIO_DEFAULT },\
}
#define I2S0_IOS2       PIO_PD19E_I2SCK0 | PIO_PD20E_I2SMCK0 | PIO_PD21E_I2SWS0 | PIO_PD22E_I2SDI0 | PIO_PD23E_I2SDO0
#define PINS_I2S0_IOS2 {\
	{ I2S0_IOS1, PIO_ADD, ID_PIOD, PIO_PERIPH_E, PIO_DEFAULT },\
}

/* ========== Pio PIN definition for I2SC1 peripheral ========== */

#define I2S1_IOS1       PIO_PB14D_I2SMCK1 | PIO_PB15D_I2SCK1 | PIO_PB16D_I2SWS1 | PIO_PB17D_I2SDI1 | PIO_PB18D_I2SDO1
#define PINS_I2S1_IOS1 {\
	{ I2S_IOS1, PIO_ADD, ID_PIOB, PIO_PERIPH_D, PIO_DEFAULT },\
}
#define I2S1_IOS2       PIO_PA14D_I2SMCK1 | PIO_PA15D_I2SCK1 | PIO_PA16D_I2SWS1 | PIO_PA17D_I2SDI1 | PIO_PA18D_I2SDO1
#define PINS_I2S1_IOS2 {\
	{ I2S_IOS2, PIO_ADD, ID_PIOA, PIO_PERIPH_D, PIO_DEFAULT },\
}

/* ========== Pio PIN definition for ISI peripheral ========== */

#define PIN_ISI_FIELD_IOS1 {\
	{ ID_PIOC, PIO_PC25C_ISI_FIELD, PIO_ADD, PIO_PERIPH_C, PIO_DEFAULT },\
}

#define PINS_ISI_IOS1 {\
	{ PIO_PC22C_ISI_VSYNC, ID_PIOC, PIO_ADD, PIO_PERIPH_C, PIO_DEFAULT },\
	{ PIO_PC23C_ISI_HSYNC, PIO_ADD, ID_PIOC, PIO_PERIPH_C, PIO_DEFAULT },\
	{ PIO_PC24C_ISI_MCK, PIO_ADD, ID_PIOC, PIO_PERIPH_C, PIO_DEFAULT },\
	{ PIO_PC21C_ISI_PCK, PIO_ADD, ID_PIOC, PIO_PERIPH_C, PIO_DEFAULT },\
	{ 0x001FFE00, PIO_ADD, ID_PIOC, PIO_PERIPH_C, PIO_DEFAULT },\
}

#define PIN_ISI_FIELD_IOS2 {\
	{ ID_PIOD, PIO_PD18E_ISI_FIELD, PIO_ADD, PIO_PERIPH_E, PIO_DEFAULT },\
}
#define PINS_ISI_IOS2 {\
	{ PIO_PD16E_ISI_VSYNC, PIO_ADD, ID_PIOD, PIO_PERIPH_E, PIO_DEFAULT },\
	{ PIO_PD17E_ISI_HSYNC, PIO_ADD, ID_PIOD, PIO_PERIPH_E, PIO_DEFAULT },\
	{ PIO_PD2E_ISI_MCK, PIO_ADD, ID_PIOD, PIO_PERIPH_E, PIO_DEFAULT },\
	{ PIO_PD15E_ISI_PCK, PIO_ADD, ID_PIOD, PIO_PERIPH_E, PIO_DEFAULT },\
	{ 0x00007FF8, PIO_ADD, ID_PIOD, PIO_PERIPH_E, PIO_DEFAULT },\
}

#define PIN_ISI_FIELD_IOS3 {\
	{ PIO_PC8F_ISI_FIELD, PIO_ADD, ID_PIOC, PIO_PERIPH_F, PIO_DEFAULT },\
}
#define PINS_ISI_IOS3 {\
	{ PIO_PC5F_ISI_VSYNC, PIO_ADD, ID_PIOC, PIO_PERIPH_F, PIO_DEFAULT },\
	{ PIO_PC6F_ISI_HSYNC, PIO_ADD, ID_PIOC, PIO_PERIPH_F, PIO_DEFAULT },\
	{ PIO_PC7F_ISI_MCK, PIO_ADD, ID_PIOC, PIO_PERIPH_F, PIO_DEFAULT },\
	{ PIO_PC4F_ISI_PCK, PIO_ADD, ID_PIOC, PIO_PERIPH_F, PIO_DEFAULT },\
	{ 0xFF000000, PIO_ADD, ID_PIOB, PIO_PERIPH_F, PIO_DEFAULT },\
	{ 0x0000000F, PIO_ADD, ID_PIOC, PIO_PERIPH_F, PIO_DEFAULT },\
}

#define PIN_ISI_FIELD_IOS4 {\
	{ ID_PIOD, PIO_PD23F_ISI_FIELD, PIO_ADD, PIO_PERIPH_F, PIO_DEFAULT },\
}
#define PINS_ISI_IOS4 {\
	{ PIO_PD21F_ISI_VSYNC, PIO_ADD, PIO_PERIPH_F, PIO_DEFAULT },\
	{ PIO_PD22F_ISI_HSYNC, PIO_ADD, PIO_PERIPH_F, PIO_DEFAULT },\
	{ PIO_PD11F_ISI_MCK, PIO_ADD, PIO_PERIPH_F, PIO_DEFAULT },\
	{ PIO_PD20F_ISI_PCK, PIO_ADD, PIO_PERIPH_F, PIO_DEFAULT },\
	{ 0x000FF000, PIO_ADD, PIO_PERIPH_F, PIO_DEFAULT },\
}

/* ========== Pio PIN definition for LCDC peripheral ========== */

/* LCD 24 bits */
#define PINS_LCD_IOS1 {\
	{ 0xFFFFF800, PIO_ADD, ID_PIOB, PIO_PERIPH_A, PIO_DEFAULT },\
	{ 0x000001FF, PIO_ADD, ID_PIOC, PIO_PERIPH_A, PIO_DEFAULT },\
}

/* LCD 18 bits */
#define PINS_LCD_IOS2 {\
	{ 0xFFFFF800, PIO_ADD, ID_PIOC, PIO_PERIPH_A, PIO_DEFAULT },\
	{ 0x00000003, PIO_ADD, ID_PIOD, PIO_PERIPH_A, PIO_DEFAULT },\
}

/* ========== Pio PIN definition for PDMIC peripheral ========== */

#define PINS_PDMIC_IOS1 {\
	{ PIO_PB26D_PDMDAT | PIO_PB27D_PDMCLK, PIO_ADD, ID_PIOB, PIO_PERIPH_D, PIO_DEFAULT },\
}
#define PINS_PDMIC_IOS2 {\
	{ PIO_PB11D_PDMDAT | PIO_PB12D_PDMCLK, PIO_ADD, ID_PIOB, PIO_PERIPH_D, PIO_DEFAULT },\
}

/* ========== Pio PIN definition for PMC peripheral ========== */

#define PINS_PCK0_IOS1 {\
	{ PIO_PD19A_PCK0, PIO_ADD, ID_PIOD, PIO_PERIPH_A, PIO_DEFAULT }}
#define PINS_PCK0_IOS2 {\
	{ PIO_PD31E_PCK0, PIO_ADD, ID_PIOD, PIO_PERIPH_E, PIO_DEFAULT }}
#define PINS_PCK0_IOS3 {\
	{ PIO_PC8D_PCK0, PIO_ADD, ID_PIOC, PIO_PERIPH_D, PIO_DEFAULT }}

#define PINS_PCK1_IOS1 {\
	{ PIO_PD6B_PCK1, PIO_ADD, ID_PIOD, PIO_PERIPH_B, PIO_DEFAULT }}
#define PINS_PCK1_IOS2 {\
	{ PIO_PC27C_PCK1, PIO_ADD, ID_PIOC, PIO_PERIPH_C, PIO_DEFAULT }}
#define PINS_PCK1_IOS3 {\
	{ PIO_PB13C_PCK1, PIO_ADD, ID_PIOB, PIO_PERIPH_C, PIO_DEFAULT }}
#define PINS_PCK1_IOS4 {\
	{ PIO_PB20E_PCK1, PIO_ADD, ID_PIOB, PIO_PERIPH_E, PIO_DEFAULT }}

#define PINS_PCK2_IOS1 {\
	{ PIO_PC28C_PCK2, PIO_ADD, ID_PIOC, PIO_PERIPH_C, PIO_DEFAULT }}
#define PINS_PCK2_IOS2 {\
	{ PIO_PD11B_PCK2, PIO_ADD, ID_PIOD, PIO_PERIPH_B, PIO_DEFAULT }}
#define PINS_PCK2_IOS3 {\
 	{ PIO_PA21B_PCK2, PIO_ADD, ID_PIOA, PIO_PERIPH_B, PIO_DEFAULT }}

/* ========== Pio PIN definition for PWM peripheral ========== */

#define PINS_PWMH0_IOS1 {\
	{ PIO_PA30D_PWMH0, PIO_ADD, ID_PIOA, PIO_PERIPH_D, PIO_DEFAULT }}
#define PINS_PWML0_IOS1 {\
	{ PIO_PA31D_PWML0, PIO_ADD, ID_PIOA, PIO_PERIPH_D, PIO_DEFAULT }}

#define PINS_PWMH1_IOS1 {\
	{ PIO_PB0D_PWMH1, PIO_ADD, ID_PIOB, PIO_PERIPH_D, PIO_DEFAULT }}
#define PINS_PWML1_IOS1 {\
	{ PIO_PB1D_PWML1, PIO_ADD, ID_PIOB, PIO_PERIPH_D, PIO_DEFAULT }}

#define PINS_PWMH2_IOS1 {\
	{ PIO_PB5C_PWMH2, PIO_ADD, ID_PIOB, PIO_PERIPH_C, PIO_DEFAULT }}
#define PINS_PWML2_IOS1 {\
	{ PIO_PB6C_PWML2, PIO_ADD, ID_PIOB, PIO_PERIPH_C, PIO_DEFAULT }}

#define PINS_PWMH3_IOS1 {\
	{ PIO_PB7C_PWMH3, PIO_ADD, ID_PIOB, PIO_PERIPH_C, PIO_DEFAULT }}
#define PINS_PWML3_IOS1 {\
	{ PIO_PB8C_PWML3, PIO_ADD, ID_PIOB, PIO_PERIPH_C, PIO_DEFAULT }}

#define PINS_EXTRG0_IOS1 {\
	{ PIO_PB3D_PWMEXTRG0, PIO_ADD, ID_PIOB, PIO_PERIPH_D, PIO_DEFAULT }}
#define PINS_EXTRG1_IOS1 {\
	{ PIO_PB10C_PWMEXTRG1, PIO_ADD, ID_PIOB, PIO_PERIPH_C, PIO_DEFAULT }}
#define PINS_PWMFI0_IOS1 {\
	{ PIO_PB2D_PWMFI0, PIO_ADD, ID_PIOB, PIO_PERIPH_D, PIO_DEFAULT }}
#define PINS_PWMFI1_IOS1 {\
	{ PIO_PB9C_PWMFI1, PIO_ADD, ID_PIOB, PIO_PERIPH_C, PIO_DEFAULT }}

/* ========== Pio PIN definition for QSPI0 peripheral ========== */

/* QSPI0 IOSET 1 */
#define PINS_QSPI0_IOS1 {\
    { 0x0000003f, PIO_ADD, ID_PIOA, PIO_PERIPH_B, PIO_DEFAULT },\
}
/* QSPI0 IOSET 2 */
#define PINS_QSPI0_IOS2 {\
    { 0x000fc000, PIO_ADD, ID_PIOA, PIO_PERIPH_C, PIO_DEFAULT },\
}
/* QSPI0 IOSET 3 */
#define PINS_QSPI0_IOS3 {\
    { 0x0fc00000, PIO_ADD, ID_PIOA, PIO_PERIPH_F, PIO_DEFAULT },\
}

/* ========== Pio PIN definition for QSPI1 peripheral ========== */

/* QSPI1 IOSET 1 */
#define PINS_QSPI1_IOS1 {\
    { 0x00000fc0, PIO_ADD, ID_PIOA, PIO_PERIPH_B, PIO_DEFAULT },\
}
/* QSPI1 IOSET 2 */
#define PINS_QSPI1_IOS2 {\
    { 0x000007e0, PIO_ADD, ID_PIOB, PIO_PERIPH_D, PIO_DEFAULT },\
}
/* QSPI1 IOSET 3 */
#define PINS_QSPI1_IOS3 {\
	{ 0x000fc000, PIO_ADD, ID_PIOB, PIO_PERIPH_E, PIO_DEFAULT },\
}

/* ========== Pio PIN definition for SPI0 peripheral ========== */

/* SPI0 IOSET 1 */

#define SPI0_BASE_IOSET1        PIO_PA14A_SPI0_SPCK | PIO_PA15A_SPI0_MOSI | PIO_PA16A_SPI0_MISO
#define SPI0_NPCS0_IOSET1       SPI0_BASE_IOSET1 | PIO_PA17A_SPI0_NPCS0
#define SPI0_NPCS1_IOSET1       SPI0_BASE_IOSET1 | PIO_PA18A_SPI0_NPCS1
#define SPI0_NPCS2_IOSET1       SPI0_BASE_IOSET1 | PIO_PA19A_SPI0_NPCS2
#define SPI0_NPCS3_IOSET1       SPI0_BASE_IOSET1 | PIO_PA20A_SPI0_NPCS3

#define PINS_SPI0_NPCS0_IOS1 {\
    { SPI0_NPCS0_IOSET1, PIO_ADD, ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT },\
}
#define PINS_SPI0_NPCS1_IOS1 {\
    { SPI0_NPCS1_IOSET1, PIO_ADD, ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT },\
}
#define PINS_SPI0_NPCS2_IOS1 {\
    { SPI0_NPCS2_IOSET1, PIO_ADD, ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT },\
}
#define PINS_SPI0_NPCS3_IOS1 {\
    { SPI0_NPCS3_IOSET1, PIO_ADD, ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT },\
}

/* SPI0 IOSET 2 */

#define SPI0_BASE_IOSET2        PIO_PB1C_SPI0_SPCK | PIO_PB0C_SPI0_MOSI
#define SPI0_NPCS0_IOSET2       PIO_PA31C_SPI0_MISO | PIO_PA30C_SPI0_NPCS0
#define SPI0_NPCS1_IOSET2       PIO_PA31C_SPI0_MISO | PIO_PA29C_SPI0_NPCS1
#define SPI0_NPCS2_IOSET2       PIO_PA31C_SPI0_MISO | PIO_PA27C_SPI0_NPCS2
#define SPI0_NPCS3_IOSET2       PIO_PA31C_SPI0_MISO | PIO_PA28C_SPI0_NPCS3

#define PINS_SPI0_NPCS0_IOS2 {\
    { SPI0_NPCS0_IOSET2, PIO_ADD, ID_PIOA, PIO_PERIPH_C, PIO_DEFAULT },\
    { SPI0_BASE_IOSET2, PIO_ADD, ID_PIOB, PIO_PERIPH_C, PIO_DEFAULT },\
}
#define PINS_SPI0_NPCS1_IOS2 {\
    { SPI0_NPCS1_IOSET2, PIO_ADD, ID_PIOA, PIO_PERIPH_C, PIO_DEFAULT },\
    { SPI0_BASE_IOSET2, PIO_ADD, ID_PIOB, PIO_PERIPH_C, PIO_DEFAULT },\
}
#define PINS_SPI0_NPCS2_IOS2 {\
    { SPI0_NPCS2_IOSET2, PIO_ADD, ID_PIOA, PIO_PERIPH_C, PIO_DEFAULT },\
    { SPI0_BASE_IOSET2, PIO_ADD, ID_PIOB, PIO_PERIPH_C, PIO_DEFAULT },\
}
#define PINS_SPI0_NPCS3_IOS2 {\
    { SPI0_NPCS3_IOSET2, PIO_ADD, ID_PIOA, PIO_PERIPH_C, PIO_DEFAULT },\
	{ SPI0_BASE_IOSET2, PIO_ADD, ID_PIOB, PIO_PERIPH_C, PIO_DEFAULT },\
}

/* ========== Pio PIN definition for SPI1 peripheral ========== */

/* SPI1 IOSET 1 */

#define SPI1_BASE_IOSET1        PIO_PC1D_SPI1_SPCK | PIO_PC2D_SPI1_MOSI | PIO_PC3D_SPI1_MISO
#define SPI1_NPCS0_IOSET1       SPI1_BASE_IOSET1 | PIO_PC4D_SPI1_NPCS0
#define SPI1_NPCS1_IOSET1       SPI1_BASE_IOSET1 | PIO_PC5D_SPI1_NPCS1
#define SPI1_NPCS2_IOSET1       SPI1_BASE_IOSET1 | PIO_PC6D_SPI1_NPCS2
#define SPI1_NPCS3_IOSET1       SPI1_BASE_IOSET1 | PIO_PC7D_SPI1_NPCS3


#define PINS_SPI1_NPCS0_IOS1 {\
    { SPI1_NPCS0_IOSET1, PIO_ADD, ID_PIOC, PIO_PERIPH_D, PIO_DEFAULT },\
}
#define PINS_SPI1_NPCS1_IOS1 {\
    { SPI1_NPCS1_IOSET1, PIO_ADD, ID_PIOC, PIO_PERIPH_D, PIO_DEFAULT },\
}
#define PINS_SPI1_NPCS2_IOS1 {\
    { SPI1_NPCS2_IOSET1, PIO_ADD, ID_PIOC, PIO_PERIPH_D, PIO_DEFAULT },\
}
#define PINS_SPI1_NPCS3_IOS1 {\
    { SPI1_NPCS3_IOSET1, PIO_ADD, ID_PIOC, PIO_PERIPH_D, PIO_DEFAULT },\
}

/* SPI1 IOSET 2 */

#define SPI1_BASE_IOSET2        PIO_PA22D_SPI1_SPCK | PIO_PA23D_SPI1_MOSI | PIO_PA24D_SPI1_MISO
#define SPI1_NPCS0_IOSET2       SPI1_BASE_IOSET2 | PIO_PA25D_SPI1_NPCS0
#define SPI1_NPCS1_IOSET2       SPI1_BASE_IOSET2 | PIO_PA26D_SPI1_NPCS1
#define SPI1_NPCS2_IOSET2       SPI1_BASE_IOSET2 | PIO_PA27D_SPI1_NPCS2
#define SPI1_NPCS3_IOSET2       SPI1_BASE_IOSET2 | PIO_PA28D_SPI1_NPCS3

#define PINS_SPI1_NPCS0_IOS2 {\
    { SPI1_NPCS0_IOSET2, PIO_ADD, ID_PIOA, PIO_PERIPH_D, PIO_DEFAULT },\
}
#define PINS_SPI1_NPCS1_IOS2 {\
    { SPI1_NPCS1_IOSET2, PIO_ADD, ID_PIOA, PIO_PERIPH_D, PIO_DEFAULT },\
}
#define PINS_SPI1_NPCS2_IOS2 {\
    { SPI1_NPCS2_IOSET2, PIO_ADD, ID_PIOA, PIO_PERIPH_D, PIO_DEFAULT },\
}
#define PINS_SPI1_NPCS3_IOS2 {\
    { SPI1_NPCS3_IOSET2, PIO_ADD, ID_PIOA, PIO_PERIPH_D, PIO_DEFAULT },\
}

/* SPI1 IOSET 3 */

#define SPI1_BASE_IOSET3        PIO_PD25A_SPI1_SPCK | PIO_PD26A_SPI1_MOSI | PIO_PD27A_SPI1_MISO
#define SPI1_NPCS0_IOSET3       SPI1_BASE_IOSET3 | PIO_PD28A_SPI1_NPCS0
#define SPI1_NPCS1_IOSET3       SPI1_BASE_IOSET3 | PIO_PD29A_SPI1_NPCS1
#define SPI1_NPCS2_IOSET3       SPI1_BASE_IOSET3 | PIO_PD30A_SPI1_NPCS2

#define PINS_SPI1_NPCS0_IOS3 {\
    { SPI1_NPCS0_IOSET3, PIO_ADD, ID_PIOD, PIO_PERIPH_A, PIO_DEFAULT },\
}
#define PINS_SPI1_NPCS1_IOS3 {\
    { SPI1_NPCS1_IOSET3, PIO_ADD, ID_PIOD, PIO_PERIPH_A, PIO_DEFAULT },\
}
#define PINS_SPI1_NPCS2_IOS3 {\
    { SPI1_NPCS2_IOSET3, PIO_ADD, ID_PIOD, PIO_PERIPH_A, PIO_DEFAULT },\
}

/* ========== Pio PIN definition for SSC0 peripheral ========== */

#define SSC0_IOS1_TX    PIO_PB20C_TK0 | PIO_PB21C_TF0 | PIO_PB22C_TD0
#define SSC0_IOS1_RX    PIO_PB23C_RD0 | PIO_PB24C_RK0 | PIO_PB25C_RF0
#define PINS_SSC0_IOS1 {\
	{ SSC0_IOS1_TX | SSC0_IOS1_RX, PIO_ADD, ID_PIOC, PIO_PERIPH_C, PIO_DEFAULT },\
}

#define SSC0_IOS2_TX    PIO_PC12E_TK0 | PIO_PC13E_TF0 | PIO_PB2PIO_PC14E_TD02C_TD0
#define SSC0_IOS2_RX    PIO_PC15E_RD0 | PIO_PC16E_RK0 | PIO_PC17E_RF0
#define PINS_SSC0_IOS2 {\
	{ SSC0_IOS2_TX | SSC0_IOS2_RX, PIO_ADD, ID_PIOC, PIO_PERIPH_E, PIO_DEFAULT },\
}

/* ========== Pio PIN definition for SSC1 peripheral ========== */

#define SSC1_IOS1_TX    PIO_PA14B_TK1 | PIO_PA15B_TF1 | PIO_PA16B_TD1
#define SSC1_IOS1_RX    PIO_PA17B_RD1 | PIO_PA18B_RK1 | PIO_PA19B_RF1
#define PINS_SSC1_IOS1 {\
	{ SSC1_IOS1_TX | SSC1_IOS1_RX, PIO_ADD, ID_PIOA, PIO_PERIPH_B, PIO_DEFAULT },\
}

#define SSC1_IOS2_TX    PIO_PB14C_TK1 | PIO_PB15C_TF1 | PIO_PB16C_TD1
#define SSC1_IOS2_RX    PIO_PB17C_RD1 | PIO_PB18C_RK1 | PIO_PB19C_RF1
#define PINS_SSC1_IOS2 {\
	{ SSC1_IOS2_TX | SSC1_IOS2_RX, PIO_ADD, ID_PIOB, PIO_PERIPH_C, PIO_DEFAULT },\
}

/* ========== Pio PIN definition for TC0 peripheral ========== */

#define PINS_TC0_TIO0_IOS1 {\
	{ PIO_PA19D_TIOA0, PIO_ADD, ID_PIOA, PIO_PERIPH_D, PIO_DEFAULT },\
	{ PIO_PA20D_TIOB0, PIO_ADD, ID_PIOA, PIO_PERIPH_D, PIO_DEFAULT },\
	{ PIO_PA21D_TCLK0, PIO_ADD, ID_PIOA, PIO_PERIPH_D, PIO_DEFAULT },\
}

#define PINS_TC0_TIO1_IOS1 {\
	{ PIO_PC3C_TIOA1, PIO_ADD, ID_PIOC, PIO_PERIPH_C, PIO_DEFAULT },\
	{ PIO_PC4C_TIOB1, PIO_ADD, ID_PIOC, PIO_PERIPH_C, PIO_DEFAULT },\
	{ PIO_PC5C_TCLK1, PIO_ADD, ID_PIOC, PIO_PERIPH_C, PIO_DEFAULT },\
}

#define PINS_TC0_TIO1_IOS2 {\
	{ PIO_PA27A_TIOA1, PIO_ADD, ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT },\
	{ PIO_PA28A_TIOB1, PIO_ADD, ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT },\
	{ PIO_PA29A_TCLK1, PIO_ADD, ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT },\
}

#define PINS_TC0_TIO1_IOS3 {\
	{ PIO_PD11A_TIOA1, PIO_ADD, ID_PIOD, PIO_PERIPH_A, PIO_DEFAULT },\
	{ PIO_PD12A_TIOB1, PIO_ADD, ID_PIOD, PIO_PERIPH_A, PIO_DEFAULT },\
	{ PIO_PD13A_TCLK1, PIO_ADD, ID_PIOD, PIO_PERIPH_A, PIO_DEFAULT },\
}

#define PINS_TC0_TIO2_IOS1 {\
	{ PIO_PB6A_TIOA2, PIO_ADD, ID_PIOB, PIO_PERIPH_A, PIO_DEFAULT },\
	{ PIO_PB7A_TIOB2, PIO_ADD, ID_PIOB, PIO_PERIPH_A, PIO_DEFAULT },\
	{ PIO_PB5A_TCLK2, PIO_ADD, PIO_CFGR_ID_PIOB, FUNC_PERIPH_A, PIO_DEFAULT },\
}

#define PINS_TC0_TIO2_IOS2 {\
	{ PIO_PB22D_TIOA2, PIO_ADD, ID_PIOB, PIO_PERIPH_D, PIO_DEFAULT },\
	{ PIO_PB23D_TIOB2, PIO_ADD, ID_PIOB, PIO_PERIPH_D, PIO_DEFAULT },\
	{ PIO_PB24D_TCLK2, PIO_ADD, ID_PIOB, PIO_PERIPH_D, PIO_DEFAULT },\
}

#define PINS_TC0_TIO2_IOS3 {\
	{ PIO_PD20A_TIOA2, PIO_ADD, ID_PIOD, PIO_PERIPH_A, PIO_DEFAULT },\
	{ PIO_PD21A_TIOB2, PIO_ADD, ID_PIOD, PIO_PERIPH_A, PIO_DEFAULT },\
	{ PIO_PD22A_TCLK2, PIO_ADD, ID_PIOD, PIO_PERIPH_A, PIO_DEFAULT },\
}

/* ========== Pio PIN definition for TC1 peripheral ========== */

#define PINS_TC1_TIO3_IOS1 {\
	{ PIO_PB9A_TIOA3, PIO_ADD, ID_PIOB, PIO_PERIPH_A, PIO_DEFAULT },\
	{ PIO_PB10A_TIOB3, PIO_ADD, ID_PIOB, PIO_PERIPH_A, PIO_DEFAULT },\
	{ PIO_PB8A_TCLK3, PIO_ADD, ID_PIOB, PIO_PERIPH_A, PIO_DEFAULT },\
}

#define PINS_TC1_TIO3_IOS2 {\
	{ PIO_PB19D_TIOA3, PIO_ADD, ID_PIOB, PIO_PERIPH_D, PIO_DEFAULT },\
	{ PIO_PB20D_TIOB3, PIO_ADD, ID_PIOB, PIO_PERIPH_D, PIO_DEFAULT },\
	{ PIO_PB21D_TCLK3, PIO_ADD, ID_PIOB, PIO_PERIPH_D, PIO_DEFAULT },\
}

#define PINS_TC1_TIO3_IOS3 {\
	{ PIO_PD29D_TIOA3, PIO_ADD, ID_PIOD, PIO_PERIPH_D, PIO_DEFAULT },\
	{ PIO_PD30D_TIOB3, PIO_ADD, ID_PIOD, PIO_PERIPH_D, PIO_DEFAULT },\
	{ PIO_PD31D_TCLK3, PIO_ADD, ID_PIOD, PIO_PERIPH_D, PIO_DEFAULT },\
}
#define PINS_TC1_TIO4_IOS1 {\
	{ PIO_PA9D_TIOA4, PIO_ADD, ID_PIOA, PIO_PERIPH_D, PIO_DEFAULT },\
	{ PIO_PA10D_TIOB4, PIO_ADD, ID_PIOA, PIO_PERIPH_D, PIO_DEFAULT },\
	{ PIO_PA11D_TCLK4, PIO_ADD, ID_PIOA, PIO_PERIPH_D, PIO_DEFAULT },\
}
#define PINS_TC1_TIO4_IOS2 {\
	{ PIO_PC9D_TIOA4, PIO_ADD, ID_PIOC, PIO_PERIPH_D, PIO_DEFAULT },\
	{ PIO_PC10D_TIOB4, PIO_ADD, ID_PIOC, PIO_PERIPH_D, PIO_DEFAULT },\
	{ PIO_PC11D_TCLK4, PIO_ADD, ID_PIOC, PIO_PERIPH_D, PIO_DEFAULT },\
}

#define PINS_TC1_TIO5_IOS1 {\
	{ PIO_PA6D_TIOA5, PIO_ADD, ID_PIOA, PIO_PERIPH_D, PIO_DEFAULT },\
	{ PIO_PA7D_TIOB5, PIO_ADD, ID_PIOA, PIO_PERIPH_D, PIO_DEFAULT },\
	{ PIO_PA8D_TCLK5, PIO_ADD, ID_PIOA, PIO_PERIPH_D, PIO_DEFAULT },\
}

#define PINS_TC1_TIO5_IOS2 {\
	{ PIO_PB28D_TIOA5, PIO_ADD, ID_PIOB, PIO_PERIPH_D, PIO_DEFAULT },\
	{ PIO_PB29D_TIOB5, PIO_ADD, ID_PIOB, PIO_PERIPH_D, PIO_DEFAULT },\
	{ PIO_PB30D_TCLK5, PIO_ADD, ID_PIOB, PIO_PERIPH_D, PIO_DEFAULT },\
}

/* ========== Pio PIN definition for TWIHS0 peripheral ========== */

/* TWI0 IOSET 1 */
#define PINS_TWI0_IOS1 {\
    { PIO_PB31D_TWD0, PIO_ADD, ID_PIOB, PIO_PERIPH_D, PIO_DEFAULT },\
    { PIO_PC0D_TWCK0, PIO_ADD, ID_PIOC, PIO_PERIPH_D, PIO_DEFAULT },\
}
/* TWI0 IOSET 2 */
#define PINS_TWI0_IOS2 {\
    { PIO_PC27E_TWD0, PIO_ADD, ID_PIOC, PIO_PERIPH_E, PIO_DEFAULT },\
    { PIO_PC28E_TWCK0, PIO_ADD, ID_PIOC, PIO_PERIPH_E, PIO_DEFAULT },\
}
/* TWI0 IOSET 3 */
#define PINS_TWI0_IOS3 {\
    { PIO_PD29E_TWD0, PIO_ADD, ID_PIOD, PIO_PERIPH_E, PIO_DEFAULT },\
    { PIO_PD30E_TWCK0, PIO_ADD, ID_PIOD, PIO_PERIPH_E, PIO_DEFAULT },\
}
/* TWI0 IOSET 4 */
#define PINS_TWI0_IOS4 {\
	{ PIO_PD21B_TWD0, PIO_ADD, ID_PIOD, PIO_PERIPH_B, PIO_DEFAULT },\
	{ PIO_PD22B_TWCK0, PIO_ADD, ID_PIOD, PIO_PERIPH_B, PIO_DEFAULT },\
}

/* ========== Pio PIN definition for TWIHS1 peripheral ========== */

/* TWI1 IOSET 1 */
#define PINS_TWI1_IOS1 {\
	{ PIO_PC6C_TWD1, PIO_ADD, ID_PIOC, PIO_PERIPH_C, PIO_DEFAULT },\
	{ PIO_PC7C_TWCK1, PIO_ADD, ID_PIOC, PIO_PERIPH_C, PIO_DEFAULT },\
}
/* TWI1 IOSET 2 */
#define PINS_TWI1_IOS2 {\
	{ PIO_PD4A_TWD1, PIO_ADD, ID_PIOD, PIO_PERIPH_A, PIO_DEFAULT },\
	{ PIO_PD5A_TWCK1, PIO_ADD, ID_PIOD, PIO_PERIPH_A, PIO_DEFAULT },\
}
/* TWI1 IOSET 3 */
#define PINS_TWI1_IOS3 {\
	{ PIO_PD19B_TWD1, PIO_ADD, ID_PIOD, PIO_PERIPH_B, PIO_DEFAULT },\
	{ PIO_PD20B_TWCK1, PIO_ADD, ID_PIOD, PIO_PERIPH_B, PIO_DEFAULT },\
}

/* ========== Pio PIN definition for UART0 peripheral ========== */

/* UART0 IOSET 1 */
#define PINS_UART0_IOS1 {\
	{ PIO_PB26C_URXD0, PIO_ADD, ID_PIOB, PIO_PERIPH_C, PIO_DEFAULT },\
	{ PIO_PB27C_UTXD0, PIO_ADD, ID_PIOB, PIO_PERIPH_C, PIO_DEFAULT },\
}

/* ========== Pio PIN definition for UART1 peripheral ========== */

/* UART1 IOSET 1 */
#define PINS_UART1_IOS1 {\
	{ PIO_PD2A_URXD1, PIO_ADD, ID_PIOD, PIO_PERIPH_A, PIO_DEFAULT },\
	{ PIO_PD3A_UTXD1, PIO_ADD, ID_PIOD, PIO_PERIPH_A, PIO_DEFAULT },\
}
/* UART1 IOSET 2 */
#define PINS_UART1_IOS2 {\
	{ PIO_PC7E_URXD1, PIO_ADD, ID_PIOC, PIO_PERIPH_E, PIO_DEFAULT },\
	{ PIO_PC8E_UTXD1, PIO_ADD, ID_PIOC, PIO_PERIPH_E, PIO_DEFAULT },\
}

/* ========== Pio PIN definition for UART2 peripheral ========== */

/* UART2 IOSET 1 */
#define PINS_UART2_IOS1 {\
	{ PIO_PD4B_URXD2, PIO_ADD, ID_PIOD, PIO_PERIPH_B, PIO_DEFAULT },\
	{ PIO_PD5B_UTXD2, PIO_ADD, ID_PIOD, PIO_PERIPH_B, PIO_DEFAULT },\
}
/* UART2 IOSET 2 */
#define PINS_UART2_IOS2 {\
	{ PIO_PD23A_URXD2, PIO_ADD, ID_PIOD, PIO_PERIPH_A, PIO_DEFAULT },\
	{ PIO_PD24A_UTXD2, PIO_ADD, ID_PIOD, PIO_PERIPH_A, PIO_DEFAULT },\
}
/* UART2 IOSET 3 */
#define PINS_UART2_IOS3 {\
	{ PIO_PD19C_URXD2, PIO_ADD, ID_PIOD, PIO_PERIPH_C, PIO_DEFAULT },\
	{ PIO_PD20C_UTXD2, PIO_ADD, ID_PIOD, PIO_PERIPH_C, PIO_DEFAULT },\
}

/* ========== Pio PIN definition for UART3 peripheral ========== */

/* UART3 IOSET 1 */
#define PINS_UART3_IOS1 {\
	{ PIO_PC12D_URXD3, PIO_ADD, ID_PIOC, PIO_PERIPH_D, PIO_DEFAULT },\
	{ PIO_PC13D_UTXD3, PIO_ADD, ID_PIOC, PIO_PERIPH_D, PIO_DEFAULT },\
}
/* UART3 IOSET 2 */
#define PINS_UART3_IOS2 {\
	{ PIO_PC31C_URXD3, PIO_ADD, ID_PIOC, PIO_PERIPH_C, PIO_DEFAULT },\
	{ PIO_PD0C_UTXD3, PIO_ADD, ID_PIOD, PIO_PERIPH_C, PIO_DEFAULT },\
}
/* UART3 IOSET 3 */
#define PINS_UART3_IOS3 {\
	{ PIO_PB11C_URXD3, PIO_ADD, ID_PIOB, PIO_PERIPH_C, PIO_DEFAULT },\
	{ PIO_PB12C_UTXD3, PIO_ADD, ID_PIOB, PIO_PERIPH_C, PIO_DEFAULT },\
}

/* ========== Pio PIN definition for UART4 peripheral ========== */

/* UART4 IOSET 1 */
#define PINS_UART4_IOS1 {\
	{ PIO_PB3A_URXD4, PIO_ADD, ID_PIOB, PIO_PERIPH_A, PIO_DEFAULT },\
	{ PIO_PB4A_UTXD4, PIO_ADD, ID_PIOB, PIO_PERIPH_A, PIO_DEFAULT },\
}

/* ========== Pio PIN definition for HSDHC peripheral ========== */

#define PIO_CFGR_FUNC_A PIO_PERIPH_A | PIO_CFGR_PDEN_DISABLED | PIO_CFGR_PUEN_ENABLED
#define PIO_CFGR_FUNC_E PIO_PERIPH_E | PIO_CFGR_PDEN_DISABLED | PIO_CFGR_PUEN_ENABLED

/* SDMMC0 IOSET 1 */
#define PINS_HSDHC0_IOS1 {\
	{ 0x00000001, PIO_ADD, ID_PIOA, PIO_CFGR_FUNC_A, PIO_DEFAULT },\
	{ 0x0000303e, PIO_ADD, ID_PIOA, PIO_CFGR_FUNC_A, PIO_DEFAULT },\
}

/* SDMMC1 IOSET 1 */
#define PINS_HSDHC1_IOS1 {\
    { 0x00400000, PIO_ADD, ID_PIOA, PIO_CFGR_FUNC_E, PIO_DEFAULT },\
    { 0x783c0000, PIO_ADD, ID_PIOA, PIO_CFGR_FUNC_E, PIO_DEFAULT },\
}

//=============================================================================
#endif // _CHIP_H_


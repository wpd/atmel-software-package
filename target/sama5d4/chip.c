/* ----------------------------------------------------------------------------
 *         SAM Software Package License
 * ----------------------------------------------------------------------------
 * Copyright (c) 2014, Atmel Corporation
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

#include "chip.h"
#include "compiler.h"

/*----------------------------------------------------------------------------
 *        Definitions
 *----------------------------------------------------------------------------*/

struct chipid {
	uint32_t    exid; /**< EXID */
	const char* name; /**< Chip Name */
};

struct peripheral_xdma {
	uint32_t id;   /**< Peripheral ID */
	uint8_t  iftx; /**< DMA Interface for TX */
	uint8_t  ifrx; /**< DMA Interface for RX */
};

/*----------------------------------------------------------------------------
 *        Local constants
 *----------------------------------------------------------------------------*/

static const struct chipid _exid_names[] = {
	{ CHIPID_EXID_SAMA5D41, "SAMA5D41" },
	{ CHIPID_EXID_SAMA5D42, "SAMA5D42" },
	{ CHIPID_EXID_SAMA5D43, "SAMA5D43" },
	{ CHIPID_EXID_SAMA5D44, "SAMA5D44" },
};

static const uint8_t _h64_peripherals[] = {
	ID_ARM,     /* 2: Performance Monitor Unit (ARM) */
	ID_XDMAC0,  /* 8: DMA Controller 0 (XDMAC0) */
	ID_CPKCC,   /* 10: Classic Public Key Crypto Controller (CPKCC) */
	ID_AESB,    /* 13: AES bridge (AESB) */
	ID_MPDDRC,  /* 16: MPDDR controller (MPDDRC) */
	ID_MATRIX0, /* 18: H64MX, 64-bit AHB Matrix (MATRIX0) */
	ID_VDEC,    /* 19: Video Decoder (VDEC) */
	ID_XDMAC1,  /* 50: DMA Controller 1 (XDMAC1) */
	ID_LCDC,    /* 51: LCD Controller (LCDC) */
	ID_ISI,     /* 52: Camera Interface (ISI) */
	ID_L2CC,    /* 67: L2 Cache Controller  (L2CC) */
};

static const struct peripheral_xdma _xdmac0_peripherals[] = {
	{ ID_HSMCI0,    0,  0 },
	{ ID_HSMCI1,    1,  1 },
	{ ID_TWI0,      2,  3 },
	{ ID_TWI1,      4,  5 },
	{ ID_TWI2,      6,  7 },
	{ ID_TWI3,      8,  9 },
	{ ID_SPI0,     10, 11 },
	{ ID_SPI1,     12, 13 },
	{ ID_SPI2,     14, 15 },
	{ ID_USART2,   16, 17 },
	{ ID_USART3,   18, 19 },
	{ ID_USART4,   20, 21 },
	{ ID_UART0,    22, 23 },
	{ ID_UART1,    24, 25 },
	{ ID_SSC0,     26, 27 },
	{ ID_SSC1,     28, 29 },
	{ ID_DBGU,     30, 31 },
	{ ID_ADC,    0xff, 32 },
	{ ID_SMD,      33, 34 },
	{ ID_USART0,   36, 37 },
	{ ID_USART1,   38, 39 },
	{ ID_AES,      41, 40 },
	{ ID_TDES,     42, 43 },
	{ ID_SHA,      44, 0xff },
};

static const struct peripheral_xdma _xdmac1_peripherals[] = {
	{ ID_HSMCI0,    0,  0 },
	{ ID_HSMCI1,    1,  1 },
	{ ID_TWI0,      2,  3 },
	{ ID_TWI1,      4,  5 },
	{ ID_TWI2,      6,  7 },
	{ ID_TWI3,      8,  9 },
	{ ID_SPI0,     10, 11 },
	{ ID_SPI1,     12, 13 },
	{ ID_SPI2,     14, 15 },
	{ ID_USART2,   16, 17 },
	{ ID_USART3,   18, 19 },
	{ ID_USART4,   20, 21 },
	{ ID_UART0,    22, 23 },
	{ ID_UART1,    24, 25 },
	{ ID_SSC0,     26, 27 },
	{ ID_SSC1,     28, 29 },
	{ ID_DBGU,     30, 31 },
	{ ID_ADC,    0xff, 32 },
	{ ID_SMD,      33, 34 },
};

/*----------------------------------------------------------------------------
 *        Private functions
 *----------------------------------------------------------------------------*/

static const struct peripheral_xdma *get_peripheral_xdma(uint32_t id, Xdmac *xdmac)
{
	const struct peripheral_xdma *_xdmac_peripherals;
	int count, i;

	if (xdmac == XDMAC0) {
		_xdmac_peripherals = _xdmac0_peripherals;
		count = ARRAY_SIZE(_xdmac0_peripherals);
	} else if (xdmac == XDMAC1) {
		_xdmac_peripherals = _xdmac1_peripherals;
		count = ARRAY_SIZE(_xdmac1_peripherals);
	} else {
		return NULL;
	}

	for (i = 0; i < count; i++) {
		if (_xdmac_peripherals[i].id == id) {
			return &_xdmac_peripherals[i];
		}
	}

	return NULL;
}

/*----------------------------------------------------------------------------
 *        Exported functions
 *----------------------------------------------------------------------------*/

const char* get_chip_name(void)
{
	int i;

	if ((DBGU->DBGU_CIDR & ~DBGU_CIDR_VERSION_Msk) ==
			CHIPID_CIDR_SAMA5D4) {
		uint32_t exid = DBGU->DBGU_EXID;
		for (i = 0; i < ARRAY_SIZE(_exid_names); i++) {
			if (_exid_names[i].exid == exid)
				return _exid_names[i].name;
		}
	}

	return "Unknown";
}

uint32_t get_twi_id_from_addr(const Twi* addr)
{
	if (addr == TWI0)
		return ID_TWI0;
#ifdef TWI1
	else if (addr == TWI1)
		return ID_TWI1;
#endif
#ifdef TWI2
	else if (addr == TWI2)
		return ID_TWI2;
#endif
#ifdef TWI3
	else if (addr == TWI3)
		return ID_TWI3;
#endif
#ifdef TWI4
	else if (addr == TWI4)
		return ID_TWI4;
#endif
	else
		return ID_PERIPH_COUNT;
}

Twi* get_twi_addr_from_id(uint32_t id)
{
	if (id == ID_TWI0)
		return TWI0;
#ifdef TWI1
	else if (id == ID_TWI1)
		return TWI1;
#endif
#ifdef TWI2
	else if (id == ID_TWI2)
		return TWI2;
#endif
#ifdef TWI3
	else if (id == ID_TWI3)
		return TWI3;
#endif
#ifdef TWI4
	else if (id == ID_TWI4)
		return TWI4;
#endif
	else
		return (void*)0;
}

uint32_t get_spi_id_from_addr(const Spi* addr)
{
	if (addr == SPI0)
		return ID_SPI0;
#ifdef SPI1
	else if (addr == SPI1)
		return ID_SPI1;
#endif
#ifdef SPI2
	else if (addr == SPI2)
		return ID_SPI2;
#endif
	else
		return ID_PERIPH_COUNT;
}

Spi* get_spi_addr_from_id(uint32_t id)
{
	if (id == ID_SPI0)
		return SPI0;
#ifdef SPI1
	else if (id == ID_SPI1)
		return SPI1;
#endif
#ifdef SPI2
	else if (id == ID_SPI2)
		return SPI2;
#endif
	else
		return (void*)0;
}

uint32_t get_pwm_id_from_addr(const Pwm* addr)
{
	if (addr == PWM)
		return ID_PWM;
	else
		return ID_PERIPH_COUNT;
}

Pwm* get_pwm_addr_from_id(uint32_t id)
{
	if (id == ID_PWM)
		return PWM;
	else
		return (void*)0;
}

uint32_t get_uart_id_from_addr(const Uart* addr)
{
	if (addr == UART0)
		return ID_UART0;
#ifdef UART1
	else if (addr == UART1)
		return ID_UART1;
#endif
#ifdef UART2
	else if (addr == UART2)
		return ID_UART2;
#endif
#ifdef UART3
	else if (addr == UART3)
		return ID_UART3;
#endif
#ifdef UART4
	else if (addr == UART4)
		return ID_UART4;
#endif
	else
		return ID_PERIPH_COUNT;
}

Uart* get_uart_addr_from_id(uint32_t id)
{
	if (id == ID_UART0)
		return UART0;
#ifdef UART1
	else if (id == ID_UART1)
		return UART1;
#endif
#ifdef UART2
	else if (id == ID_UART2)
		return UART2;
#endif
#ifdef UART3
	else if (id == ID_UART3)
		return UART3;
#endif
#ifdef UART4
	else if (id == ID_UART4)
		return UART4;
#endif
	else
		return (void*)0;
}

uint32_t get_usart_id_from_addr(const Usart* addr)
{
	if (addr == USART0)
		return ID_USART0;
#ifdef USART1
	else if (addr == USART1)
		return ID_USART1;
#endif
#ifdef USART2
	else if (addr == USART2)
		return ID_USART2;
#endif
#ifdef USART3
	else if (addr == USART3)
		return ID_USART3;
#endif
#ifdef USART4
	else if (addr == USART4)
		return ID_USART4;
#endif
	else return ID_PERIPH_COUNT;
}

Usart* get_usart_addr_from_id(uint32_t id)
{
	if (id == ID_USART0)
		return USART0;
#ifdef USART1
	else if (id == ID_USART1)
		return USART1;
#endif
#ifdef USART2
	else if (id == ID_USART2)
		return USART2;
#endif
#ifdef USART3
	else if (id == ID_USART3)
		return USART3;
#endif
#ifdef USART4
	else if (id == ID_USART4)
		return USART4;
#endif
	else
		return (void*)0;
}

uint32_t get_tc_id_from_addr(const Tc* addr)
{
	if (addr == TC0)
		return ID_TC0;
#ifdef TC1
	else if (addr == TC1)
		return ID_TC1;
#endif
#ifdef TC2
	else if (addr == TC2)
		return ID_TC2;
#endif
	else
		return ID_PERIPH_COUNT;
}

Tc* get_tc_addr_from_id(uint32_t id)
{
	if (id == ID_TC0)
		return TC0;
#ifdef TC1
	else if (id == ID_TC1)
		return TC1;
#endif
#ifdef TC2
	else if (id == ID_TC2)
		return TC2;
#endif
	else
		return (void*)0;
}

uint32_t get_gmac_id_from_addr(const Gmac* addr)
{
	if (addr == GMAC0)
		return ID_GMAC0;
	else if (addr == GMAC1)
		return ID_GMAC1;
	else
		return ID_PERIPH_COUNT;
}

uint32_t get_mci_id_from_addr(const Hsmci* addr)
{
	if (addr == HSMCI0)
		return ID_HSMCI0;
	else if (addr == HSMCI1)
		return ID_HSMCI1;
	else
		return ID_PERIPH_COUNT;
}

uint32_t get_ssc_id_from_addr(const Ssc* addr)
{
	if (addr == SSC0)
		return ID_SSC0;
	else if (addr == SSC1)
		return ID_SSC1;
	else
		return ID_PERIPH_COUNT;
}

uint32_t get_ebi_addr_from_cs(uint32_t cs)
{
	switch (cs) {
	case 0:
		return EBI_CS0_ADDR;
	case 1:
		return EBI_CS1_ADDR;
	case 2:
		return EBI_CS2_ADDR;
	case 3:
		return EBI_CS3_ADDR;
	default:
		return 0;
	}
}

Matrix* get_peripheral_matrix(uint32_t id)
{
	int i;

	for (i = 0; i < ARRAY_SIZE(_h64_peripherals); i++)
		if (_h64_peripherals[i] == id)
			return MATRIX0; // AHB 64-bit matrix
	return MATRIX1; // AHB 32-bit matrix
}

uint32_t get_peripheral_clock_divider(uint32_t id)
{
	Matrix* matrix = get_peripheral_matrix(id);

	if (matrix == MATRIX1) {
		if (PMC->PMC_MCKR & PMC_MCKR_H32MXDIV_H32MXDIV2)
			return 2;
		else
			return 1;
	}

	return 1;
}

uint8_t get_peripheral_xdma_channel(uint32_t id, Xdmac *xdmac, bool transmit)
{
	const struct peripheral_xdma *periph_xdma = get_peripheral_xdma(id, xdmac);
	if (periph_xdma) {
		return transmit ? periph_xdma->iftx : periph_xdma->ifrx;
	} else {
		return 0xff;
	}
}

bool is_peripheral_on_xdma_controller(uint32_t id, Xdmac *xdmac)
{
	return get_peripheral_xdma(id, xdmac) != NULL;
}

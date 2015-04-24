/* ----------------------------------------------------------------------------
 *         SAM Software Package License
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

/** \addtogroup usart_module Working with USART
 * \section Purpose
 * The USART driver provides the interface to configure and use the USART peripheral.\n
 *
 * The USART supports several kinds of communication modes such as full-duplex asynchronous/
 * synchronous serial communication,RS485 with driver control signal,ISO7816,SPI and Test modes.
 *
 * To start a USART transfer with \ref dmad_module "DMA" support, the user could follow these steps:
 * <ul>
 * <li> Configure USART with expected mode and baudrate(see \ref USART_Configure), which could be done by:
 * -# Resetting and disabling transmitter and receiver by setting US_CR(Control Register). </li>
 * -# Configuring the USART in a specific mode by setting USART_MODE bits in US_MR(Mode Register) </li>
 * -# Setting baudrate which is different from mode to mode.
   </li>
 * <li> Enable transmitter or receiver respectively by set US_CR_TXEN or US_CR_RXEN in US_CR.</li>
 * <li> Read from or write to the peripheral with  \ref dmad_module </li>
 * </ul>
 *
 * \section Usage
 * <ul>
 * <li>  Enable or disable USART transmitter or receiver using
 * USART_SetTransmitterEnabled() and USART_SetReceiverEnabled().
 * <li>  Enable or disable USART interrupt using USART_EnableIt() or USART_DisableIt().
 * </li>
 * </ul>
 *
 * For more accurate information, please look at the USART section of the
 * Datasheet.
 *
 * Related files :\n
 * \ref usart.c\n
 * \ref usart.h\n
*/

/**
 * \file
 *
 * Implementation of USART (Universal Synchronous Asynchronous Receiver Transmitter)
 * controller.
 *
 */
/*------------------------------------------------------------------------------
 *         Headers
 *------------------------------------------------------------------------------*/

#include "chip.h"
#include "serial/flexcom_usart.h"

#include "utils/trace.h"

#include <assert.h>
#include <string.h>

/*------------------------------------------------------------------------------
 *         Exported functions
 *------------------------------------------------------------------------------*/

/**
 * \brief Configures an USART peripheral with the specified parameters.
 *
 *
 *  \param usart  Pointer to the USART peripheral to configure.
 *  \param mode  Desired value for the USART mode register (see the datasheet).
 *  \param baudrate  Baudrate at which the USART should operate (in Hz).
 *  \param masterClock  Frequency of the system master clock (in Hz).
 */
void flexcom_usart_configure(Flexcom * pflexcom, uint32_t mode,
			     uint32_t baudrate, uint32_t masterClock)
{
	/* Reset and disable receiver & transmitter */
	pflexcom->FX_US_CR =
	    FX_US_CR_RSTRX | FX_US_CR_RSTTX | FX_US_CR_RXDIS | FX_US_CR_TXDIS;
	/* Configure mode */
	pflexcom->FX_US_MR = mode;
	/* Configure baudrate */
	/* Asynchronous, no oversampling */
	if (((mode & FX_US_MR_SYNC) == 0) && ((mode & FX_US_MR_OVER) == 0)) {
		pflexcom->FX_US_BRGR = (masterClock / baudrate) / 16;
	}
	if (((mode & FX_US_MR_USART_MODE_SPI_MASTER) ==
	     FX_US_MR_USART_MODE_SPI_MASTER)
	    || ((mode & FX_US_MR_SYNC) == FX_US_MR_SYNC)) {
		if ((mode & FX_US_MR_USCLKS_Msk) == FX_US_MR_USCLKS_MCK) {
			usart->FX_US_BRGR = masterClock / baudrate;
		} else {
			if ((mode & FX_US_MR_USCLKS_DIV) == FX_US_MR_USCLKS_DIV) {
				pflexcom->FX_US_BRGR =
				    masterClock / baudrate / 8;
			}
		}
	}
	/* TODO other modes */
}

/**
 * \brief Enables or disables the transmitter of an USART peripheral.
 *
 *
 * \param usart  Pointer to an USART peripheral
 * \param enabled  If true, the transmitter is enabled; otherwise it is
 *                disabled.
 */
void flexcom_usart_set_transmitter_enabled(Flexcom * pflexcom, uint8_t enabled)
{
	if (enabled) {
		pflexcom->FX_US_CR = FX_US_CR_TXEN;
	} else {
		pflexcom->FX_US_CR = FX_US_CR_TXDIS;
	}
}

/**
 * \brief Enables or disables the receiver of an USART peripheral
 *
 *
 * \param usart  Pointer to an USART peripheral
 * \param enabled  If true, the receiver is enabled; otherwise it is disabled.
 */
void flexcom_usart_set_receiverenabled(Flexcom * pflexcom, uint8_t enabled)
{
	if (enabled) {
		pflexcom->FX_US_CR = FX_US_CR_RXEN;
	} else {
		pflexcom->FX_US_CR = FX_US_CR_RXDIS;
	}
}

/**
 * \brief Enables or disables the Request To Send (RTS) of an USART peripheral
 *
 *
 * \param usart  Pointer to an USART peripheral
 * \param enabled  If true, the RTS is enabled (0); otherwise it is disabled.
 */
void flexcom_usart_set_rts_enabled(Flexcom * pflexcom, uint8_t enabled)
{
	if (enabled) {
		pflexcom->FX_US_CR = FX_US_CR_RTSEN;
	} else {
		pflexcom->FX_US_CR = FX_US_CR_RTSDIS;
	}
}

/**
 * \brief Sends one packet of data through the specified USART peripheral. This
 * function operates synchronously, so it only returns when the data has been
 * actually sent.
 *
 *
 * \param usart  Pointer to an USART peripheral.
 * \param data  Data to send including 9nth bit and sync field if necessary (in
 *        the same format as the US_THR register in the datasheet).
 * \param timeOut  Time out value (0 = no timeout).
 */
void flexcom_usart_write(Flexcom * pflexcom, uint16_t data,
			 volatile uint32_t timeOut)
{
	if (timeOut == 0) {
		while ((pflexcom->FX_US_CSR & FX_US_CSR_TXEMPTY) == 0) ;
	} else {
		while ((pflexcom->FX_US_CSR & FX_US_CSR_TXEMPTY) == 0) {
			if (timeOut == 0) {
				TRACE_ERROR("USART_Write: Timed out.\n\r");
				return;
			}
			timeOut--;
		}
	}
	pflexcom->US_THR = data;
}

/**
 * \brief  Reads and return a packet of data on the specified USART peripheral. This
 * function operates asynchronously, so it waits until some data has been
 * received.
 *
 * \param usart  Pointer to an USART peripheral.
 * \param timeOut  Time out value (0 -> no timeout).
 */
uint16_t flexcom_usart_read(Flexcom * pflexcom, volatile uint32_t timeOut)
{
	if (timeOut == 0) {
		while ((pflexcom->FX_US_CSR & FX_US_CSR_RXRDY) == 0) ;
	} else {
		while ((pflexcom->FX_US_CSR & FX_US_CSR_RXRDY) == 0) {
			if (timeOut == 0) {
				TRACE_ERROR
				    ("FLEXCOM USART_Read: Timed out.\n\r");
				return 0;
			}
			timeOut--;
		}
	}
	return pflexcom->US_RHR;
}

/**
 * \brief  Returns 1 if some data has been received and can be read from an USART;
 * otherwise returns 0.
 *
 * \param usart  Pointer to an USART instance.
 */
uint8_t flexcom_usart_is_data_available(Flexcom * pflexcom)
{
	if ((pflexcom->FX_US_CSR & FX_US_CSR_RXRDY) != 0) {
		return 1;
	} else {
		return 0;
	}
}

/**
 * \brief  Sets the filter value for the IRDA demodulator.
 *
 * \param pFlexcom  Pointer to an USART instance.
 * \param filter  Filter value.
 */
void flexcom_usart_set_irda_filter(Flexcom * pflexcom, uint8_t filter)
{
	assert(pflexcom != NULL);
	pflexcom->FX_US_IF = filter;
}

/**
 * \brief  Sends one packet of data through the specified USART peripheral. This
 * function operates synchronously, so it only returns when the data has been
 * actually sent.
 *
 * \param usart  Pointer to an USART peripheral.
 * \param c  Character to send
 */
void flexcom_usart_put_char(Flexcom * pflexcom, uint8_t c)
{
	/* Wait for the transmitter to be ready */
	while ((pflexcom->FX_US_CSR & FX_US_CSR_TXEMPTY) == 0) ;
	/* Send character */
	pflexcom->FX_US_THR = c;
	/* Wait for the transfer to complete */
	while ((pflexcom->FX_US_CSR & FX_US_CSR_TXEMPTY) == 0) ;
}

/**
 * \brief   Return 1 if a character can be read in USART
 * \param usart  Pointer to an USART peripheral.
 */
uint32_t flexcom_usart_is_rx_ready(Flexcom * pflexcom)
{
	return (pflexcom->FX_US_CSR & FX_US_CSR_RXRDY);
}

/**
 * \brief   Get present status
 * \param usart  Pointer to an USART peripheral.
 */
uint32_t USART_GetStatus(Flexcom * pflexcom)
{
	return pflexcom->FX_US_CSR;
}

/**
 * \brief   Enable interrupt
 * \param usart  Pointer to an USART peripheral.
 * \param mode  Interrupt mode.
 */
void flexcom_usart_enable_it(Flexcom * pflexcom, uint32_t mode)
{
	pflexcom->FX_US_IER = mode;
}

/**
 * \brief   Disable interrupt
 * \param usart  Pointer to an USART peripheral.
 * \param mode  Interrupt mode.
 */
void flexcom_usart_disable_it(Flexcom * pflexcom, uint32_t mode)
{
	pflexcom->FX_US_IDR = mode;
}

/**
 * \brief   Return interrupt mask
 * \param usart  Pointer to an USART peripheral.
 */
uint32_t flexcom_usart_get_it_mask(Flexcom * pflexcom)
{
	return pflexcom->FX_US_IMR;
}

/**
 * \brief  Reads and returns a character from the USART.
 *
 * \note This function is synchronous (i.e. uses polling).
 * \param usart  Pointer to an USART peripheral.
 * \return Character received.
 */
uint8_t flexcom_usart_get_char(Flexcom * pflexcom)
{
	while ((pflexcom->FX_US_CSR & FX_US_CSR_RXRDY) == 0) ;
	return pflexcom->FX_US_RHR;
}
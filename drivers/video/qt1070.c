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

/**
  * \file
  *
  * Implementation QT1070 driver.
  *
  */

/*----------------------------------------------------------------------------
 *        Headers
 *----------------------------------------------------------------------------*/

#include "board.h"

#include "peripherals/twi.h"
#include "peripherals/twid.h"

#include "video/qt1070.h"

/*----------------------------------------------------------------------------
 *
 *----------------------------------------------------------------------------*/



/*----------------------------------------------------------------------------
 *        Local functions
 *----------------------------------------------------------------------------*/

/**
 * \brief Read one byte of data from QT1070 Register.
 *
 * \param qt1070 Pointer to the driver structure.
 * \param reg_addr Register address to read.
 * \return value in the given register.
 */
static uint8_t _qt1070_read_reg(struct _qt1070* qt1070, uint8_t reg_addr)
{
	uint8_t data;

	qt1070->twid->slave_addr = QT1070_SLAVE_ADDRESS;
	qt1070->twid->iaddr = reg_addr;
	qt1070->twid->isize = 1;
	struct _buffer in = {
		.data = &data,
		.size = 1
	};
	twid_transfert(qt1070->twid, &in, 0, NULL, 0);
	return data;
}

/**
 * \brief  Write one byte of data to QT1070 Register.
 *
 * \param qt1070 Pointer to the driver structure.
 * \param reg_addr Register address to write.
 * \param data    Data to write.
 */
static void _qt1070_write_reg(struct _qt1070* qt1070, uint32_t reg_addr,
			      uint8_t data)
{
	uint8_t ldata = data;

	struct _buffer out = {
		.data = &ldata,
		.size = 1
	};
	qt1070->twid->slave_addr = QT1070_SLAVE_ADDRESS;
	qt1070->twid->iaddr = reg_addr;
	qt1070->twid->isize = 1;
	twid_transfert(qt1070->twid, 0, &out, NULL, 0);
}

/*----------------------------------------------------------------------------
 *        Exported functions
 *----------------------------------------------------------------------------*/

uint8_t qt1070_get_chip_id(struct _qt1070* qt1070)
{
	return _qt1070_read_reg(qt1070, QT1070_CHIP_ID);
}

uint8_t qt1070_get_firmware_version(struct _qt1070* qt1070)
{
	return _qt1070_read_reg(qt1070, QT1070_REG_FIRMWARE_VERSION);
}

uint8_t qt1070_get_detection_status(struct _qt1070* qt1070)
{
	return _qt1070_read_reg(qt1070, QT1070_REG_DETECTION_STATUS);
}

uint8_t qt1070_get_key_status(struct _qt1070* qt1070)
{
	return _qt1070_read_reg(qt1070, QT1070_REG_KEY_STATUS);
}

uint16_t qt1070_get_key_signal(struct _qt1070* qt1070, uint8_t key)
{
	uint8_t data[2];
	data[0] = _qt1070_read_reg(qt1070,
				   QT1070_REG_KEY0_SIGNAL_MSB + key * 2);
	data[1] = _qt1070_read_reg(qt1070,
				   QT1070_REG_KEY0_SIGNAL_LSB + key * 2);
	return (data[0] << 8) | data[1];
}

uint16_t qt1070_get_key_reference(struct _qt1070* qt1070,
				  uint8_t key)
{
	uint8_t data[2];
	data[0] = _qt1070_read_reg(qt1070, QT1070_REG_REFDATA0_MSB + key * 2);
	data[1] = _qt1070_read_reg(qt1070, QT1070_REG_REFDATA0_LSB + key * 2);
	return (data[0] << 8) | data[1];
}

void qt1070_set_threshold(struct _qt1070* qt1070, uint8_t key,
			  uint8_t threshold)
{
	// Do not use a setting of 0 as this causes a key to go into detection
	// when its signal is equal to its reference.
	if (threshold)
		_qt1070_write_reg(qt1070, QT1070_REG_NTHR_KEY0 + key, threshold);
}

void qt1070_set_ave_aks(struct _qt1070* qt1070, uint8_t key, uint8_t ave,
			uint8_t aks)
{
	_qt1070_write_reg(qt1070,
			  QT1070_REG_AVEAKS_KEY0 + key, (ave << 3) | aks);
}

void qt1070_set_detection_integrator(struct _qt1070* qt1070, uint8_t key,
				     uint8_t di)
{
	_qt1070_write_reg(qt1070, QT1070_REG_DI_KEY0 + key, di);
}

void qt1070_start_calibrate(struct _qt1070* qt1070)
{
	_qt1070_write_reg(qt1070, QT1070_REG_CALIRATE, 1);
}
void qt1070_start_reset(struct _qt1070* qt1070)
{
	_qt1070_write_reg(qt1070, QT1070_REG_RESET, 1);
}

uint8_t qt1070_configure(struct _qt1070* qt1070, struct _twi_desc* twid)
{
	uint8_t status = TWID_SUCCESS;

	qt1070->twid = twid;
	twid_configure(twid);

	/*
	qt1070_start_calibrate(qt1070);
	do {
		status = qt1070_get_key_status(qt1070)
	} while ( status & QT1070_CALIBRATE_BIT) ;
	*/

	qt1070->chip_id = qt1070_get_chip_id(qt1070);
	qt1070->firmware_version = qt1070_get_firmware_version(qt1070);

	return status;
}

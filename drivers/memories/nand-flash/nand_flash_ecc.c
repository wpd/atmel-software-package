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

/** \file */

/*---------------------------------------------------------------------- */
/*         Headers                                                       */
/*---------------------------------------------------------------------- */

#include "peripherals/pio.h"
#include "peripherals/hsmc.h"
#include "peripherals/pmecc.h"

#include "misc/cache.h"

#include "nand_flash.h"
#include "nand_flash_common.h"
#include "nand_flash_spare_scheme.h"
#include "nand_flash_ecc.h"

#include "trace.h"
#include "hamming.h"

#include <assert.h>
#include <string.h>


/*---------------------------------------------------------------------- */
/*         Local variables                                               */
/*---------------------------------------------------------------------- */

CACHE_ALIGNED static uint8_t spare_buf[NAND_MAX_PAGE_SPARE_SIZE];

/*---------------------------------------------------------------------- */
/*         Local functions                                               */
/*---------------------------------------------------------------------- */

/**
 * \brief Reads the data and/or spare of a page of a NANDFLASH chip, and verify that
 * the data is valid using the ECC information contained in the spare. If one
 * buffer pointer is 0, the corresponding area is not saved.
 * \param nand  Pointer to an EccNandFlash instance.
 * \param block  Number of block to read from.
 * \param page  Number of page to read inside given block.
 * \param data  Data area buffer.
 * \param spare  Spare area buffer.
 * \return 0 if the data has been read and is valid; otherwise returns either
 * NAND_ERROR_CORRUPTEDDATA or ...
 */
static uint8_t ecc_read_page_with_swecc(const struct _nand_flash *nand,
		uint16_t block, uint16_t page, void *data, void *spare)
{
	uint8_t error;
	uint8_t hamming[NAND_MAX_SPARE_ECC_BYTES];
	uint16_t page_data_size = nand_model_get_page_data_size(&nand->model);
	uint8_t page_spare_size = nand_model_get_page_spare_size(&nand->model);

	/* Start by reading the spare data */
	error = nand_raw_read_page(nand, block, page, NULL, spare_buf);
	if (error) {
		trace_error("nand_ecc_read_page: Failed to read page\r\n");
		return error;
	}

	/* Then reading the data */
	error = nand_raw_read_page(nand, block, page, data, NULL);
	if (error) {
		trace_error("nand_ecc_read_page: Failed to read page\r\n");
		return error;
	}

	/* Retrieve ECC information from page and verify the data */
	nand_spare_scheme_read_ecc(nand_model_get_scheme(&nand->model),
					spare_buf, hamming);
	error = hamming_verify_256x(data, page_data_size, hamming);
	if (error && (error != HAMMING_ERROR_SINGLEBIT)) {
		trace_error("nand_ecc_read_page: at B%d.P%d Unrecoverable data\r\n",
					block, page);
		return NAND_ERROR_CORRUPTEDDATA;
	}
	if (spare) {
		memcpy(spare, spare_buf, page_spare_size);
	}
	return 0;
}

/**
 * \brief Reads the data page of a NANDFLASH chip, and verify that
 * the data is valid by PMECC module. If one
 * \param nand  Pointer to an EccNandFlash instance.
 * \param block  Number of block to read from.
 * \param page  Number of page to read inside given block.
 * \param data  Data area buffer.
 * \return 0 if the data has been read and is valid; otherwise returns either
 * NAND_ERROR_CORRUPTEDDATA or ...
 */
static uint8_t ecc_read_page_with_pmecc(const struct _nand_flash *nand,
		uint16_t block, uint16_t page, void *data)
{
	volatile uint32_t pmecc_status;
	uint8_t error;
	uint16_t i;
	uint16_t page_spare_size = nand_model_get_page_spare_size(&nand->model);

	if (!data)
		return NAND_ERROR_ECC_NOT_COMPATIBLE;

	/* Start by reading the spare data */
	error = nand_raw_read_page(nand, block, page, data, NULL);
	if (error) {
		trace_error("nand_ecc_read_page: Failed to read page\r\n");
		return error;
	}
	pmecc_status = hsmc_pmecc_error_status();
	if (pmecc_status) {
		/* Check if the spare area was erased */
		nand_raw_read_page(nand, block, page, NULL, spare_buf);
		for (i = 0 ; i < page_spare_size; i++) {
			if (spare_buf[i] != 0xff)
				break;
		}
		if (i == page_spare_size)
			pmecc_status = 0;
	}

	/* bit correction will be done directly in destination buffer. */
	if (pmecc_correction(pmecc_status, (uint32_t)data)) {
		hsmc_pmecc_auto_disable();
		hsmc_pmecc_disable();
		trace_error("nand_ecc_read_page: at B%d.P%d Unrecoverable data\r\n",
				block, page);
		return NAND_ERROR_CORRUPTEDDATA;
	}

	hsmc_pmecc_auto_disable();
	hsmc_pmecc_disable();
	return 0;
}

/**
 * \brief Writes the data and/or spare area of a NANDFLASH page, after calculating an
 * ECC for the data area and storing it in the spare. If no data buffer is
 * provided, the ECC is read from the existing page spare. If no spare buffer
 * is provided, the spare area is still written with the ECC information
 * calculated on the data buffer.
 * \param nand Pointer to an EccNandFlash instance.
 * \param block  Number of the block to write in.
 * \param page  Number of the page to write inside the given block.
 * \param data  Data area buffer, can be 0.
 * \param spare  Spare area buffer, can be 0.
 * \return 0 if successful; otherwise returns an error code.
 */
static uint8_t ecc_write_page_with_swecc(const struct _nand_flash *nand,
		uint16_t block, uint16_t page, void *data, void *spare)
{
	uint8_t error;
	uint8_t hamming[NAND_MAX_SPARE_ECC_BYTES];
	uint16_t page_data_size = nand_model_get_page_data_size(&nand->model);
	uint16_t page_spare_size = nand_model_get_page_spare_size(&nand->model);

	/* Compute ECC on the new data, if provided */
	/* If not provided, hamming code set to 0xFFFF.. to keep existing bytes */
	memset(hamming, 0xFF, NAND_MAX_SPARE_ECC_BYTES);
	if (data) {
		/* Compute hamming code on data */
		hamming_compute_256x(data, page_data_size, hamming);
	}

	/* Store code in spare buffer (if no buffer provided, use a temp. one) */
	if (!spare) {
		spare = spare_buf;
		memset(spare, 0xFF, page_spare_size);
	}
	nand_spare_scheme_write_ecc(nand_model_get_scheme(&nand->model),
			spare, hamming);

	/* Perform write operation */
	error = nand_raw_write_page(nand, block, page, data, spare);
	if (error) {
		trace_error("nand_ecc_write_page: Failed to write page\r\n");
		return error;
	}

	return 0;
}

/**
 * \brief Writes the data area of a NANDFLASH page, The PMECC module generates
 * redundancy at encoding time. When a NAND write page operation is performed.
 * The redundancy is appended to the page and written in the spare area.
 * \param nand Pointer to an EccNandFlash instance.
 * \param block  Number of the block to write in.
 * \param page  Number of the page to write inside the given block.
 * \param data  Data area buffer, can be 0.
 * \return 0 if successful; otherwise returns an error code.
 */
static uint8_t ecc_write_page_with_pmecc(const struct _nand_flash *nand,
		uint16_t block, uint16_t page, void *data)
{
	uint8_t error;

	/* Perform write operation */
	error = nand_raw_write_page(nand, block, page, data, NULL);
	hsmc_pmecc_disable();
	if (error) {
		trace_error("nand_ecc_write_page: Failed to write page\r\n");
		return error;
	}

	return 0;
}

/*------------------------------------------------------------------------------ */
/*         Exported functions */
/*------------------------------------------------------------------------------ */

/**
 * \brief Reads the data and/or spare of a page of a NANDFLASH chip, and verify that
 * the data is valid using the ECC information contained in the spare. If one
 * buffer pointer is 0, the corresponding area is not saved.
 * \param nand  Pointer to an EccNandFlash instance.
 * \param block  Number of block to read from.
 * \param page  Number of page to read inside given block.
 * \param data  Data area buffer.
 * \param spare  Spare area buffer.
 * \return 0 if the data has been read and is valid; otherwise returns either
 * NAND_ERROR_CORRUPTEDDATA or ...
 */
uint8_t nand_ecc_read_page(const struct _nand_flash *nand,
		uint16_t block, uint16_t page, void *data, void *spare)
{
	NAND_TRACE("nand_ecc_read_page(B#%d:P#%d)\r\n", block, page);
	assert(data || spare);

	if (nand_is_using_pmecc()) {
		if (spare)
			return NAND_ERROR_ECC_NOT_COMPATIBLE;
		return ecc_read_page_with_pmecc(nand, block, page, data);
	}

	if (nand_is_using_software_ecc())
		return ecc_read_page_with_swecc(nand, block, page, data, spare);

	if (nand_is_using_no_ecc())
		return nand_raw_read_page(nand, block, page, data, spare);

	return NAND_ERROR_ECC_NOT_COMPATIBLE;
}

/**
 * \brief Writes the data and/or spare area of a NANDFLASH page, after calculating an
 * ECC for the data area and storing it in the spare. If no data buffer is
 * provided, the ECC is read from the existing page spare. If no spare buffer
 * is provided, the spare area is still written with the ECC information
 * calculated on the data buffer.
 * \param nand Pointer to an EccNandFlash instance.
 * \param block  Number of the block to write in.
 * \param page  Number of the page to write inside the given block.
 * \param data  Data area buffer, can be 0.
 * \param spare  Spare area buffer, can be 0.
 * \return 0 if successful; otherwise returns an error code.
 */
uint8_t nand_ecc_write_page(const struct _nand_flash *nand,
	uint16_t block, uint16_t page, void *data, void *spare)
{
	NAND_TRACE("nand_ecc_write_page(B#%d:P#%d)\r\n", block, page);
	assert(data || spare);

	if (nand_is_using_pmecc()) {
		if (spare)
			return NAND_ERROR_ECC_NOT_COMPATIBLE;
		return ecc_write_page_with_pmecc(nand, block, page, data);
	}

	if (nand_is_using_software_ecc())
		return ecc_write_page_with_swecc(nand, block, page, data, spare);

	if (nand_is_using_no_ecc())
		return nand_raw_write_page(nand, block, page, data, spare);

	return NAND_ERROR_ECC_NOT_COMPATIBLE;
}

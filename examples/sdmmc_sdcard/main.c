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
 * \page sdcard Secure Digital Memory Card.
 * An example using the SDMMC peripheral of SAMA5D2x
 * Microcontrollers.
 *
 * \section Purpose
 *
 * The Secure Digital Memory Card example will help new users
 * get familiar with the SDMMC peripheral used in Atmel's
 * SAMA5D2x microcontrollers.
 *
 * \section Requirements
 *
 * This package is compatible with the evaluation boards listed below:
 * - SAMA5D2-VB
 * - SAMA5D2-XULT
 *
 * \section Description
 *
 * The demonstration program... TODO
 *
 * \section Usage
 *
 * -# Build the program and download it inside the evaluation board. Please
 *    refer to the
 *    <a href="http://www.atmel.com/dyn/resources/prod_documents/6421B.pdf">
 *    SAM-BA User Guide</a>, the
 *    <a href="http://www.atmel.com/dyn/resources/prod_documents/doc6310.pdf">
 *    GNU-Based Software Development</a>
 *    application note or to the
 *    <a href="ftp://ftp.iar.se/WWWfiles/arm/Guides/EWARM_UserGuide.ENU.pdf">
 *    IAR EWARM User Guide</a>,
 *    depending on your chosen solution.
 * -# On the computer, open and configure a terminal application
 *    (e.g. HyperTerminal on Microsoft Windows) with these settings:
 *   - 57600 bauds
 *   - 8 bits of data
 *   - No parity
 *   - 1 stop bit
 *   - No flow control
 * -# In the terminal window, the
 *    following text should appear (values depend on the board and chip used):
 *    \code
 *     -- Secure Digital Memory Card (SDMMC) Example xxx --
 *     -- xxxxxx-xx
 *     -- Compiled: xxx xx xxxx xx:xx:xx --
 *
 *     SD card menu:
 *     TODO
 *     \endcode
 * -# Input command according to the menu.
 *
 * \section References
 * - TODO
 */

/** \file
 *
 *  This file contains all the specific code for the SD Card
 *  example.
 */

/*----------------------------------------------------------------------------
 *        Headers
 *----------------------------------------------------------------------------*/

#include <stdint.h>

#include "board.h"
#include "chip.h"
#include "trace.h"
#include "misc/console.h"
#include "peripherals/sdmmc.h"
#include "peripherals/wdt.h"
#include "peripherals/pmc.h"
#include "peripherals/pio.h"
#include "peripherals/aic.h"
#include "libsdmmc/libsdmmc.h"
#include "fatfs/src/ff.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

/*----------------------------------------------------------------------------
 *        Local definitions
 *----------------------------------------------------------------------------*/

/* Configure whether the on-board DDR3-SDRAM should be used */
#define USE_EXT_RAM                   1

#ifdef VARIANT_DDRAM
# undef USE_EXT_RAM
# define USE_EXT_RAM                  1
#endif

#if USE_EXT_RAM
# define BLOCK_CNT_MAX                65536ul
# define DMADL_CNT_MAX                512u
#else
# define BLOCK_CNT_MAX                24u
# define DMADL_CNT_MAX                1u
#endif

#define BLOCK_CNT                     3u

/* Allocate 2 Timers/Counters, that are not used already by the libraries and
 * drivers this example depends on. */
#define TIMER0_MODULE                 ID_TC0
#define TIMER0_CHANNEL                0
#define TIMER1_MODULE                 ID_TC0
#define TIMER1_CHANNEL                1u

/*----------------------------------------------------------------------------
 *        Local variables
 *----------------------------------------------------------------------------*/

const char test_file_path[] = "test_data.bin";

/* Driver instance data (a.k.a. MCI driver instance) */
static struct sdmmc_set drv0 = { 0 };
static struct sdmmc_set drv1 = { 0 };

/* Library instance data (a.k.a. SDCard driver instance) */
ALIGNED(L1_CACHE_BYTES) static sSdCard lib0 = { .pDrv = 0 };
ALIGNED(L1_CACHE_BYTES) static sSdCard lib1 = { .pDrv = 0 };

/* Buffer dedicated to the SDMMC Driver. Refer to sdmmc_initialize(). */
#if USE_EXT_RAM
SECTION(".region_ddr")
#endif
static uint32_t dma_table[DMADL_CNT_MAX * SDMMC_DMADL_SIZE];

/* Read/write data buffer.
 * It may receive data transferred from the device by the DMA. As the L1 data
 * cache won't notice when RAM is updated directly, the driver will invalidate
 * any matching cache lines.
 * May this buffer fail to be aligned on cache lines, cached changes to adjacent
 * variables would be lost at the time the dirty and shared cache lines were
 * invalidated.
 * Alternatively, we might consider allocating this buffer from a
 * non-cacheable memory region. */
#if USE_EXT_RAM
SECTION(".region_ddr")
#endif
ALIGNED(L1_CACHE_BYTES) static uint8_t data_buf[
    ROUND_UP_MULT(BLOCK_CNT_MAX * 512ul, (unsigned long)L1_CACHE_BYTES)];

/* File system object.
 * Similarly to the main data buffer above, we need to align FATFS::win[] on
 * data cache lines.
 * Padding implemented below is valid as of FatFs Module R0.10b, with structure
 * packing kept disabled. Proper alignment is checked at runtime, anyway. */
struct padded_fatfs { uint8_t padding[16]; FATFS fs; };
ALIGNED(L1_CACHE_BYTES) static struct padded_fatfs fs_header;

/* File object.
 * Similarly to the main data buffer above, we need to align FIL::buf[] on
 * data cache lines. */
struct padded_fil { uint8_t padding[28]; FIL file; };
ALIGNED(L1_CACHE_BYTES) static struct padded_fil f_header;

static uint8_t slot;
static bool use_dma;

/*----------------------------------------------------------------------------
 *        Local functions
 *----------------------------------------------------------------------------*/

/**
 * \brief Trace buffer contents in hex.
 */
static void print_buffer(uint32_t len, const uint8_t *data)
{
	const char format[] = { ' ', '%', '0', '2', 'x', '\0' };
	const uint8_t * const data_nxt = data + len;
	uint32_t ix;

	for (ix = 0; data < data_nxt; data++, ix++) {
		if (ix % 32 == 0)
			printf("\n\r%03lx:    ", ix);
		printf(format + (ix % 4 ? 1 : 0), *data);
	}
	printf("\n\r");
}

/**
 * \brief Display main menu.
 */
static void display_menu(void)
{
	printf("\n\rSD Card menu:\n\r");
	printf("   h: Display this menu\n\r");
	printf("   t: Toggle between Slot%c0%c(e.MMC) and Slot%c1%c(removable"
	    " card)\n\r", slot ? ' ' : '[', slot ? ' ' : ']', slot ? '[' : ' ',
	    slot ? ']' : ' ');
	printf("   d: Select either %cCPU programmed I/O%c or %cDMA operation%c"
	    "\n\r", use_dma ? ' ' : '[', use_dma ? ' ' : ']',
	    use_dma ? '[' : ' ', use_dma ? ']' : ' ');
	printf("   i: Display device info\n\r");
	printf("   l: Mount FAT file system and list files\n\r");
	printf("   r: Read the file named '%s'\n\r", test_file_path);
	printf("   w: Perform a basic RAW read/write test.\n\r");
	printf("\n\r");
}

static void initialize(void)
{
	/* As long as we do not transfer from/to SDMMC0 and SDMMC1 peripherals
	 * at the same time, we can share a single DMA buffer between the two
	 * instances. */
	sdmmc_initialize(&drv0, SDMMC0, ID_SDMMC0,
	    TIMER0_MODULE, TIMER0_CHANNEL,
	    use_dma ? dma_table : NULL, use_dma ? ARRAY_SIZE(dma_table) : 0);
#if 1
	/* Disable this after development phase. Declare the device on the
	 * SDMMC0 slot i.e. the e.MMC as a non-removable device. Hence, whatever
	 * the configuration of the BOOT_DISABLE jumper, this device is always
	 * enabled. */
	SDMMC0->SDMMC_MC1R |= SDMMC_MC1R_FCD;
#endif
	sdmmc_initialize(&drv1, SDMMC1, ID_SDMMC1,
	    TIMER1_MODULE, TIMER1_CHANNEL,
	    use_dma ? dma_table : NULL, use_dma ? ARRAY_SIZE(dma_table) : 0);
	SDD_InitializeSdmmcMode(&lib0, &drv0, 0);
	SDD_InitializeSdmmcMode(&lib1, &drv1, 0);
}

static bool open_device(sSdCard *pSd)
{
	uint8_t rc, card_type;

	printf("\n\r==========================================\n\r");
	rc = SD_Init(pSd);
	if (rc != SDMMC_OK) {
		printf("-E- SD/MMC device initialization failed: %d\n\r", rc);
		return false;
	}
	printf("-I- SD/MMC device initialization successful\n\r");
	card_type = SD_GetCardType(pSd);
	if (card_type & CARD_TYPE_bmSDMMC) {
		printf("-I- Device memory size: %lu MiB, %lu * %luB\n\r",
		    SD_GetTotalSizeKB(pSd) / 1024ul, SD_GetNumberBlocks(pSd),
		    SD_GetBlockSize(pSd));
		SD_DumpCID(pSd->CID);
		SD_DumpCSD(pSd->CSD);
	}
	if (card_type & CARD_TYPE_bmMMC)
		SD_DumpExtCSD(pSd->EXT);
	if (card_type & CARD_TYPE_bmSDIO)
		SDIO_DumpCardInformation(pSd);
	return true;
}

static bool close_device(sSdCard *pSd)
{
	SD_DeInit(pSd);
	return true;
}

static bool mount_volume(uint8_t slot_ix, sSdCard *pSd, FATFS *fs)
{
	char lfn[_MAX_LFN * 2 + 1];
	char *fn;
	const TCHAR drive_path[] = { '0' + slot_ix, ':', '\0' };
	DIR dir = { 0 };
	FILINFO fno = {
		.lfname = lfn,
		.lfsize = sizeof(lfn),
	};
	FRESULT res;
	bool is_dir, rc = true;

	memset(fs, 0, sizeof(FATFS));
	res = f_mount(fs, drive_path, 1);
	if (res != FR_OK) {
		printf("Failed to mount FAT file system, error %d\n\r", res);
		return false;
	}
	res = f_opendir(&dir, drive_path);
	if (res != FR_OK) {
		printf("-E- Failed to change to root directory, error %d\n\r", res);
		return false;
	}
	printf("Listing the files present in the root directory:\n\r");
	for ( ; ; ) {
		res = f_readdir(&dir, &fno);
		if (res != FR_OK) {
			printf("-E- Error (%d) while listing files\n\r", res);
			rc = false;
			break;
		}
		if (fno.fname[0] == '\0')
			break;
		is_dir = fno.fattrib & AM_DIR ? true : false;
		fn = fno.lfname[0] != '\0' ? fno.lfname : fno.fname;
		printf("    %s%s%c\n\r", is_dir ? "[" : "", fn, is_dir ? ']' : ' ');
	}

	res = f_closedir(&dir);
	if (res != FR_OK) {
		printf("-E- Failed to close directory, error %d\n\r", res);
		rc = false;
	}
	return rc;
}

static bool read_file(uint8_t slot_ix, sSdCard *pSd, FATFS *fs)
{
	const TCHAR drive_path[] = { '0' + slot_ix, ':', '\0' };
	const UINT buf_size = BLOCK_CNT_MAX * 512ul;
	TCHAR file_path[sizeof(drive_path) + sizeof(test_file_path)];
	uint32_t file_size;
	UINT len;
	FRESULT res;
	bool rc = true;

	memset(fs, 0, sizeof(FATFS));
	res = f_mount(fs, drive_path, 1);
	if (res != FR_OK) {
		printf("Failed to mount FAT file system, error %d\n\r", res);
		return false;
	}
	strcpy(file_path, drive_path);
	strcat(file_path, test_file_path);
	res = f_open(&f_header.file, file_path, FA_OPEN_EXISTING | FA_READ);
	if (res != FR_OK) {
		printf("Failed to open \"%s\", error %d\n\r", file_path, res);
		return false;
	}
	for (file_size = 0, len = buf_size; res == FR_OK && len == buf_size;
	    file_size += len) {
		res = f_read(&f_header.file, data_buf, buf_size, &len);
		if (res != FR_OK)
			printf("Error %d while attempting to read file\n\r", res);
	}
	if (res == FR_OK)
		printf("Read %lu bytes\n\r", file_size);

	res = f_close(&f_header.file);
	if (res != FR_OK) {
		printf("Failed to close file, error %d\n\r", res);
		return false;
	}
	return rc;
}

static bool unmount_volume(uint8_t slot_ix, sSdCard *pSd)
{
	const TCHAR drive_path[] = { '0' + slot_ix, ':', '\0' };
	FRESULT res;
	bool rc = true;

	res = f_mount(NULL, drive_path, 0);
	if (res != FR_OK)
		rc = false;
	SD_DeInit(pSd);
	return rc;
}

/*----------------------------------------------------------------------------
 *        Exported functions
 *----------------------------------------------------------------------------*/

/* Refer to sdmmc_ff.c */
bool SD_GetInstance(uint8_t index, sSdCard **holder);

bool SD_GetInstance(uint8_t index, sSdCard **holder)
{
	assert(holder);

	switch (index) {
	case 0:
		*holder = &lib0;
		break;
	case 1:
		*holder = &lib1;
		break;
	default:
		return false;
	}
	return true;
}

/**
 *  \brief SD Card Application entry point.
 *
 *  \return Unused (ANSI-C compatibility).
 */
int main(void)
{
	const struct _pin emmc_pins[] = SDMMC0_PINS;
	const struct _pin sd_pins[] = SDMMC1_PINS;
	sSdCard *lib = NULL;
	uint8_t user_key, rc;

	/* Disable Watchdog */
	wdt_disable();

	/* Configure console */
	board_cfg_console();

	/* Output example information */
	printf("\r\n\r\n\r\n");
	printf("-- SD Card Example " SOFTPACK_VERSION " --\n\r") ;
	printf("-- " BOARD_NAME "\n\r");
	printf("-- Compiled: " __DATE__ " " __TIME__ " --\n\r");

#if USE_EXT_RAM && !defined(VARIANT_DDRAM)
	board_cfg_ddram();
#endif

	pmc_enable_peripheral(TIMER0_MODULE);
#if TIMER1_MODULE != TIMER0_MODULE
	pmc_enable_peripheral(TIMER1_MODULE);
#endif

	/* The SDMMC peripherals are clocked by their Peripheral Clock, the
	 * Master Clock, and a Generated Clock (at least on SAMA5D2x).
	 * Configure GCLKx = <PLLA clock> divided by 1
	 * As of writing, the PLLA clock runs at 498 MHz */
	pmc_configure_gck(ID_SDMMC0, PMC_PCR_GCKCSS_PLLA_CLK, 1 - 1);
	pmc_enable_gck(ID_SDMMC0);
	pmc_enable_peripheral(ID_SDMMC0);

	pmc_configure_gck(ID_SDMMC1, PMC_PCR_GCKCSS_PLLA_CLK, 1 - 1);
	pmc_enable_gck(ID_SDMMC1);
	pmc_enable_peripheral(ID_SDMMC1);

	pio_configure(emmc_pins, ARRAY_SIZE(emmc_pins));
	pio_configure(sd_pins, ARRAY_SIZE(sd_pins));

	if ((uint32_t)&data_buf % L1_CACHE_BYTES
	    || sizeof(data_buf) % L1_CACHE_BYTES
	    || (uint32_t)&lib0.EXT % L1_CACHE_BYTES
	    || sizeof(lib0.EXT) % L1_CACHE_BYTES
	    || (uint32_t)&lib1.EXT % L1_CACHE_BYTES
	    || sizeof(lib1.EXT) % L1_CACHE_BYTES
	    || (uint32_t)&fs_header.fs.win % L1_CACHE_BYTES
	    || sizeof(fs_header.fs.win) % L1_CACHE_BYTES
	    || (uint32_t)&f_header.file.buf % L1_CACHE_BYTES
	    || sizeof(f_header.file.buf) % L1_CACHE_BYTES) {
		printf("WARNING: buffers are not aligned on data cache lines."
		    " Please fix this before enabling DMA.\n\r");
		use_dma = false;
	}
	else
		use_dma = true;
	initialize();

	/* Display menu */
	slot = 1;
	display_menu();

	while (true) {
		user_key = tolower(console_get_char());
		switch (user_key) {
		case 'h':
			display_menu();
			break;
		case 't':
			slot = slot ? 0 : 1;
			display_menu();
			break;
		case 'd':
			use_dma = use_dma ? false : true;
			initialize();
			display_menu();
			break;
		case 'i':
			lib = slot ? &lib1 : &lib0;
			if (SD_GetStatus(lib) == SDMMC_NOT_SUPPORTED) {
				printf("Device not detected.\n\r");
				break;
			}
			open_device(lib);
			close_device(lib);
			break;
		case 'l':
			lib = slot ? &lib1 : &lib0;
			if (SD_GetStatus(lib) == SDMMC_NOT_SUPPORTED) {
				printf("Device not detected.\n\r");
				break;
			}
			mount_volume(slot, lib, &fs_header.fs);
			unmount_volume(slot, lib);
			break;
		case 'r':
			lib = slot ? &lib1 : &lib0;
			if (SD_GetStatus(lib) == SDMMC_NOT_SUPPORTED) {
				printf("Device not detected.\n\r");
				break;
			}
			read_file(slot, lib, &fs_header.fs);
			unmount_volume(slot, lib);
			break;
		case 'w':
			lib = slot ? &lib1 : &lib0;
			if (SD_GetStatus(lib) == SDMMC_NOT_SUPPORTED) {
				printf("Device not detected.\n\r");
				break;
			}
			if (open_device(lib)) {
				uint32_t block = 254;

				printf("Reading blocks #%lu-%lu\n\r", block, block + BLOCK_CNT - 1);
				rc = SD_Read(lib, block, data_buf, BLOCK_CNT, NULL, NULL);
				if (rc != SDMMC_OK)
					printf("-E- %s\n\r", SD_StringifyRetCode(rc));
				if (rc == SDMMC_OK) {
					printf("Printing contents of blocks #%lu-%lu before we alter them\n\r", block, block + BLOCK_CNT - 1);
					print_buffer(BLOCK_CNT * 512ul, data_buf);

					printf("Rewriting blocks #%lu-%lu\n\r", block, block + BLOCK_CNT - 1);
					memset(data_buf, 0x55, BLOCK_CNT * 512ul);
					rc = SD_Write(lib, block, data_buf, BLOCK_CNT, NULL, NULL);
					if (rc != SDMMC_OK)
						printf("-E- %s\n\r", SD_StringifyRetCode(rc));
				}
				if (rc == SDMMC_OK) {
					memset(data_buf, 0, BLOCK_CNT * 512ul);
					printf("Reading blocks #%lu-%lu again\n\r", block, block + BLOCK_CNT - 1);
					rc = SD_Read(lib, block, data_buf, BLOCK_CNT, NULL, NULL);
					if (rc != SDMMC_OK)
						printf("-E- %s\n\r", SD_StringifyRetCode(rc));
				}
				if (rc == SDMMC_OK) {
					printf("Printing contents of blocks #%lu-%lu\n\r", block, block + BLOCK_CNT - 1);
					print_buffer(BLOCK_CNT * 512ul, data_buf);
				}
			}
			close_device(lib);
			break;
		}
	}

	/* This code is never reached */
}
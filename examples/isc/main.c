/* ----------------------------------------------------------------------------
 *         SAM Software Package License
 * ----------------------------------------------------------------------------
 * Copyright (c) 2016, Atmel Corporation
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
 * \page isc ISC Example
 *
 * \section Purpose
 *
 * This example demonstrates the ISC (Image Sensor controller) of an SAMA5D2
 * Xplained board.
 *
 * \section Requirements
 *
 * This package can be used with SAMA5D2-EK  with
 * On-board ISC interface and a external CMOS-type image sensor board.
 *
 * \section Description
 * The provided program uses the Image Sensor Controller (ISC) system,
 * it manages incoming data from a parallel CMOS sensor (ov7740) . It
 * support sensor with a data width of 8 bits in YUV format and 10
 * bits raw Bayer format. This example shows how to configure the
 * internal image processor includes color filter array interpolation,
 * gamma correction, 12 bits to 10 bits compression, color space conversion,
 * luminance adjustment. It introduces how to samples data stream to expected
 * data format and transfer with DMA master module.
 *
 * \section Usage
 *  -# Build the program and download it inside the SAMA5D2-EK board.
 *  Please refer to the Getting Started with SAM V71 Microcontrollers.pdf
 * -# On the computer, open and configure a terminal application
 *    (e.g. HyperTerminal on Microsoft Windows) with these settings:
 *   - 115200 bauds
 *   - 8 bits of data
 *   - No parity
 *   - 1 stop bit
 *   - No flow control
 * -# Start the application.
 * -# In the terminal window, the following text should appear:
 *    \code
 *     -- ISC Example xxx --
 *      -- SAMxxxxx-xx
 *     -- Compiled: xxx xx xxxx xx:xx:xx --
 *    \endcode
 * The user can then choose any of the available options to perform the
 * described action.
 *
 * \section References
 * - lcdc.c
 * - twihs.c
 * - twihsd.c
 * - isc.c
 */

/**
 * \file
 *
 * This file contains all the specific code for the ISC example.
 */

/*----------------------------------------------------------------------------
 *        Headers
 *----------------------------------------------------------------------------*/
#include "board.h"
#include "chip.h"

#include "misc/cache.h"
#include "peripherals/aic.h"
#include "peripherals/pmc.h"
#include "peripherals/pio.h"
#include "peripherals/xdmad.h"

#include "misc/console.h"
#include "peripherals/twi.h"
#include "peripherals/twid.h"
#include "peripherals/isc.h"

#include "video/lcdd.h"
#include "video/image_sensor_inf.h"
#include "timer.h"

#include "lcd_draw.h"
#include "trace.h"

#include <stdbool.h>
#include <stdio.h>

/*----------------------------------------------------------------------------
 *        Local definitions
 *----------------------------------------------------------------------------*/

/** DMA memory address for DMA package 8/16 and DMA YC422P */
#define ISC_OUTPUT_BASE_ADDRESS  (DDR_CS_ADDR + 16 * 1024 * 1024)
#define ISC_OUTPUT_BASE_ADDRESS1  (ISC_OUTPUT_BASE_ADDRESS + 0x100000)
#define ISC_OUTPUT_BASE_ADDRESS2  (ISC_OUTPUT_BASE_ADDRESS1 + 0x100000)

/** DMA memory address for Histogram */
#define ISC_HIS_BASE_ADDRESS  (0x22000000)

/** Supported LCD mode in this example */
#define LCD_MODE_YUV          (LCDC_HEOCFG1_YUVEN | LCDC_HEOCFG1_YUVMODE_16BPP_YCBCR_MODE2)
#define LCD_MODE_RGB565       LCDC_HEOCFG1_RGBMODE_16BPP_RGB_565
#define LCD_MODE_YUV422_PLANAR   (LCDC_HEOCFG1_YUVEN |LCDC_HEOCFG1_YUVMODE_16BPP_YCBCR_PLANAR)
#define LCD_MODE_YUV422_SEMIPLANAR (LCDC_HEOCFG1_YUVEN |LCDC_HEOCFG1_YUVMODE_16BPP_YCBCR_SEMIPLANAR)
#define LCD_MODE_YUV420_PLANAR   (LCDC_HEOCFG1_YUVEN |LCDC_HEOCFG1_YUVMODE_12BPP_YCBCR_PLANAR)
#define LCD_MODE_YUV420_SEMIPLANAR   (LCDC_HEOCFG1_YUVEN |LCDC_HEOCFG1_YUVMODE_12BPP_YCBCR_SEMIPLANAR)

/** Maximum number of frame buffer */
#define ISC_MAX_NUM_FRAME_BUFFER    1

/** TWI clock frequency in Hz. */
#define TWCK                  400000

/* GAMMA and HISTOGRAM definitions */
#define GAMMA_28
#define GAMMA_ENTRIES   64

#define HISTOGRAM_GR   0
#define HISTOGRAM_R    1
#define HISTOGRAM_GB   2
#define HISTOGRAM_B    3
#define HISTOGRAM_Y    4

#define HIST_ENTRIES   512
#define HIST_RGGB_BAYER  5

#define AE_TUNING

/*----------------------------------------------------------------------------
 *        Local Type
 *----------------------------------------------------------------------------*/

/** Sensor status or return code */
typedef enum _awb_status {
	AWB_INIT = 0,
	AWB_WAIT_HIS_READY,
	AWB_WAIT_DMA_READY,
	AWB_WAIT_ISC_PERFORMED,
} awb_status_t;

/*----------------------------------------------------------------------------
 *        Local variables/constants
 *----------------------------------------------------------------------------*/

/** Supported sensor profiles */
static const sensor_profile_t *sensor_profiles[6] = {
	&ov2640_profile,
	&ov2643_profile,
	&ov5640_profile,
	&ov7670_profile,
	&ov7740_profile,
	&ov9740_profile
};

/** PIO pins to configured. */
const struct _pin pins_twi[] = ISC_TWI_PINS;

CACHE_ALIGNED struct _isc_dma_view0 dma_descs[ISC_MAX_NUM_FRAME_BUFFER + 1];

CACHE_ALIGNED struct _isc_dma_view2 dma_descs2[ISC_MAX_NUM_FRAME_BUFFER + 1];

/** TWI driver instance.*/
static struct _twi_desc twid = {
	.addr = ISC_TWI_ADDR,
	.freq = TWCK,
	.transfert_mode = TWID_MODE_POLLING
};

static awb_status_t awb_status_machine;

/** LCD buffer.*/
static uint8_t *pHeoBuffer =  (uint8_t*)ISC_OUTPUT_BASE_ADDRESS;
static uint8_t *pHeoBuffer1 =  (uint8_t*)ISC_OUTPUT_BASE_ADDRESS1;
static uint8_t *pHeoBuffer2 =  (uint8_t*)ISC_OUTPUT_BASE_ADDRESS2;

/* Image size in preview mode */
static uint32_t image_width, image_height;

static uint8_t sensor_idx;

/* Image output bit width */
static sensor_output_bit_t sensor_output_bit_width;

/* Sensor mode for YUV or RAW BAYER */
static sensor_output_format_t sensor_mode;

/* LCD mode */
static uint32_t lcd_mode;

/** DMA channel */
static struct _xdmad_channel *xdmad_channel;

volatile static uint32_t capture_started = 0;

static uint32_t histogram_count[HIST_RGGB_BAYER];

static uint32_t histogram_y[HIST_ENTRIES];

static uint32_t histogram_idx_isc;

volatile static bool histogram_done;
volatile static bool histogram_read;
volatile static bool ae_awb;

/* Color space matrix setting */
static struct _color_space cs = {
	0x42, 0x81, 0x19 ,
	0x10, 0xFDA, 0xFB6,
	0x70, 0x80, 0x70,
	0xFA2, 0xFEE, 0x80};

#if defined GAMMA_22
/* Gamma table with gamma 1/2.2 */
const uint32_t gamma_table[GAMMA_ENTRIES] = {
	0x2B0079 ,0x9C0039 ,0xD4002B ,0xFF0024 ,0x122001F,0x141001C,0x15D0019,0x1760018,
	0x18E0016,0x1A40015,0x1B80014,0x1CC0013,0x1DE0012,0x1F00011,0x2010010,0x2110010,
	0x221000F,0x230000F,0x23F000E,0x24D000E,0x25B000D,0x269000D,0x276000D,0x283000D,
	0x28F000C,0x29C000C,0x2A8000C,0x2B4000C,0x2BF000B,0x2CA000B,0x2D6000B,0x2E0000B,
	0x2EB000A,0x2F6000A,0x300000A,0x30A000A,0x314000A,0x31E000A,0x328000A,0x3310009,
	0x33B0009,0x3440009,0x34D0009,0x3570009,0x3600009,0x3680009,0x3710009,0x37A0009,
	0x3820008,0x38B0008,0x3930008,0x39B0008,0x3A40008,0x3AC0008,0x3B40008,0x3BC0008,
	0x3C40008,0x3CB0008,0x3D30008,0x3DB0008,0x3E20007,0x3EA0007,0x3F10007,0x3F90007
};
#endif

#if defined GAMMA_18
/* Gamma table with gamma 1/1.8 */
const uint32_t gamma_table[GAMMA_ENTRIES] = {
	0x65, 0x66002F, 0x950025,0xBB0020, 0xDB001D, 0xF8001A, 0x1130018, 0x12B0017,
	0x1420016,0x1580014,0x16D0013,0x1810012,0x1940012,0x1A60012,0x1B80011,0x1C90010,
	0x1DA0010,0x1EA000F,0x1FA000F,0x209000F,0x218000F,0x227000E,0x235000E,0x243000E,
	0x251000E,0x25F000D,0x26C000D,0x279000D,0x286000D,0x293000C,0x2A0000C,0x2AC000C,
	0x2B8000C,0x2C4000C,0x2D0000B,0x2DC000B,0x2E7000B,0x2F3000B,0x2FE000B,0x309000B,
	0x314000B,0x31F000A,0x32A000A,0x334000B,0x33F000A,0x349000A,0x354000A,0x35E000A,
	0x368000A,0x372000A,0x37C000A,0x386000A,0x3900009,0x399000A,0x3A30009,0x3AD0009,
	0x3B60009,0x3BF000A,0x3C90009,0x3D20009,0x3DB0009,0x3E40009,0x3ED0009,0x3F60009
};

#endif

#if defined GAMMA_28
/* Gamma table with gamma 1/2.8 */
const uint32_t gamma_table[GAMMA_ENTRIES] = {
	0xE6,0xE80040,0x129002D,0x1570025,0x17C001F,0x19C001B,0x1B70019,0x1D00016,
	0x1E70014,0x1FC0013,0x20F0012,0x2210011,0x2330010,0x243000F,0x253000E,0x261000E,
	0x270000D,0x27D000D,0x28A000D,0x297000C,0x2A3000C,0x2AF000C,0x2BB000B,0x2C6000B,
	0x2D1000A,0x2DB000A,0x2E6000A,0x2F00009,0x2FA0009,0x3030009,0x30D0009,0x3160009,
	0x31F0008,0x3280008,0x3300009,0x3390008,0x3410008,0x3490008,0x3510008,0x3590008,
	0x3610008,0x3690007,0x3700008,0x3780007,0x37F0007,0x3860007,0x38D0007,0x3940007,
	0x39B0007,0x3A20007,0x3A90006,0x3AF0007,0x3B60006,0x3BC0007,0x3C30006,0x3C90006,
	0x3CF0007,0x3D60006,0x3DC0006,0x3E20006,0x3E80006,0x3EE0005,0x3F40005,0x3F90006
};
#endif

/**
 * \brief Callback entry for histogram DMA transfer done.
 */
static void _dma_callback(struct _xdmad_channel *channel, void *arg)
{
	cache_invalidate_region((uint32_t*)ISC_HIS_BASE_ADDRESS, HIST_ENTRIES * sizeof(uint32_t));
	histogram_read = true;
}

/**
 * \brief Configure xDMA to read Histogram entries.
 */
static void xdma_read_histogram(uint32_t buf)
{
	struct _xdmad_cfg xdmad_cfg;
	xdmad_cfg.ubc = HIST_ENTRIES;
	xdmad_cfg.sa = (uint32_t *)(&(ISC->ISC_HIS_ENTRY[0]));
	xdmad_cfg.da =  (uint32_t* )buf;
	xdmad_cfg.cfg = XDMAC_CC_MBSIZE_SINGLE
	              | XDMAC_CC_TYPE_MEM_TRAN
	              | XDMAC_CC_CSIZE_CHK_1
	              | XDMAC_CC_DWIDTH_WORD
	              | XDMAC_CC_SIF_AHB_IF0
	              | XDMAC_CC_DIF_AHB_IF0
	              | XDMAC_CC_SAM_INCREMENTED_AM
	              | XDMAC_CC_DAM_INCREMENTED_AM;
	xdmad_cfg.bc = 0;
	xdmad_cfg.ds = 0;
	xdmad_cfg.sus = 0;
	xdmad_cfg.dus = 0;
	xdmad_configure_transfer(xdmad_channel, &xdmad_cfg, 0, 0);
	xdmad_set_callback(xdmad_channel, _dma_callback, NULL);
	xdmad_start_transfer(xdmad_channel);
}

/**
 * \brief Convert float data to HEX with giving format.
 * \param sign_bit length of sign in bit
 * \param magnitude_bit length of magnitude in bit
 * \param fractional_bit length of fractional in bit
 * \param f float value to be converted
 */
static uint32_t float2hex(uint8_t sign_bit, uint8_t magnitude_bit, uint8_t fractional_bit, float f)
{
	uint32_t hex;
	if(!sign_bit){
		if (f < 0.0) return 0;
		hex = (uint32_t) (f * (1 << fractional_bit));
	} else {
		if (f < 0.0) {
			hex = (uint32_t)( (f) * (-1.0) * (1 << fractional_bit));
			hex = ~hex - 1;
		} else {
			hex = (uint32_t)(f * (1 << fractional_bit));
		}
	}
	return hex;
}

/*----------------------------------------------------------------------------
 *        Local functions
 *----------------------------------------------------------------------------*/
/**
 * \brief ISC interrupt handler.
 */
static void isc_handler(void)
{
	uint32_t status;
	status = isc_interrupt_status();
	if ((status & ISC_INTSR_HD) == ISC_INTSR_HD) {
	}
	if ((status & ISC_INTSR_VD) == ISC_INTSR_VD){
		if (!capture_started){
			isc_start_capture();
			capture_started = 1;
		}
	}
	if ((status & ISC_INTSR_DDONE) == ISC_INTSR_DDONE) {
	}

	if ((status & ISC_INTSR_HISDONE) == ISC_INTSR_HISDONE) {
		histogram_done = true;
	}

	if ((status & ISC_INTSR_LDONE) == ISC_INTSR_LDONE) {
	}
}

/**
 * \brief TWI initialization.
 */
static void configure_twi(void)
{
	/* Configure TWI pins. */
	pio_configure(pins_twi, ARRAY_SIZE(pins_twi));
	/* Enable TWI peripheral clock */
	pmc_enable_peripheral(get_twi_id_from_addr(ISC_TWI_ADDR));
	/* Configure TWI */
	twid_configure(&twid);
}

/**
 * \brief Configure LCD controller.
 */
static void configure_lcd(void)
{
	lcdd_enable_layer(LCDD_HEO, 0);

	if (sensor_mode == YUV_422) {
		lcdd_configure_input_mode(LCDD_HEO, LCD_MODE_YUV);
		lcdd_create_canvas(LCDD_HEO,
				   pHeoBuffer,
				   16,
				   0,
				   0,
				   image_width,
				   image_height);
	} else {
		if (lcd_mode == LCD_MODE_YUV422_PLANAR){
			lcdd_configure_input_mode(LCDD_HEO, LCD_MODE_YUV422_PLANAR);
			lcdd_create_canvas_yuv_planar(
										LCDD_HEO,
										pHeoBuffer,
										pHeoBuffer1,
										pHeoBuffer2,
										16,
										0,
										0,
										image_width,
										image_height);
		} else if (lcd_mode == LCD_MODE_YUV422_SEMIPLANAR){
			lcdd_configure_input_mode(LCDD_HEO, LCD_MODE_YUV422_SEMIPLANAR);
			lcdd_create_canvas_yuv_semiplanar(
										LCDD_HEO,
										pHeoBuffer,
										pHeoBuffer1,
										16,
										0,
										0,
										image_width,
										image_height);
		} else if (lcd_mode == LCD_MODE_YUV420_PLANAR){
			lcdd_configure_input_mode(LCDD_HEO, LCD_MODE_YUV420_PLANAR);
			lcdd_create_canvas_yuv_planar(
										LCDD_HEO,
										pHeoBuffer,
										pHeoBuffer1,
										pHeoBuffer2,
										12,
										0,
										0,
										image_width,
										image_height);
		} else if (lcd_mode == LCD_MODE_YUV420_SEMIPLANAR){
			lcdd_configure_input_mode(LCDD_HEO, LCD_MODE_YUV420_SEMIPLANAR);
			lcdd_create_canvas_yuv_semiplanar(
										LCDD_HEO,
										pHeoBuffer,
										pHeoBuffer1,
										12,
										0,
										0,
										image_width,
										image_height);
		} else if (lcd_mode == LCD_MODE_RGB565){
			lcdd_configure_input_mode(LCDD_HEO, LCD_MODE_RGB565);
			lcdd_create_canvas(LCDD_HEO,
					   pHeoBuffer,
					   16,
					   0,
					   0,
					   image_width,
					   image_height);
		}
	}
	lcdd_enable_layer(LCDD_HEO, 1);
}

/**
 * \brief ISC initialization.
 */
static void configure_isc(void)
{
	uint32_t i;
	/* Configurer the Parallel Front End module performs data
	 * re-sampling across clock domain boundary. ISC_PFE_CFG0.BPS
	 * shows the number of bits per sample depends on the bit
	 * width of sensor output. The PFE module outputs a 12-bit
	 * data on the vp_data[11:0] bus */
	isc_software_reset();
	isc_pfe_set_video_mode(ISC_PFE_CFG0_MODE_PROGRESSIVE);
	isc_pfe_set_bps(ISC_PFE_CFG0_BPS(sensor_output_bit_width));
	isc_pfe_set_sync_polarity(0, ISC_PFE_CFG0_VPOL);

	/* Set Continuous Acquisition mode */
	isc_pfe_set_continuous_shot();
	isc_cfa_enabled(0);
	isc_wb_enabled(0);
	isc_gamma_enabled(0, 0);
	isc_csc_enabled(0);
	isc_sub422_enabled(0);
	isc_sub420_configure(0,0);
	isc_update_profile();
	if (sensor_mode == RAW_BAYER) {
		/* In a single-sensor system, each cell on the sensor
		 * has a specific color filter and microlens
		 * positioned above it. The raw data obtained from the
		 * sensor do not have the full R/G/B information at
		 * each cell position. Color interpolation is required
		 * to retrieve the missing components. */
		isc_cfa_enabled(1);
		isc_cfa_configure(ISC_CFA_CFG_BAYCFG_BGBG, 1);

		/* The White Balance (WB) module captures the data bus
		 * from the PFE module, each Bayer color component (R,
		 * Gr, B, Gb) can be manually adjusted using an offset
		 * and a gain. */
		isc_wb_enabled(1);
		isc_wb_set_bayer_pattern(ISC_WB_CFG_BAYCFG_BGBG);
		/* Test value for White balance setting */
		isc_wb_adjust_bayer_color(0, 0, 0, 0, 0x200, 0x200, 0x200, 0x200);

		/* Set up Gamma table with gamma 1/2.2 */
		for (i = 0; i< GAMMA_ENTRIES; i++){
			ISC->ISC_GAM_RENTRY[i] = gamma_table[i];
			ISC->ISC_GAM_GENTRY[i] = gamma_table[i];
			ISC->ISC_GAM_BENTRY[i] = gamma_table[i];
		}
		// Performs a 12-bit to 10-bit with simply 2 bit cut.
		isc_gamma_enabled(1, 0);

#if defined GAMMA_TUNING
		// Performs a 12-bit to 10-bit with power function.
		isc_gamma_enabled(1, ISC_GAM_CTRL_BENABLE |
				  ISC_GAM_CTRL_GENABLE |
				  ISC_GAM_CTRL_RENABLE);
#endif
		if ((lcd_mode == LCD_MODE_YUV422_PLANAR) \
			|| (lcd_mode == LCD_MODE_YUV420_PLANAR )\
			|| (lcd_mode == LCD_MODE_YUV422_SEMIPLANAR) \
			|| (lcd_mode == LCD_MODE_YUV420_SEMIPLANAR )\
			) {
			/* By converting an image from RGB to YCbCr
			 * color space, it is possible to separate Y,
			 * Cb and Cr information. */
			isc_csc_enabled(1);
			isc_csc_configure(&cs);
			/* The color space conversion output stream is
			 * a full-bandwidth YCbCr 4:4:4 signal. The
			 * chrominance subsampling divides the
			 * horizontal chrominance sampling rate by
			 * two */
			isc_sub422_enabled(1);
			if ((lcd_mode == LCD_MODE_YUV420_PLANAR) || (lcd_mode == LCD_MODE_YUV420_SEMIPLANAR)){
				isc_sub420_configure(1, 0);
			}
			/* Configure limited YCrcb output format
			 * before the DMA master module */
			isc_rlp_configure(ISC_RLP_CFG_MODE_YYCC_LIMITED, 0);
			/* Set DAM for 16-bit YC422P with stream
			 * descriptor view 2 for YCbCr planar pixel
			 * stream */
			if (lcd_mode == LCD_MODE_YUV422_PLANAR){
				isc_dma_configure_input_mode(ISC_DCFG_IMODE_YC422P |
											ISC_DCFG_YMBSIZE_BEATS8 |
											ISC_DCFG_CMBSIZE_BEATS8);
			} else if (lcd_mode == LCD_MODE_YUV422_SEMIPLANAR){
				isc_dma_configure_input_mode(ISC_DCFG_IMODE_YC422SP |
											ISC_DCFG_YMBSIZE_BEATS8 |
											ISC_DCFG_CMBSIZE_BEATS8);
			} else if (lcd_mode == LCD_MODE_YUV420_PLANAR){
				isc_dma_configure_input_mode(ISC_DCFG_IMODE_YC420P |
											ISC_DCFG_YMBSIZE_BEATS8 |
											ISC_DCFG_CMBSIZE_BEATS8);
			} else if (lcd_mode == LCD_MODE_YUV420_SEMIPLANAR){
				isc_dma_configure_input_mode(ISC_DCFG_IMODE_YC420SP |
											ISC_DCFG_YMBSIZE_BEATS8 |
											ISC_DCFG_CMBSIZE_BEATS8);
			}
			isc_dma_configure_desc_entry((uint32_t)&dma_descs2[0]);
			if ((lcd_mode == LCD_MODE_YUV422_PLANAR) \
				|| (lcd_mode == LCD_MODE_YUV420_PLANAR)){
				isc_dma_enable(ISC_DCTRL_DVIEW_PLANAR | ISC_DCTRL_DE);
			}
			if ((lcd_mode == LCD_MODE_YUV422_SEMIPLANAR)\
				|| (lcd_mode == LCD_MODE_YUV420_SEMIPLANAR )){
				isc_dma_enable(ISC_DCTRL_DVIEW_SEMIPLANAR | ISC_DCTRL_DE);
			}
			isc_dma_adderss(0, ISC_OUTPUT_BASE_ADDRESS, 0);
			isc_dma_adderss(0, ISC_OUTPUT_BASE_ADDRESS2, 0);
			isc_dma_adderss(0, ISC_OUTPUT_BASE_ADDRESS1, 0);
		}
		if (lcd_mode == LCD_MODE_RGB565) {
			/* Configure RGB 565 output format before the
			 * DMA master module */
			isc_rlp_configure(ISC_RLP_CFG_MODE_RGB565, 0);
			/* Set DAM for 16-bit packaged stream with
			 * descriptor view 0 used for the data stream
			 * is packed. */
			isc_dma_configure_input_mode(ISC_DCFG_IMODE_PACKED16);
			isc_dma_configure_desc_entry((uint32_t)&dma_descs[0]);
			isc_dma_enable(ISC_DCTRL_DVIEW_PACKED | ISC_DCTRL_DE);
			isc_dma_adderss(0, ISC_OUTPUT_BASE_ADDRESS, 0);
		}
	}
	if (sensor_mode == YUV_422) {
		isc_cfa_enabled(0);
		isc_wb_enabled(0);
		isc_gamma_enabled(0, 0);
		isc_csc_enabled(0);
		isc_sub422_enabled(0);
		isc_sub420_configure(0,0);
		/* Configure DAT8 output format before the DMA master module */
		isc_rlp_configure(ISC_RLP_CFG_MODE_DAT8, 0);
		/* Set DAM for 8-bit packaged stream with descriptor view 0 used
		   for the data stream is packed*/
		isc_dma_configure_input_mode(ISC_DCFG_IMODE_PACKED8);
		isc_dma_configure_desc_entry((uint32_t)&dma_descs[0]);
		isc_dma_enable(ISC_DCTRL_DVIEW_PACKED | ISC_DCTRL_DE);
		isc_dma_adderss(0, ISC_OUTPUT_BASE_ADDRESS, 0);
	}

	isc_histogram_enabled(1);
	isc_clear_histogram_table();
	isc_update_profile();

	aic_set_source_vector(ID_ISC, isc_handler);
	isc_enable_interrupt(ISC_INTEN_VD
			      | ISC_INTEN_DDONE
			      | ISC_INTEN_LDONE
			      | ISC_INTEN_HISDONE
			      | ISC_INTEN_HDTO
			      | ISC_INTEN_VDTO);
	isc_interrupt_status();
	capture_started = 0;
	aic_enable(ID_ISC);
}

/**
 * \brief Set up DMA Descriptors.
 */
static void configure_dma_linklist(void)
{
	uint32_t i;

	aic_disable(ID_ISC);
	isc_software_reset();
	if ((lcd_mode == LCD_MODE_YUV422_PLANAR) \
		|| (lcd_mode == LCD_MODE_YUV420_PLANAR)){
		for(i = 0; i < ISC_MAX_NUM_FRAME_BUFFER; i++) {
			dma_descs2[i].ctrl =
				ISC_DCTRL_DVIEW_PLANAR | ISC_DCTRL_DE;
			dma_descs2[i].next_desc = (uint32_t)&dma_descs2[i + 1 ];
			dma_descs2[i].addr0 = (uint32_t)ISC_OUTPUT_BASE_ADDRESS;
			dma_descs2[i].stride0 = 0;
			dma_descs2[i].addr1 = (uint32_t)ISC_OUTPUT_BASE_ADDRESS2;
			dma_descs2[i].stride1 = 0;
			dma_descs2[i].addr2 =  (uint32_t)ISC_OUTPUT_BASE_ADDRESS1;
			dma_descs2[i].stride2 = 0;
		}
		dma_descs2[i-1].next_desc = (uint32_t)&dma_descs2[0];
		cache_clean_region(dma_descs2, sizeof(dma_descs2));

	} else if ((lcd_mode == LCD_MODE_YUV422_SEMIPLANAR) \
				|| (lcd_mode == LCD_MODE_YUV420_SEMIPLANAR)){
		for(i = 0; i < ISC_MAX_NUM_FRAME_BUFFER; i++) {
			dma_descs2[i].ctrl =
				ISC_DCTRL_DVIEW_SEMIPLANAR | ISC_DCTRL_DE;
			dma_descs2[i].next_desc = (uint32_t)&dma_descs2[i + 1 ];
			dma_descs2[i].addr0 = (uint32_t)ISC_OUTPUT_BASE_ADDRESS;
			dma_descs2[i].stride0 = 0;
			dma_descs2[i].addr1 = (uint32_t)ISC_OUTPUT_BASE_ADDRESS1 + 1;
			dma_descs2[i].stride1 = 0;
			dma_descs2[i].addr2 = 0;
			dma_descs2[i].stride2 = 0;
		}
		dma_descs2[i-1].next_desc = (uint32_t)&dma_descs2[0];
		cache_clean_region(dma_descs2, sizeof(dma_descs2));
	} else {
		for(i = 0; i < ISC_MAX_NUM_FRAME_BUFFER; i++) {
			dma_descs[i].ctrl =
				ISC_DCTRL_DVIEW_PACKED | ISC_DCTRL_DE;
			dma_descs[i].next_desc =(uint32_t)&dma_descs[i + 1];
			dma_descs[i].addr = (uint32_t)ISC_OUTPUT_BASE_ADDRESS;
			dma_descs[i].stride = 0;
		}
		dma_descs[i-1].next_desc = (uint32_t)&dma_descs[0];
		cache_clean_region(dma_descs, sizeof(dma_descs));
	}
}

/**
 * \brief Calculate gain for RGGB bayer and perform them with ISC WB module.
 */
static void awb_update(void)
{
	float gain[HIST_RGGB_BAYER];
	uint32_t gain_049[HIST_RGGB_BAYER];
	gain[HISTOGRAM_GB] = 1.0000000;
	gain[HISTOGRAM_GR] = (float)(histogram_count[HISTOGRAM_GB]) / \
						(float)(histogram_count[HISTOGRAM_GR]);
	gain[HISTOGRAM_R] = (float)(histogram_count[HISTOGRAM_GB]) / \
						(float)(histogram_count[HISTOGRAM_R]);
	gain[HISTOGRAM_B] = (float)(histogram_count[HISTOGRAM_GB]) / \
						(float)(histogram_count[HISTOGRAM_B]);

	gain_049[HISTOGRAM_GB] = float2hex(0, 4, 9, gain[HISTOGRAM_GB]);
	gain_049[HISTOGRAM_GR] = float2hex(0, 4, 9, gain[HISTOGRAM_GR]);
	gain_049[HISTOGRAM_B] = float2hex(0, 4, 9, gain[HISTOGRAM_B]);
	gain_049[HISTOGRAM_R] = float2hex(0, 4, 9, gain[HISTOGRAM_R]);
	isc_wb_adjust_bayer_color(0, 0, 0, 0,
					gain_049[HISTOGRAM_R],
					gain_049[HISTOGRAM_GR],
					gain_049[HISTOGRAM_B],
					gain_049[HISTOGRAM_GB]);
	isc_update_profile();
}

#if defined AE_TUNING
/**
 * \brief Calculate gain/offset for Y and perform them with ISC CBC module.
 */
static void ae_update(void)
{
	uint16_t i;
	float ygain, offset;
	uint32_t ycount;
	uint16_t dark_threshold, bright_threshold;
	uint32_t gain_110, offset_138;

	ycount = 0;
	dark_threshold = 0;
	for (i = 0; i < HIST_ENTRIES; i++) {
		ycount += histogram_y[i];
		if (ycount >= histogram_count[HISTOGRAM_Y] * 5 / 100) {
			dark_threshold = i;
			break;
		}
	}

	ycount = 0;
	bright_threshold = i;
	for (i = HIST_ENTRIES - 1; i > 1; i--) {
		ycount += histogram_y[i];
		if (ycount >= histogram_count[HISTOGRAM_Y]) {
			bright_threshold = i;
			break;
		}
	}
	//printf("<%d, %d> ", bright_threshold, dark_threshold);
	ygain = 512.0/ ((float)(bright_threshold - dark_threshold));
	offset = (512.0 / ygain) - (float)bright_threshold;
	//printf("<%f, %f> ",ygain,offset);
	gain_110 = float2hex(1, 10, 0, ygain);
	offset_138 = float2hex(1, 3, 8, offset);

	isc_cbc_configure(0, 0, gain_110, offset_138);
	isc_update_profile();
	//printf("<%x, %x> ", gain_110, offset_138);
}
#endif

/**
 * \brief Count up 4 channel R/G/Gr/Gb histogram data.
 */
static void histogram_count_up(void)
{
	uint32_t *v;
	uint32_t i;
	
	v = (uint32_t *)ISC_HIS_BASE_ADDRESS;
	histogram_count[histogram_idx_isc] = 0;
	for (i = 0; i < HIST_ENTRIES; i++){
		if (histogram_idx_isc < HISTOGRAM_Y){
			histogram_count[histogram_idx_isc]+= (*v) * i;
		} else {
			histogram_count[histogram_idx_isc]+= *v ;
		}
		if(histogram_idx_isc == HISTOGRAM_Y){
			histogram_y[i] = (*v);
		}
		v++;
	}
}

/**
 * \brief Status machine for auto white balance.
 */
static bool auto_white_balance(void)
{
	if (awb_status_machine == AWB_INIT){
		isc_histogram_configure(histogram_idx_isc, ISC_HIS_CFG_BAYSEL_BGBG, 1);
		isc_update_profile();
		histogram_done = false;
		histogram_read = false;
		isc_update_histogram_table();
		awb_status_machine = AWB_WAIT_HIS_READY;
		return true;
	}
	if (awb_status_machine == AWB_WAIT_HIS_READY){
		if(!histogram_done)
			return true;
		histogram_done = false;
		xdma_read_histogram(ISC_HIS_BASE_ADDRESS);
		awb_status_machine = AWB_WAIT_DMA_READY;
		return true;
	}
	if (awb_status_machine == AWB_WAIT_DMA_READY){
		if(!histogram_read)
			return true;
		histogram_read = false;
		histogram_count_up();
		histogram_idx_isc++;
		if(histogram_idx_isc < HIST_RGGB_BAYER ) {
			awb_status_machine = AWB_INIT;
		} else {
			awb_status_machine = AWB_WAIT_ISC_PERFORMED;
		}
		return true;
	}
	if (awb_status_machine == AWB_WAIT_ISC_PERFORMED){
		awb_update();
#if defined AE_TUNING
		ae_update();
#endif
		histogram_idx_isc = 0;
		awb_status_machine = AWB_INIT;
		return true;
	}
	return true;
}

/*----------------------------------------------------------------------------
 *        Global functions
 *----------------------------------------------------------------------------*/
/**
 * \brief Application entry point for ISI example.
 *
 * \return Unused (ANSI-C compatibility).
 */
extern int main(void)
{
	int i;
	uint8_t key;

	/* Output example information */
	console_example_info("ISC Example");

	/* TWI Initialize */
	configure_twi();

	/* Allocate a XDMA channel. */
	xdmad_channel = xdmad_allocate_channel(XDMAD_PERIPH_MEMORY, XDMAD_PERIPH_MEMORY);

	if (!xdmad_channel) {
		printf("-E- Can't allocate XDMA channel\n\r");
		return 0;
	}

	printf("Image Sensor Selection:\n\r");
	for (i = 0; i < ARRAY_SIZE(sensor_profiles); i++)
		printf("- '%d' %s\n\r", i + 1, sensor_profiles[i]->name);
	for(;;) {
		printf("Press [1..%d] to select supported sensor\n\r",
			ARRAY_SIZE(sensor_profiles));
		key = console_get_char();
		if ((key >= '1') && (key <= ('1' + ARRAY_SIZE(sensor_profiles)))) {
			sensor_idx = key - '1';
			break;
		}
	}
	
restart_sensor:

	printf("------------ %s -------------\n\r",
		sensor_profiles[sensor_idx]->name);
	printf("- 'Y' Test YUV mode input       -\n\r");
	printf("- 'B' Test RAW BAYER mode input -\n\r");
	printf("---------------------------------\n\r");
	for(;;) {
		key = console_get_char();
		if ((key == 'Y') || (key == 'y')) {
			sensor_mode = YUV_422;
			break;
		}
		if ((key == 'B') || (key == 'b')) {
			sensor_mode = RAW_BAYER;
			break;
		}
	}

	if (sensor_setup(&twid, sensor_profiles[sensor_idx], QVGA, sensor_mode) != SENSOR_OK){
		printf("-E- Sensor setup failed.");
		while (1);
	}

	/* Retrieve sensor output format and size */
	sensor_get_output(QVGA, sensor_mode, &sensor_output_bit_width,
			  &image_width, &image_height);

	if (sensor_mode == RAW_BAYER){
		printf("-----------------------------------\n\r");
		printf("- '0' Test RGB565 output \n\r");
		printf("- '1' Test YC422P output \n\r");
		printf("- '2' Test YC422SP output \n\r");
		printf("- '3' Test YC420P output \n\r");
		printf("- '4' Test YC420SP output \n\r");
		printf("-----------------------------------\n\r");
		for(;;) {
			key = console_get_char();
			if (key == '0') {
				lcd_mode = LCD_MODE_RGB565;
				break;
			}else if (key == '1') {
				lcd_mode = LCD_MODE_YUV422_PLANAR;
				break;
			} else if (key == '2') {
				lcd_mode = LCD_MODE_YUV422_SEMIPLANAR;
				break;
			} else if (key == '3') {
				lcd_mode = LCD_MODE_YUV420_PLANAR;
				break;
			} else if (key == '4') {
				lcd_mode = LCD_MODE_YUV420_SEMIPLANAR;
				break;
			}
		}
	} else {
		lcd_mode = LCD_MODE_YUV;
	}

	printf("-I- Bit width = %d, Image Width = %d, Image Height=%d \n\r",
	       (unsigned int)(sensor_output_bit_width + 8),
	       (unsigned int)image_width,
	       (unsigned int)image_height);

	/* Configure LCD */
	configure_lcd();
	configure_dma_linklist();
	configure_isc();
	awb_status_machine = AWB_INIT;
	ae_awb = false;
	printf("-I- Preview start. \n\r");
	printf("-I- press 'S' or 's' to switch ISC mode. \n\r");
	if (sensor_mode == RAW_BAYER) {
		printf("-I- press 'A' or 'a' to start auto white balance & AE. \n\r");
	}
	
	for(;;) {
		if (console_is_rx_ready()) {
			key = console_get_char();
			if ((key == 'S') || (key == 's')) {
				isc_stop_capture();
				break;
			}
			if (sensor_mode == RAW_BAYER) {
				if ((key == 'A') || (key == 'a')) {
					ae_awb = true;
					auto_white_balance();
				}
			}
		}
		if (ae_awb)
			auto_white_balance();
	}
	goto restart_sensor;
}

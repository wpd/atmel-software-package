/* ----------------------------------------------------------------------------
 *         SAM Software Package License
 * ----------------------------------------------------------------------------
 * Copyright (c) 2013, Atmel Corporation
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
 */

/*------------------------------------------------------------------------------
 *         Headers
 *------------------------------------------------------------------------------*/

#include "io/omnivision.h"

/*------------------------------------------------------------------------------
 *         Local Variables
 *------------------------------------------------------------------------------*/

const struct ov_reg ov7740_yuv_vga[]= {
	{0x12 ,0x80},
//{0x11 ,0x07},

	{0x55 ,0x40},
	{0x11 ,0x02},

	{0x12 ,0x00},
	{0xd5 ,0x10},
	{0x0c ,0x12},
	{0x0d ,0x34},
	{0x17 ,0x25},
	{0x18 ,0xa0},
	{0x19 ,0x03},
	{0x1a ,0xf0},
	{0x1b ,0x89}, //;was 81
	{0x22 ,0x03}, //;new
	{0x29 ,0x18}, //;was 17
	{0x2b ,0xf8},
	{0x2c ,0x01},
	{0x31 ,0xa0},
	{0x32 ,0xf0},
	{0x33 ,0xc4}, //;was44
	{0x35 ,0x05}, //;new
	{0x36 ,0x3f},
	{0x04 ,0x60},
	{0x27 ,0x80}, //;delete "42 3a b4"
	{0x3d ,0x0f},
	{0x3e ,0x80},
	{0x3f ,0x40},
	{0x40 ,0x7f},
	{0x41 ,0x6a},
	{0x42 ,0x29},
	{0x44 ,0x22}, //;was 11
	{0x45 ,0x41},
	{0x47 ,0x02},
	{0x49 ,0x64},
	{0x4a ,0xa1},
	{0x4b ,0x40},
	{0x4c ,0x1a},
	{0x4d ,0x50},
	{0x4e ,0x13},
	{0x64 ,0x00},
	{0x67 ,0x88},
	{0x68 ,0x1a},

	{0x14 ,0x28}, //;38/28/18 for 16/8/4x gain ceiling
	{0x24 ,0x3c},
	{0x25 ,0x30},
	{0x26 ,0x72},
	{0x50 ,0x97},
	{0x51 ,0x1f}, //;0fc/7e/3f/1f for 60/30/15/7.5fps, 60Hz
	{0x52 ,0x00}, //;[7:6]/[5:4] 2 msb for 60/50Hz
	{0x53 ,0x00},
	{0x20 ,0x00},
	{0x21 ,0xcf}, //;01/23/57/cf for 60/30/15/7.5fps
	{0x50, 0x4b}, // ;12e/97/4b/25 for 60/30/15/7.5fps, 50Hz
	{0x38 ,0x14},
	{0xe9 ,0x00},
	{0x56 ,0x55},
	{0x57 ,0xff},
	{0x58 ,0xff},
	{0x59 ,0xff},
	{0x5f ,0x04},
	{0xec ,0x00},
	{0x13 ,0xff},

	{0x80 ,0x7f}, //;[6]/[5] for BPC/WPC
	{0x81 ,0x3f},
	{0x82 ,0x32},
	{0x83 ,0x01},
	{0x38 ,0x11},
	{0x84 ,0x70},
	{0x85 ,0x00},
	{0x86 ,0x03},
	{0x87 ,0x01},
	{0x88 ,0x05},
	{0x89 ,0x30},
	{0x8d ,0x30},
	{0x8f ,0x85},
	{0x93 ,0x30},
	{0x95 ,0x85},
	{0x99 ,0x30},
	{0x9b ,0x85},

	{0x9c ,0x08},
	{0x9d ,0x12},
	{0x9e ,0x23},
	{0x9f ,0x45},
	{0xa0 ,0x55},
	{0xa1 ,0x64},
	{0xa2 ,0x72},
	{0xa3 ,0x7f},
	{0xa4 ,0x8b},
	{0xa5 ,0x95},
	{0xa6 ,0xa7},
	{0xa7 ,0xb5},
	{0xa8 ,0xcb},
	{0xa9 ,0xdd},
	{0xaa ,0xec},
	{0xab ,0x1a},

	{0xce ,0x78},
	{0xcf ,0x6e},
	{0xd0 ,0x0a},
	{0xd1 ,0x0c},
	{0xd2 ,0x84},
	{0xd3 ,0x90},
	{0xd4 ,0x1e},

	{0x5a ,0x24},
	{0x5b ,0x1f},
	{0x5c ,0x88},
	{0x5d ,0x60},

	{0xac ,0x6e},
	{0xbe ,0xff},
	{0xbf ,0x00},

	{0x0f ,0x1d},
	{0x0f ,0x1f},
	{0xFF, 0xFF}
};

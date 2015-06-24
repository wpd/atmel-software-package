/* ----------------------------------------------------------------------------
 *         ATMEL Microcontroller Software Support
 * ----------------------------------------------------------------------------
 * Copyright (c) 2013, Atmel Corporation
 */

#ifndef _SHDWC_H
#define _SHDWC_H

/*------------------------------------------------------------------------------
 *         Headers
 *------------------------------------------------------------------------------*/

#include "chip.h"

#include <stdint.h>

/*------------------------------------------------------------------------------
 *         Definitions
 *------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
 *         Global functions
 *------------------------------------------------------------------------------*/

extern void shdwc_configure_wakeup_mode (uint32_t config);
extern void shdwc_set_wakeup_input (uint32_t input_enable, uint32_t input_type);
extern void shdwc_do_shutdown(void);
extern uint32_t shdwc_get_status(void);

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
#endif //#ifndef _SHDWC_H
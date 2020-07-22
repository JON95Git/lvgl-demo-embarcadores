/**
 * @file disp.h
 * 
 */

#ifndef DISP_H
#define DISP_H

/*********************
 *      INCLUDES
 *********************/
#include <stdint.h>
#include "lv_color.h"
#include "lv_area.h"

/*********************
 *      DEFINES
 *********************/
#ifdef LANDSCAPE
#define TFT_HOR_RES 	320 
#define TFT_VER_RES 		240
#else
#define TFT_HOR_RES 240
#define TFT_VER_RES 320
#endif

#define TFT_EXT_FB		1		/*Frame buffer is located into an external SDRAM*/
#define TFT_USE_GPU		1		/*Enable hardware accelerator*/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 * GLOBAL PROTOTYPES
 **********************/
void tft_init(void);

/**********************
 *      MACROS
 **********************/

#endif

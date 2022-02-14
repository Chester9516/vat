/*
 * Copyright (c) 2003-2008, System IC Div., LG Electronics, Inc.
 * All Rights Reserved
 *
 * This is UNPUBLISHED PROPRIETARY SOURCE CODE of LG Electronics, Inc.
 * and may contain proprietary, confidential and trade secret information of
 * LG Electronics, Inc. and/or its partners.
 *  
 * The contents of this file may not be disclosed to third parties or
 * copied or duplicated in any form, in whole or in part, without the prior
 * written permission of LG Electronics, Inc.
 *
 */

#include "LGDT3305.h"

#ifndef __LGDT_UTIL_H__
#define __LGDT_UTIL_H__

// Add USB-I2C interface : j.y. won 2007.06.03
typedef enum {
	LGDT_NO_INTERFACE = 0,
	LGDT_LPT		  = 1,
	LGDT_USB		  = 2,
	LGDT_ERROR_INTERFACE = 255,
} LgdtInterface_t;

// for "C" compiler
#if defined( __cplusplus )
extern "C"{
#endif
int Lgdt_I2C_Write(DATA08 nAddrDev,DATA08 *pData, DATA08 nSizeData);
int Lgdt_I2C_Read(DATA08 nAddrDev, DATA16 nAddrReg, DATA08 *pData, DATA08 nSizeData);
int Lgdt_SetActiveI2C(void *pi2c);
extern int LgdtDelay(DATA16 delay);
#if defined( __cplusplus )
}
#endif


#endif

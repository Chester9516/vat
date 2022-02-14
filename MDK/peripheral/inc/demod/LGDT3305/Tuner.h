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


#ifndef __LGDT_Tuner_H__
#define __LGDT_Tuner_H__

/*------------------------------------------------------//
// Tuner Model ID
//------------------------------------------------------*/
typedef enum {
	LGDT_TUNER_PED_ET55D= 13,
	LGDT_TUNER_PED_61HSR= 14,

	LGDT_TUNER_LGIT_701 = 21,
	LGDT_TUNER_LGIT_001 = 23,

	LGDT_TUNER_ALPS_TDQU= 32,
	LGDT_TUNER_ALPS_TDQU_H04A= 33,

	LGDT_TUNER_SHARP_F2341 = 42,

	LGDT_TUNER_THOMSON7683	= 52,

	LGDT_TUNER_SANYO_06AP = 61,

	LGDT_TUNER_CHIP_NXP18271 = 71,

	LGDT_TUNER_CHIP_XC5000 = 81,

	LGDT_TUNER_CHIP_MT2131 = 91,

	LGDT_TUNER_CHIP_NMI120 = 92,
	LGDT_TUNER_CHIP_MXL603 = 93,
	
} LgdtTuner_t;

typedef struct {
	LgdtTuner_t	modelTuner ;	// tuner type
	double		m_RF;			// RF frequency
	double		m_IF;			// IF frequency
	unsigned int m_IF_level;	// IF AGC LEVEL
	unsigned int m_LPF;			// Low-pass Filter for NXP
	unsigned int m_Mode;		// Operation Mode
	unsigned char m_IsExternalAGC ;	// AGC mode
	unsigned char m_bmanualIF;		// manual IF frequency
	unsigned int  m_Reserved;
} LgdtTunerConfig_t;

// Added by j.y. won 2007.06.03
//extern LgdtTuner_t modelTuner;
extern LgdtTunerConfig_t cfgTuner;


#endif

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
 *  Revision History
 *
 *	 Rev#	 : Author
 * Date 	
 * Description
 *-----------------------------------------------------------------------------

    
 * 0.0.1 : jywon
    (June 21, 2007)
	1. Draft

 * 0.9.0 : jywon
    (Feb 2, 2008)
	1. Version for final test
	2. Add Chip tuner (TDA18271, XC5000)
    (Feb 8, 2008)
	1. Add Chip tuner (MT2131)
    (Feb 19, 2008)
	1. set NCO freqeuncy, spectrum, DSAWBPY, DAGCBPY according to the if freqeuncy.

  * 0.9.1 : jywon
    (Feb 21, 2008)
	1. Intial setting value : when using RF AGC
	2. LgdtCheckFreqOffset() function
	3. Remove old tuner
	4. Modify TunerThomson7683(), TunerLGIT_001() 
	5. Add Code for PLL sweep, optional
	   => (change initial setting : 0x012A[5] for PLL sweep)

  * 0.9.2 : jywon
    (Feb 28, 2008)
	1. Modify tuner function : XC5000, MT2131, TunerALPS_TDQU_H04A()
	(Mar 05, 2008)
	2. Modify QAM channel scan : check (AGC lock -> TR lock) 

  * 0.9.3 : jywon
    (Mar 8, 2008)
	1. Modify tuner function : XC5000 (RF_FREQ register, firmware 1.4.67)

  * 0.9.4 : jywon
    (Mar 10, 2008)
	1. Modify intial setting : GCONCIR(@0x0116[7])<='0'
	2. Modify tuner function : XC5000 (RF_MODE, firmware 1.4.68)

 * 
 *------------------------------------------------------------------------------
 */
#include <math.h>
#include "demod/LGDT3305/Tuner.h"
#include "demod/LGDT3305/LGDT_Util.h"

#define	bool	int
#define	false 	0
#define	true	1

#ifndef __LGDT3305_API_H__
#define __LGDT3305_API_H__

#define	LGDT_VERSION_MAJOR	0
#define	LGDT_VERSION_MINOR	9
#define	LGDT_VERSION_CUSTOM	4

typedef enum{
	LGDT_ERR_NO			=   0,
	LGDT_ERR_I2C		=   1,
	LGDT_ERR_INIT		=   2,
	LGDT_ERR_RUN		=   4,
	LGDT_ERR_PARAM		=   8,	/*-----		Invalid parameter check				-----*/
	LGDT_ERR_CHECK_FUNC	=  16,	/*-----		Error returned from LgdtCheck...()	-----*/
	LGDT_ERR_ALTER_FUNC	=  32,	/*-----		Error returned from LgdtAlter...()	-----*/
	LGDT_ERR_SCAN		=  64,	/*-----		Channel Scan						-----*/
	LGDT_ERR_ANT		= 128,	/*-----		Smart Antenna						-----*/
	LGDT_ERR_TUNER		= 256,	/*-----		Reserved for a tuner manufacturer	-----*/
	LGDT_ERR_OTHER		= 512,
}LgdtReturn_t;


typedef enum{
	LGDT_QAM64	= 0x00,
	LGDT_QAM256	= 0x01,
	LGDT_VSB	= 0x03,
	LGDT_ANTENNA= 0x05,
}LgdtOperMode_t;


typedef enum{
	LGDT_SIGNAL_NO			= 0,	/*----- little or no input power detected.		-----*/
	LGDT_SIGNAL_WEAK		= 1,	/*----- some power detected.					-----*/
	LGDT_SIGNAL_MODERATE	= 2,
	LGDT_SIGNAL_STRONG		= 4,
	LGDT_SIGNAL_VERY_STRONG	= 8
}LgdtSignalCondition_t;


typedef enum{
	LGDT_PARAL  = 0x00,
	LGDT_SERIAL = 0x01
}LgdtMpegMode_t;


typedef enum{
	LGDT_CLOCK_LOW  = 0x00,
	LGDT_CLOCK_HIGH = 0x01
}LgdtClock_t;


typedef enum {
	LGDT_NOT_INVERTED = 0,
	LGDT_INVERTED = 1
} LgdtInvert_t;


typedef enum {
	LGDT_PolyPRBS15 = 0,
	LGDT_PolyPRBS23 = 1,
	LGDT_PolyPRBS   = 2,			/*-----		 etc	-----*/
} LgdtPolyPRBS_t;


typedef struct {
	DATA08		Major;
	DATA08		Minor;
	DATA08		Custom;
	LgdtTuner_t	Tuner;
} LgdtVersion_s;

typedef enum {
	LGDT_CAN = 0x00,
	LGDT_NXP = 0x01, 
	LGDT_MICROTUNE = 0x02,
	LGDT_DEFAULT = 0x03,
} LgdtSettingMode;

LgdtReturn_t LgdtRepeaterI2C(bool mode);

int LgdtRun(LgdtOperMode_t modeOper, DATA_FLOAT nFreq);

LgdtReturn_t LgdtInitVSB();

LgdtReturn_t LgdtInitFAT(LgdtOperMode_t, bool);
LgdtReturn_t LgdtResetFAT(void);
LgdtReturn_t LgdtResetBER(void);
LgdtReturn_t LgdtCheckStatusFAT();
LgdtReturn_t LgdtCheckVersion(LgdtVersion_s*);

LgdtReturn_t LgdtTunerInitAFC(DATA_FLOAT);
LgdtReturn_t LgdtTunerAFC();

LgdtReturn_t LgdtIsLockCR(bool* );
LgdtReturn_t LgdtIsLockSync(bool* );
LgdtReturn_t LgdtIsLockTR(bool* );
LgdtReturn_t LgdtIsLockFEC(bool* , bool* , bool* );
LgdtReturn_t LgdtIsLockBER(bool* );
LgdtReturn_t LgdtIsLockComb(bool* );
LgdtReturn_t LgdtIsLockAGC(bool* );

LgdtReturn_t LgdtCheckRegi(DATA16, DATA08*);
LgdtReturn_t LgdtCheckSnrFAT(double*);
LgdtReturn_t LgdtCheckSnrVSB_EQ(DATA_DOUBLE* nPT);
LgdtReturn_t LgdtCheckPacketErr(DATA16*);
LgdtReturn_t LgdtCheckOperMode(LgdtOperMode_t*);
LgdtReturn_t LgdtCheckSignalCondition(LgdtSignalCondition_t*);
LgdtReturn_t LgdtCheckFreqOffset(float*);			// modified by j.y. won : 2007.06.13
LgdtReturn_t LgdtCheckIFAcc(short*);
LgdtReturn_t LgdtCheckDCValue(DATA08*);
LgdtReturn_t LgdtCheckIntMode(DATA08* , DATA08* );
LgdtReturn_t LgdtCheckSpectrumFAT(LgdtInvert_t*);
LgdtReturn_t LgdtCheckScatFAT(DATA16* , DATA16* );
LgdtReturn_t LgdtCheckBer(float* );
LgdtReturn_t LgdtCheckPolyPRBS(LgdtPolyPRBS_t* );

LgdtReturn_t LgdtAlterRegi(DATA16 , DATA08 );
LgdtReturn_t LgdtAlterMpegMode(LgdtMpegMode_t);
LgdtReturn_t LgdtAlterMpegPolarity(LgdtClock_t , LgdtClock_t );
LgdtReturn_t LgdtAlterAgcData(DATA16);
LgdtReturn_t LgdtAlterAgcInput(DATA16 , DATA16 );
LgdtReturn_t LgdtAlterAgcPolarity(LgdtInvert_t , LgdtInvert_t );
LgdtReturn_t LgdtAlterAgcBandwidth(DATA16 , DATA16 );
LgdtReturn_t LgdtAlterSpectrumFAT(LgdtInvert_t);
LgdtReturn_t LgdtAlterInvertPRBS(LgdtInvert_t);
LgdtReturn_t LgdtAlterPolyPRBS(LgdtPolyPRBS_t, DATA32);

LgdtReturn_t LgdtTunerFreq(float, bool, LgdtOperMode_t);
LgdtReturn_t LgdtTunerInit(float, bool, LgdtOperMode_t);
LgdtReturn_t LgdtTunerSetPLL(float freq, bool IsExternalAGC, LgdtOperMode_t mode);
LgdtReturn_t LgdtTunerConfigSet(LgdtTunerConfig_t m_cfg);
LgdtReturn_t LgdtDownloadTunerFW(void);

int lg3305_lock_check(void); 

#endif


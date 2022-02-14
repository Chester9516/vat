/*****************************************************************************
 *
 *	Driver APIs for VSB/QAM Demodulator LGDT3305
 *
 *  Copyright (c) 2003-2008, System IC Div., LG Electronics, Inc.
 *  All Rights Reserved
 *
 *  File Name:      LGDT3305API.cpp
 *
 *  Description:    
 *
 *  Revision History:
 *
 *    Date		Author	Description
 *  -------------------------------------------------------------------------
 *   07-01-2006	pyjung	Original
 *   01-31-2008	jywon	Modify
 *
 ***************************************************************************/
//#include <sys_config.h>
//#include <retcode.h>
//#include <types.h>
//#include <osal/osal.h>
//#include <api/libc/alloc.h>
//#include <api/libc/printf.h>
//#include <api/libc/string.h>
//#include <hal/hal_gpio.h>
//#include <hld/hld_dev.h>
//#include <hld/nim/nim_dev.h>
//#include <hld/nim/nim.h>
//#include <hld/nim/nim_tuner.h>
//#include <bus/i2c/i2c.h>
//#include <bus/tsi/tsi.h>

//#include "globaldefs.h"
#include <stdio.h>
#include "demod/LGDT3305/LGDT3305.h"
#include "demod/LGDT3305/LGDT3305API.h"
#include "demod/LGDT3305/Tuner.h"

//#define DEBUGGING_FOR_LGE_SIC
//#undef DEBUGGING_FOR_LGE_SIC


int LG3305_Tune(int Freq,int mode);

//#undef INITIAL_SET
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//																		   //
//							Global Variables							   //
//																		   //
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//

/*----- for CheckStatus() -----*/
#ifdef VSB_PLL_SWEEP
static bool flagStart = true;
#endif

/*----- for LgdtAutoFreqControlInit() and LgdtAutoFreqControl() -----*/
DATA_FLOAT	gFreqCur = 0;				/*	Frequency Given to tune tuner		*/
int		gCountAFC = -1;					/*	AFC swing counter					*/

//LgdtTuner_t modelTuner = LGDT_TUNER_ALPS_TDQU;
LgdtTunerConfig_t cfgTuner={
		LGDT_TUNER_CHIP_MXL603,
		0.0,		// RF frequency
		6.0,		// IF frequency
		0,			// IF level
		0x1F,			// LPF
		LGDT_VSB,	// Operation Mode
		0,			// isExternalAGC (default false)
		0,			// manual IF frequency
		0			// Reserved
};

/*----- private functions are declared here		-----*/
LgdtReturn_t	LgdtTunerAFC();
LgdtReturn_t LgdtSetManualIfandSpectrum(bool complex_multiplier, bool vsb_inversion);

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//																		   //
//							Run Functions								   //
//																		   //
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//

/////////////////////////////////////////////////////////////////////////////
//
//  Name: LgdtResetFAT
//
//  Description:    Resets the demod all blocks - nSFTRST(@0x0002[0])
//
//  Parameters:     void
//
//  Returns:        (LgdtReturn_t)
//
//  Revision History:
//
//    Date		Author	Description
//  -------------------------------------------------------------------------
//   07-01-2006	pyjung	Original
//
/////////////////////////////////////////////////////////////////////////////
LgdtReturn_t LgdtResetFAT(void)
{
	LgdtReturn_t	nRet;
	DATA08			nData;

	nRet = LgdtCheckRegi(0x0002, &nData);
	if (nRet != LGDT_ERR_NO)
		return (LgdtReturn_t)(nRet|LGDT_ERR_INIT);

	nData = nData & 0xFE;
	nRet = LgdtAlterRegi(0x0002, nData);
	if (nRet != LGDT_ERR_NO)
		return (LgdtReturn_t)(nRet|LGDT_ERR_INIT);

	LgdtDelay((DATA16)20);       // reset delay

	nData |= 0x01;
	nRet = LgdtAlterRegi(0x0002, nData);
	if (nRet != LGDT_ERR_NO)
		return (LgdtReturn_t)(nRet|LGDT_ERR_INIT);

	return LGDT_ERR_NO;
}

/////////////////////////////////////////////////////////////////////////////
//
//  Name: LgdtResetBER
//
//  Description:    Resets BER blocks in the demod - BERTRST(@0x0800[0])
//
//  Parameters:     void
//
//  Returns:        (LgdtReturn_t)
//
//  Revision History:
//
//    Date		Author	Description
//  -------------------------------------------------------------------------
//   07-01-2006	pyjung	Original
//   10-24-2007	jywon	Modify
//
/////////////////////////////////////////////////////////////////////////////
LgdtReturn_t LgdtResetBER(void)
{
	LgdtReturn_t	nRet;
	DATA08			nData;

// added by j.y. won : 2007.10.24
// FEC RESET....
	nRet = LgdtCheckRegi(0x0504, &nData);
	if (nRet != LGDT_ERR_NO)
		return (LgdtReturn_t)(nRet|LGDT_ERR_INIT);
	nData = 0xF7;
	nRet = LgdtAlterRegi(0x0504, nData);
	if (nRet != LGDT_ERR_NO)
		return (LgdtReturn_t)(nRet|LGDT_ERR_INIT);

	LgdtDelay(20);
	nData |= 0x08;
	nRet = LgdtAlterRegi(0x0504, nData);
	if (nRet != LGDT_ERR_NO)
		return (LgdtReturn_t)(nRet|LGDT_ERR_INIT);

	nRet = LgdtCheckRegi(0x0800, &nData);
	if (nRet != LGDT_ERR_NO)
		return (LgdtReturn_t)(nRet|LGDT_ERR_INIT);

	nData &= 0xFE;
	nRet = LgdtAlterRegi(0x0800, (nData|0x01) );
	if (nRet != LGDT_ERR_NO)
		return (LgdtReturn_t)(nRet|LGDT_ERR_INIT);
	
	LgdtDelay((DATA16)DELAY);

	nRet = LgdtAlterRegi(0x0800, nData);
	if (nRet != LGDT_ERR_NO)
		return (LgdtReturn_t)(nRet|LGDT_ERR_INIT);

	return LGDT_ERR_NO;
}

/////////////////////////////////////////////////////////////////////////////
//
//  Name: LgdtRepeaterI2C
//
//  Description:    Enable/Disable  the I2C repeater - nENI2CRPT(@0x0001[5])
//
//  Parameters:     (bool) true = ENABLE, false = DISABLE
//
//  Returns:        (LgdtReturn_t)
//
//  Revision History:
//
//    Date		Author	Description
//  -------------------------------------------------------------------------
//   07-01-2006	pyjung	Original
//   08-21-2007	jywon	Modify
//
/////////////////////////////////////////////////////////////////////////////
LgdtReturn_t LgdtRepeaterI2C(bool mode)
{
	LgdtReturn_t	nRet;
	DATA08			nData;

	// Add 1ms delay by j.y. won : 2007.08.21 (due to tuner)
	LgdtDelay(1);

	nRet = LgdtCheckRegi(0x0001, &nData);
	if (nRet != LGDT_ERR_NO)
	{
		//SYS_Debug("%s %d error\n",__FUNCTION__,__LINE__);
		return (LgdtReturn_t)(nRet|LGDT_ERR_INIT);
	}

	nData &= 0xDF;

	if (mode == true)	/*----- I2C Repeater Turn ON	-----*/
	{
		nRet = LgdtAlterRegi(0x0001, nData);
		if (nRet != LGDT_ERR_NO)
		{
			//SYS_Debug("%s %d error\n",__FUNCTION__,__LINE__);
			return (LgdtReturn_t)(nRet|LGDT_ERR_INIT);
		}
	}
	else				/*----- I2C Repeater Turn OFF	-----*/
	{
		nData |= 0x20;
		nRet = LgdtAlterRegi(0x0001, nData);
		if (nRet != LGDT_ERR_NO)
		{
			//SYS_Debug("%s %d error\n",__FUNCTION__,__LINE__);
			return (LgdtReturn_t)(nRet|LGDT_ERR_INIT);
		}
	}

	// Add 1ms delay by j.y. won : 2007.08.21 (due to tuner)
	LgdtDelay(1);

	return LGDT_ERR_NO;
}

/////////////////////////////////////////////////////////////////////////////
//
//  Name :			LgdtCheckStatusFAT
//
//  Description :	Checks Demod status and call appropriate functions
//
//  Parameters :	void
//
//  Returns :		(LgdtReturn_t)
//
//  Dependencies:   (int)gCount1 - Global Variables used in this function 
//
//  Revision History:
//
//    Date		Author	Description
//  -------------------------------------------------------------------------
//   07-01-2006	pyjung	Original
//
/////////////////////////////////////////////////////////////////////////////
LgdtReturn_t LgdtCheckStatusVSB();
LgdtReturn_t LgdtCheckStatusQAM();

LgdtReturn_t LgdtCheckStatusFAT()
{
	LgdtOperMode_t	modeOper;
	LgdtReturn_t	nRet;

	nRet = LgdtCheckOperMode(&modeOper);
	switch(modeOper)
	{
		case LGDT_VSB:
			nRet = LgdtCheckStatusVSB();
			break;

		case LGDT_QAM64:
		case LGDT_QAM256:
			nRet = LgdtCheckStatusQAM();
			break;

		default:
			nRet = LGDT_ERR_PARAM;
	}
	return nRet;
}


LgdtReturn_t LgdtCheckStatusVSB()
{
#ifdef VSB_PLL_SWEEP
	int cnt1;
	bool	flag = false;
	LgdtReturn_t	nRet;
	DATA08	readData;

	cnt1 = 0;	flag = false;
	do{
		LgdtDelay((DATA16)50);
		// Check Carrier Lock
		nRet = LgdtIsLockCR(&flag);
		if(nRet != LGDT_ERR_NO)
			return LGDT_ERR_INIT;

		cnt1++;
	}while((!flag) && (cnt1<=1));

	if(!flag){
		LgdtTunerAFC();
		return LGDT_ERR_INIT;
	}

	if (flagStart == true)
	{
		LgdtDelay((DATA16)150);
	}

	LgdtDelay((DATA16)50);

	LgdtIsLockSync(&flag);

	if (flag == false)
	{
		flagStart = false;
	}
	else				//  (flag == true), sync OK
	{
		LgdtDelay((DATA16)300);
		flagStart = true;
	}

	return LGDT_ERR_NO;
#else
	// you can modify the delay, for updating interval.
	LgdtDelay(400);
	return LGDT_ERR_NO;
#endif	// #ifdef VSB_PLL_SWEEP
}

LgdtReturn_t LgdtCheckStatusQAM()
{
	return LGDT_ERR_NO;
}

/////////////////////////////////////////////////////////////////////////////
//
//  Name: LgdtCheckVersion
//
//  Description:    Shows API version.
//
//  Parameters:     (LgdtVersion_s*)pVersion
//
//  Returns:        (LgdtReturn_t)
//
//  Revision History:
//
//    Date		Author	Description
//  -------------------------------------------------------------------------
//   07-01-2006	pyjung	Original
//
/////////////////////////////////////////////////////////////////////////////
LgdtReturn_t LgdtCheckVersion(LgdtVersion_s *pVersion)
{
	pVersion->Major = LGDT_VERSION_MAJOR;
	pVersion->Minor = LGDT_VERSION_MINOR;
	pVersion->Custom= LGDT_VERSION_CUSTOM;

	pVersion->Tuner = cfgTuner.modelTuner;

	return LGDT_ERR_NO;
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//																		   //
//							Init Functions								   //
//																		   //
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//

/////////////////////////////////////////////////////////////////////////////
//
//  Name :			LgdtInitFAT
//
//  Description :	Initializes among 8VSB, 64QAM or 256QAM
//
//  Parameters :	(LgdtOperMode_t)modeOper - Selects an Operation Mode.
//					(bool)IsExtAGC - Is External AGC used or not used.
//
//  Returns :		(LgdtReturn_t)
//
//  Revision History:
//
//    Date		Author	Description
//  -------------------------------------------------------------------------
//   07-01-2006	pyjung	Original
//
/////////////////////////////////////////////////////////////////////////////
LgdtReturn_t LgdtInitVSB(void);
LgdtReturn_t LgdtInitQAM(LgdtOperMode_t modeOper);
LgdtReturn_t LgdtInitQAM_ExternAGC(LgdtOperMode_t mode);
LgdtReturn_t LgdtInitVSB_ExternAGC(void);

LgdtReturn_t LgdtInitFAT(LgdtOperMode_t modeOper, bool IsExtAGC)
{
	LgdtReturn_t nRet = LGDT_ERR_NO;

	if (IsExtAGC)
	{
		switch(modeOper)	/*---- When External AGC is used : Manaul AGC	-----*/
		{
			case LGDT_QAM64:
			case LGDT_QAM256:
				nRet = LgdtInitQAM_ExternAGC(modeOper);		//	Check AGC mode
				if (nRet != LGDT_ERR_NO)
				{
					nRet = (LgdtReturn_t)(nRet|LGDT_ERR_INIT);
				}
				break;
			case LGDT_VSB:
				nRet = LgdtInitVSB_ExternAGC();
				if (nRet != LGDT_ERR_NO)
				{
					nRet = (LgdtReturn_t)(nRet|LGDT_ERR_INIT);
				}
				break;
			default:
				nRet = (LgdtReturn_t)(LGDT_ERR_INIT|LGDT_ERR_PARAM);
				break;
		}
	}
	else
	{
		switch(modeOper)	/*---- When Internal AGC is used : Automatic AGC	-----*/
		{
			case LGDT_QAM64:
			case LGDT_QAM256:
				nRet = LgdtInitQAM(modeOper);
				if (nRet != LGDT_ERR_NO)
				{
					nRet = (LgdtReturn_t)(nRet|LGDT_ERR_INIT);
				}		
				break;
			case LGDT_VSB:
				nRet = LgdtInitVSB();
				if (nRet != LGDT_ERR_NO)
				{
					nRet = (LgdtReturn_t)(nRet|LGDT_ERR_INIT);
				}
				break;
			default:
				nRet = (LgdtReturn_t)(LGDT_ERR_INIT|LGDT_ERR_PARAM);
				break;
		}
	}

	/*----- nSFTRST						-----*/
	nRet = LgdtResetFAT();
	if (nRet != LGDT_ERR_NO)
		return nRet;
	return nRet;
}

LgdtReturn_t LgdtInitVSB(void)
{
	DATA08		 pData;
	LgdtReturn_t nRet;

	//	1) OPERMODE[1:0] <-- '11'b
	nRet = LgdtCheckRegi(0x0000, &pData);
	if(nRet != LGDT_ERR_NO)
		return (LgdtReturn_t)(nRet|LGDT_ERR_INIT);

	pData &= 0xFC;
	pData |= 0x03;

	nRet = LgdtAlterRegi(0x0000, pData);
	if(nRet != LGDT_ERR_NO)
		return (LgdtReturn_t)(nRet|LGDT_ERR_INIT);

	//	2) DAGCREF[15:0] <-- 0x32C4 (@0x0012,0x0013)
	nRet = LgdtAlterRegi(0x0012, 0x32);
	if (nRet != LGDT_ERR_NO)
		return (LgdtReturn_t)(nRet|LGDT_ERR_INIT);

	nRet = LgdtAlterRegi(0x0013, 0xC4);
	if (nRet != LGDT_ERR_NO)
		return (LgdtReturn_t)(nRet|LGDT_ERR_INIT);

	// 3) LOCKDTEN  <- '0' (@0x0314[1])
	nRet = LgdtAlterRegi(0x0314, 0xE1);
	if(nRet != LGDT_ERR_NO)
		return (LgdtReturn_t)(nRet|LGDT_ERR_INIT);

	// 4) IFBW[15:0] <-- 0x8000 (@0x0308,0x0309)
	nRet = LgdtAlterRegi(0x0308, 0x80);
	if(nRet != LGDT_ERR_NO)
		return (LgdtReturn_t)(nRet|LGDT_ERR_INIT);

	nRet = LgdtAlterRegi(0x0309, 0x00);
	if(nRet != LGDT_ERR_NO)
		return (LgdtReturn_t)(nRet|LGDT_ERR_INIT);

	// 5) ACQEN <- '0'b (@0x030C[2])
	nRet = LgdtAlterRegi(0x030C, 0x30);
	if(nRet != LGDT_ERR_NO)
		return (LgdtReturn_t)(nRet|LGDT_ERR_INIT);

	// 6) QAMFBEXT <- '0'b (@0x043F[2])
	nRet = LgdtAlterRegi(0x043F, 0xAA);
	if(nRet != LGDT_ERR_NO)
		return (LgdtReturn_t)(nRet|LGDT_ERR_INIT);

#ifdef	VSB_PLL_SWEEP
	nRet = LgdtAlterRegi(0x012A, 0xC9);
	if(nRet != LGDT_ERR_NO)
		return (LgdtReturn_t)(nRet|LGDT_ERR_INIT);
#endif
	// 7) GCONCIR <- '0'b (@0x0116[7]
	nRet = LgdtCheckRegi(0x0116, &pData);
	if(nRet != LGDT_ERR_NO)
		return (LgdtReturn_t)(nRet|LGDT_ERR_INIT);

	pData &= 0x7F;

	nRet = LgdtAlterRegi(0x0116, pData);
	if(nRet != LGDT_ERR_NO)
		return (LgdtReturn_t)(nRet|LGDT_ERR_INIT);

	// 8) Reset
	nRet = LgdtCheckRegi(0x0002, &pData);
	if(nRet != LGDT_ERR_NO)
		return (LgdtReturn_t)(nRet|LGDT_ERR_INIT);

	pData &= 0xFE;
	
	nRet = LgdtAlterRegi(0x0002, pData);
	if(nRet != LGDT_ERR_NO)
		return (LgdtReturn_t)(nRet|LGDT_ERR_INIT);

	LgdtDelay((DATA16)20);       // reset delay

	nRet = LgdtCheckRegi(0x0002, &pData);
	if(nRet != LGDT_ERR_NO)
		return (LgdtReturn_t)(nRet|LGDT_ERR_INIT);

	pData &= 0xFE;
	pData |= 0x01;
	
	nRet = LgdtAlterRegi(0x0002, pData);
	if(nRet != LGDT_ERR_NO)
		return (LgdtReturn_t)(nRet|LGDT_ERR_INIT);

	//printf("LgdtInitVSB = %d\n", nRet);
	return LGDT_ERR_NO;
}

LgdtReturn_t LgdtInitVSB_ExternAGC(void)
{
	DATA08		 pData;
	LgdtReturn_t nRet;


	//	1) OPERMODE[1:0] <-- '11'b
	nRet = LgdtCheckRegi(0x0000, &pData);
	if(nRet != LGDT_ERR_NO)
		return (LgdtReturn_t)(nRet|LGDT_ERR_INIT);

	pData &= 0xFC;
	pData |= 0x03;

	nRet = LgdtAlterRegi(0x0000, pData);
	if(nRet != LGDT_ERR_NO)
		return (LgdtReturn_t)(nRet|LGDT_ERR_INIT);

	//	2) DAGCREF[15:0] <-- 0x32C4 (@0x0012,0x0013)
	nRet = LgdtAlterRegi(0x0012, 0x32);
	if (nRet != LGDT_ERR_NO)
		return (LgdtReturn_t)(nRet|LGDT_ERR_INIT);

	nRet = LgdtAlterRegi(0x0013, 0xC4);
	if (nRet != LGDT_ERR_NO)
		return (LgdtReturn_t)(nRet|LGDT_ERR_INIT);

	//	3) LOCKDTEN  <- '0' (@0x0314[1])
	nRet = LgdtAlterRegi(0x0314, 0xE1);
	if(nRet != LGDT_ERR_NO)
		return (LgdtReturn_t)(nRet|LGDT_ERR_INIT);

	//  4) IFBW[15:0] <-- 0x8000 (@0x0308,0x0309)
	nRet = LgdtAlterRegi(0x0308, 0x80);
	if(nRet != LGDT_ERR_NO)
		return (LgdtReturn_t)(nRet|LGDT_ERR_INIT);

	nRet = LgdtAlterRegi(0x0309, 0x00);
	if(nRet != LGDT_ERR_NO)
		return (LgdtReturn_t)(nRet|LGDT_ERR_INIT);

	//  5) ACQEN <- '0'b (@0x030C[2])
	nRet = LgdtAlterRegi(0x030C, 0x30);
	if(nRet != LGDT_ERR_NO)
		return (LgdtReturn_t)(nRet|LGDT_ERR_INIT);

	// 6) QAMFBEXT <- '0'b (@0x043F[2])
	nRet = LgdtAlterRegi(0x043F, 0xAA);
	if(nRet != LGDT_ERR_NO)
		return (LgdtReturn_t)(nRet|LGDT_ERR_INIT);

	/////////////////////////////////////////
	// for using RF AGC
	//  7) REFD[11:0] <- 0x04C0 (@0x0302, 0x0303)
	nRet = LgdtAlterRegi(0x0302, 0x04);
	if(nRet != LGDT_ERR_NO)
		return (LgdtReturn_t)(nRet|LGDT_ERR_INIT);

	nRet = LgdtAlterRegi(0x0303, 0xC0);
	if(nRet != LGDT_ERR_NO)
		return (LgdtReturn_t)(nRet|LGDT_ERR_INIT);

	//  8) RFBW[15:0] <-- 0x8000 (@0x0306,0x0307)
	nRet = LgdtAlterRegi(0x0306, 0x80);
	if(nRet != LGDT_ERR_NO)
		return (LgdtReturn_t)(nRet|LGDT_ERR_INIT);

	nRet = LgdtAlterRegi(0x0307, 0x00);
	if(nRet != LGDT_ERR_NO)
		return (LgdtReturn_t)(nRet|LGDT_ERR_INIT);

#ifdef	VSB_PLL_SWEEP
	nRet = LgdtAlterRegi(0x012A, 0xC9);
	if(nRet != LGDT_ERR_NO)
		return (LgdtReturn_t)(nRet|LGDT_ERR_INIT);
#endif
	// 9) GCONCIR <- '0'b (@0x0116[7]
	nRet = LgdtCheckRegi(0x0116, &pData);
	if(nRet != LGDT_ERR_NO)
		return (LgdtReturn_t)(nRet|LGDT_ERR_INIT);

	pData &= 0x7F;

	nRet = LgdtAlterRegi(0x0116, pData);
	if(nRet != LGDT_ERR_NO)
		return (LgdtReturn_t)(nRet|LGDT_ERR_INIT);

	// 10) Reset
	nRet = LgdtCheckRegi(0x0002, &pData);
	if(nRet != LGDT_ERR_NO)
		return (LgdtReturn_t)(nRet|LGDT_ERR_INIT);

	pData &= 0xFE;
	
	nRet = LgdtAlterRegi(0x0002, pData);
	if(nRet != LGDT_ERR_NO)
		return (LgdtReturn_t)(nRet|LGDT_ERR_INIT);

	LgdtDelay((DATA16)20);       // reset delay

	nRet = LgdtCheckRegi(0x0002, &pData);
	if(nRet != LGDT_ERR_NO)
		return (LgdtReturn_t)(nRet|LGDT_ERR_INIT);

	pData &= 0xFE;
	pData |= 0x01;
	
	nRet = LgdtAlterRegi(0x0002, pData);
	if(nRet != LGDT_ERR_NO)
		return (LgdtReturn_t)(nRet|LGDT_ERR_INIT);

	return LGDT_ERR_NO;

}

LgdtReturn_t LgdtInitQAM(LgdtOperMode_t mode)
{
	DATA08		 pData;
	LgdtReturn_t nRet;

	/*-----	1) OPERMODE[1:0]			-----*/
	switch(mode)
	{
		case LGDT_QAM64:
			nRet = LgdtCheckRegi(0x0000, &pData);
			if(nRet != LGDT_ERR_NO)
				return (LgdtReturn_t)(nRet|LGDT_ERR_INIT);

			pData &= 0xFC;

			nRet = LgdtAlterRegi(0x0000, 0x00);
			if (nRet != LGDT_ERR_NO)
				return nRet;

			//	2) DAGCREF[15:0] <-- 0x2A00 (@0x0012,0x0013)
			nRet = LgdtAlterRegi(0x0012, 0x2A);
			if (nRet != LGDT_ERR_NO)
				return nRet;

			nRet = LgdtAlterRegi(0x0013, 0x00);
			if (nRet != LGDT_ERR_NO)
				return nRet;

			break;
		case LGDT_QAM256:
			nRet = LgdtCheckRegi(0x0000, &pData);
			if(nRet != LGDT_ERR_NO)
				return (LgdtReturn_t)(nRet|LGDT_ERR_INIT);

			pData &= 0xFC;
			pData |= 0x01;

			nRet = LgdtAlterRegi(0x0000, 0x01);
			if (nRet != LGDT_ERR_NO)
				return nRet;

			//	2) DAGCREF[15:0] <-- 0x2A80 (@0x0012,0x0013)
			nRet = LgdtAlterRegi(0x0012, 0x2A);
			if (nRet != LGDT_ERR_NO)
				return nRet;

			nRet = LgdtAlterRegi(0x0013, 0x80);
			if (nRet != LGDT_ERR_NO)
				return nRet;
		
			break;
		default:

			break;
	}

	// 3) LOCKDTEN  <- '1' (@0x0314[1])
	nRet = LgdtAlterRegi(0x0314, 0xE3);
	if(nRet != LGDT_ERR_NO)
		return (LgdtReturn_t)(nRet|LGDT_ERR_INIT);

	//  4) IFBW[15:0] <-- 0x8888 (@0x0308,0x0309)
	nRet = LgdtAlterRegi(0x0308, 0x88);
	if(nRet != LGDT_ERR_NO)
		return (LgdtReturn_t)(nRet|LGDT_ERR_INIT);

	nRet = LgdtAlterRegi(0x0309, 0x88);
	if(nRet != LGDT_ERR_NO)
		return (LgdtReturn_t)(nRet|LGDT_ERR_INIT);

	//  5) ACQEN <- '1'b (@0x030C[2])
	nRet = LgdtAlterRegi(0x030C, 0x34);
	if(nRet != LGDT_ERR_NO)
		return (LgdtReturn_t)(nRet|LGDT_ERR_INIT);

	// 6) QAMFBEXT <- '1'b (@0x043F[2])
	nRet = LgdtAlterRegi(0x043F, 0xAE);
	if(nRet != LGDT_ERR_NO)
		return (LgdtReturn_t)(nRet|LGDT_ERR_INIT);

	// 6) reset
	nRet = LgdtCheckRegi(0x0002, &pData);
	if(nRet != LGDT_ERR_NO)
		return (LgdtReturn_t)(nRet|LGDT_ERR_INIT);

	pData &= 0xFE;
	
	nRet = LgdtAlterRegi(0x0002, pData);
	if(nRet != LGDT_ERR_NO)
		return (LgdtReturn_t)(nRet|LGDT_ERR_INIT);

	LgdtDelay((DATA16)20);       // reset delay

	nRet = LgdtCheckRegi(0x0002, &pData);
	if(nRet != LGDT_ERR_NO)
		return (LgdtReturn_t)(nRet|LGDT_ERR_INIT);

	pData &= 0xFE;
	pData |= 0x01;
	
	nRet = LgdtAlterRegi(0x0002, pData);
	if(nRet != LGDT_ERR_NO)
		return (LgdtReturn_t)(nRet|LGDT_ERR_INIT);

	return LGDT_ERR_NO;
}

LgdtReturn_t LgdtInitQAM_ExternAGC(LgdtOperMode_t mode)
{
	DATA08		 pData;
	LgdtReturn_t nRet;

	/*-----	1) OPERMODE[1:0]			-----*/
	switch(mode)
	{
		case LGDT_QAM64:
			nRet = LgdtCheckRegi(0x0000, &pData);
			if(nRet != LGDT_ERR_NO)
				return (LgdtReturn_t)(nRet|LGDT_ERR_INIT);

			pData &= 0xFC;

			nRet = LgdtAlterRegi(0x0000, 0x00);
			if (nRet != LGDT_ERR_NO)
				return nRet;

			//	2) DAGCREF[15:0] <-- 0x2A00 (@0x0012,0x0013)
			nRet = LgdtAlterRegi(0x0012, 0x2A);
			if (nRet != LGDT_ERR_NO)
				return nRet;

			nRet = LgdtAlterRegi(0x0013, 0x00);
			if (nRet != LGDT_ERR_NO)
				return nRet;

			break;
		case LGDT_QAM256:
			nRet = LgdtCheckRegi(0x0000, &pData);
			if(nRet != LGDT_ERR_NO)
				return (LgdtReturn_t)(nRet|LGDT_ERR_INIT);

			pData &= 0xFC;
			pData |= 0x01;

			nRet = LgdtAlterRegi(0x0000, 0x01);
			if (nRet != LGDT_ERR_NO)
				return nRet;

			//	2) DAGCREF[15:0] <-- 0x2A80 (@0x0012,0x0013)
			nRet = LgdtAlterRegi(0x0012, 0x2A);
			if (nRet != LGDT_ERR_NO)
				return nRet;

			nRet = LgdtAlterRegi(0x0013, 0x80);
			if (nRet != LGDT_ERR_NO)
				return nRet;
		
			break;
		default:

			break;
	}

	// 3) LOCKDTEN  <- '1' (@0x0314[1])
	nRet = LgdtAlterRegi(0x0314, 0xE3);
	if(nRet != LGDT_ERR_NO)
		return (LgdtReturn_t)(nRet|LGDT_ERR_INIT);

	//  4) IFBW[15:0] <-- 0x8888 (@0x0308,0x0309)
	nRet = LgdtAlterRegi(0x0308, 0x88);
	if(nRet != LGDT_ERR_NO)
		return (LgdtReturn_t)(nRet|LGDT_ERR_INIT);

	nRet = LgdtAlterRegi(0x0309, 0x88);
	if(nRet != LGDT_ERR_NO)
		return (LgdtReturn_t)(nRet|LGDT_ERR_INIT);

	//  5) ACQEN <- '1'b (@0x030C[2])
	nRet = LgdtAlterRegi(0x030C, 0x34);
	if(nRet != LGDT_ERR_NO)
		return (LgdtReturn_t)(nRet|LGDT_ERR_INIT);

	// 6) QAMFBEXT <- '1'b (@0x043F[2])
	nRet = LgdtAlterRegi(0x043F, 0xAE);
	if(nRet != LGDT_ERR_NO)
		return (LgdtReturn_t)(nRet|LGDT_ERR_INIT);

	/////////////////////////////////////////
	// for using RF AGC
	//  7) REFD[11:0] <- 0x046B (@0x0302, 0x0303)
	nRet = LgdtAlterRegi(0x0302, 0x04);
	if(nRet != LGDT_ERR_NO)
		return (LgdtReturn_t)(nRet|LGDT_ERR_INIT);

	nRet = LgdtAlterRegi(0x0303, 0x6B);
	if(nRet != LGDT_ERR_NO)
		return (LgdtReturn_t)(nRet|LGDT_ERR_INIT);

	//  8) RFBW[15:0] <-- 0x8889 (@0x0306,0x0307)
	nRet = LgdtAlterRegi(0x0306, 0x88);
	if(nRet != LGDT_ERR_NO)
		return (LgdtReturn_t)(nRet|LGDT_ERR_INIT);

	nRet = LgdtAlterRegi(0x0307, 0x89);
	if(nRet != LGDT_ERR_NO)
		return (LgdtReturn_t)(nRet|LGDT_ERR_INIT);

	// 9) reset
	nRet = LgdtCheckRegi(0x0002, &pData);
	if(nRet != LGDT_ERR_NO)
		return (LgdtReturn_t)(nRet|LGDT_ERR_INIT);

	pData &= 0xFE;
	
	nRet = LgdtAlterRegi(0x0002, pData);
	if(nRet != LGDT_ERR_NO)
		return (LgdtReturn_t)(nRet|LGDT_ERR_INIT);

	LgdtDelay((DATA16)20);       // reset delay

	nRet = LgdtCheckRegi(0x0002, &pData);
	if(nRet != LGDT_ERR_NO)
		return (LgdtReturn_t)(nRet|LGDT_ERR_INIT);

	pData &= 0xFE;
	pData |= 0x01;
	
	nRet = LgdtAlterRegi(0x0002, pData);
	if(nRet != LGDT_ERR_NO)
		return (LgdtReturn_t)(nRet|LGDT_ERR_INIT);

	return LGDT_ERR_NO;
}


//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//																		   //
//					Register Access Functions							   //
//																		   //
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//

/////////////////////////////////////////////////////////////////////////////
//
//  Name :			LgdtCheckRegi
//
//  Description :	Reads registers from LGDT3305
//
//  Parameters :	(DATA16)nAddr - The register address to read.
//					(DATA08*)pData - The data to be read.
//
//  Returns :		(LgdtReturn_t)
//
//  Revision History:
//
//    Date		Author	Description
//  -------------------------------------------------------------------------
//   07-01-2006	pyjung	Original
//
/////////////////////////////////////////////////////////////////////////////
LgdtReturn_t LgdtCheckRegi(DATA16 nAddr, DATA08* pData)
{
	DATA08			nSize = 1;
	DATA08			nData;
	LgdtReturn_t	nRet;

	nRet = (LgdtReturn_t)Lgdt_I2C_Read(ID_LGDT3305, nAddr, &nData, nSize);
	if (nRet != LGDT_ERR_NO) 
		return LGDT_ERR_I2C;	

	*pData = nData;
	return LGDT_ERR_NO;
}

/////////////////////////////////////////////////////////////////////////////
//
//  Name :			LgdtAlterRegi
//
//  Description :	write data to a register of LGDT3305
//
//  Parameters :	(DATA16)nAddr - The register address to write.
//					(DATA08)nData - The data to write.
//
//  Returns :		(LgdtReturn_t)
//
//  Revision History:
//
//    Date		Author	Description
//  -------------------------------------------------------------------------
//   07-01-2006	pyjung	Original
//
/////////////////////////////////////////////////////////////////////////////
LgdtReturn_t LgdtAlterRegi(DATA16 nAddr, DATA08 nData)
{
	//DATA08			nTemp;
	DATA08			nDataWrite[4];
	LgdtReturn_t	nRet = LGDT_ERR_NO;

//	nDataWrite[0] = ID_LGDT3305;

	nDataWrite[0]=(nAddr >> 8) & 0xFF;		  /* for 16 bits sub addresses */
	nDataWrite[1]=nAddr & 0xFF;
	nDataWrite[2]=nData; 					  /* for 8 bits sub addresses */  

	nRet = (LgdtReturn_t)Lgdt_I2C_Write(ID_LGDT3305,nDataWrite, 3);
	if (nRet != LGDT_ERR_NO)					
		nRet = (LgdtReturn_t)(nRet|LGDT_ERR_I2C);

	return (LgdtReturn_t)nRet;
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//																		   //
//							Tuner Functions								   //
//																		   //
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
LgdtReturn_t LgdtTunerFreq(DATA_FLOAT nFreq, bool IsExternalAGC, LgdtOperMode_t modeOper);

/////////////////////////////////////////////////////////////////////////////
//
//  Name :			LgdtTunerAFC
//
//  Description :	Automatic Frequency Control.
//					Tune the tuner with shifted frequencies from a given one.
//
//  Parameters :	void
//
//  Returns :		(LgdtReturn_t)
//
//  Dependencies:	LgdtInitAFC() should be called before this is called.
//					(int)gCountAFC - Global Variables used in this function.
//					(DATA_FLOAT)gFreqCur - A given frequency. 
//
//
//  Revision History:
//
//    Date		Author	Description
//  -------------------------------------------------------------------------
//   07-01-2006	pyjung	Original
//
/////////////////////////////////////////////////////////////////////////////
LgdtReturn_t LgdtTunerAFC()
{
	DATA_FLOAT		freq = gFreqCur;
	DATA_FLOAT		FREQ_STEP = (DATA_FLOAT)0.1;
	LgdtReturn_t	nRet;

	if ( (gFreqCur == 0) || (gCountAFC < 0) )
	{
		return LGDT_ERR_NO;		/*----- Not ready to check	-----*/
	}

	switch(gCountAFC)
	{
		case 1:
			freq -= (FREQ_STEP * 1);
			gCountAFC++;
			break;
		case 2:
			freq -= (FREQ_STEP * 2);
			gCountAFC++;
			break;
		case 3:
			freq += (FREQ_STEP * 1);
			gCountAFC++;
			break;
		case 4:
			freq += (FREQ_STEP * 2);
			gCountAFC++;
			break;
		default:
			freq = (DATA_FLOAT)gFreqCur;
			gCountAFC++;
			break;
	}

	if (gCountAFC > 5)
		gCountAFC = 0;
	
	nRet = (LgdtReturn_t)LgdtTunerFreq(freq, false, LGDT_VSB);	
	if (nRet != LGDT_ERR_NO)
		return (LgdtReturn_t)(nRet|LGDT_ERR_TUNER);

	/*----- Core Reset : This is integral to reset the demod after the tuner setting		-----*/
	nRet = LgdtResetFAT();
	if (nRet != LGDT_ERR_NO)
		return nRet;

	return LGDT_ERR_NO;
}

/////////////////////////////////////////////////////////////////////////////
//
//  Name :			LgdtTunerInitAFC
//
//  Description :	Initialize for AFC Routine.
//
//  Parameters :	(DATA_FLOAT)nFreq
//
//  Returns :		(LgdtReturn_t)
//
//  Dependencies:   (int)gCountAFC - Global Variables used in this function.
//					(DATA_FLOAT)gFreqCur - A given frequency. 
//
//  Referenced:		LgdtInitTuner()
//
//  Revision History:
//
//    Date		Author	Description
//  -------------------------------------------------------------------------
//   07-01-2006	pyjung	Original
//
/////////////////////////////////////////////////////////////////////////////
LgdtReturn_t LgdtTunerInitAFC(DATA_FLOAT nFreq)
{
	gFreqCur = nFreq;
	gCountAFC = 0;

	return LGDT_ERR_NO;
}

/////////////////////////////////////////////////////////////////////////////
//
//  Name :			LgdtInitTuner
//
//  Description :	Tuner init includes tuner tuning and AFC init.
//
//  Parameters :	(DATA_FLOAT)nFreq - Frequency to tune tuner.
//					(bool)IsExtAGC - Is External AGC used or not used.
//					(LgdtOperMode_t)modeOper - Operation mode.
//
//  Returns :		(LgdtReturn_t)
//
//  Revision History:
//
//    Date		Author	Description
//  -------------------------------------------------------------------------
//   07-01-2006	pyjung	Original
//   02-18-2008	jywon	Modified
/////////////////////////////////////////////////////////////////////////////
LgdtReturn_t LgdtTunerInit(DATA_FLOAT nFreq, bool IsExtAGC, LgdtOperMode_t modeOper)
{
	LgdtReturn_t nRet;

	cfgTuner.m_RF = nFreq;
	cfgTuner.m_Mode = (int)modeOper;
	cfgTuner.m_IsExternalAGC = (IsExtAGC)?1:0;

	/*----- for AFC Routine Init	-----*/
	nRet = LgdtTunerInitAFC((DATA_FLOAT)nFreq);
	if (nRet != LGDT_ERR_NO)
		return (LgdtReturn_t)(nRet|LGDT_ERR_TUNER);

	//LgdtRepeaterI2C(ON);

	//nmi_tnr_init();

	//LgdtRepeaterI2C(OFF);

	LgdtRepeaterI2C(ON);

	nRet=LgdtSetManualIfandSpectrum(true, false);
	if(nRet!=LGDT_ERR_NO){
		LgdtRepeaterI2C(OFF);
		//SYS_Debug("LgdtSetManualIfandSpectrum  Fail\n");
		return LGDT_ERR_INIT;
	}
	LgdtRepeaterI2C(OFF);
	
	/*----- Call the tuner setup function, which is implemented in Tuner.cpp	-----*/
	nRet = (LgdtReturn_t)LgdtTunerFreq(nFreq, IsExtAGC, modeOper);
	if (nRet != LGDT_ERR_NO)
		return (LgdtReturn_t)(nRet|LGDT_ERR_TUNER);

	return LGDT_ERR_NO;
}

LgdtReturn_t LgdtTunerFreq(DATA_FLOAT nFreq, bool IsExternalAGC, LgdtOperMode_t modeOper)
{
	LgdtReturn_t nRet = LGDT_ERR_NO;

	cfgTuner.m_RF = nFreq;
	cfgTuner.m_Mode = (int)modeOper;
	cfgTuner.m_IsExternalAGC = (IsExternalAGC)?1:0;

	//LgdtRepeaterI2C(ON);

	//NMI120_Tune((unsigned long)(nFreq*1000000),0);

	//MXL603_Tune((int)(nFreq)*1000,modeOper);
	//MXL603_Tune((int)(nFreq),modeOper); // MHz

	//LgdtRepeaterI2C(OFF);

	if(nRet != LGDT_ERR_NO)
		return LGDT_ERR_TUNER;

	/*----- FAT Reset : This is essential to reset the demod after the tuner setting	-----*/
	nRet = LgdtResetFAT();
	if (nRet != LGDT_ERR_NO)
		return nRet;

	return (LgdtReturn_t)0;
}

/////////////////////////////////////////////////////////////////////////////
//
//  Name :			LgdtTunerSetPLL
//
//  Description :	Tuner tuner tuning 
//
//  Parameters :	(float)freq - Frequency to tune tuner.
//					(bool)IsExtAGC - Is External AGC used or not used.
//					(LgdtOperMode_t)mode - Operation mode.
//
//  Returns :		(LgdtReturn_t)
//
//  Revision History:
//
//    Date		Author	Description
//  -------------------------------------------------------------------------
//   01-01-2008	jywon	Original
//
/////////////////////////////////////////////////////////////////////////////
LgdtReturn_t LgdtTunerSetPLL(float freq, bool IsExternalAGC, LgdtOperMode_t mode)
{
	LgdtReturn_t nRet;

	/*----- for AFC Routine Init	-----*/
	nRet = LgdtTunerInitAFC((DATA_FLOAT)freq);
	if (nRet != LGDT_ERR_NO)
		return (LgdtReturn_t)(nRet|LGDT_ERR_TUNER);

	/*----- Call the tuner setup function, which is implemented in Tuner.cpp	-----*/
	nRet = (LgdtReturn_t)LgdtTunerFreq(freq, IsExternalAGC, mode);
	if (nRet != LGDT_ERR_NO)
		return (LgdtReturn_t)(nRet|LGDT_ERR_TUNER);

	return (LgdtReturn_t)LGDT_ERR_NO;
}

LgdtReturn_t LgdtTunerConfigSet(LgdtTunerConfig_t m_cfg)
{
	cfgTuner.m_bmanualIF = m_cfg.m_bmanualIF;
//	cfgTuner.m_RF = m_cfg.m_RF;
	cfgTuner.m_IF = m_cfg.m_IF;
//	cfgTuner.m_IF_level = m_cfg.m_IF_level;
	cfgTuner.m_LPF = (m_cfg.m_LPF&0x1F);

	return LGDT_ERR_NO;
}


/////////////////////////////////////////////////////////////////////////////
//
//  Name :			LgdtSetManualIfandSpectrum
//
//  Description :	Set If frequency, Spectrum, digital SAW filter on/off  
//
//  Parameters :	(bool)complex_multiplier
//						   true : complex multiplier
//						   false : real multiplier
//               	(bool)vsb_inversion
//                         true : vsb spectrum inverted
//                         false : vsb spectrum non-inverted
//
//  Returns :		(LgdtReturn_t)
//
//  Revision History:
//
//    Date		Author	Description
//  -------------------------------------------------------------------------
//   01-01-2008	jywon	Original
//
/////////////////////////////////////////////////////////////////////////////
LgdtReturn_t LgdtSetManualIfandSpectrum(bool complex_multiplier, bool vsb_inversion)
{
	int n_nco = 0;
	double if_freq=0.0, nco_freq=0.0;
	double pilot_offset = 2.690559;

	LgdtReturn_t nRet = LGDT_ERR_NO;
	DATA08		pData;
	DATA08		n_nco_b1, n_nco_b2, n_nco_b3, n_nco_b4;

	if(complex_multiplier)		// complex multiplier
	{
		// Downmode set - complex multiplier
		nRet = LgdtCheckRegi(0x0007, &pData);
		if(nRet != LGDT_ERR_NO)
			return LGDT_ERR_INIT;
		pData |= 0x40;
		nRet = LgdtAlterRegi(0x0007, pData);
		if(nRet != LGDT_ERR_NO)
			return LGDT_ERR_INIT;

		switch(cfgTuner.modelTuner)
		{
			case LGDT_TUNER_CHIP_NXP18271:
			case LGDT_TUNER_CHIP_XC5000:
			case LGDT_TUNER_CHIP_NMI120:
			case LGDT_TUNER_CHIP_MXL603:	
				// NCO frequency set
				if(cfgTuner.m_Mode == LGDT_QAM64 || cfgTuner.m_Mode == LGDT_QAM256){
						// QAM mode
					if(cfgTuner.m_bmanualIF)
						if_freq=cfgTuner.m_IF;
					else{
						if(cfgTuner.modelTuner==LGDT_TUNER_CHIP_NXP18271)
							if_freq=4.0;
						else
							if_freq=6.0;
					}
				
					nco_freq = if_freq/25.0*1073741824.0;			// NCOCTF = (fc/fs)*2^30
					n_nco = (int)nco_freq;
					n_nco_b4 = (n_nco>>24)&0x3f;
					n_nco_b4 |= 0x40;
					n_nco_b3 = (n_nco>>16)&0xff;
					n_nco_b2 = (n_nco>>8)&0xff;
					n_nco_b1 = n_nco&0xff;
			
					// set spectrum 
					if(vsb_inversion)
						LgdtAlterSpectrumFAT(LGDT_NOT_INVERTED);
					else
						LgdtAlterSpectrumFAT(LGDT_INVERTED);
				}else{		// VSB mode
					if(cfgTuner.m_bmanualIF)
						if_freq=cfgTuner.m_IF;
					else{
						if(cfgTuner.modelTuner==LGDT_TUNER_CHIP_NXP18271)
							if_freq=3.25;
						else
							if_freq=6.0;
					}
			
					// for spectrum inversion
					nRet = LgdtCheckRegi(0x0126, &pData);
					if(nRet != LGDT_ERR_NO)
						return LGDT_ERR_INIT;
			
					if(vsb_inversion){			// VSB spectrum inversion
						pData |= 0x80;
						nco_freq = -(if_freq+pilot_offset)/25.0*67108864.0; 		// NCOCTF = (fc/fs)*2^26
					}else{						// VSB spectrum non-inversion
						pData &= 0x7F;
						nco_freq = (if_freq-pilot_offset)/25.0*67108864.0;			// NCOCTF = (fc/fs)*2^26
					}
					nRet = LgdtAlterRegi(0x0126, pData);
					if(nRet != LGDT_ERR_NO)
						return LGDT_ERR_INIT;
			
					n_nco = (int)nco_freq;
					n_nco_b4 = (n_nco>>24)&0x3f;
					n_nco_b4 |= 0x40;
					n_nco_b3 = (n_nco>>16)&0xff;
					n_nco_b2 = (n_nco>>8)&0xff;
					n_nco_b1 = n_nco&0xff;
				}
			
				if(if_freq >5.8 && if_freq <6.2){
					// DSAWBPY, DAGCBPY enable
					nRet = LgdtCheckRegi(0x000D, &pData);
					if(nRet != LGDT_ERR_NO)
						return LGDT_ERR_INIT;
			
					pData &= 0x3F;
					nRet = LgdtAlterRegi(0x000D, pData);
					if(nRet != LGDT_ERR_NO)
						return LGDT_ERR_INIT;
				}else{
					// DSAWBPY, DAGCBPY disable
					nRet = LgdtCheckRegi(0x000D, &pData);
					if(nRet != LGDT_ERR_NO)
						return LGDT_ERR_INIT;
			
					pData |= 0xC0;
					nRet = LgdtAlterRegi(0x000D, pData);
					if(nRet != LGDT_ERR_NO)
						return LGDT_ERR_INIT;
				}
				break;
		
			default:
				// NCO frequency set
				if(cfgTuner.m_Mode == LGDT_QAM64 || cfgTuner.m_Mode == LGDT_QAM256){
							// QAM mode
					// set spectrum 
					if(vsb_inversion)
						LgdtAlterSpectrumFAT(LGDT_NOT_INVERTED);
					else
						LgdtAlterSpectrumFAT(LGDT_INVERTED);

				}else{		// VSB mode
					// spectrum non-inversion
					nRet = LgdtCheckRegi(0x0126, &pData);
					if(nRet != LGDT_ERR_NO)
						return LGDT_ERR_INIT;

					if(vsb_inversion)
						pData |= 0x80;
					else
						pData &= 0x7F;
					nRet = LgdtAlterRegi(0x0126, pData);
					if(nRet != LGDT_ERR_NO)
						return LGDT_ERR_INIT;
				}

				n_nco_b4 = 0x80;
				n_nco_b3 = 0x00;
				n_nco_b2 = 0x00;
				n_nco_b1 = 0x00;

				// DSAWBPY, DAGCBPY enable
				nRet = LgdtCheckRegi(0x000D, &pData);
				if(nRet != LGDT_ERR_NO)
					return LGDT_ERR_INIT;

				pData &= 0x3F;
				nRet = LgdtAlterRegi(0x000D, pData);
				if(nRet != LGDT_ERR_NO)
					return LGDT_ERR_INIT;

				break;
		}

		nRet = LgdtAlterRegi(0x0106, n_nco_b4);
		if(nRet != LGDT_ERR_NO)
			return LGDT_ERR_INIT;
		nRet = LgdtAlterRegi(0x0107, n_nco_b3);
		if(nRet != LGDT_ERR_NO)
			return LGDT_ERR_INIT;
		nRet = LgdtAlterRegi(0x0108, n_nco_b2);
		if(nRet != LGDT_ERR_NO)
		return LGDT_ERR_INIT;
		nRet = LgdtAlterRegi(0x0109, n_nco_b1);
		if(nRet != LGDT_ERR_NO)
			return LGDT_ERR_INIT;
	}else{						// real multiplier
		// Downmode set - real multiplier
		nRet = LgdtCheckRegi(0x0007, &pData);
		if(nRet != LGDT_ERR_NO)
			return LGDT_ERR_INIT;
		pData &= 0xBF;
		nRet = LgdtAlterRegi(0x0007, pData);
		if(nRet != LGDT_ERR_NO)
			return LGDT_ERR_INIT;

		// DSAWBPY, DAGCBPY enable
		nRet = LgdtCheckRegi(0x000D, &pData);
		if(nRet != LGDT_ERR_NO)
			return LGDT_ERR_INIT;

		pData &= 0x3F;
		nRet = LgdtAlterRegi(0x000D, pData);
		if(nRet != LGDT_ERR_NO)
			return LGDT_ERR_INIT;

		switch(cfgTuner.modelTuner)
		{
			case LGDT_TUNER_CHIP_NXP18271:
			case LGDT_TUNER_CHIP_XC5000:
			case LGDT_TUNER_CHIP_NMI120:	
			case LGDT_TUNER_CHIP_MXL603:	
				// NCO frequency set
				if(cfgTuner.m_Mode == LGDT_QAM64 || cfgTuner.m_Mode == LGDT_QAM256){
						// QAM mode
					if(cfgTuner.m_bmanualIF)
						if_freq=cfgTuner.m_IF;
					else{
						if(cfgTuner.modelTuner==LGDT_TUNER_CHIP_NXP18271)
							if_freq=4.0;
						else
							if_freq=6.0;
					}
				
					nco_freq = if_freq/25.0*1073741824.0;			// NCOCTF = (fc/fs)*2^30
					n_nco = (int)nco_freq;
					n_nco_b4 = (n_nco>>24)&0x3f;
					n_nco_b4 |= 0x40;
					n_nco_b3 = (n_nco>>16)&0xff;
					n_nco_b2 = (n_nco>>8)&0xff;
					n_nco_b1 = n_nco&0xff;
					// set spectrum 
					LgdtAlterSpectrumFAT(LGDT_NOT_INVERTED);
				}else{		// VSB mode
					if(cfgTuner.m_bmanualIF)
						if_freq=cfgTuner.m_IF;
					else{
						if(cfgTuner.modelTuner==LGDT_TUNER_CHIP_NXP18271)
							if_freq=3.25;
						else
							if_freq=6.0;
					}
			
					// for spectrum inversion
					nRet = LgdtCheckRegi(0x0126, &pData);
					if(nRet != LGDT_ERR_NO)
						return LGDT_ERR_INIT;
			
					if(vsb_inversion){			// VSB spectrum inversion
						pData |= 0x80;
						nco_freq = if_freq/25.0*67108864.0; 		// NCOCTF = (fc/fs)*2^26
					}else{						// VSB spectrum non-inversion
						pData &= 0x7F;
						nco_freq = -if_freq/25.0*67108864.0;			// NCOCTF = (-fc/fs)*2^26
					}
					nRet = LgdtAlterRegi(0x0126, pData);
					if(nRet != LGDT_ERR_NO)
						return LGDT_ERR_INIT;
			
					n_nco = (int)nco_freq;
					n_nco_b4 = (n_nco>>24)&0x3f;
					n_nco_b4 |= 0x40;
					n_nco_b3 = (n_nco>>16)&0xff;
					n_nco_b2 = (n_nco>>8)&0xff;
					n_nco_b1 = n_nco&0xff;
				}
			
				break;
		
			default:
				n_nco_b4 = 0x80;
				n_nco_b3 = 0x00;
				n_nco_b2 = 0x00;
				n_nco_b1 = 0x00;
				break;
		}
		nRet = LgdtAlterRegi(0x0106, n_nco_b4);
		if(nRet != LGDT_ERR_NO)
			return LGDT_ERR_INIT;
		nRet = LgdtAlterRegi(0x0107, n_nco_b3);
		if(nRet != LGDT_ERR_NO)
			return LGDT_ERR_INIT;
		nRet = LgdtAlterRegi(0x0108, n_nco_b2);
		if(nRet != LGDT_ERR_NO)
		return LGDT_ERR_INIT;
		nRet = LgdtAlterRegi(0x0109, n_nco_b1);
		if(nRet != LGDT_ERR_NO)
			return LGDT_ERR_INIT;
	}

	return LGDT_ERR_NO;
}

LgdtReturn_t LgdtCheckRegi_Multi(DATA16 nAddr, DATA08* pData, DATA08 nSize)
{
	int i;
	LgdtReturn_t	nRet;

	nRet = (LgdtReturn_t)Lgdt_I2C_Read(ID_LGDT3305, nAddr, pData, nSize);
	if (nRet != LGDT_ERR_NO) 
		return LGDT_ERR_I2C;	

	//SYS_Debug("\n LgdtCheckRegi_Multi() - addr = 0x%X, data = ", nAddr);
	for(i=0; i<nSize; i++){
		//SYS_Debug("  0x%X  ", *(pData+i));
	}
	//SYS_Debug("\n");

	return LGDT_ERR_NO;
}

int LgdtRun(LgdtOperMode_t modeOper, DATA_FLOAT nFreq)
{
	LgdtReturn_t retVal;

//	DATA08			pData;
//	LgdtReturn_t	nRet;


	//-----     Step 1. Tuner Setup     -----//
	retVal = LgdtTunerInit(nFreq, 0, modeOper);
	printf("LgdtRun Step 1.Tuner Setup %d\n", retVal);
//	if(retVal != LGDT_ERR_NO)
//		return (LgdtReturn_t)retVal;

	//-----     Step 2. Demod Init     -----//
	retVal = LgdtInitFAT(modeOper, false);
	printf("LgdtRun Step 2.Demod Setup %d\n", retVal);
	if(retVal != LGDT_ERR_NO)
		return (LgdtReturn_t)retVal;

	retVal = LgdtAlterMpegMode(LGDT_PARAL);
	LgdtAlterMpegPolarity(1,1);
	printf("LgdtRun LgdtAlterMpegMode %d\n", retVal);
	
//	nRet = LgdtCheckRegi(0x050E, &pData);
//	aprintf("LgdtRun 0x050E =  %02x\n", pData);

	return LGDT_ERR_NO;
}



/*
	input parameter 

	Freq = KHz, ex 474000
	mode = 0x03 : ATSC mode

*/
int LG3305_Tune(int Freq,int mode)
{
	LgdtReturn_t		retVal;
	int lg3305_lock;
	int wait_time = 0;
	
	//SYS_Debug("Freq = %d   mode = %d\n",Freq,mode);
	retVal = LgdtRun(mode, (float)(Freq)); // MHZ
//	retVal = LgdtRun(mode, (float)(Freq/1000));

	do
	{
		lg3305_lock = lg3305_lock_check();
		if( lg3305_lock == 1 ) break;
		//osal_task_sleep(10);	// TODO - GOODSW : Should be add for sleep 
		LgdtDelay(10); // ÑÓÊ±10ms
		wait_time++;
	} while ( wait_time < 50 );

	printf("LG3305_Tune = %d, lock = %d\n",retVal, lg3305_lock);

	if( lg3305_lock == 1 ) return 1;
	else return 0;

#if 0
	retVal = LgdtRun(0x00, (float)(Freq/1000));

	wait_time = 0;
	do
	{
		lg3305_lock = lg3305_lock_check();
		if( lg3305_lock == 1 ) break;
		// osal_task_sleep(10); // TODO - GOODSW : Should be add for sleep 
		LgdtDelay(10);
	} while ( wait_time < 40 );
#endif



}

int Check_LG3305(void){
	DATA16 regAddr = 0x0808;	// 0x0808 is dummy register, RESET VALUE=0x00
	DATA08 dataTest = 0x55;
	DATA08 pData=0;
	DATA08 pOrg=0;
	LgdtReturn_t retVal;

	retVal=LgdtCheckRegi(regAddr, &pOrg);
	if(retVal!=LGDT_ERR_NO)
		return false;

	retVal=LgdtAlterRegi(regAddr, dataTest);
	if(retVal!=LGDT_ERR_NO)
		return false;
	retVal=LgdtCheckRegi(regAddr, &pData);
	if(retVal!=LGDT_ERR_NO)
		return false;

	printf("[checkConnection] 0x%04x = %02x:%02x (org=%02x)\n", regAddr, dataTest, pData, pOrg);

	retVal=LgdtAlterRegi(regAddr, pOrg);   
   
	if(pData==dataTest)      
		return true;
	else {
		int i;
		for (i = 0; i <= 0x1f; i++) {
			retVal = LgdtCheckRegi((DATA16)i, &pData);
			if (retVal != LGDT_ERR_NO) {
				printf("read i2c error\n");
				break;
			}
		}
		return false;
	}
}



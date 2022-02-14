/*****************************************************************************
 *
 *	Driver APIs for VSB/QAM Demodulator LGDT3305
 *
 *  Copyright (c) 2003-2008, System IC Div., LG Electronics, Inc.
 *  All Rights Reserved
 *
 *  File Name:      LGDT3305API_ALTER.cpp
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

#include "demod/LGDT3305/LGDT3305.h"
#include "demod/LGDT3305/LGDT3305API.h"

//#define DEBUGGING_FOR_LGE_SIC
#undef DEBUGGING_FOR_LGE_SIC

/////////////////////////////////////////////////////////////////////////////
//
//  Name:			LgdtAlterPolyPRBS
//
//  Description:    Alters coefficients of PRBS Polynomial - PRBSPOLY(@ 0x0803~0x0805)
//
//  Parameters:     (LgdtPolyPRBS_t)
//					(DATA32) - only valid if LgdtInvert_t is LGDT_PolyPRBS.
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
LgdtReturn_t LgdtAlterPolyPRBS(LgdtPolyPRBS_t modePRBS, DATA32 nData)
{
	const DATA32 MODE15 = 0x00C001;		/*--- Refers to the Datasheet	---*/
	const DATA32 MODE23 = 0x840001;		/*--- Refers to the Datasheet	---*/
	
	DATA08			pData;
	LgdtReturn_t	nRet;

	switch(modePRBS)
	{
		case LGDT_PolyPRBS:
			break;
		case LGDT_PolyPRBS15:
			nData = MODE15;
			break;
		case LGDT_PolyPRBS23:
			nData = MODE23;
			break;
		default:
			return (LgdtReturn_t)(LGDT_ERR_NO|LGDT_ERR_PARAM);
	}

	pData = (DATA08)((nData >> 16) & 0xFF);
	nRet = LgdtAlterRegi(0x0803, pData);
	if(nRet != LGDT_ERR_NO)
		return (LgdtReturn_t)(nRet|LGDT_ERR_ALTER_FUNC);

	pData = (DATA08)((nData >> 8) & 0xFF);
	nRet = LgdtAlterRegi(0x0804, pData);
	if(nRet != LGDT_ERR_NO)
		return (LgdtReturn_t)(nRet|LGDT_ERR_ALTER_FUNC);

	pData = (DATA08)(nData & 0xFF);
	nRet = LgdtAlterRegi(0x0805, pData);
	if(nRet != LGDT_ERR_NO)
		return (LgdtReturn_t)(nRet|LGDT_ERR_ALTER_FUNC);

	LgdtDelay((DATA16)DELAY);	 
	LgdtResetBER();

	return LGDT_ERR_NO;
}

/////////////////////////////////////////////////////////////////////////////
//
//  Name:			LgdtAlterInvertPRBS
//
//  Description:    Alters PRBS Invert or Non Invert - PRBSINV(@ 0x0800[2])
//
//  Parameters:     (LgdtInvert_t)
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
LgdtReturn_t LgdtAlterInvertPRBS(LgdtInvert_t isPol)
{
	DATA08			pData;
	DATA08			nData;
	LgdtReturn_t	nRet;

	nRet = LgdtCheckRegi(0x0800, &pData);
	if(nRet != LGDT_ERR_NO)
		return (LgdtReturn_t)(nRet|LGDT_ERR_ALTER_FUNC);
	nData = pData & 0xFB;

	if (isPol == LGDT_INVERTED)
		nData |= 0x04;								/*---	Bit #3 to HIGH	---*/

	nRet = LgdtAlterRegi(0x0800, nData);
	if(nRet != LGDT_ERR_NO)
		return (LgdtReturn_t)(nRet|LGDT_ERR_ALTER_FUNC);

	nRet = LgdtResetFAT();
	if(nRet != LGDT_ERR_NO)
		return (LgdtReturn_t)nRet;

	return LGDT_ERR_NO;
}

/////////////////////////////////////////////////////////////////////////////
//
//  Name:			LgdtAlterMpegMode
//
//  Description:    Alters MPEG output stream format either serial or parallel
//					- TPSERIAL(@0x050E[5])
//
//  Parameters:     (LgdtMpegMode_t)
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
LgdtReturn_t LgdtAlterMpegMode(LgdtMpegMode_t isMPEG)
{
	DATA08		 pData;
	DATA08		 nTemp;
	LgdtReturn_t nRet;

	if ( (isMPEG != LGDT_PARAL) && (isMPEG != LGDT_SERIAL) )
		return (LgdtReturn_t)(LGDT_ERR_PARAM|LGDT_ERR_ALTER_FUNC);

	nTemp = isMPEG << 5;

	nRet = LgdtCheckRegi(0x050E, &pData);
	if (nRet != LGDT_ERR_NO)
		return (LgdtReturn_t)(nRet|LGDT_ERR_ALTER_FUNC);

	pData &= 0xDF;
	pData |= nTemp;

	nRet = LgdtAlterRegi(0x050E, pData);
	if(nRet != LGDT_ERR_NO)
	{
		return (LgdtReturn_t)(nRet|LGDT_ERR_ALTER_FUNC);
	}

	nRet = LgdtResetFAT();
	if (nRet != LGDT_ERR_NO)
	{
		return nRet;
	}

	return LGDT_ERR_NO;
}

/////////////////////////////////////////////////////////////////////////////
//
//  Name:			LgdtAlterMpegPolarity
//
//  Description:    Alters MPEG output data format
//					- TPCLKEDG(@0x050E[3]) Controls TPCLK(pin)
//					- TPVALPOL(@0x050E[0]) Controls TPVALID(pin)
//
//  Parameters:     (LgdtClock_t)
//					(LgdtClock_t)
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
LgdtReturn_t LgdtAlterMpegPolarity(LgdtClock_t isCLKEDG, LgdtClock_t isVALID)
{
	DATA08			pData;
	DATA08			nTemp;
	LgdtReturn_t	nRet;

	nTemp = 0x00;
	if (isCLKEDG == LGDT_CLOCK_HIGH)
		nTemp |= 0x08;
	if (isVALID == LGDT_CLOCK_HIGH)
		nTemp |= 0x01;

	nRet = LgdtCheckRegi(0x050E, &pData);
	if (nRet != LGDT_ERR_NO)
		return (LgdtReturn_t)(nRet|LGDT_ERR_ALTER_FUNC);

	pData &= 0xF6;
	pData |= nTemp;

#if TS_OUT_CLK_269MHZ

	pData |= 0x80;  // TS clk 2.69MHz Setting

#endif

    // fixed clock mode
    pData |= 0x40; //TPCLKMOD Control TP clock mode for VSB and QAM annex-B/C mode.
                   //‘0’ => Gated clock mode (This mode does not support QAM annex-C)
                   //‘1’ => Fixed clock mode
                   
	nRet = LgdtAlterRegi(0x050E, pData);
	if(nRet != LGDT_ERR_NO)
	{
		return (LgdtReturn_t)(nRet|LGDT_ERR_ALTER_FUNC);
	}

	nRet = LgdtResetFAT();
	if (nRet != LGDT_ERR_NO)
	{
		return (LgdtReturn_t)(nRet|LGDT_ERR_ALTER_FUNC);
	}

	return LGDT_ERR_NO;
}

/////////////////////////////////////////////////////////////////////////////
//
//  Name:			LgdtAlterAgcData
//
//  Description:    Alters AGC power reference value manually.
//
//  Parameters:     (DATA16)
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
LgdtReturn_t LgdtAlterAgcData(DATA16 nData)
{
	DATA08			pData;
	LgdtReturn_t	nRet;

	pData = (DATA08)nData;
	nRet = LgdtAlterRegi(0x0300, pData);
	if(nRet != LGDT_ERR_NO)
		return (LgdtReturn_t)(nRet|LGDT_ERR_ALTER_FUNC);

	pData = (DATA08)(nData>>8);
	nRet = LgdtAlterRegi(0x0301, pData);
	if(nRet != LGDT_ERR_NO)
		return (LgdtReturn_t)(nRet|LGDT_ERR_ALTER_FUNC);

	return LGDT_ERR_NO;
}

/////////////////////////////////////////////////////////////////////////////
//
//  Name:			LgdtAlterAgcInput
//
//  Description:    Alters RF and IF gain value, respectively.
//					- RFIXVAL(@0x0310[3:0]/0x0311)
//					- IFIXVAL(@0x0312[3:0]/0x0313)
//
//  Parameters:     (DATA16)
//					(DATA16)
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
LgdtReturn_t LgdtAlterAgcInput(DATA16 nRF, DATA16 nIF)
{
	DATA08		 pData;
	LgdtReturn_t nRet;
	
	/*-----		RF Gain Value		-----*/
	pData = nRF >> 8;
	pData &= 0x0F;

	nRet = LgdtAlterRegi(0x0310, pData);
	if(nRet != LGDT_ERR_NO)
		return (LgdtReturn_t)(nRet|LGDT_ERR_ALTER_FUNC);

	pData = nRF & 0xFF;
	
	nRet = LgdtAlterRegi(0x0311, pData);
	if(nRet != LGDT_ERR_NO)
		return (LgdtReturn_t)(nRet|LGDT_ERR_ALTER_FUNC);

	/*-----		IF Gain Value		-----*/
	pData = nIF >> 8;
	pData &= 0x0F;

	nRet = LgdtAlterRegi(0x0312, pData);
	if(nRet != LGDT_ERR_NO)
		return (LgdtReturn_t)(nRet|LGDT_ERR_ALTER_FUNC);

	pData = nIF & 0xFF;

	nRet = LgdtAlterRegi(0x0313, pData);
	if(nRet != LGDT_ERR_NO)
		return (LgdtReturn_t)(nRet|LGDT_ERR_ALTER_FUNC);
	
	return LGDT_ERR_NO;
}

/////////////////////////////////////////////////////////////////////////////
//
//  Name:			LgdtAlterAgcPolarity
//
//  Description:    Selects polarity signal on RF and IF, respectively.
//					- RFPOL(@0x030D[4])
//					- IFPOL(@0x030D[5])
//
//  Parameters:     (LgdtInvert_t)
//					(LgdtInvert_t)
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
LgdtReturn_t LgdtAlterAgcPolarity(LgdtInvert_t polRF, LgdtInvert_t polIF)
{
	DATA08			pData;
	DATA08			nTemp;
	LgdtReturn_t	nRet;

	nTemp = 0x00;

	if (polRF == LGDT_INVERTED)
		nTemp |= 0x10;
	if (polIF == LGDT_INVERTED)
		nTemp |= 0x20;

	nRet = LgdtCheckRegi(0x030D, &pData);
	if (nRet != LGDT_ERR_NO)
		return (LgdtReturn_t)(nRet|LGDT_ERR_ALTER_FUNC);

	pData &= 0xCF;
	pData |= nTemp;

	nRet = LgdtAlterRegi(0x030D, pData);
	if(nRet != LGDT_ERR_NO)
		return (LgdtReturn_t)(nRet|LGDT_ERR_ALTER_FUNC);

	return LGDT_ERR_NO;
}

/////////////////////////////////////////////////////////////////////////////
//
//  Name:			LgdtAlterAgcBandwidth
//
//  Description:    Alters the Loop Bandwidth Control Bits for RF AGC and IF AGC.
//					- RFBW(@0x0306,0x0307)
//					- IFBW(@0x0308,0x0309)
//
//  Parameters:     (DATA16)
//					(DATA16)
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
LgdtReturn_t LgdtAlterAgcBandwidth(DATA16 nRF, DATA16 nIF)
{
	DATA08		 pData;
	LgdtReturn_t nRet;

	/*-----		RF Bandwidth Control	-----*/
	pData = nRF >> 8;
	pData &= 0xFF;
	nRet = LgdtAlterRegi(0x0306, pData);
	if(nRet != LGDT_ERR_NO)
		return (LgdtReturn_t)(nRet|LGDT_ERR_ALTER_FUNC);

	pData = nRF & 0xFF;
	nRet = LgdtAlterRegi(0x0307, pData);
	if(nRet != LGDT_ERR_NO)
		return (LgdtReturn_t)(nRet|LGDT_ERR_ALTER_FUNC);
	
	/*-----		IF Bandwidth Control	-----*/
	pData = nIF >> 8;
	pData &= 0xFF;
	nRet = LgdtAlterRegi(0x0308, pData);
	if(nRet != LGDT_ERR_NO)
		return (LgdtReturn_t)(nRet|LGDT_ERR_ALTER_FUNC);

	pData = nIF & 0xFF;
	nRet = LgdtAlterRegi(0x0309, pData);
	if(nRet != LGDT_ERR_NO)
		return (LgdtReturn_t)(nRet|LGDT_ERR_ALTER_FUNC);
	
	return LGDT_ERR_NO;
}

/////////////////////////////////////////////////////////////////////////////
//
//  Name:			LgdtAlterSpectrumFAT
//
//  Description:    Alters In Band Signal Spectrum Status - SPECINV(@0x0504[1])
//
//  Parameters:     (LgdtInvert_t)
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
LgdtReturn_t LgdtAlterSpectrumFAT(LgdtInvert_t isPol)
{
	DATA08		 pData;
	LgdtReturn_t nRet;

	nRet = LgdtCheckRegi(0x0504, &pData);
	if(nRet != LGDT_ERR_NO)
		return (LgdtReturn_t)(nRet|LGDT_ERR_ALTER_FUNC);

	pData &= 0xF5;				/*----- 0xF5 : 1111 x1x1 -----*/
	if (isPol == LGDT_INVERTED)
		pData |= 0x02;

	nRet = LgdtAlterRegi(0x0504, pData);
	if(nRet != LGDT_ERR_NO)
		return (LgdtReturn_t)(nRet|LGDT_ERR_ALTER_FUNC);

	/*----- FEC Not-Reset  -----*/
	nRet = LgdtAlterRegi(0x0504, pData | 0x08);
	if(nRet != LGDT_ERR_NO)
		return (LgdtReturn_t)(nRet|LGDT_ERR_ALTER_FUNC);


	return LGDT_ERR_NO;
}


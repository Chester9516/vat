/*****************************************************************************
 *
 *	Driver APIs for VSB/QAM Demodulator LGDT3305
 *
 *  Copyright (c) 2003-2008, System IC Div., LG Electronics, Inc.
 *  All Rights Reserved
 *
 *  File Name:      LGDT3305API_CHECK.cpp
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

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//																		   //
//							Check Functions								   //
//																		   //
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//

/////////////////////////////////////////////////////////////////////////////
//
//  Name:			LgdtCheckOperMode
//
//  Description:    Checks the Operation Mode of the Demod - OPERMODE(@0x0000[1:0])
//
//  Parameters:     (LgdtOperMode_t*)
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
LgdtReturn_t LgdtCheckOperMode(LgdtOperMode_t* modeOper)
{
	DATA08			pData;
	LgdtReturn_t	nRet;

	nRet = LgdtCheckRegi(0x0000, &pData);
	if (nRet != LGDT_ERR_NO)
		return (LgdtReturn_t)(nRet|LGDT_ERR_CHECK_FUNC);

	pData &= 0x03;
	switch(pData)
	{
		case LGDT_QAM64:
			*modeOper = LGDT_QAM64;
			break;
		case LGDT_QAM256:
			*modeOper = LGDT_QAM256;
			break;
		case LGDT_VSB:
			*modeOper = LGDT_VSB;
			break;
		default:
			return LGDT_ERR_CHECK_FUNC;
	}

	return LGDT_ERR_NO;
}


/////////////////////////////////////////////////////////////////////////////
//
//  Name:			LgdtCheckSnrVSB
//
//  Description:    Shows SNR value for VSB - EQ_MSE(@0x0413[3:0], 0x0414 and 0x0415)
//
//  Parameters:     (DATA_DOUBLE*) : A SNR value
//
//  Returns:        (LgdtReturn_t)
//
//  Revision History:
//
//    Date		Author	Description
//  -------------------------------------------------------------------------
//   07-01-2006	pyjung	Original
//   06-18-2007	jywon	Modify
//
/////////////////////////////////////////////////////////////////////////////
LgdtReturn_t LgdtCheckSnrVSB_EQ(DATA_DOUBLE* nPT)
{
	DATA08			pData;
	DATA32			nData = 0;
	LgdtReturn_t	nRet;

// EQ SNR
	nRet = LgdtCheckRegi(0x0413, &pData);
	if (nRet != LGDT_ERR_NO)
		return (LgdtReturn_t)(nRet|LGDT_ERR_CHECK_FUNC);
	nData = pData & 0x0F;
	nData <<= 16;

	nRet = LgdtCheckRegi(0x0414, &pData);
	if (nRet != LGDT_ERR_NO)
		return (LgdtReturn_t)(nRet|LGDT_ERR_CHECK_FUNC);

	nData |= (pData << 8);						// 2006-09-21, by sungwee

	nRet = LgdtCheckRegi(0x0415, &pData);
	if (nRet != LGDT_ERR_NO)
		return (LgdtReturn_t)(nRet|LGDT_ERR_CHECK_FUNC);
	nData |= pData;

	if ((nData < 0x0FFFFF) && (nData > 0))
	{
		*nPT = 10 * log10((DATA_DOUBLE)(25 * 32 * 32) / nData);       // Bug fixed by j.y. won : 2007.06.18
	}
	else
		*nPT = 0;

	if (*nPT < 0)
		*nPT = 0;

	return LGDT_ERR_NO;
}

/////////////////////////////////////////////////////////////////////////////
//
//  Name:			LgdtCheckSnrVSB
//
//  Description:    Shows SNR value for VSB - PT_MSE(@0x0417[2:0], 0x0418 and 0x0419)
//
//  Parameters:     (DATA_DOUBLE*) : A SNR value
//
//  Returns:        (LgdtReturn_t)
//
//  Revision History:
//
//    Date		Author	Description
//  -------------------------------------------------------------------------
//   07-01-2006	pyjung	Original
//   06-18-2007	jywon	Modify
//
/////////////////////////////////////////////////////////////////////////////
LgdtReturn_t LgdtCheckSnrVSB(DATA_DOUBLE* nPT)
{
	DATA08			pData;
	DATA32			nData = 0;
	LgdtReturn_t	nRet;

// PT SNR
	nRet = LgdtCheckRegi(0x0417, &pData);
	if (nRet != LGDT_ERR_NO)
		return (LgdtReturn_t)(nRet|LGDT_ERR_CHECK_FUNC);
	nData = pData & 0x0F;
	nData <<= 16;

	nRet = LgdtCheckRegi(0x0418, &pData);
	if (nRet != LGDT_ERR_NO)
		return (LgdtReturn_t)(nRet|LGDT_ERR_CHECK_FUNC);
	nData |= (pData << 8);						// 2006-09-21, by sungwee

	nRet = LgdtCheckRegi(0x0419, &pData);
	if (nRet != LGDT_ERR_NO)
		return (LgdtReturn_t)(nRet|LGDT_ERR_CHECK_FUNC);
	nData |= pData;

	if ((nData < 0x0FFFFF) && (nData > 0))
	{
		*nPT = 10 * log10((DATA_DOUBLE)(25 * 32 * 32) / nData);       // Bug fixed by j.y. won : 2007.06.18
	}
	else
		*nPT = 0;

	if (*nPT < 0)
		*nPT = 0;

	return LGDT_ERR_NO;
}

/////////////////////////////////////////////////////////////////////////////
//
//  Name:			LgdtCheckSnrQAM
//
//  Description:    Shows SNR value for QAM - MSEQAM(@0x011B and 0x011C)
//
//  Parameters:     (DATA_DOUBLE*) : A SNR value
//
//  Returns:        (LgdtReturn_t)
//
//  Revision History:
//
//    Date		Author	Description
//  -------------------------------------------------------------------------
//   07-01-2006	pyjung	Original
//   06-18-2007	jywon	Modify
//
/////////////////////////////////////////////////////////////////////////////
LgdtReturn_t LgdtCheckSnrQAM(DATA_DOUBLE* nEQ, LgdtOperMode_t modeOper)
{
	LgdtReturn_t	nRet;
	DATA08			pData;
	DATA16			nData = 0;
	DATA32			nSignalPower = 0;

	/*----- Assgin Ps(Signal Power) based upon the mode -----*/
	if (modeOper == LGDT_QAM64)
		nSignalPower = 688128;
	else if (modeOper == LGDT_QAM256)
		nSignalPower = 696320;
	else
		return (LgdtReturn_t)(LGDT_ERR_PARAM|LGDT_ERR_CHECK_FUNC);
	/*----- End of assigning Ps								-----*/

	nRet = LgdtCheckRegi(0x011B, &pData);
	if (nRet != LGDT_ERR_NO)
		return (LgdtReturn_t)(nRet|LGDT_ERR_CHECK_FUNC);
	nData = pData << 8;

	nRet = LgdtCheckRegi(0x011C, &pData);
	if (nRet != LGDT_ERR_NO)
		return (LgdtReturn_t)(nRet|LGDT_ERR_CHECK_FUNC);
	nData |= pData;

	if ((nData != 0xFFFF) && (nData !=0))
	{
		*nEQ = 10 * log10((DATA_DOUBLE)nSignalPower / nData);		// Bug fixed by j.y. won : 2007.06.18
	}
	else
		*nEQ = 0;

	if (*nEQ < 0)
		*nEQ = 0;
		
	return LGDT_ERR_NO;
}

/////////////////////////////////////////////////////////////////////////////
//
//  Name:			LgdtCheckSnrFAT
//
//  Description:    Shows Signal to Noise Ratio Value
//
//  Parameters:     (DATA_DOUBLE*) : A SNR value
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
LgdtReturn_t LgdtCheckSnrFAT(DATA_DOUBLE* nSNR)
{
	DATA_DOUBLE		nData = 0;
	LgdtOperMode_t	modeOper;
	LgdtReturn_t	nRet;

	nRet = LgdtCheckOperMode(&modeOper);
	if (nRet != LGDT_ERR_NO)
		return (LgdtReturn_t)(nRet|LGDT_ERR_CHECK_FUNC);

	switch(modeOper)
	{
		case LGDT_QAM64:
		case LGDT_QAM256:
			nRet = LgdtCheckSnrQAM(&nData, modeOper);
			if (nRet != LGDT_ERR_NO)
				return (LgdtReturn_t)(nRet|LGDT_ERR_CHECK_FUNC);
			*nSNR = nData;
			break;
		default:
			nRet = LgdtCheckSnrVSB(&nData);
			if (nRet != LGDT_ERR_NO)
				return (LgdtReturn_t)(nRet|LGDT_ERR_CHECK_FUNC);
			*nSNR = nData;
			break;
	}

	return LGDT_ERR_NO;
}

/////////////////////////////////////////////////////////////////////////////
//
//  Name:			LgdtCheckSignalCondition
//
//  Description:    Checks Signal Condition based on the below guideline
//
//		- VSB mode
//			LGDT_SIGNAL_NO			:	 ~ 0
//			LGDT_SIGNAL_WEAK		:  0 ~ 10
//			LGDT_SIGNAL_MODERATE	: 10 ~ 16
//			LGDT_SIGNAL_STRONG		: 16 ~ 24
//			LGDT_SIGNAL_VERY_STRONG : 24 ~
//
//		- 64QAM mode
//			LGDT_SIGNAL_NO			:	 ~ 0
//			LGDT_SIGNAL_WEAK		:  0 ~ 20
//			LGDT_SIGNAL_MODERATE	: 20 ~ 25
//			LGDT_SIGNAL_STRONG		: 26 ~ 30
//			LGDT_SIGNAL_VERY_STRONG : 31 ~
//
//		- 256QAM mode
//			LGDT_SIGNAL_NO			:	 ~ 0
//			LGDT_SIGNAL_WEAK		:  0 ~ 28
//			LGDT_SIGNAL_MODERATE	: 28 ~ 30
//			LGDT_SIGNAL_STRONG		: 31 ~ 32
//			LGDT_SIGNAL_VERY_STRONG : 33 ~
//
//  Parameters:     (LgdtSignalCondition_t*)
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
LgdtReturn_t LgdtCheckSignalCondition(LgdtSignalCondition_t* nStatus)
{
	DATA_DOUBLE		nSNR;
	LgdtOperMode_t	modeOper;
	LgdtReturn_t	nRet;
	
	nRet = LgdtCheckOperMode(&modeOper);
	if (nRet != LGDT_ERR_NO)
		return (LgdtReturn_t)(nRet|LGDT_ERR_CHECK_FUNC);

	*nStatus = LGDT_SIGNAL_NO;
	switch(modeOper)
	{
		case LGDT_QAM64:
			nRet = LgdtCheckSnrQAM(&nSNR, modeOper);
			if (nRet != LGDT_ERR_NO)
				return (LgdtReturn_t)(nRet|LGDT_ERR_CHECK_FUNC);

			if		( (nSNR > 0 ) && (nSNR <= 20) ) *nStatus = LGDT_SIGNAL_WEAK;
			else if ( (nSNR > 20) && (nSNR <= 25) ) *nStatus = LGDT_SIGNAL_MODERATE;
			else if ( (nSNR > 25) && (nSNR <= 30) ) *nStatus = LGDT_SIGNAL_STRONG;
			else if (nSNR > 31)	*nStatus = LGDT_SIGNAL_VERY_STRONG;
			break;

		case LGDT_QAM256:
			nRet = LgdtCheckSnrQAM(&nSNR, modeOper);
			if (nRet != LGDT_ERR_NO)
				return (LgdtReturn_t)(nRet|LGDT_ERR_CHECK_FUNC);

			if		( (nSNR > 0 ) && (nSNR <= 27) ) *nStatus = LGDT_SIGNAL_WEAK;
			else if ( (nSNR > 27) && (nSNR <= 30) ) *nStatus = LGDT_SIGNAL_MODERATE;
			else if ( (nSNR > 31) && (nSNR <= 32) ) *nStatus = LGDT_SIGNAL_STRONG;
			else if (nSNR > 33)	*nStatus = LGDT_SIGNAL_VERY_STRONG;
			break;

		default:
			nRet = LgdtCheckSnrVSB(&nSNR);
			if (nRet != LGDT_ERR_NO)
				return (LgdtReturn_t)(nRet|LGDT_ERR_CHECK_FUNC);

			if		( (nSNR > 0 ) && (nSNR <= 10) ) *nStatus = LGDT_SIGNAL_WEAK;
			else if ( (nSNR > 10) && (nSNR <= 16) ) *nStatus = LGDT_SIGNAL_MODERATE;
			else if ( (nSNR > 16) && (nSNR <= 24) ) *nStatus = LGDT_SIGNAL_STRONG;
			else if (nSNR >24)	*nStatus = LGDT_SIGNAL_VERY_STRONG;
			break;
	}

	return LGDT_ERR_NO;
}

/////////////////////////////////////////////////////////////////////////////
//
//  Name:			LgdtCheckPacketErr
//
//  Description:    Shows uncorrectable errors on the input data of the FEC decoder.
//					 - PKTERROR(@ 0x050C~0x050D)
//
//  Parameters:     (DATA16*) : A Number of Packet Errors
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
LgdtReturn_t LgdtCheckPacketErr(DATA16* nErr)
{
	DATA08			pData;
	DATA16			nData;
	LgdtReturn_t	nRet;

	nRet = LgdtCheckRegi(0x050C, &pData);
	if (nRet != LGDT_ERR_NO)
		return (LgdtReturn_t)(nRet|LGDT_ERR_CHECK_FUNC);

	nData = pData << 8;

	nRet = LgdtCheckRegi(0x050D, &pData);
	if (nRet != LGDT_ERR_NO)
		return (LgdtReturn_t)(nRet|LGDT_ERR_CHECK_FUNC);

	nData |= pData;
	*nErr = nData;

	return LGDT_ERR_NO;
}

/////////////////////////////////////////////////////////////////////////////
//
//  Name:			LgdtCheckIFAcc
//
//  Description:    Checks IF AGC Accumulator value - IFACC(@ 0x0318, 0x0319)
//
//  Parameters:     (DATA16*)
//
//  Returns:        (LgdtReturn_t)
//
//  Revision History:
//
//    Date		Author	Description
//  -------------------------------------------------------------------------
//   07-01-2006	pyjung	Original
//   06-18-2007	jywon	Modify
//
/////////////////////////////////////////////////////////////////////////////
LgdtReturn_t LgdtCheckIFAcc(short* nAcc)			
{
	DATA08			pData;
	short			nData = 0;
	LgdtReturn_t	nRet;

	nRet = LgdtCheckRegi(0x0318, &pData);
	if(nRet != LGDT_ERR_NO)
		return (LgdtReturn_t)(nRet|LGDT_ERR_CHECK_FUNC);
	pData &= 0x0F;
	nData = (short)pData << 8;

	nRet = LgdtCheckRegi(0x0319, &pData);
	if(nRet != LGDT_ERR_NO)
		return (LgdtReturn_t)(nRet|LGDT_ERR_CHECK_FUNC);
	nData |= (short)pData;

	if(nData & 0x0800)							// Modified by j.y.won : 2007.06.18, 2's complement value
		*nAcc = nData - 0x1000;
	else
		*nAcc = nData;

	return LGDT_ERR_NO;
}

/////////////////////////////////////////////////////////////////////////////
//
//  Name: LgdtCheckBer
//
//  Description:    Checks BERT value
//					- BERTISEL(@0x0800[1])
//					- BERTPERD(@0x0801[4:0])
//					- BITERROR(@0x080A, 0x080B, 0x080C, 0x080D)
//
//  Parameters:     (float*)nBer : a bit error ratio
//
//  Returns:        (LgdtReturn_t)
//
//  Revision History:
//
//    Date		Author	Description
//  -------------------------------------------------------------------------
//   07-01-2006	pyjung	Original
//   06-18-2007	jywon	Modify
//   10-24-2007	jywon	Modify
//
/////////////////////////////////////////////////////////////////////////////
LgdtReturn_t LgdtCheckBer(float* nBer)
{
	DATA08			pData;
	DATA32			nPeriod;
	DATA32			nErr1, nErr2, nErr3, nErr4;
	LgdtReturn_t	nRet;

	/*----- Checks BERTPERD							-----*/
	nRet = LgdtCheckRegi(0x0801, &pData);
	if(nRet != LGDT_ERR_NO)
		return (LgdtReturn_t)(nRet|LGDT_ERR_CHECK_FUNC);
	pData &= 0x1F;
	// added by j.y. won : 2007.10.24
	if(pData < 4)	pData = 4;

	/*----- Period = 2^BERTPERD						-----*/
	nPeriod = 1<<pData;		

	/*----- Reads 4 registers						-----*/
	nRet = LgdtCheckRegi(0x080A, &pData);
	if(nRet != LGDT_ERR_NO)
		return (LgdtReturn_t)(nRet|LGDT_ERR_CHECK_FUNC);
	nErr1 = (DATA32)pData << 24;		

	nRet = LgdtCheckRegi(0x080B, &pData);
	if(nRet != LGDT_ERR_NO)
		return (LgdtReturn_t)(nRet|LGDT_ERR_CHECK_FUNC);
	nErr2 = (DATA32)pData << 16;        

	nRet = LgdtCheckRegi(0x080C, &pData);
	if(nRet != LGDT_ERR_NO)
		return (LgdtReturn_t)(nRet|LGDT_ERR_CHECK_FUNC);
	nErr3 = (DATA32)pData << 8;         

	nRet = LgdtCheckRegi(0x080D, &pData);
	if(nRet != LGDT_ERR_NO)
		return (LgdtReturn_t)(nRet|LGDT_ERR_CHECK_FUNC);
	nErr4 = (DATA32)pData;              

	/*----- Calculation for the average value during the nPeriod	-----*/

	// Modified by j.y. won : 2007.06.18
	*nBer = (float)(nErr1+nErr2+nErr3+nErr4)/nPeriod;

	return LGDT_ERR_NO;
}

/////////////////////////////////////////////////////////////////////////////
//
//  Name: LgdtCheckPolyPRBS
//
//  Description:    Checks coefficients of PRBS Polynomial - PRBSPOLY(@ 0x0803~0x0805)
//
//  Parameters:     (LgdtPolyPRBS_t*)
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
LgdtReturn_t LgdtCheckPolyPRBS(LgdtPolyPRBS_t* modePRBS)
{
	DATA32 MODE15 = 0x00C001;		/*--- Refers to the Datasheet	---*/
	DATA32 MODE23 = 0x840001;		/*--- Refers to the Datasheet	---*/

	DATA08			pData;
	DATA32			nData;
	LgdtReturn_t	nRet;

	/*----- Read the Register Value				-----*/
	nRet = LgdtCheckRegi(0x0803, &pData);
	if(nRet != LGDT_ERR_NO)
		return (LgdtReturn_t)(nRet|LGDT_ERR_CHECK_FUNC);
	nData = pData << 16;

	nRet = LgdtCheckRegi(0x0804, &pData);
	if(nRet != LGDT_ERR_NO)
		return (LgdtReturn_t)(nRet|LGDT_ERR_CHECK_FUNC);
	nData |= pData << 8;

	nRet = LgdtCheckRegi(0x0805, &pData);
	if(nRet != LGDT_ERR_NO)
		return (LgdtReturn_t)(nRet|LGDT_ERR_CHECK_FUNC);
	nData |= pData;

	/*----- Compare and Check the Poly mode	-----*/
	*modePRBS = LGDT_PolyPRBS;

	if( nData == MODE15 )
	{
		*modePRBS = LGDT_PolyPRBS15;
	}

	if( nData == MODE23 )
	{
		*modePRBS = LGDT_PolyPRBS23;
	}
	
	return LGDT_ERR_NO;
}

/////////////////////////////////////////////////////////////////////////////
//
//  Name:			LgdtCheckFreqOffset
//
//  Description:    Checks Carrier Frequency Offset value - CRFREQO(@ 0x0118~0x011A)
//
//  Parameters:     (DATA16*)
//
//  Returns:        (LgdtReturn_t)
//
//  Revision History:
//
//    Date		Author	Description
//  -------------------------------------------------------------------------
//   07-01-2006	pyjung	Original
//   02-21-2008	jywon	Modified
//
/////////////////////////////////////////////////////////////////////////////
LgdtReturn_t LgdtCheckFreqOffset(float* nOffset)
{
	int				nConvert       = 0;
	DATA08			pData;
	int				nTemp			= 0;
	DATA_DOUBLE		nRate_VSB	= 21520000.0;	
	DATA_DOUBLE		nRate_QAM	= 25000000.0;	
	LgdtReturn_t	nRet;
	LgdtOperMode_t	mode;

	nRet = LgdtCheckRegi(0x0118, &pData);
	if(nRet != LGDT_ERR_NO)
		return (LgdtReturn_t)(nRet|LGDT_ERR_CHECK_FUNC);
	nTemp = (int)pData << 16;

	nRet = LgdtCheckRegi(0x0119, &pData);
	if(nRet != LGDT_ERR_NO)
		return (LgdtReturn_t)(nRet|LGDT_ERR_CHECK_FUNC);
	nTemp |= (int)pData << 8;

	nRet = LgdtCheckRegi(0x011A, &pData);
	if(nRet != LGDT_ERR_NO)
		return (LgdtReturn_t)(nRet|LGDT_ERR_CHECK_FUNC);
	nTemp |= (int)pData;

	if(nTemp & 0x00800000)								
		nConvert = (int)nTemp - 0x01000000 ;
	else
		nConvert = (int)nTemp;

	nRet = LgdtCheckOperMode(&mode);
	if( nRet != LGDT_ERR_NO )
		return (LgdtReturn_t)(nRet|LGDT_ERR_CHECK_FUNC);

	if( mode == LGDT_QAM64 || mode == LGDT_QAM256)			
		(*nOffset) = (float)(nConvert*pow(2,-29)*nRate_QAM);		// QAM mode			
	else
		(*nOffset) = (float)(nConvert*pow(2,-24)*nRate_VSB);		// VSB mode		
		

	return LGDT_ERR_NO;
}

/////////////////////////////////////////////////////////////////////////////
//
//  Name:			LgdtCheckScatFAT
//
//  Description:    Checks the Constellation for VSB/QAM
//					- SCAT_I(@0x040F[1:0]/0x0410)
//					- SCAT_Q(@0x0411[1:0]/0x0412)
//
//  Parameters:     (DATA16*)
//					(DATA16*)
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
LgdtReturn_t LgdtCheckScatFAT(DATA16* i, DATA16* q)
{
	LgdtReturn_t	nRet;
	DATA08			pData;
	DATA16			nData = 0;

	/*----- Scatter I : LSB 10 bits + MSB 2 bits		----*/
	nRet = LgdtCheckRegi(0x040F, &pData);
	if(nRet != LGDT_ERR_NO)
		return (LgdtReturn_t)(nRet|LGDT_ERR_CHECK_FUNC);
	nData = pData & 0x03;
	nData <<= 8;

	nRet = LgdtCheckRegi(0x0410, &pData);
	if(nRet != LGDT_ERR_NO)
		return (LgdtReturn_t)(nRet|LGDT_ERR_CHECK_FUNC);
	nData |= pData;
	*i = nData;

	/*----- Scatter Q : LSB 10 bits + MSB 2 bits		----*/
	nRet = LgdtCheckRegi(0x0411, &pData);
	if(nRet != LGDT_ERR_NO)
		return (LgdtReturn_t)(nRet|LGDT_ERR_CHECK_FUNC);
	nData = pData & 0x03;
	nData <<= 8;

	nRet = LgdtCheckRegi(0x0412, &pData);
	if(nRet != LGDT_ERR_NO)
		return (LgdtReturn_t)(nRet|LGDT_ERR_CHECK_FUNC);
	nData |= pData;
	*q = nData;

	/*-----

	The routine below is required to implement in the caller function
	Because, the value includes minus(-) value which should be converted after passed thru the argument type

		q = i2c.read(ChipID,0x0636);
		i = i2c.read(ChipID,0x0637);

		if ((q & 0x80) > 0)
			q = (q & 0x7F) - 128;

		if ((i & 0x80) > 0)
			i = (i & 0x7F) - 128;

	-----*/

	return LGDT_ERR_NO;
}

/////////////////////////////////////////////////////////////////////////////
//
//  Name:			LgdtCheckDCValue
//
//  Description:    Checks DC Value of VSB Operations - DCVALUE(0x021A)
//
//  Parameters:     (DATA08*)
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
LgdtReturn_t LgdtCheckDCValue(DATA08* nValue)
{
	DATA08			pData;
	LgdtReturn_t	nRet;

	nRet = LgdtCheckRegi(0x021A, &pData);
	if(nRet != LGDT_ERR_NO)
		return (LgdtReturn_t)(nRet|LGDT_ERR_CHECK_FUNC);

	*nValue = pData;
	return LGDT_ERR_NO;
}

/////////////////////////////////////////////////////////////////////////////
//
//  Name:			LgdtCheckIntMode
//
//  Description:    Checks Steps and Increases of the Interleaving
//					 - INTMODE(@ 0x050A[7:4])
//
//  Parameters:     (DATA08*) : I value, The Number of Taps
//					(DATA08*) : J value, Increase
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
LgdtReturn_t LgdtCheckIntMode(DATA08* i, DATA08* j)
{
	DATA08			pData;
	LgdtReturn_t	nRet;
	
	nRet = LgdtCheckRegi(0x050A, &pData);
	if(nRet != LGDT_ERR_NO)
		return (LgdtReturn_t)(nRet|LGDT_ERR_CHECK_FUNC);

	pData &= 0xF0;
	switch(pData)
	{
		case 0x00  :	*i = 128;	*j =  1;	break;
		case 0x10  :	*i = 128;	*j =  1;	break;
		case 0x20  :	*i = 128;	*j =  2;	break;
		case 0x30  :	*i =  64;	*j =  2;	break;
		case 0x40  :	*i = 128;	*j =  3;	break;
		case 0x50  :	*i =  32;	*j =  4;	break;
		case 0x60  :	*i = 128;	*j =  4;	break;
		case 0x70  :	*i =  16;	*j =  8;	break;
		case 0x80  :	*i = 128;	*j =  5;	break;
		case 0x90  :	*i =   8;	*j = 16;	break;
		case 0xA0  :	*i = 128;	*j =  6;	break;
		case 0xB0  :	*i =   4;	*j = 32;	break;		/* Reserved	*/
		case 0xC0  :	*i = 128;	*j =  7;	break;
		case 0xD0  :	*i =   2;	*j = 64;	break;		/* Reserved	*/
		case 0xE0  :	*i = 128;	*j =  8;	break;
		case 0xF0  :	*i =   1;	*j =128;	break;		/* Reserved	*/
	}
	return LGDT_ERR_NO;
}

/////////////////////////////////////////////////////////////////////////////
//
//  Name:			LgdtCheckSpectrumFAT
//
//  Description:    Checks the Signal Spectrum Status - SPECINV(@0x0504[1])
//
//  Parameters:     (LgdtInvert_t*)
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
LgdtReturn_t LgdtCheckSpectrumFAT(LgdtInvert_t *pol)
{
	DATA08		 pData;
	LgdtReturn_t nRet;

	nRet = LgdtCheckRegi(0x0504, &pData);
	if(nRet != LGDT_ERR_NO)
		return (LgdtReturn_t)(nRet|LGDT_ERR_ALTER_FUNC);

	pData &= 0x02;
	if (pData == 0x02)
		*pol = LGDT_INVERTED;
	else
		*pol = LGDT_NOT_INVERTED;
	
	return LGDT_ERR_NO;
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//																		   //
//							IsLock Functions							   //
//																		   //
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//

/////////////////////////////////////////////////////////////////////////////
//
//  Name:			LgdtIsLockCR
//
//  Description:    Checks Carrier Recovery Lock status. Available for VSB and QAM. 
//					- CLOCKVSB(@ 0x011D[7]), CLOCKQAM(@ 0x011D[2:0])
//
//  Parameters:     (bool*)isLock
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
LgdtReturn_t LgdtIsLockCR(bool *isLock)
{
	DATA08				pData;
	LgdtOperMode_t		modeOper;
	LgdtReturn_t		nRet;

	nRet = LgdtCheckRegi(0x011D, &pData);
	if (nRet != LGDT_ERR_NO)
		return (LgdtReturn_t)(nRet|LGDT_ERR_CHECK_FUNC);

	nRet = LgdtCheckOperMode(&modeOper);
	if(nRet != LGDT_ERR_NO)
		return (LgdtReturn_t)(nRet|LGDT_ERR_CHECK_FUNC);

	*isLock = false;
	if (modeOper == LGDT_VSB)
	{
		pData &= 0x80;
		if (pData == 0x80) 
			*isLock = true;		
	}
	else
	{
		pData &= 0x02;
		if (pData == 0x02)
			*isLock = true;
	}

	return LGDT_ERR_NO;
}

/////////////////////////////////////////////////////////////////////////////
//
//  Name:			LgdtIsLockSync
//
//  Description:    Checks Sync Lock status. (VSB Only)
//					 - nSyncLoc(@ 0x003[3])
//
//  Parameters:     (bool*)isLock
//
//  Returns:        (LgdtReturn_t)
//
//  Revision History:
//
//    Date		Author	Description
//  -------------------------------------------------------------------------
//   07-01-2006	pyjung	Original
//   01-31-2008 jywon	Modified
//
/////////////////////////////////////////////////////////////////////////////
LgdtReturn_t LgdtIsLockSync(bool* isLock)
{
	DATA08				pData;
	LgdtReturn_t		nRet;
	LgdtOperMode_t		modeOper;

	*isLock = false;
	nRet = LgdtCheckRegi(0x0003, &pData);
	if (nRet != LGDT_ERR_NO)
		return (LgdtReturn_t)(nRet|LGDT_ERR_CHECK_FUNC);

	nRet = LgdtCheckOperMode(&modeOper);


	if( modeOper == LGDT_VSB )
	{
		pData &= 0x08;
		if (pData != 0x08)		
			*isLock = true;
	}
	else
	{
		pData &= 0x04;
		if (pData == 0x04)		
			*isLock = true;
	}

	return LGDT_ERR_NO;
}

/*
LgdtReturn_t LgdtSyncCheck_Test(bool* isLock)
{

	DATA08				pData;
	LgdtReturn_t		nRet;

	nRet = LgdtCheckRegi(0x0409, &pData);
	if (nRet != LGDT_ERR_NO)
		return (LgdtReturn_t)(nRet|LGDT_ERR_CHECK_FUNC);

	*isLock = false;
	pData &= 0x02;
	if (pData != 0x02)			
		*isLock = true;

	return LGDT_ERR_NO;
}
*/

/////////////////////////////////////////////////////////////////////////////
//
//  Name:			LgdtIsLockTR
//
//  Description:    Checks Timing Recovery Lock status. QAM ONLY.
//					 - TRLOCK(@0x021B[2:0]
//
//  Parameters:     (bool*)isLock
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
LgdtReturn_t LgdtIsLockTR(bool* isLock)
{
	DATA08			pData;
	LgdtReturn_t	nRet;

	*isLock = false;
	nRet = LgdtCheckRegi(0x021B, &pData);
	if (nRet != LGDT_ERR_NO)
		return (LgdtReturn_t)(nRet|LGDT_ERR_CHECK_FUNC);

	pData &= 0x06;			/*----- greater than 2 is fine	-----*/
	if (pData > 0)		
		*isLock = true;

	return LGDT_ERR_NO;
}

/////////////////////////////////////////////////////////////////////////////
//
//  Name:			LgdtIsLockFEC
//
//  Description:    Checks Forward Error Correction Related Lock status
//					- VDLOCK  (@0x050A[3])
//					- FRAMELCK(@0x050A[2])
//					- MPEGLOCK(@0x050A[0])
//
//  Parameters:     (bool*)flag
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
LgdtReturn_t LgdtIsLockFEC(bool* isViterbi, bool* isFec, bool* isMpeg)
{
	DATA08			pData;
	DATA08			nData;
	LgdtReturn_t	nRet;

	*isViterbi = false;
	*isFec = false;
	*isMpeg = false;

	nRet = LgdtCheckRegi(0x050A, &pData);
	if(nRet != LGDT_ERR_NO)
		return (LgdtReturn_t)(nRet|LGDT_ERR_CHECK_FUNC);

	nData = pData & 0x08;
	if (nData == 0x08)
		*isViterbi = true;

	nData = pData & 0x04;
	if (nData == 0x04)
		*isFec = true;

	nData = pData & 0x01;
	if (nData == 0x01)
		*isMpeg = true;

	return LGDT_ERR_NO;
}

/////////////////////////////////////////////////////////////////////////////
//
//  Name:			LgdtIsLockBER
//
//  Description:    Checks BERT Lock status - BERTLOCK(@0x0809[0])
//
//  Parameters:     (bool*)isLock
//
//  Returns:        (LgdtReturn_t)
//
//  Revision History:
//
//    Date		Author	Description
//  -------------------------------------------------------------------------
//   07-01-2006	pyjung	Original
//   06-18-2007	jywon	Modify
//
/////////////////////////////////////////////////////////////////////////////
LgdtReturn_t LgdtIsLockBER(bool *isLock)
{
	DATA08		 pData;
	LgdtReturn_t nRet;

	*isLock = false;

	// Added by j.y. won : 2007.06.18
	nRet = LgdtCheckRegi(0x0800, &pData);
	if(nRet != LGDT_ERR_NO)
		return (LgdtReturn_t)(nRet|LGDT_ERR_CHECK_FUNC);
	if(pData&0x01)
		return LGDT_ERR_OTHER;

	nRet = LgdtCheckRegi(0x0809, &pData);
	if(nRet != LGDT_ERR_NO)
		return (LgdtReturn_t)(nRet|LGDT_ERR_CHECK_FUNC);

	pData &= 0x01;
	if(pData == 0x01) 
		*isLock = true;

	return LGDT_ERR_NO;
}

/////////////////////////////////////////////////////////////////////////////
//
//  Name:			LgdtIsLockComb
//
//  Description:    Checks Comb Filter status - nCOMBST(@0x0409[0])
//
//  Parameters:     (bool*)isLock
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
LgdtReturn_t LgdtIsLockComb(bool* isLock)
{
	DATA08		 pData;
	LgdtReturn_t nRet;

	nRet = LgdtCheckRegi(0x0409, &pData);
	if (nRet != LGDT_ERR_NO)
		return (LgdtReturn_t)(nRet|LGDT_ERR_CHECK_FUNC);

	*isLock = false;
	pData &= 0x01;
	if (pData == 0x00)
		*isLock = true;

	return LGDT_ERR_NO;
}

/////////////////////////////////////////////////////////////////////////////
//
//  Name:			LgdtIsLockAGC
//
//  Description:    Checks AGC Lock status - AGCLOCK(@0x031E[0])
//                  (QAM ONLY)
//
//  Parameters:     (bool*)isLock
//
//  Returns:        (LgdtReturn_t)
//
//  Revision History:
//
//    Date		Author	Description
//  -------------------------------------------------------------------------
//   08-31-2007	jywon	Original
//
/////////////////////////////////////////////////////////////////////////////
LgdtReturn_t LgdtIsLockAGC(bool* isLock)
{
	DATA08		 pData;
	LgdtReturn_t nRet;

	nRet = LgdtCheckRegi(0x031E, &pData);
	if (nRet != LGDT_ERR_NO)
		return (LgdtReturn_t)(nRet|LGDT_ERR_CHECK_FUNC);

	*isLock = false;
	pData &= 0x01;
	if (pData)
		*isLock = true;

	return LGDT_ERR_NO;
}

/////////////////////////////////////////////////////////////////////////////
//
//  Name:			LgdtAntCheckSnr
//
//  Description:    Check SNR value. (ANTSNR(@0x092B/C/D))
//                  - snr = 10 * log10(21*1024*819) / ANTSNR
//
//  Parameters:     (double*) : SNR value
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
LgdtReturn_t LgdtAntCheckSnr(double *snr)
{
	DATA32		 data;
	DATA08		 data_p;
	LgdtReturn_t retVal;

	retVal = LgdtCheckRegi(0x092B, &data_p);
	if (retVal != LGDT_ERR_NO)
		return (LgdtReturn_t)(retVal|LGDT_ERR_ANT);
	data = data_p & 0xFF;
	data <<= 16;

	retVal = LgdtCheckRegi(0x092C, &data_p);
	if (retVal != LGDT_ERR_NO)
		return (LgdtReturn_t)(retVal|LGDT_ERR_ANT);
	data_p &= 0xFF;
	data |= (data_p << 8);

	retVal = LgdtCheckRegi(0x092D, &data_p);
	if (retVal != LGDT_ERR_NO)
		return (LgdtReturn_t)(retVal|LGDT_ERR_ANT);
	data_p &= 0xFF;
	data |= data_p;

	if ((data < 0xFFFFFF) && (data > 0))
	{
		//+----- Scaling value was confirmed by 이태원s, 구자혁s on Sept. 23, 3005	-----+//
		*snr = (double)( 10*log10((21*1024*819)/data) );
	}
	else
	{
		*snr = 0;

#ifdef DEBUGGING_FOR_LGE_SIC
		printf("\n Error : LgdtAntCheckSnr() - Invalid register value");
#endif
		return LGDT_ERR_ANT;
	}

	if (*snr < 0)
		*snr = 0;

	return LGDT_ERR_NO;
}

int lg3305_lock_check(void)
{
	int lock;

//	lock = 1;

	LgdtIsLockSync(&lock);

	return lock;
}

int lg3305_agc_check(void)
{
	int agc;

	agc = 60;

	return agc;
}

int lg3305_snr_check(void)
{
	int snr;

	snr  = 60;

	return snr;

}


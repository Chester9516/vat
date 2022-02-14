/*****************************************************************************
 *
 *	Driver APIs for VSB/QAM Demodulator LGDT3305
 *
 *  Copyright (c) 2003-2008, System IC Div., LG Electronics, Inc.
 *  All Rights Reserved
 *
 *  File Name:      LGDT_Util.cpp
 *
 *  Description:    
 *
 *  Revision History:
 *
 *    Date		Author	Description
 *  -------------------------------------------------------------------------
 *   07-01-2006	pyjung	Original
 *   06-03-2007	jywon	Modify
 *
 ***************************************************************************/

#include "demod/LGDT3305/LGDT3305API.h"
#include "demod/LGDT3305/LGDT_Util.h"
#include "vatek_system.h"

static Pvatek_i2c g_vi2c = NULL;
static vatek_result _I2C_Write(uint16_t usSlaveAddr,  uint8_t *pucBuff,  uint16_t *pusSize)
{
    vatek_result result = vatek_result_unknown;

    if (pucBuff == NULL || pusSize == NULL)
        return vatek_result_invalidparm;
    
    if ((result = g_vi2c->start(g_vi2c->hboard, usSlaveAddr, 0)) != vatek_result_success)
        {
            printf("reuslt= %d \n", result);
            return vatek_result_idle;
        }
    
    if ((result = g_vi2c->write(g_vi2c->hboard, pucBuff, *pusSize)) != vatek_result_success)
        return vatek_result_overrange;

    if ((result = g_vi2c->stop(g_vi2c->hboard)) != vatek_result_success)
        return vatek_result_hwfail;
    
    return result;
}

static vatek_result _I2C_Read(uint16_t usSlaveAddr,  uint8_t *pucBuff, uint16_t *pusSize)
{
    vatek_result result = vatek_result_unknown;

	usSlaveAddr = usSlaveAddr | 0x01;
    
    if ((result = g_vi2c->start(g_vi2c->hboard, usSlaveAddr, 0)) != vatek_result_success)
        return vatek_result_unsupport;

    if ((result = g_vi2c->read(g_vi2c->hboard, pucBuff, *pusSize)) != vatek_result_success)
        return vatek_result_negative;

    if ((result = g_vi2c->stop(g_vi2c->hboard)) != vatek_result_success)
        return vatek_result_bufoverflow;

    return result;
}

int Lgdt_SetActiveI2C(void *pi2c)
{
    if (pi2c == NULL)
        return 1;

    g_vi2c = (Pvatek_i2c)pi2c;
    return 0;
}

/////////////////////////////////////////////////////////////////////////////
//
//  Name: Lgdt_I2C_Read
//
//  Description:    Reads registers from 'addrReg' address.
//
//  Parameters:     (DATA08)nAddrDev	- the address of the slave device.
//					(DATA16)nAddrReg	- the address of the register to read.
//					(DATA08)*pData		- data to be read.
//					(DATA08)nSizeData	- size of data to read.
//
//  Returns:        (int)0
//
//  Revision History:
//
//    Date		Author	Description
//  -------------------------------------------------------------------------
//   07-01-2006	pyjung	Original
//
/////////////////////////////////////////////////////////////////////////////


int Lgdt_I2C_Read(DATA08 nAddrDev, DATA16 nAddrReg, DATA08 *pData, DATA08 nSizeData)
{
    vatek_result result = vatek_result_success;
    uint16_t size = 0;
    uint8_t Buff[3] = {0};

    Buff[0] = (unsigned char)((nAddrReg >> 8) & 0xff);
    Buff[1] = (unsigned char)(nAddrReg & 0xff);

    size = 2;
    result = _I2C_Write(nAddrDev,Buff,&size);
    if (result != vatek_result_success)
        printf("0x%X\n", result);

    size = 1;
    result |= _I2C_Read(nAddrDev,pData,&size);

    if (result == vatek_result_success)
        return 0;

    printf("0x%X\n", result);
    return 1;
    
    #if 0
	int			i=0;
	int			retval = 0;
	unsigned char pBuff[20];

	pBuff[0] = (unsigned char)((nAddrReg >> 8) & 0xff);
	pBuff[1] = (unsigned char)(nAddrReg & 0xff);



	/* If OEM data is not required, customer should treat devId as I2C slave Address */

	
 	L_Data0[0]=(nAddrReg >> 8) & 0xFF;	 	/* for 16 bits sub addresses */
	L_Data0[1]=(nAddrReg ) & 0xFF;	

    	if(I2c0ReadWrite(I2C_WRITE,nAddrDev,&L_Data0[0],2) == I2C_OK){  //I2C_WRITE
  
	    	L_Data0[0]  = 0;
	    	L_Data0[1]  = 0;
		Rx_Data0[0] =  0;

		if(I2c0ReadWrite(I2C_READ,nAddrDev,&L_Data0[0],1)== I2C_OK){
			*pData = Rx_Data0[0];
			return 0;	
		}
	}				
    #endif	
}

/////////////////////////////////////////////////////////////////////////////
//
//  Name: Lgdt_I2C_Write
//
//  Description:    Sends data to a slave device.
//					The 1st data array is device address.
//					The 2nd data array is register address to write.
//					The data is located from the 3rd data array.
//
//  Parameters:     (DATA08)*pData	- data to write. This include addresses
//									 of the slave device and the register to write.
//					(DATA08)nSizeData- size of data.
//
//  Returns:        (int)0
//
//  Revision History:
//
//    Date		Author	Description
//  -------------------------------------------------------------------------
//   07-01-2006	pyjung	Original
//
/////////////////////////////////////////////////////////////////////////////
int Lgdt_I2C_Write(DATA08 nAddrDev,DATA08 *pData, DATA08 nSizeData)
{
    vatek_result result = vatek_result_success;
    uint16_t size = 0;

    size = nSizeData;
    result = _I2C_Write(nAddrDev,pData,&size);
    if (result == vatek_result_success)
        return 0;

    return 1;

    #if 0
	int		retval = 1;
	int		i=0;

	/* TODO - GOODSW : Should be add to write i2c function
	retval = i2c_write(SP5650_I2C_TYPE,nAddrDev,pData,nSizeData);

	if( retval != 0 )
	{
		SYS_Debug("%s-%d	error = %d\n",__FUNCTION__,__LINE__,retval);
	}
*/

    	L_Data0[0]=pData[0]; // Just copy to global buffer
	L_Data0[1]=pData[1];
	L_Data0[2]=pData[2];

    	if(I2c0ReadWrite(I2C_WRITE,nAddrDev,&L_Data0[0],3)  != I2C_OK){  //I2C_WRITE
	  	return 1;
	}
    #endif
}


/////////////////////////////////////////////////////////////////////////////
//
//  Name: LgdtDelay
//
//  Description:    delay execution for x milliseconds
//
//  Parameters:     (DATA16)nDelayTime - Delay time in milliseconds
//
//  Returns:        (int)0
//
//  Revision History:
//
//    Date		Author	Description
//  -------------------------------------------------------------------------
//   07-01-2006	pyjung	Original
//
/////////////////////////////////////////////////////////////////////////////
int LgdtDelay(DATA16 nDelayTime)
{
    vatek_system_delay((uint32_t)nDelayTime);
	return 0;
}


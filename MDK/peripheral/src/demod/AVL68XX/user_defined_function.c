/*
 *           Copyright 2007-2014 Availink, Inc.
 *
 *  This software contains Availink proprietary information and
 *  its use and disclosure are restricted solely to the terms in
 *  the corresponding written license agreement. It shall not be 
 *  disclosed to anyone other than valid licensees without
 *  written permission of Availink, Inc.
 *
 */

#include "demod/AVL68XX/user_defined_function.h"
#include "vatek_system.h"
#include "i2c.h"

static Pvatek_i2c g_vi2c = NULL;
AVL_uint32 AVL_IBSP_SetActiveI2C(void *pi2c)
{
    g_vi2c = (Pvatek_i2c)pi2c;
    
    return 0;
}

AVL_uint32 AVL_IBSP_Reset(void)
{
    /*
	HAL_GPIO_WritePin(AV_RST_GPIO_Port, AV_RST_Pin, GPIO_PIN_RESET);	
	HAL_Delay(50);
	HAL_GPIO_WritePin(AV_RST_GPIO_Port, AV_RST_Pin, GPIO_PIN_SET);
	HAL_Delay(1000);
	*/
    return(0);
}

AVL_uint32 AVL_IBSP_GetTick(void)
{
    return (AVL_uint32)vatek_system_gettick();
}

AVL_uint32 AVL_IBSP_Delay(AVL_uint32 uiDelay_ms)
{
    return (AVL_uint32)vatek_system_delay(uiDelay_ms);
}

AVL_uint32 AVL_IBSP_I2C_Read(AVL_uint16 usSlaveAddr,  AVL_puchar pucBuff, AVL_puint16 pusSize)
{
    /*
	usSlaveAddr = usSlaveAddr<<1 | 0x01;
	Phal_lite_i2c pi2c = nim_core_get_main_i2c();
	hal_lite_result res = pi2c->start(pi2c->hi2c, usSlaveAddr,0);
	int32_t tmp_size = (int32_t)*pusSize;
	if (is_hal_lite_success(res))
	{
		res = pi2c->read(pi2c->hi2c, pucBuff, tmp_size);
		if (is_hal_lite_success(res))res = pi2c->stop(pi2c->hi2c);
	}
	
	if (is_hal_lite_success(res)){
		//operation_wait();
		return(0);
	}else return res;

	return 0;
	*/
	
    vatek_result result = vatek_result_unknown;

	usSlaveAddr = usSlaveAddr<<1 | 0x01;
    if ((result = g_vi2c->start(g_vi2c->hboard, usSlaveAddr, 0)) != vatek_result_success)
        return vatek_result_i2cfail;

    if ((result = g_vi2c->read(g_vi2c->hboard, pucBuff, *pusSize)) != vatek_result_success)
        return vatek_result_i2cfail;

    if ((result = g_vi2c->stop(g_vi2c->hboard)) != vatek_result_success)
        return vatek_result_i2cfail;

    return result;
}

AVL_uint32 AVL_IBSP_I2C_Write(AVL_uint16 usSlaveAddr, AVL_puchar pucBuff,  AVL_puint16  pusSize)
{
    /*
	usSlaveAddr = usSlaveAddr<<1;
	Phal_lite_i2c pi2c = nim_core_get_main_i2c();
	hal_lite_result res = pi2c->start(pi2c->hi2c, usSlaveAddr,0);
	int32_t tmp_size = (int32_t)*pusSize;
	if (is_hal_lite_success(res))
	{
		res = pi2c->write(pi2c->hi2c, pucBuff, tmp_size);
		if (is_hal_lite_success(res))res = pi2c->stop(pi2c->hi2c);
	}
	
	if (is_hal_lite_success(res)){
		//operation_wait();	
		return(0);
	}else return res;
	return 0;
    */

    #if 1
    vatek_result result = vatek_result_unknown;

    if (pucBuff == NULL || pusSize == NULL)
        return vatek_result_invalidparm;
    
    usSlaveAddr = usSlaveAddr<<1;
    if ((result = g_vi2c->start(g_vi2c->hboard, usSlaveAddr, 0)) != vatek_result_success)
        return vatek_result_i2cfail;

    if ((result = g_vi2c->write(g_vi2c->hboard, pucBuff, *pusSize)) != vatek_result_success)
        return vatek_result_i2cfail;

    if ((result = g_vi2c->stop(g_vi2c->hboard)) != vatek_result_success)
        return vatek_result_i2cfail;

    return result;
    #endif
    #if 0
    HAL_StatusTypeDef ret = 0;
    usSlaveAddr = usSlaveAddr<<1;
    ret = HAL_I2C_Master_Transmit(g_vi2c->hboard, usSlaveAddr, pucBuff, *pusSize, 200);
    printf("ret = %d\n", ret);

    return ret;
    #endif
}

AVL_uint32 AVL_IBSP_Initialize(void)
{
    return(0);
}

AVL_uint32 AVL_IBSP_InitSemaphore(AVL_psemaphore pSemaphore)
{
    return(0);
}

AVL_uint32 AVL_IBSP_ReleaseSemaphore(AVL_psemaphore pSemaphore)
{
	//operation_wait();
    return(0);
}

AVL_uint32 AVL_IBSP_WaitSemaphore(AVL_psemaphore pSemaphore)
{
    return(0);
}

AVL_uint32 AVL_IBSP_Dispose(void)
{
    return(0);
}



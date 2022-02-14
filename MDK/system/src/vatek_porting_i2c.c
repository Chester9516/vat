
#include "vatek_api.h"

#if defined(STM32F407xx) || defined(STM32F401xC)
#include <stm32f4xx.h>

#define STM32_LL_I2C_TIMEOUT        200

#define IS_CR1_FLAG_SET(a,flag)     ((a->Instance->CR1 & flag) == flag)
#define CR1_FLAG_SET(a,flag)        (a->Instance->CR1 |= flag)
#define CR1_FLAG_CLR(a,flag)        (a->Instance->CR1 &= ~flag)
#define DR_SET(a,byte)              (a->Instance->DR = byte)
#define DR_GET(a)                   (a->Instance->DR)

static vatek_result ll_i2c_wait_flag(I2C_HandleTypeDef* hi2c,uint32_t flag,FlagStatus status)
{
    uint32_t ntick = HAL_GetTick();
    
    while((__HAL_I2C_GET_FLAG(hi2c,flag) ? SET : RESET) != status)
    {
        if((HAL_GetTick() - ntick) > STM32_LL_I2C_TIMEOUT)
            return vatek_result_timeout;
    }
    
    return vatek_result_success;
}

static vatek_result ll_i2c_wait_flag_clr(I2C_HandleTypeDef* hi2c,uint32_t flag)
{
    return ll_i2c_wait_flag(hi2c,flag,RESET);
}

static vatek_result ll_i2c_wait_flag_set(I2C_HandleTypeDef* hi2c,uint32_t flag)
{
    return ll_i2c_wait_flag(hi2c,flag,SET);
}

static vatek_result ll_i2c_send_start(I2C_HandleTypeDef* hi2c)
{
    CR1_FLAG_SET(hi2c,I2C_CR1_START);
    return ll_i2c_wait_flag_set(hi2c,I2C_FLAG_SB); 
}

static vatek_result ll_i2c_check_busy(I2C_HandleTypeDef* hi2c, uint8_t lock)
{
    vatek_result result = vatek_result_badstatus;
    
    if(hi2c->Lock == HAL_LOCKED)
    {
		if(lock == 0)
            return vatek_result_success;
		else
		    return vatek_result_badstatus;
	}
	
    result = ll_i2c_wait_flag_clr(hi2c,I2C_FLAG_BUSY);
    if (result == vatek_result_success && lock)
        hi2c->Lock = HAL_LOCKED;
    
    return result;
}

static vatek_result ll_i2c_set_device_address(I2C_HandleTypeDef *si2c,uint8_t devaddr,int32_t nwrite)
{
    vatek_result result = vatek_result_success;
    
    if ((result = ll_i2c_wait_flag_set(si2c,I2C_FLAG_SB)) == vatek_result_success)
    {
        if(nwrite)
            DR_SET(si2c,I2C_7BIT_ADD_WRITE(devaddr));
        else
            DR_SET(si2c,I2C_7BIT_ADD_READ(devaddr));
        
        result = ll_i2c_wait_flag_set(si2c,I2C_FLAG_ADDR);
    }
    
    if(__HAL_I2C_GET_FLAG(si2c, I2C_FLAG_AF) == SET)
    {
        /* Generate Stop */
    }
    else
    {
		si2c->State = HAL_I2C_STATE_READY;
        /* Clear ADDR flag */
        __HAL_I2C_CLEAR_ADDRFLAG(si2c); 
	}
    
    return result;    
}

#endif

vatek_result vatek_porting_i2c_set_speed(Pboard_handle hboard, uint32_t speedkhz)
{
    vatek_result result = vatek_result_success;
    return result;
}

vatek_result vatek_porting_i2c_start(Pboard_handle hboard, uint8_t devaddr, uint32_t restart)
{
    vatek_result result = vatek_result_success;

#if defined(STM32F407xx) || defined(STM32F401xC)
    int32_t iswrite = !(devaddr & 1);
    uint8_t addr = devaddr & 0xFE;
	uint8_t is_lock_i2c = (restart)? 0:1;
    I2C_HandleTypeDef* si2c = (I2C_HandleTypeDef*)hboard;

    if ((result = ll_i2c_check_busy(si2c, is_lock_i2c)) != vatek_result_success)
        return result;

    if(!IS_CR1_FLAG_SET(si2c,I2C_CR1_PE))
        CR1_FLAG_SET(si2c,I2C_CR1_PE);

    CR1_FLAG_CLR(si2c,I2C_CR1_POS);       
    if ((result = ll_i2c_send_start(si2c)) != vatek_result_success)
    {
        si2c->Lock = HAL_UNLOCKED;
        return result;
    }
    
    result = ll_i2c_set_device_address(si2c,addr,iswrite);
#endif
    
    return result;
}

vatek_result vatek_porting_i2c_write(Pboard_handle hboard, uint8_t* pbuf, uint32_t len)
{
    vatek_result result = vatek_result_success;

#if defined(STM32F407xx) || defined(STM32F401xC)
    I2C_HandleTypeDef* si2c = (I2C_HandleTypeDef*)hboard;
  
    while(len && result == vatek_result_success)
    {
        if ((result = ll_i2c_wait_flag_set(si2c,I2C_FLAG_TXE)) != vatek_result_success)
            break;

        DR_SET(si2c,*pbuf++);
        len--;
        result = ll_i2c_wait_flag_set(si2c,I2C_FLAG_BTF);
    }

    if (result == vatek_result_success)
        result = ll_i2c_wait_flag_set(si2c,I2C_FLAG_TXE);
#endif
    
    return result;
}

vatek_result vatek_porting_i2c_read(Pboard_handle hboard, uint8_t* pbuf, uint32_t len)
{
    vatek_result result = vatek_result_success;
    
#if defined(STM32F407xx) || defined(STM32F401xC)
    I2C_HandleTypeDef* si2c = (I2C_HandleTypeDef*)hboard;
    if(len == 1)
    {
        CR1_FLAG_CLR(si2c,I2C_CR1_ACK);
    }
    else if(len == 2)
    {
        CR1_FLAG_CLR(si2c,I2C_CR1_ACK);
        CR1_FLAG_SET(si2c,I2C_CR1_POS);
    }
    else 
        CR1_FLAG_SET(si2c,I2C_CR1_ACK);

    __HAL_I2C_CLEAR_ADDRFLAG(si2c);

    while(len && result == vatek_result_success)
    {
        if(len <= 3)
        {
            if(len == 1)
            {
                if ((result = ll_i2c_wait_flag_set(si2c,I2C_FLAG_RXNE)) == vatek_result_success)
                {
                    *pbuf++ = DR_GET(si2c);
                    len--;
                }
            }
            else if(len == 2)
            {
                if ((result = ll_i2c_wait_flag_set(si2c,I2C_FLAG_BTF)) == vatek_result_success)
                {
                    *pbuf++ = DR_GET(si2c);
                    len--;
                    *pbuf++ = DR_GET(si2c);
                    len--;
                }
            }
            else
            {
                result = ll_i2c_wait_flag_set(si2c,I2C_FLAG_BTF);
                CR1_FLAG_CLR(si2c,I2C_CR1_ACK);                  
                if(result == vatek_result_success)
                {
                    *pbuf++ = DR_GET(si2c);
                    len--;
                    if ((result = ll_i2c_wait_flag_set(si2c,I2C_FLAG_BTF)) == vatek_result_success)      
                    {
                        *pbuf++ = DR_GET(si2c);
                        len--;
                        *pbuf++ = DR_GET(si2c);
                        len--;
                    }
                }
            }
        }
        else
        {
            result = ll_i2c_wait_flag_set(si2c,I2C_FLAG_RXNE);
            if (result != vatek_result_success)
                return result;

            *pbuf++ = DR_GET(si2c);
            len--;
            if(__HAL_I2C_GET_FLAG(si2c, I2C_FLAG_BTF) == SET){
                *pbuf++ = DR_GET(si2c);
                len--;
            }        
        }    
    }
#endif

    return result;
}

vatek_result vatek_porting_i2c_stop(Pboard_handle hboard)
{
    vatek_result result = vatek_result_success;

#if defined(STM32F407xx) || defined(STM32F401xC)
    I2C_HandleTypeDef* si2c = (I2C_HandleTypeDef*)hboard;
	si2c->Instance->CR1 |= I2C_CR1_STOP;
    si2c->State = HAL_I2C_STATE_READY;
    si2c->Lock = HAL_UNLOCKED;
#endif

    return result;
}



#include "vatek_api.h"

#if defined(GD32F30X_HD)
#include "gd32f30x.h"
#endif

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

#if defined(GD32F30X_HD)
		int32_t iswrite = !(devaddr & 1);
		if(!restart)
			
		/* wait until I2C bus is idle */
    while(i2c_flag_get(I2C0, I2C_FLAG_I2CBSY));

    /* send a start condition to I2C bus */
    i2c_start_on_bus(I2C0);
    
    /* wait until SBSEND bit is set */
    while(!i2c_flag_get(I2C0, I2C_FLAG_SBSEND));
		
		if(iswrite)
		/* send slave address to I2C bus */
			i2c_master_addressing(I2C0, devaddr, I2C_TRANSMITTER); 
    else
			i2c_master_addressing(I2C0, devaddr, I2C_RECEIVER); 
    /* wait until ADDSEND bit is set */
    while(!i2c_flag_get(I2C0, I2C_FLAG_ADDSEND));
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

#if defined(GD32F30X_HD)
		
		/* clear the ADDSEND bit */
    i2c_flag_clear(I2C0,I2C_FLAG_ADDSEND); //check whether chip need restart clear
    
    /* wait until the transmit data buffer is empty */
    while(SET != i2c_flag_get(I2C0, I2C_FLAG_TBE));
		
		while(len && result == vatek_result_success){
			/* wait until I2C bus is idle */
			i2c_data_transmit(I2C0, *pbuf++);
			/* wait until BTC bit is set */
			while(!i2c_flag_get(I2C0, I2C_FLAG_BTC));
			
			len--;
		}

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

#if defined(GD32F30X_HD)

    if(len < 3){
        /* disable acknowledge */
        i2c_ack_config(I2C0,I2C_ACK_DISABLE);
    }
    
    /* clear the ADDSEND bit */
    i2c_flag_clear(I2C0,I2C_FLAG_ADDSEND);
    
    if(1 == len){
        /* send a stop condition to I2C bus */
        i2c_stop_on_bus(I2C0);
    }
    
    /* while there is data to be read */
    while(len){
        if(3 == len){
            /* wait until BTC bit is set */
            while(!i2c_flag_get(I2C0, I2C_FLAG_BTC));

            /* disable acknowledge */
            i2c_ack_config(I2C0,I2C_ACK_DISABLE);
        }
        if(2 == len){
            /* wait until BTC bit is set */
            while(!i2c_flag_get(I2C0, I2C_FLAG_BTC));
            
        }
        
        /* wait until the RBNE bit is set and clear it */
        if(i2c_flag_get(I2C0, I2C_FLAG_RBNE)){
            /* read a byte from the EEPROM */
            *pbuf = i2c_data_receive(I2C0);
            
            /* point to the next location where the byte read will be saved */
            pbuf++;
            
            /* decrement the read bytes counter */
            len--;
        } 
    }
    
    /* wait until the stop condition is finished */
    while(I2C_CTL0(I2C0)&0x0200);
    
    /* enable acknowledge */
    i2c_ack_config(I2C0,I2C_ACK_ENABLE);

    i2c_ackpos_config(I2C0,I2C_ACKPOS_CURRENT);
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
	
#if defined(GD32F30X_HD)
	if(i2c_flag_get(I2C0, I2C_FLAG_I2CBSY) == SET){
		i2c_stop_on_bus(I2C0);
		/* wait until the stop condition is finished */
    while(I2C_CTL0(I2C0)&0x0200);
	}
#endif
    return result;
}


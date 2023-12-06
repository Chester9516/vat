
#include "vatek_api.h"

#if defined(STM32F407xx) || defined(STM32F401xC)
#include "stm32f4xx.h"
#include "gpio.h"
#endif

#if defined(GD32F30X_HD)
#include "systick.h"
#include "main.h"
#endif
uint32_t vatek_porting_system_gettick(void)
{
    uint32_t ret = 0;
    
#if defined(STM32F407xx) || defined(STM32F401xC)
    ret = HAL_GetTick();     
#endif

#if defined(GD32F30X_HD)
		
#endif
    return ret;
}

vatek_result vatek_porting_system_delay(uint32_t msec)
{
    vatek_result result = vatek_result_success;
    
#if defined(STM32F407xx) || defined(STM32F401xC)
    HAL_Delay(msec);     
#endif

#if defined(GD32F30X_HD)
		delay_1ms(msec);
#endif
	
    return result;
}

vatek_result vatek_porting_system_mainreset(Pboard_handle hboard)
{
    vatek_result result = vatek_result_success;
    
#if defined(STM32F407xx) || defined(STM32F401xC)
    #if defined(VATEK_B3) || defined(VATEK_B2) || defined(VATEK_A3)
    HAL_GPIO_WritePin(VATEK_RST_GPIO_Port, VATEK_RST_Pin, GPIO_PIN_SET);
    HAL_Delay(50);
    HAL_GPIO_WritePin(VATEK_RST_GPIO_Port, VATEK_RST_Pin, GPIO_PIN_RESET);
    HAL_Delay(50);
    HAL_GPIO_WritePin(VATEK_RST_GPIO_Port, VATEK_RST_Pin, GPIO_PIN_SET);
    HAL_Delay(50);
    #endif
#endif
	
#if defined(GD32F30X_HD)
	#if defined(VATEK_B3) || defined(VATEK_B2) || defined(VATEK_A3) ||defined(VATEK_B3_PLUS)
		gpio_bit_write(VATEK_RST_GPIO_Port, VATEK_RST_Pin, SET);
    delay_1ms(50);
    gpio_bit_write(VATEK_RST_GPIO_Port, VATEK_RST_Pin, RESET);
    delay_1ms(50);
    gpio_bit_write(VATEK_RST_GPIO_Port, VATEK_RST_Pin, SET);
    delay_1ms(50);
	#endif
#endif

    return result;
}

vatek_result vatek_porting_system_phyreset(Pboard_handle hboard, phy_type type)
{
    vatek_result result = vatek_result_success;
    
#if defined(STM32F407xx) || defined(STM32F401xC)
    switch (type)
    {
        #if defined(PHY_EP9555E) || defined(PHY_EP9351)
        case phy_type_ep9351:
        case phy_type_ep9555e:
            HAL_GPIO_WritePin(EP_RST_GPIO_Port, EP_RST_Pin, GPIO_PIN_SET);
            HAL_Delay(50);
            HAL_GPIO_WritePin(EP_RST_GPIO_Port, EP_RST_Pin, GPIO_PIN_RESET);
            HAL_Delay(50);
            HAL_GPIO_WritePin(EP_RST_GPIO_Port, EP_RST_Pin, GPIO_PIN_SET);
            HAL_Delay(50);
            break;
        #endif

        #if defined(PHY_ADV7182A)
        case phy_type_adv7182a_cvbs:
        case phy_type_adv7182a_ypbpr:
            HAL_GPIO_WritePin(AV_RST_GPIO_Port, AV_RST_Pin, GPIO_PIN_SET);
            HAL_Delay(50);
            HAL_GPIO_WritePin(AV_RST_GPIO_Port, AV_RST_Pin, GPIO_PIN_RESET);
            HAL_Delay(50);
            HAL_GPIO_WritePin(AV_RST_GPIO_Port, AV_RST_Pin, GPIO_PIN_SET);
            HAL_Delay(50);
            break;
        #endif

        #if defined(PHY_ADV7611)
        case phy_type_adv7611:
            HAL_GPIO_WritePin(ADV7611_RST_GPIO_Port, ADV7611_RST_Pin, GPIO_PIN_SET);
            HAL_Delay(50);
            HAL_GPIO_WritePin(ADV7611_RST_GPIO_Port, ADV7611_RST_Pin, GPIO_PIN_RESET);
            HAL_Delay(50);
            HAL_GPIO_WritePin(ADV7611_RST_GPIO_Port, ADV7611_RST_Pin, GPIO_PIN_SET);
            HAL_Delay(50);
            break;
        #endif

        #if defined(PHY_ADV7180)
        case phy_type_adv7180:
            HAL_GPIO_WritePin(ADV7180_RST_GPIO_Port, ADV7180_RST_Pin, GPIO_PIN_SET);
            HAL_Delay(50);
            HAL_GPIO_WritePin(ADV7180_RST_GPIO_Port, ADV7180_RST_Pin, GPIO_PIN_RESET);
            HAL_Delay(50);
            HAL_GPIO_WritePin(ADV7180_RST_GPIO_Port, ADV7180_RST_Pin, GPIO_PIN_SET);
            HAL_Delay(50);
            break;
        #endif
        
        #if defined(PHY_H1)
        case phy_type_h1:
            HAL_GPIO_WritePin(H1_RST_GPIO_Port, H1_RST_Pin, GPIO_PIN_SET);
            HAL_Delay(50);
            HAL_GPIO_WritePin(H1_RST_GPIO_Port, H1_RST_Pin, GPIO_PIN_RESET);
            HAL_Delay(50);
            HAL_GPIO_WritePin(H1_RST_GPIO_Port, H1_RST_Pin, GPIO_PIN_SET);
            HAL_Delay(50);
            break;
        #endif

        default:
            break;
    }
#endif

#if defined(GD32F30X_HD)
		switch (type)
    {
        #if defined(PHY_EP9555E) || defined(PHY_EP9351)
        case phy_type_ep9351:
        case phy_type_ep9555e:
            gpio_bit_write(EP_RST_GPIO_Port, EP_RST_Pin, SET);
            delay_1ms(50);
            gpio_bit_write(EP_RST_GPIO_Port, EP_RST_Pin, RESET);
            delay_1ms(50);
            gpio_bit_write(EP_RST_GPIO_Port, EP_RST_Pin, SET);
            delay_1ms(50);
            break;
        #endif

        #if defined(PHY_ADV7182A)
        case phy_type_adv7182a_cvbs:
        case phy_type_adv7182a_ypbpr:
            gpio_bit_write(AV_RST_GPIO_Port, AV_RST_Pin, SET);
            delay_1ms(50);
            gpio_bit_write(AV_RST_GPIO_Port, AV_RST_Pin, RESET);
            delay_1ms(50);
            gpio_bit_write(AV_RST_GPIO_Port, AV_RST_Pin, SET);
            delay_1ms(50);
            break;
        #endif

        #if defined(PHY_ADV7611)
        case phy_type_adv7611:
            gpio_bit_write(ADV7611_RST_GPIO_Port, ADV7611_RST_Pin, SET);
            delay_1ms(50);
            gpio_bit_write(ADV7611_RST_GPIO_Port, ADV7611_RST_Pin, RESET);
            delay_1ms(50);
            gpio_bit_write(ADV7611_RST_GPIO_Port, ADV7611_RST_Pin, SET);
            delay_1ms(50);
            break;
        #endif

        #if defined(PHY_ADV7180)
        case phy_type_adv7180:
            gpio_bit_write(ADV7180_RST_GPIO_Port, ADV7180_RST_Pin, SET);
            delay_1ms(50);
            gpio_bit_write(ADV7180_RST_GPIO_Port, ADV7180_RST_Pin, RESET);
            delay_1ms(50);
            gpio_bit_write(ADV7180_RST_GPIO_Port, ADV7180_RST_Pin, SET);
            delay_1ms(50);
            break;
        #endif
        
        #if defined(PHY_H1)
        case phy_type_h1:
            gpio_bit_write(H1_RST_GPIO_Port, H1_RST_Pin, SET);
            delay_1ms(50);
            gpio_bit_write(H1_RST_GPIO_Port, H1_RST_Pin, RESET);
            delay_1ms(50);
            gpio_bit_write(H1_RST_GPIO_Port, H1_RST_Pin, SET);
            delay_1ms(50);
            break;
        #endif

        default:
            break;
    }
#endif
    return result;
}

vatek_result vatek_porting_system_rfreset(Pboard_handle hboard, rf_type type)
{
    vatek_result result = vatek_result_success;
    
#if defined(STM32F407xx) || defined(STM32F401xC)
    switch (type)
    {
        #if defined(RF_R2)
        case rf_type_r2:
            HAL_GPIO_WritePin(RF_RST_GPIO_Port, RF_RST_Pin, GPIO_PIN_SET); ;
            HAL_Delay(50);
            HAL_GPIO_WritePin(RF_RST_GPIO_Port, RF_RST_Pin, GPIO_PIN_RESET);
            HAL_Delay(50);
            HAL_GPIO_WritePin(RF_RST_GPIO_Port, RF_RST_Pin, GPIO_PIN_SET);
            HAL_Delay(50);
            break;
        #endif
        
        default:
            break;
    }
#endif
	
#if defined(GD32F30X_HD)
		switch (type)
    {
        #if defined(RF_R2)
        case rf_type_r2:
            gpio_bit_write(RF_RST_GPIO_Port, RF_RST_Pin, GPIO_PIN_SET); ;
            delay_1ms(50);
            gpio_bit_write(RF_RST_GPIO_Port, RF_RST_Pin, GPIO_PIN_RESET);
            delay_1ms(50);
            gpio_bit_write(RF_RST_GPIO_Port, RF_RST_Pin, GPIO_PIN_SET);
            delay_1ms(50);
            break;
        #endif
        
        default:
            break;
    }
#endif
    return result;
}

vatek_result vatek_porting_system_tunerreset(Pboard_handle hboard, tuner_type type)
{
    vatek_result result = vatek_result_success;
   
#if defined(STM32F407xx) || defined(STM32F401xC)
    switch (type)
    {
        #if defined(TUNER_MXL608)
        case tuner_type_mxl608:
            HAL_GPIO_WritePin(AV_RST_GPIO_Port, AV_RST_Pin, GPIO_PIN_SET);
            HAL_Delay(50);
            HAL_GPIO_WritePin(AV_RST_GPIO_Port, AV_RST_Pin, GPIO_PIN_RESET);
            HAL_Delay(50);
            HAL_GPIO_WritePin(AV_RST_GPIO_Port, AV_RST_Pin, GPIO_PIN_SET);
            HAL_Delay(50);
            break;
        #endif
        
        default:
            break;
    }
#endif
	
#if defined(GD32F30X_HD)
		switch (type)
    {
        #if defined(TUNER_MXL608)
        case tuner_type_mxl608:
            gpio_bit_write(AV_RST_GPIO_Port, AV_RST_Pin, GPIO_PIN_SET);
            delay_1ms(50);
            gpio_bit_write(AV_RST_GPIO_Port, AV_RST_Pin, GPIO_PIN_RESET);
            delay_1ms(50);
            gpio_bit_write(AV_RST_GPIO_Port, AV_RST_Pin, GPIO_PIN_SET);
            delay_1ms(50);
            break;
        #endif
        
        default:
            break;
    }
#endif
    return result;

}

vatek_result vatek_porting_system_demodreset(Pboard_handle hboard, demod_type type)
{
    vatek_result result = vatek_result_success;
    
#if defined(STM32F407xx) || defined(STM32F401xC)
    switch (type)
    {
        #if defined(DEMOD_AVL68XX)
        case demod_type_avl68xx:
            HAL_GPIO_WritePin(EP_RST_GPIO_Port, EP_RST_Pin, GPIO_PIN_SET);
            HAL_Delay(50);
            HAL_GPIO_WritePin(EP_RST_GPIO_Port, EP_RST_Pin, GPIO_PIN_RESET);
            HAL_Delay(50);
            HAL_GPIO_WritePin(EP_RST_GPIO_Port, EP_RST_Pin, GPIO_PIN_SET);
            HAL_Delay(50);
            break;
        #endif

        #if defined(DEMOD_LGDT3305)
        case demod_type_lgdt3305:
            HAL_GPIO_WritePin(TUNER_RST_GPIO_Port, TUNER_RST_Pin, GPIO_PIN_SET);
            HAL_Delay(50);
            HAL_GPIO_WritePin(TUNER_RST_GPIO_Port, TUNER_RST_Pin, GPIO_PIN_RESET);
            HAL_Delay(50);
            HAL_GPIO_WritePin(TUNER_RST_GPIO_Port, TUNER_RST_Pin, GPIO_PIN_SET);
            HAL_Delay(50);
            break;
        #endif

        default:
            break;
    }
#endif
		
#if defined(GD32F30X_HD)
		switch (type)
    {
        #if defined(DEMOD_AVL68XX)
        case demod_type_avl68xx:
            gpio_bit_write(EP_RST_GPIO_Port, EP_RST_Pin, GPIO_PIN_SET);
            delay_1ms(50);
            gpio_bit_write(EP_RST_GPIO_Port, EP_RST_Pin, GPIO_PIN_RESET);
            delay_1ms(50);
            gpio_bit_write(EP_RST_GPIO_Port, EP_RST_Pin, GPIO_PIN_SET);
            delay_1ms(50);
            break;
        #endif

        #if defined(DEMOD_LGDT3305)
        case demod_type_lgdt3305:
            gpio_bit_write(TUNER_RST_GPIO_Port, TUNER_RST_Pin, GPIO_PIN_SET);
            delay_1ms(50);
            gpio_bit_write(TUNER_RST_GPIO_Port, TUNER_RST_Pin, GPIO_PIN_RESET);
            delay_1ms(50);
            gpio_bit_write(TUNER_RST_GPIO_Port, TUNER_RST_Pin, GPIO_PIN_SET);
            delay_1ms(50);
            break;
        #endif

        default:
            break;
    }
#endif
    return result;

}


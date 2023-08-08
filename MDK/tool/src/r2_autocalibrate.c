
#include <stdio.h>
#include <stdlib.h>  
#include <string.h>
#include "vatek_api.h"
#include "vatek_system.h"
#include "r2_autocalibrate_internal.h"

static Phbroadcast bc_handle = NULL;
static Phrf rf_handle = NULL;
static Pvatek_uart uart_handle = NULL;
static r2_tune_parm r2tune = {0};

static uint8_t receivebuf[R2_TUNE_BUF_LENGTH] = {0};
static uint8_t uart_receive(uint8_t **ptr)
{
    uint8_t ret = 0;
    uint8_t tmp = 0;

    if (ptr == NULL)
        return 0;

    memset(receivebuf, '\0', R2_TUNE_BUF_LENGTH);
    *ptr = &receivebuf[0];
    
    while (uart_handle->receive(uart_handle->hboard, &tmp, 1, 300) == vatek_result_success)
    {
        if (tmp == '\n' || tmp == '\r')
            break;
    
        receivebuf[ret] = tmp;
        ret++;
        if (ret >= R2_TUNE_BUF_LENGTH)
            break;
    }

    return ret;
}

static void uart_transmit(uint8_t *ptr, uint32_t len)
{
    int i = 0;
    while (*ptr && (i < len))
    {
        uart_handle->transmit(uart_handle->hboard, ptr, 1, 2000);
        ptr++;
        i++;
    }
}

static uint32_t str_to_hex(char *str){
	uint32_t hexVal = 0;
    
	hexVal = strtol(str, NULL, 16);	

	return hexVal;
}

static vatek_result r2autok_playsinewave(void)
{ 
    // (VATEK) ==> (rf)
    vatek_result result = vatek_result_unknown;

    /* set MODULATOR parameter */
    modulator_base_parm mod_base_parm = {0};
    modulator_sinewave_parm mod_sinewave_parm = {0};
    mod_base_parm.type = m_type_sinewave;
    mod_sinewave_parm.freq = 1000;
    result = vatek_broadcast_modulator_setparm(bc_handle, mod_base_parm, (Pmodulator_advanced_parm)&mod_sinewave_parm);
    if (result != vatek_result_success)
    {
        R2AUTOK_ERR("vatek_broadcast_modulator_setparm fail: %d", result);
        return result;
    }

    /* start BROADCAST */
    result = vatek_broadcast_start(bc_handle);
    if (result != vatek_result_success)
    {
        R2AUTOK_ERR("vatek_broadcast_start fail: %d", result);
        return result;
    }

    return result;
}

static vatek_result r2autok_setcalibrate(void)
{
    vatek_result result = vatek_result_unknown;
    
    /* stop RF */
    result = vatek_rf_stop(rf_handle);
    if (!(result == vatek_result_success || result == vatek_result_badstatus))
    {
        R2AUTOK_ERR("vatek_rf_stop fail: %d", result);
        return result;
    }

    /* set RF calibrate */
    r2_calibrate cal_parm = {0};
    cal_parm.gpio = r2tune.gpio;
    cal_parm.gain = r2tune.gain;
    cal_parm.i_offset = r2tune.i_offset;
    cal_parm.q_offset = r2tune.q_offset;
    cal_parm.image_offset = r2tune.image_offset;
    cal_parm.phase_offset = r2tune.phase_offset;
    result = vatek_rf_setcalibrate(rf_handle, (Prf_calibrate)&cal_parm);
    if (result != vatek_result_success)
    {
        R2AUTOK_ERR("vatek_rf_setcalibrate fail: %d", result);
        return result;
    }

    R2AUTOK_LOG("gpio = 0x%lX", cal_parm.gpio);
    R2AUTOK_LOG("gain = 0x%lX", cal_parm.gain);
    R2AUTOK_LOG("i_offset = 0x%lX", cal_parm.i_offset);
    R2AUTOK_LOG("q_offset = 0x%lX", cal_parm.q_offset);
    R2AUTOK_LOG("image_offset = 0x%lX", cal_parm.image_offset);
    R2AUTOK_LOG("phase_offset = 0x%lX", cal_parm.phase_offset);

    /* start RF */
    result = vatek_rf_start(rf_handle, r2tune.frequency);
    if (result != vatek_result_success)
    {
        R2AUTOK_ERR("vatek_rf_start fail: %d", result);
        return result;
    }

    return result;
}

static void r2autok_tuneack(void)
{
    uint8_t val = 0;
    
    val = FC_READ_R2_TUNE_ACK;
    uart_transmit(&val ,1);
}

static uint8_t r2autok_receivecmd(uint8_t **cmd)
{
    uint8_t len = 0;
    uint8_t *ptr = NULL;

    if (cmd == NULL)
        return 0;

    *cmd = NULL;
    len = uart_receive(&ptr);
    if (len <= 0)
        return 0;

    if (len == 1 && ptr[0] == FC_READ_R2_TUNE)
    {
        r2autok_tuneack();
        return 0;
    }
    else
    {
        *cmd = ptr;
        return len;
    }

    return len;
}

static uint8_t r2autok_parsingcmd(uint8_t *cmd, uint8_t len)
{
    uint8_t ret = 0;
    uint8_t tmp_buf[R2_TUNE_BUF_LENGTH] = "";
    char spliter = '#', header[2] = "##";
    char *buf = NULL;
    char *p[256], *loop_ptr1 = NULL,*loop_ptr2 = NULL;
    uint32_t in = 0;

    memcpy(tmp_buf, cmd, len);
    
    if(tmp_buf[0] != spliter || tmp_buf[1] != spliter) 
        return ret; 

    buf = (char *)tmp_buf;
    while((p[in] = strtok_r(buf, header, &loop_ptr1)) != NULL)
    {
        buf = p[in];
        while((p[in] = strtok_r(buf, &spliter, &loop_ptr2)) != NULL)
        {
            if(!strcmp(p[in], header)){  //search to the end of list
                break;
            }
            buf = NULL;
            in++;
        }
        buf = NULL;
    }

    switch (str_to_hex(p[0]))
    {
        case R2_TUNE_MODULATION:
            r2tune.path_tag = str_to_hex(p[1]);
            r2tune.frequency = str_to_hex(p[2]);
            r2tune.status = r2_tune_status_setfreq;
            ret = R2_TUNE_MODULATION;
            break;
            
        case R2_TUNE_R2_REG:
            r2tune.path_tag = str_to_hex(p[1]);
            r2tune.gain = str_to_hex(p[2]);
            r2tune.gpio = str_to_hex(p[3]);
            r2tune.i_offset = str_to_hex(p[4]) & 0xFF;
            r2tune.q_offset = str_to_hex(p[5]) & 0xFF;
            r2tune.image_offset = str_to_hex(p[6]);
            r2tune.phase_offset = str_to_hex(p[7]);
            r2tune.status = r2_tune_status_setcalibrate;
            ret = R2_TUNE_R2_REG;
            break;
            
        case R2_TUNE_START_TABLE:
            r2tune.status = r2_tune_status_startsavetable;
            ret = R2_TUNE_START_TABLE;
            break;

        case R2_TUNE_TABLE_RULE:
            r2tune.status = r2_tune_status_puttable;
            ret = R2_TUNE_TABLE_RULE;
            break;
            
        case R2_TUNE_END_TABLE:
            r2tune.status = r2_tune_status_endsavetable;
            ret = R2_TUNE_END_TABLE;
            break;

        case R2_TUNE_FINISH:
            r2tune.status = r2_tune_status_finish;
            ret = R2_TUNE_FINISH;
            break;
            
        default:
            break;

    }

    R2AUTOK_LOG("%d, %s",len, cmd);
    
    return ret;
}   

vatek_result r2_autocalibrate_init(Pboard_handle bh_main, Pboard_handle bh_rf, Pboard_handle bh_uart)
{
    vatek_result result = vatek_result_success;
    
    /* init VATEK broadcast */
    result = vatek_broadcast_create(bh_main, &bc_handle);
    if (result != vatek_result_success)
    {
        R2AUTOK_ERR("broadcast create fail : %d", result);
        return result;
    }
    
    /* init RF : r2_via_vatek */
    result = vatek_rf_create(bh_rf, rf_type_r2_via_vatek, &rf_handle);  
    if (result != vatek_result_success)
    {
        R2AUTOK_ERR("rf create fail : %d", result);
        return result;
    }
    
    /* broadcast sinewave */
    result = r2autok_playsinewave();
    if (result != vatek_result_success)
    {
        R2AUTOK_ERR("r2autok_sinewave fail: %d", result);
        return result;
    }

    /* create uart */
    result = vatek_system_uart_create(bh_uart, &uart_handle);  
    if (result != vatek_result_success)
    {
        R2AUTOK_ERR("vatek_system_uart_create fail : %d", result);
        return result;
    }

    return result;
}

vatek_result r2_autocalibrate_start(void)
{
    /*  PROCESS of TUNE R2
        <<<<<<<<DEVICE-MCU>>>>>>>>                      <<<<<<<<HOST-PC>>>>>>>>                                         
        1.                                  <--         FC_READ_R2_TUNE(0xbc)
        2.FC_READ_R2_TUNE_ACK(0xbb)         -->                     
        3.                                  <--         "##VA#", R2_TUNE_MODULATION(0x20),'#', PATH TAG(0x00/0x01)(1B),'#', Frequency(4B), "##","\r\n"
        4.(MCU Save changes to modulator,   
            start broadcasting...)
            FC_READ_R2_TUNE_ACK(0xbb)       --> 
            
     -->5.                                  <--         "##VA#", R2_TUNE_R2_REG(0x21),'#', PATH TAG(0x00/0x01)(1B) ,'#', PA reg4(2B),'#', FILTER reg5(1B),'#', I reg13(1B),'#', Q reg14(1B),'#', IMAGE reg15(2B),'#', PHASE reg16(2B),"##","\r\n"
     |  6.(HOST Save changes to R2)
     |    FC_READ_R2_TUNE_ACK(0xbb)         -->             
     <--7.                                              (DEVICE identify signal, and save register...)  
    
        ================================= SAVE FILE ==================================
        
        8.                                  <--         "##VA#", R2_TUNE_START_TABLE(0x22),'#', number of list(1B),'#', PATH TAG(0x00/0x01)(1B) ,'#', PATH TAG(0x00/0x01)(1B), "##","\r\n"
        9.FC_READ_R2_TUNE_ACK(0xbb)         -->
     -->10.                                 <--         "##VA#", R2_TUNE_TABLE_RULE(0x23),'#', PATH TAG(0x00/0x01)(1B),'#', Frequency(4B),'#', PA reg4(2B),'#', FILTER reg5(1B),'#', I reg13(1B),'#', Q reg14(1B),'#', IMAGE reg15(2B),'#', PHASE reg16(2B), "##","\r\n"
     <--11.FC_READ_R2_TUNE_ACK(0xbb)        -->         
        12.                                 <--         "##VA#", R2_TUNE_END_TABLE(0x24),'#', number of list(1B),'#', PATH TAG(0x00/0x01)(1B) ,'#', PATH TAG(0x00/0x01)(1B), "##","\r\n"
        13.FC_READ_R2_TUNE_ACK(0xbb)        -->         
        14.(Burn calibration tables to flash)
    */

    uint8_t *commnad = NULL;
    uint8_t len = 0;

    while(1)
    {
        switch (r2tune.status)
        {
            case r2_tune_status_idle:
                len = r2autok_receivecmd(&commnad);
                if (len)
                {
                    r2autok_parsingcmd(commnad, len);
                }
                break;

            case r2_tune_status_setfreq:
                r2tune.status = r2_tune_status_idle;
                r2autok_tuneack();
                break;
                
            case r2_tune_status_setcalibrate:
                r2autok_setcalibrate();
                r2tune.status = r2_tune_status_idle;
                r2autok_tuneack();
                break;

            case r2_tune_status_startsavetable:
                /*TODO: start save calibrate table, number of table*/
                r2tune.status = r2_tune_status_idle;
                r2autok_tuneack();
                break;
            
            case r2_tune_status_puttable:
                /*TODO: put calibrate table*/
                r2tune.status = r2_tune_status_idle;
                r2autok_tuneack();
                break;
                
            case r2_tune_status_endsavetable:
                /*TODO: end save calibrate table, number of table*/
                r2tune.status = r2_tune_status_idle;
                r2autok_tuneack();
                break;
                
            case r2_tune_status_finish:
                r2tune.status = r2_tune_status_idle;
                r2autok_tuneack();
                break;
                
            default:
                r2tune.status = r2_tune_status_idle;
                r2autok_tuneack();
                break;
        }
        
    }
}


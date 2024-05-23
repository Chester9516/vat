#include "vatek_define.h"
#include "vatek_epg.h"
#include "vatek_lic_arib_japan.h"

vatek_result hal_lite_write_table(Phms_handle pi2c,Phalreg_table ptable,int32_t tablesize)
{
    uint32_t val = 0;
    int32_t i = 0;
    vatek_result nres = vatek_hms_read_hal(pi2c,HALREG_SYS_STATUS_0,&val);
    if(is_success(nres))
    {
        if(val != SYS_STATUS_IDLE)nres = vatek_result_badstatus;
        else
        {
            nres = vatek_hms_read_hal(pi2c,HALREG_BROADCAST_CNTL,&val);
            if(is_success(nres))
            {
                if(val != 0)nres = vatek_result_badstatus;
            }
        }
    }
    
    if(is_success(nres))
    {
        for(i = 0;i < tablesize;i++)
        {
            nres = vatek_hms_write_hal(pi2c,ptable[i].address,ptable[i].defval);
            if(!is_success(nres))break;
        }
    }
    return nres;
}

vatek_result vatek_board_set_rule_prop(const char* name,uint32_t val)
{
    printf("set prop : %-32s - %08x\r\n",name,val);
    return licensed_prop_set_uint32(name,val);
}

vatek_result vatek_board_set_rule_string(const char* name,uint8_t* pbuf,int32_t len)
{
    
    #define ARIB_CHAR(ch)	(ch + 0x80)
    static uint8_t jis_buf[20];
    int32_t jislen = len + 2;
    int32_t i = 0;
    if(jislen > 20)jislen = 20;
    
    jis_buf[0] = 0x1b;
    jis_buf[1] = 0x7E;
    
    for(i = 2;i < jislen;i++)
    {
        jis_buf[i] = ARIB_CHAR(*pbuf++);
    }
    return licensed_prop_set_string(name,&jis_buf[0],jislen);
    //return licensed_prop_set_string(name,pbuf,len);
}
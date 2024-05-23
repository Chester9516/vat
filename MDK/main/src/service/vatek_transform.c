
#include "vatek_system.h"
#include "vatek_hms.h"
#include "vatek_tsp.h"
#include "vatek_enum.h"
#include "vatek_tsmux.h"
#include "vatek_modulator.h"
#include "vatek_psitable.h"
#include "vatek_psispec_rule.h"
#include "vatek_psispec_default.h"
#include "vatek_capture.h"


#define TRANSFORM_ERR(fmt,...)    VATEK_ERR(tf,fmt,##__VA_ARGS__)
#define TRANSFORM_LOG(fmt,...)    VATEK_LOG(tf,fmt,##__VA_ARGS__)

static vatek_result transform_ctrl(Phms_handle handle, uint32_t cmd, uint32_t *errcode)
{
    vatek_result result = vatek_result_unknown;
    uint32_t timeout = 0;

    if ((result = vatek_hms_write_hal(handle, HALREG_TRANSFORM_CNTL, cmd)) == vatek_result_success)
    {
        while (timeout < 50) //5 sec
        {
            vatek_system_delay(100);
            if ((result = vatek_hms_read_hal(handle, HALREG_TRANSFORM_CNTL, errcode)) != vatek_result_success)
                return result;
            if (*errcode == 0)
                break;
            timeout++;
        }

        if (timeout >= 50)
            result = vatek_result_timeout;
        else
        {
            if ((result = vatek_hms_read_hal(handle, HALREG_SYS_ERRCODE, errcode)) != vatek_result_success)
                return result;

            if (*errcode != HALREG_ERR_SUCCESS)
                result = vatek_result_hwfail;
        }
    }

    return result;
}

static vatek_result transform_reset(Phms_handle handle)
{
    vatek_result result = vatek_result_unknown;
    uint32_t timeout = 0;

    if (handle == NULL)
        return vatek_result_invalidparm;

    TRANSFORM_LOG("transform reset");
    if ((result = vatek_hms_reset(handle)) != vatek_result_success)
        return result;

    //polling until chip ready or timeout (5 sec)
    TRANSFORM_LOG("waiting chip ready");
    while (((result = vatek_hms_issystemidle(handle)) != vatek_result_success) && timeout <= 50)
    {  
        vatek_system_delay(100);
        timeout++;
    }
    
    if (timeout >= 50)
    {
        TRANSFORM_ERR("reset timeout");
        result = vatek_result_timeout;
    }
    
    TRANSFORM_LOG("chip ready");
    
    return result;
}

static vatek_result transform_getstatus(Phms_handle handle, broadcast_status *status)
{
    vatek_result result = vatek_result_unknown;
    uint32_t value = 0;

    if (handle == NULL || status == NULL)
        return vatek_result_invalidparm;

    *status = bc_status_unknown;
    if ((result = vatek_hms_read_hal(handle, HALREG_BCINFO_STATUS, &value)) != vatek_result_success)
        return result;

    switch (value)
    {
        case BCSTATUS_IDLE:
            *status = bc_status_idle; 
            break;

        case BCSTATUS_WAIT_SOURCE:
            *status = bc_status_wait_source;
            break;

        case BCSTATUS_BROADCAST:
            *status = bc_status_broadcast;
            break;

        case BCSTATUS_FINISH:
            *status = bc_status_finish;
            break;
            
        case BCSTATUS_FAIL_SOURCE:
            *status = bc_status_fail_source;
            break;

        case BCSTATUS_FAIL_TIMEOUT:
            *status = bc_status_fail_timeout;
            break;

        case BCSTATUS_FAIL_CODECDROP:
            *status = bc_status_fail_codecdrop;
            break;

        case BCSTATUS_FAIL_BUFFER:
            *status = bc_status_fail_buffer;
            break;

        case BCSTATUS_FAIL_MUXER:
            *status = bc_status_fail_muxer;
            break;

        case BCSTATUS_FAIL_ENCODE:
            *status = bc_status_fail_encode;
            break;

        case BCSTATUS_FAIL_MEDIA:
            *status = bc_status_fail_media;
            break;

        case BCSTATUS_FAIL_DEMUX:
            *status = bc_status_fail_demux;
            break;

        default:
        case BCSTATUS_FAIL_UNKNOWN:
            *status = bc_status_fail_unknown;
            break;
    }

    return result;
}

vatek_result vatek_transform_create(Pboard_handle hboard, Phtransform *handle)
{
    vatek_result result = vatek_result_unknown;
    Phms_handle hms_handle = NULL;
    
    if (hboard == NULL || handle == NULL)
        return vatek_result_invalidparm;

    *handle = NULL;
    if ((result = vatek_hms_create(hboard, &hms_handle)) != vatek_result_success)
        return result;
    hms_handle->type = hms_type_transform;
    *handle = hms_handle;
#if !defined(VATEK_V1)
    if ((result = transform_reset(*handle)) != vatek_result_success) //try not repeat V1 in V1 process 20231128
        return result;
#endif
    return result;
}

vatek_result vatek_transform_destroy(Phtransform handle)
{
    vatek_result result = vatek_result_unknown;

    if (handle == NULL)
        return vatek_result_invalidparm;

    result = vatek_hms_destroy((Phms_handle)handle);

    return result;
}

vatek_result vatek_transform_start(Phtransform handle)
{
    vatek_result result = vatek_result_unknown;
    uint32_t errcode;
    uint32_t timeout = 0;
    broadcast_status status = bc_status_unknown;
    
    if (handle == NULL)
        return vatek_result_invalidparm;

    if ((result = transform_ctrl(handle, BC_START, &errcode)) != vatek_result_success)
    {
        TRANSFORM_ERR("BC_START fail, errcode = 0x%lX", errcode);
        return result;
    }

    while (timeout < 20) // 2 sec
    {
        vatek_system_delay(100);
        if ((result = transform_getstatus(handle, &status)) != vatek_result_success)
            return result;

        if (status == bc_status_broadcast)
            break;
        
        timeout++;
    }
    
    return result;
}

vatek_result vatek_transform_stop(Phtransform handle)
{
    vatek_result result = vatek_result_unknown;
    
    if (handle == NULL)
        return vatek_result_invalidparm;
    
    uint32_t errcode;
    if ((result = transform_ctrl(handle, BC_STOP, &errcode)) != vatek_result_success)
        TRANSFORM_ERR("BC_STOP fail, errcode = 0x%lX", errcode);
    
    if ((result = transform_reset(handle)) != vatek_result_success)
        return result;

    return result;
}

vatek_result vatek_transform_reset(  Phtransform handle)
{
    vatek_result result = vatek_result_unknown;
    
    if( handle == NULL)
        return vatek_result_invalidparm;
    
    if( ( result = transform_reset( handle)) != vatek_result_success)
        return result;
    
    return result;
}

vatek_result vatek_transform_bcstatus(Phtransform handle, broadcast_status *status)
{
    vatek_result result = vatek_result_unknown;

    if (handle == NULL || status == NULL)
        return vatek_result_invalidparm;

    if ((result = transform_getstatus(handle, status)) != vatek_result_success)
        return result;
    
    return result;
}

vatek_result vatek_transform_chipstatus(Phtransform handle, chip_status *status)
{
    vatek_result result = vatek_result_unknown;
    uint32_t value = 0;

    if (handle == NULL || status == NULL)
        return vatek_result_invalidparm;

    *status = chip_status_unknown;
    if ((result = vatek_hms_read_hal(handle, HALREG_SYS_STATUS_0, &value)) != vatek_result_success)
        return result;

    if (value == 0xFFFFFFFF || value == 0)
    {
        *status = chip_status_idle;
        return result;
    }

    if((value & SYS_STATUS_TAG) == SYS_STATUS_TAG)
    {
        switch (value)
        {
            case SYS_STATUS_IDLE:
                *status = chip_status_wait_command;
                break;

            case SYS_STATUS_RUN:
                *status = chip_status_running;
                break;

            default:
                *status = chip_status_fail;
                break;
        }
    }
    else
        *status = chip_status_badfw;
    
    return result;
}

#if 1
vatek_result vatek_transform_enum_getlist(Phtransform handle, Penum_list *list)
{
    vatek_result result = vatek_result_unknown;
    
    if (handle == NULL || list == NULL)
        return vatek_result_invalidparm;

    if ((result = vatek_enum_reset((Phms_handle)handle)) != vatek_result_success)
        return result;

    if ((result = vatek_enum_create((Phms_handle)handle)) != vatek_result_success)
        return result;
    
    if ((result = vatek_enum_getlist((Phms_handle)handle, list)) != vatek_result_success)
        return result;
    
    if((result = vatek_enum_destroy((Phms_handle)handle)) != vatek_result_success)
        return result;

    return result;
}

vatek_result vatek_transform_capture(Phtransform handle, Ppsitable_parm *raw_table, Pcapture_param param)
{
    vatek_result result = vatek_result_unknown;
    
    if (handle == NULL || raw_table == NULL || param == NULL)
        return vatek_result_invalidparm;

    if ((result = vatek_capture_reset((Phms_handle)handle)) != vatek_result_success)
        return result;

    if ((result = vatek_capture_create((Phms_handle)handle, param)) != vatek_result_success)
        return result;
        
    if ((result = vatek_capture_gettable((Phms_handle)handle, raw_table)) != vatek_result_success)
        return result;
        
    if((result = vatek_capture_destroy((Phms_handle)handle)) != vatek_result_success)
        return result;

    return result;
}
#endif

vatek_result vatek_transform_tsp_setinputparm_ts(Phtransform handle, ts_input_parm parm)
{
    vatek_result result = vatek_result_unknown;
    
    if (handle == NULL)
        return vatek_result_invalidparm;

    result = vatek_tsp_setinputparm_ts((Phms_handle)handle, parm);

    return result;
}

vatek_result vatek_transform_tsp_setfilterparm(Phtransform handle, tsp_filter_parm parm)
{
    vatek_result result = vatek_result_unknown;
    
    if (handle == NULL)
        return vatek_result_invalidparm;

    result = vatek_tsp_setfilterparm((Phms_handle)handle, parm);

    return result;
}

vatek_result vatek_transform_tsmux_setparm(Phtransform handle, tsmux_type type)
{
    vatek_result result = vatek_result_unknown;

    if (handle == NULL)
        return vatek_result_invalidparm;

    result = vatek_tsmux_setparm((Phms_handle)handle, type, NULL);
    
    return result;
}

vatek_result vatek_transform_modulator_setparm(Phtransform handle, modulator_base_parm parm_base, Pmodulator_advanced_parm parm_adv)
{
    vatek_result result = vatek_result_unknown;

    if (handle == NULL || parm_adv == NULL)
        return vatek_result_invalidparm;

    result =  vatek_modulator_setparm((Phms_handle)handle, parm_base, parm_adv);
    
    return result;
}

vatek_result vatek_transform_psitable_register(Phtransform handle, Ppsitablelist_parm parm)
{
    vatek_result result = vatek_result_unknown;

    if (handle == NULL || parm == NULL)
        return vatek_result_invalidparm;

    result =  vatek_psitable_register((Phms_handle)handle, parm);
    
    return result;
}

vatek_result vatek_transform_psitable_insert(Phtransform handle, uint16_t tspacket_num, uint8_t *tspackets)
{
    vatek_result result = vatek_result_unknown;

    if (handle == NULL || tspackets == NULL)
        return vatek_result_invalidparm;

    result = vatek_psitable_insert((Phms_handle)handle, tspacket_num, tspackets);
    
    return result;
}

vatek_result vatek_transform_psispec_rule_init(Phtransform handle, Ppsispec_rule rule)
{
    vatek_result result = vatek_result_unknown;

    if (handle == NULL || rule == NULL)
        return vatek_result_invalidparm;

    if ((result = vatek_psispec_rule_init(rule)) != vatek_result_success)
    {
        return result;
    }
    
    return result;
}

vatek_result vatek_transform_psispec_rule_settablerule(Phtransform handle, const char *name, uint32_t version, uint32_t interval)
{
    vatek_result result = vatek_result_unknown;

    if (handle == NULL || name == NULL)
        return vatek_result_invalidparm;

    if ((result = vatek_psispec_rule_setruletable(name, version, interval)) != vatek_result_success)
    {
        return result;
    }

    return result;
}

vatek_result vatek_transform_psispec_rule_setchannelrule(Phtransform handle, const char *name, void *buf, uint32_t len)
{
    vatek_result result = vatek_result_unknown;

    if (handle == NULL || name == NULL)
        return vatek_result_invalidparm;

    if ((result = vatek_psispec_rule_setchannelprop(name, buf, len)) != vatek_result_success)
    {
        return result;
    }

    return result;
}

vatek_result vatek_transform_psispec_rule_start(Phtransform handle)
{
    vatek_result result = vatek_result_unknown;

    if (handle == NULL)
        return vatek_result_invalidparm;

    if ((result = vatek_psispec_rule_start_v2(handle)) != vatek_result_success)
    {
        return result;
    }

    if ((result = vatek_psispec_rule_end_v2(handle)) != vatek_result_success)
    {
        return result;
    }
    
    return result;
}

vatek_result vatek_transform_psispec_rule_addprogram_psip(Phtransform handle, Penum_program program, Ppsip_props props)
{
    vatek_result result = vatek_result_unknown;

    if (handle == NULL || program == NULL || props == NULL)
        return vatek_result_invalidparm;

    if ((result = vatek_psispec_rule_setprogramprop_psip(props)) != vatek_result_success)
    {
        return result;
    }

    if ((result = vatek_psispec_rule_addprogram((Phms_handle)handle,  program)) != vatek_result_success)
    {
        return result;
    }
        
    if ((result = vatek_psispec_rule_end_v2((Phms_handle)handle)) != vatek_result_success)
    {
        return result;
    }

    return result;
}

vatek_result vatek_transform_psispec_default_init(Phtransform handle, psispec_default_type type, psispec_country_code country)
{
    vatek_result result = vatek_result_unknown;
    
    if(handle==NULL) return vatek_result_invalidparm;
    
    result = vatek_psispec_default_init_tr((Phms_handle)handle, type, country);
    if(result!=vatek_result_success) return result;
    
    return result;
}

vatek_result vatek_transform_psispec_default_channel_config(Ppsispec_default_channel channel)
{
    vatek_result result = vatek_result_unknown;
    
    if(channel==NULL) return vatek_result_invalidparm;
    
    result = vatek_psispec_default_ch_config(channel);
    if(result!=vatek_result_success) return result;
    
    return result;
}

vatek_result vatek_transform_psispec_default_program_add(Ppsispec_default_program program)
{
    vatek_result result = vatek_result_unknown;
    
    if(program==NULL) return vatek_result_invalidparm;
    Ppsispec_default_program_tr prog = program;
    
    result = vatek_psispec_default_program_add(prog);
    if(result!=vatek_result_success) return result;
    
    return result;
}

vatek_result vatek_transform_psispec_default_program_end(void)
{
    vatek_result result = vatek_result_unknown;
    
    result = vatek_psispec_default_program_end();
    if(result!=vatek_result_success) return result;
    
    return result;
}

vatek_result vatek_transform_psispec_default_stream_add(Ppsispec_default_stream stream)
{
    vatek_result result = vatek_result_unknown;
    
    if(stream==NULL) return vatek_result_invalidparm;
    Ppsispec_default_stream_tr stream_tr = stream;
    
    result = vatek_psispec_default_stream_add(stream_tr);
    if(result!=vatek_result_success) return result;
    
    return result;
}

vatek_result vatek_transform_psispec_default_start(void)
{
    vatek_result result = vatek_result_unknown;
    
    result = vatek_psispec_default_start_tr();
    if(result!=vatek_result_success) return result;
    
    return result;
}

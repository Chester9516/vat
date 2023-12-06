
#include "vatek_system.h"
#include "vatek_hms.h"
#include "vatek_tsmux.h"

#define MAX_PID         0x1FFF
#define MIN_PID         0
#define MAX_PROGRAM_NUM 0xFFFF
#define MIN_PROGRAM_NUM 0
#define MAX_TSID        0xFFFF
#define MIN_TSID        0

static vatek_result tsmux_pure_setparm(Phms_handle handle, Ptsmux_pure_parm parm)
{
    vatek_result result = vatek_result_unknown;
    uint32_t en_function = 0;

    if (handle == NULL || parm == NULL)
        return vatek_result_unknown;

    if (parm->pcr_pid > MAX_PID)
    {
        TSMUX_ERR("pcr pid overrange");
        return vatek_result_overrange;
    }

    if (parm->padding_pid > MAX_PID)
    {
        TSMUX_ERR("padding pid overrange");
        return vatek_result_overrange;
    }
    
#if 0
    if (handle->type == hms_type_transform)
    {
        //disable PCR
        if ((result = vatek_hms_read_hal_v1(handle, HALREG_MUX_FLAGS, &remux_flag)) != vatek_result_success)
            return result;

        remux_flag |= MUX_FLAG_DISABLE_PCR;
        if ((result = vatek_hms_write_hal_v1(handle, HALREG_MUX_FLAGS, remux_flag)) != vatek_result_success)
            return result;
    }
#endif
    
    if ((result = vatek_hms_write_hal_v1(handle, HALREG_MUX_PCR_PID, parm->pcr_pid)) != vatek_result_success)
        return result;
    
    if ((result = vatek_hms_write_hal_v1(handle, HALREG_MUX_PADDING_PID, parm->padding_pid)) != vatek_result_success)
        return result;
    
    if ((result = vatek_hms_write_hal_v1(handle, HALREG_MUXPSI_MODE, MUX_PURE)) != vatek_result_success)
        return result;
    
    if ((result = vatek_hms_read_hal_v1(handle, HALREG_MUX_FLAGS, &en_function)) == vatek_result_success)
    {
        en_function |= parm->en_function;
        if ((result = vatek_hms_write_hal_v1(handle, HALREG_MUX_FLAGS, en_function)) != vatek_result_success)
            return result;
    }else return result;

    return result;
}

static vatek_result tsmux_iso13818_setparm(Phms_handle handle, Ptsmux_iso13818_parm parm)
{
    vatek_result result = vatek_result_unknown;

    if (parm->pmtpid > MAX_PID)
    {
        TSMUX_ERR("pmt pid overrange");
        return vatek_result_overrange;
    } 

    if (parm->tsid > MAX_TSID)
    {
        TSMUX_ERR("tsid overrange");
        return vatek_result_overrange;
    }
    
    if (parm->pcr_pid > MAX_PID)
    {
        TSMUX_ERR("pcr pid overrange");
        return vatek_result_overrange;
    }

    if (parm->padding_pid > MAX_PID)
    {
        TSMUX_ERR("padding pid overrange");
        return vatek_result_overrange;
    }

    if (parm->program_num > MAX_PROGRAM_NUM)
    {
        TSMUX_ERR("program num overrange");
        return vatek_result_overrange;
    }

    if ((result = vatek_hms_write_hal_v1(handle, HALREG_MUXPSI_MODE, MUX_ISO13818)) != vatek_result_success)
        return result;

    if ((result = vatek_hms_write_hal_v1(handle, HALREG_ENCODER_PMTPID, parm->pmtpid)) != vatek_result_success)
        return result;
    
    if ((result = vatek_hms_write_hal_v1(handle, HALREG_MUXPSI_ISO_TSID, parm->tsid)) != vatek_result_success)
        return result;
    
    if ((result = vatek_hms_write_hal_v1(handle, HALREG_MUXPSI_ISO_PROGRAM_NUM, parm->program_num)) != vatek_result_success)
        return result;
    
    if ((result = vatek_hms_write_hal_v1(handle, HALREG_MUX_PCR_PID, parm->pcr_pid)) != vatek_result_success)
        return result;
    
    if ((result = vatek_hms_write_hal_v1(handle, HALREG_MUX_PADDING_PID, parm->padding_pid)) != vatek_result_success)
        return result;

    return result;
}

static vatek_result tsmux_rule_setparm(Phms_handle handle, Ptsmux_rule_parm parm)
{
    vatek_result result = vatek_result_unknown;
    uint32_t en_function = 0;

    if (handle == NULL || parm == NULL)
        return vatek_result_unknown;

    if (parm->pcr_pid > MAX_PID)
    {
        TSMUX_ERR("pcr pid overrange");
        return vatek_result_overrange;
    }

    if (parm->padding_pid > MAX_PID)
    {
        TSMUX_ERR("padding pid overrange");
        return vatek_result_overrange;
    }
#if 0
    if (handle->type == hms_type_transform)
    {
        //disable PCR
        if ((result = vatek_hms_read_hal_v1(handle, HALREG_MUX_FLAGS, &remux_flag)) != vatek_result_success)
            return result;

        remux_flag |= MUX_FLAG_DISABLE_PCR;
        if ((result = vatek_hms_write_hal_v1(handle, HALREG_MUX_FLAGS, remux_flag)) != vatek_result_success)
            return result;
    }
#endif
    if ((result = vatek_hms_write_hal_v1(handle, HALREG_MUX_PCR_PID, parm->pcr_pid)) != vatek_result_success)
        return result;
    
    if ((result = vatek_hms_write_hal_v1(handle, HALREG_MUX_PADDING_PID, parm->padding_pid)) != vatek_result_success)
        return result;
    
    if ((result = vatek_hms_write_hal_v1(handle, HALREG_MUXPSI_MODE, MUX_PURE)) != vatek_result_success)
        return result;
    
    if ((result = vatek_hms_read_hal_v1(handle, HALREG_MUX_FLAGS, &en_function)) == vatek_result_success)
    {
        en_function |= parm->en_function;
        if ((result = vatek_hms_write_hal_v1(handle, HALREG_MUX_FLAGS, en_function)) != vatek_result_success)
            return result;
    }else return result;
    
    

    return result;
}

static vatek_result tsmux_default_setparm(Phms_handle handle, Ptsmux_default_parm parm)
{
    vatek_result result = vatek_result_unknown;
    uint32_t en_function = 0;

    if (handle == NULL || parm == NULL)
        return vatek_result_unknown;
    
    if (parm->pmt_pid > MAX_PID)
    {
        TSMUX_ERR("pmt pid overrange");
        return vatek_result_overrange;
    } 

    if (parm->pcr_pid > MAX_PID)
    {
        TSMUX_ERR("pcr pid overrange");
        return vatek_result_overrange;
    }

    if (parm->padding_pid > MAX_PID)
    {
        TSMUX_ERR("padding pid overrange");
        return vatek_result_overrange;
    }
    
#if 0
    if (handle->type == hms_type_transform)
    {
        //disable PCR
        if ((result = vatek_hms_read_hal_v1(handle, HALREG_MUX_FLAGS, &remux_flag)) != vatek_result_success)
            return result;

        remux_flag |= MUX_FLAG_DISABLE_PCR;
        if ((result = vatek_hms_write_hal_v1(handle, HALREG_MUX_FLAGS, remux_flag)) != vatek_result_success)
            return result;
    }
#endif
    if ((result = vatek_hms_write_hal_v1(handle, HALREG_MUXPSI_MODE, MUX_PURE)) != vatek_result_success)
        return result;
    
    if ((result = vatek_hms_write_hal_v1(handle, HALREG_ENCODER_PMTPID, parm->pmt_pid)) != vatek_result_success)
        return result;

    if ((result = vatek_hms_write_hal_v1(handle, HALREG_MUX_PCR_PID, parm->pcr_pid)) != vatek_result_success)
        return result;
    
    if ((result = vatek_hms_write_hal_v1(handle, HALREG_MUX_PADDING_PID, parm->padding_pid)) != vatek_result_success)
        return result;
    
    if ((result = vatek_hms_read_hal_v1(handle, HALREG_MUX_FLAGS, &en_function)) == vatek_result_success)
    {
        en_function |= parm->en_function;
        if ((result = vatek_hms_write_hal_v1(handle, HALREG_MUX_FLAGS, en_function)) != vatek_result_success)
            return result;
    }else return result;

    return result;
}

static vatek_result tsmux_transform_setparm(Phms_handle handle, tsmux_type type)
{
    vatek_result result = vatek_result_unknown;
    uint32_t val = 0;

    if (handle == NULL)
        return vatek_result_unknown;
    
    switch(type)
    {
        case tsmux_type_rule:
            val |= MUX_SPEC;
            break;

        case tsmux_type_pure:
            val |= MUX_PURE;
            break;  
        
        case tsmux_type_default:
            val |= MUX_DEFSPEC;
            break;
        case tsmux_type_iso13818:
        default :
            val |= MUX_PURE;
            break;
    }
#if 0
    //disable PCR
    if ((result = vatek_hms_read_hal_v1(handle, HALREG_MUX_FLAGS, &remux_flag)) == vatek_result_success)
    {
        remux_flag |= MUX_FLAG_DISABLE_PCR;
        if ((result = vatek_hms_write_hal_v1(handle, HALREG_MUX_FLAGS, remux_flag)) != vatek_result_success)
            return result;
    }else return result;
#endif    
    if ((result = vatek_hms_write_hal_v1(handle, HALREG_MUXPSI_MODE, val)) != vatek_result_success)
        return result;

    return result;
}

vatek_result vatek_tsmux_setparm_v1(Phms_handle handle, tsmux_type type, Ptsmux_parm parm)
{
    vatek_result result = vatek_result_unknown;

    // parm is null in transfrom serivce.
    if ((parm == NULL && handle->type == hms_type_broadcast) || handle == NULL)
        return vatek_result_invalidparm;

    if ((result = vatek_hms_issystemidle_v1(handle)) != vatek_result_success)
        return result;

    if (type > tsmux_type_max)
    {
        TSMUX_ERR("tsmux type overrange");
        return vatek_result_overrange;
    }

    if (handle->type == hms_type_transform && type == tsmux_type_iso13818)
    {
        TSMUX_ERR("unsupport iso13818 on transform");
        return vatek_result_unsupport;
    }
    
    if (handle->type == hms_type_transform)
    {
        result = tsmux_transform_setparm(handle, type);
    }
    else if (handle->type == hms_type_broadcast)
    {
        switch (type)
        {
            case tsmux_type_iso13818:
                result = tsmux_iso13818_setparm(handle, (Ptsmux_iso13818_parm)parm);
                break;

            case tsmux_type_rule:
                result = tsmux_rule_setparm(handle, (Ptsmux_rule_parm)parm);
                break;

            case tsmux_type_pure:
                result = tsmux_pure_setparm(handle, (Ptsmux_pure_parm)parm);
                break;  
            
            case tsmux_type_default:
                result = tsmux_default_setparm(handle, (Ptsmux_default_parm)parm);
                break;
            
            default :
                result = vatek_result_unsupport;
                break;
        }
    }

    if (result != vatek_result_success)
        TSMUX_ERR("tsmux setparm fail");

    return result;
}


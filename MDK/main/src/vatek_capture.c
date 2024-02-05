
#include "vatek_system.h"
#include "vatek_hms.h"
#include "vatek_capture.h"

static Ppsitable_parm g_capture_table = NULL;

static vatek_result capture_ctrl(Phms_handle handle, uint32_t cmd, uint32_t *errcode)
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

static vatek_result capture_getstatus(Phms_handle handle, broadcast_status *status)
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

static void *raw_malloc(void *ptr, uint32_t size)
{
    void *ret = NULL;
    if (ptr == NULL)
        ret = malloc(size);
    else 
        ret = realloc(ptr, size);

    return ret;
}

vatek_result vatek_capture_create(Phms_handle handle, Pcapture_param param)
{
    vatek_result result = vatek_result_unknown;
    uint32_t errcode;
    broadcast_status status = bc_status_unknown;
    uint32_t timeout = 0;
    uint32_t timeout_max = 500; //10 sec
    
    if (handle == NULL)
        return vatek_result_invalidparm;

    if ((result = vatek_hms_issystemidle(handle)) != vatek_result_success)
        return result;

    if ((result = vatek_hms_write_hal(handle, HALREG_TRANSFORM_MODE, TRANSFORM_CAPTURE)) != vatek_result_success)
        return result;
    
    if ((result = vatek_hms_write_hal(handle, HALREG_TRCAPTURE_PID, param->pid)) != vatek_result_success)
        return result;
    
    if ((result = vatek_hms_write_hal(handle, HALREG_TRCAPTURE_TABLEID, param->table_id)) != vatek_result_success)
        return result;
    
    if ((result = vatek_hms_write_hal(handle, HALREG_TRCAPTURE_TIMEOUT, 10000)) != vatek_result_success)
        return result;
    
    if ((result = vatek_hms_write_hal(handle, HALREG_TRCAPTURE_SECTION_NUM, param->section_num)) != vatek_result_success)
        return result;
    
    if ((result = vatek_hms_write_hal(handle, HALREG_TRCAPTURE_PACKET_NUMS, 16)) != vatek_result_success)
        return result;
    
    if ((result = capture_ctrl(handle, BC_START, &errcode)) != vatek_result_success)
    {
        CAPTURE_ERR("BC_START fail, errcode = 0x%lX", errcode);
        return result;
    }
    
    while (timeout < timeout_max) 
    {
        vatek_system_delay(100);
        if ((result = capture_getstatus(handle, &status)) != vatek_result_success)
        {
            CAPTURE_ERR("capture_getstatus fail");
            return result;
        }

        if (status == bc_status_finish)
        {
            break;
        }
        else if (status >= bc_status_fail_unknown)
        {
            CAPTURE_ERR("capture start fail : %d", status);
            return vatek_result_hwfail;
        }
        timeout++;
    }
    
    if (timeout >= timeout_max)
        return vatek_result_timeout;
    
    return result;
}

vatek_result vatek_capture_destroy(Phms_handle handle)
{
    vatek_result result = vatek_result_unknown;
    uint32_t errcode;
    broadcast_status status = bc_status_unknown;
    uint32_t timeout = 0;
    uint32_t timeout_max = 100; //10 sec
    
    if ((result = capture_ctrl(handle, BC_STOP, &errcode)) != vatek_result_success)
    {
        CAPTURE_ERR("BC_STOP fail, errcode = 0x%lX", errcode);
        return result;
    }

    timeout = 0;
    while (timeout < timeout_max) 
    {
        vatek_system_delay(100);
        if ((result = capture_getstatus(handle, &status)) != vatek_result_success)
        {
            CAPTURE_ERR("capture_getstatus fail");
            return result;
        }

        if (status == bc_status_idle)
        {
            break;
        }
        else if (status >= bc_status_fail_unknown)
        {
            CAPTURE_ERR("capture stop fail : %d", status);
            return vatek_result_hwfail;
        }
        timeout++;
    }
    
    if (timeout >= timeout_max)
        return vatek_result_timeout;
    
    return result;
}

vatek_result vatek_capture_reset(Phms_handle handle)
{
    vatek_result result = vatek_result_success;

    if (handle == NULL)
        return vatek_result_invalidparm;
    
    if (g_capture_table != NULL)
    {
        if (g_capture_table->tspackets != NULL)
        {
            free(g_capture_table->tspackets);
            g_capture_table->tspackets = NULL;
        }
        
        g_capture_table->interval_ms = 0;
        g_capture_table->tspacket_num = 0;
        free(g_capture_table);
        g_capture_table = NULL;
    }
    
    return result;
}

vatek_result vatek_capture_gettable(Phms_handle handle, Ppsitable_parm *raw)
{
    vatek_result result = vatek_result_unknown;
    uint32_t val = 0;
    uint32_t addr = HALRANGE_PLAYLOAD_START;
    uint32_t offset = 0;
    
    Ppsitable_parm raw_psi = (Ppsitable_parm)raw_malloc(NULL, sizeof(psitable_parm));
    if (raw_psi == NULL) return vatek_result_memfail;

    if ((result = vatek_hms_read_hal(handle, addr++, &val)) == vatek_result_success)
    {
        if (val != RAWPSI_EN_TAG) return vatek_result_format;
    }else return result;
    
    if ((result = vatek_hms_read_hal(handle, addr++, &val)) == vatek_result_success)
    {
        raw_psi->interval_ms = val;
        if ((result = vatek_hms_read_hal(handle, addr++, &val)) != vatek_result_success) return result;
        raw_psi->tspacket_num = val;
    }else return result;
    
    raw_psi->tspackets = (uint8_t*)raw_malloc(raw_psi->tspackets, PSI_TSPACKET_LEN * 16);
    if (raw_psi->tspackets == NULL)
        return vatek_result_memfail;

    while (result == vatek_result_success)
    {
        if ((result = vatek_hms_read_hal(handle, addr++, &val)) != vatek_result_success)
            return result;
        
        if (val == RAWPSI_EN_ENDTAG)
        {
            val = offset/PSI_TSPACKET_LEN;      // cal & replace tspacket_num.
            raw_psi->tspacket_num = val;
            *raw = raw_psi;
            return vatek_result_success;
        }else if (val == RAWPSI_EN_TAG)
        {
            // skip unnecessary interval_ms & tspacket_num val.
            addr++;
            addr++;
        }else
        {
            raw_psi->tspackets[offset] = val>>24;
						printf("0x%x ",raw_psi->tspackets[offset]);
            raw_psi->tspackets[offset+1] = val>>16;
						printf("0x%x ",raw_psi->tspackets[offset+1]);
            raw_psi->tspackets[offset+2] = val>>8;
						printf("0x%x ",raw_psi->tspackets[offset+2]);
            raw_psi->tspackets[offset+3] = val;
						printf("0x%x |\r\n",raw_psi->tspackets[offset+3]);
            offset += 4;
        }
    }
    return result;
}

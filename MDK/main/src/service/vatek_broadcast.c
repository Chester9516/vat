
#include "vatek_system.h"
#include "vatek_hms.h"
#include "vatek_encoder.h"
#include "vatek_tsmux.h"
#include "vatek_modulator.h"
#include "vatek_psitable.h"
#include "vatek_userdata.h"
#include "vatek_psispec_rule.h"
#include "vatek_psispec_default.h"
#include "main.h"

#if defined(LOG_TABLE)
#include "vatek_log_table.h"
#endif

#define BROADCAST_ERR(fmt,...)    VATEK_ERR(bc,fmt,##__VA_ARGS__)
#define BROADCAST_LOG(fmt,...)    VATEK_LOG(bc,fmt,##__VA_ARGS__)

vatek_result vatek_broadcast_chipstatus(Phbroadcast handle, chip_status *status);

#if defined(LOG_TABLE)
uint32_t vi_type, vi_resolution, vi_framerate, vi_aspectrate, ve_type, ae_type, psi_type, mod_type, rf_freq;

static vatek_result broadcast_dump( Phms_handle handle)
{
    vatek_result result = vatek_result_unknown;
    
    if(handle==NULL) return vatek_result_invalidparm;
    
    //uint32_t vi_type, vi_resolution, vi_framerate, vi_aspectrate, ve_type, ae_type, psi_type, mod_type, rf_freq;
    
    result = vatek_hms_read_hal( handle, HALREG_ENCODER_MODE, &vi_type);
    result |= vatek_hms_read_hal( handle, HALREG_VIDEO_RESOLUTION, &vi_resolution);
    result |= vatek_hms_read_hal( handle, HALREG_VIDEO_FRAMERATE,  &vi_framerate);
    result |= vatek_hms_read_hal( handle, HALREG_VIDEO_ASPECTRATE, &vi_aspectrate);
    result |= vatek_hms_read_hal( handle, HALREG_VIDEO_CODEC, &ve_type);
    result |= vatek_hms_read_hal( handle, HALREG_AUDIO_CODEC, &ae_type);
    result |= vatek_hms_read_hal( handle, HALREG_MUXPSI_MODE, &psi_type);
    result |= vatek_hms_read_hal( handle, HALREG_MOD_MODE, &mod_type);
    result |= vatek_hms_read_hal( handle, HALREG_RF_COM_FREQ, &rf_freq);
    if( result == vatek_result_success)
    {
        BROADCAST_LOG("");
        BROADCAST_LOG("%10s : %s", "vi_type"  , CONVERT_LOG(vi_source_type , vi_type));
        BROADCAST_LOG("%10s : %s", "resolution"  , CONVERT_LOG(vi_resolution_type , vi_resolution));
        BROADCAST_LOG("%10s : %s", "framerate"   , CONVERT_LOG(vi_framerate_type  , vi_framerate));
        BROADCAST_LOG("%10s : %s", "aspectrate"  , CONVERT_LOG(vi_aspectrate_type , vi_aspectrate));
        BROADCAST_LOG("%10s : %s", "ve_type"  , CONVERT_LOG(ve_encoder_type, ve_type));
        BROADCAST_LOG("%10s : %s", "ae_type"  , CONVERT_LOG(ae_encoder_type, ae_type));
        BROADCAST_LOG("%10s : %s", "psi_type" , CONVERT_LOG(psi_mode_type  , psi_type));
        BROADCAST_LOG("%10s : %s", "mod_type" , CONVERT_LOG(mod_mode_type  , mod_type));
        BROADCAST_LOG("%10s : %d", "rf_freq"  , rf_freq);
    }
    
    return result;
}
#endif

static vatek_result broadcast_ctrl(Phms_handle handle, uint32_t cmd, uint32_t *errcode)
{
    vatek_result result = vatek_result_unknown;
    uint32_t timeout = 0;

    if ((result = vatek_hms_write_hal(handle, HALREG_BROADCAST_CNTL, cmd)) == vatek_result_success)
    {
        while (timeout < 50) //5 sec
        {
            vatek_system_delay(100);
            if ((result = vatek_hms_read_hal(handle, HALREG_BROADCAST_CNTL, errcode)) != vatek_result_success)
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

static vatek_result broadcast_reset(Phms_handle handle)
{
    vatek_result result = vatek_result_unknown;
    uint32_t timeout = 0;

    if (handle == NULL)
        return vatek_result_invalidparm;

    BROADCAST_LOG("broadcast reset");
    if ((result = vatek_hms_reset(handle)) != vatek_result_success)
        return result;

    //polling until chip ready or timeout (5 sec)
    BROADCAST_LOG("waiting chip ready");
    while (((result = vatek_hms_issystemidle(handle)) != vatek_result_success) && timeout <= 50)
    {  
        vatek_system_delay(100);
        timeout++;
    }
    
    if (timeout >= 50)
    {
        BROADCAST_ERR("reset timeout");
        result = vatek_result_timeout;
    }
    
    BROADCAST_LOG("chip ready");
    
    return result;
}

static vatek_result broadcast_getstatus(Phbroadcast handle, broadcast_status *status)
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

vatek_result broadcast_getinfo(Phbroadcast handle, broadcast_infotype type, uint32_t *value)
{
    vatek_result result = vatek_result_unknown;
    
    if(handle==NULL || value==NULL)
        return vatek_result_invalidparm;
    
    switch(type)
    {
        case bc_infotype_chipid:
            result = vatek_hms_read_hal(handle, HALREG_CHIP_ID, value);
        break;
        
        case bc_infotype_fw_ver:
            result = vatek_hms_read_hal(handle, HALREG_FW_VER, value);
        break;
        
        case bc_infotype_sdk_ver:
            *value = SDK_VERSION;
        break;
        
        case bc_infotype_service_mode:
            result = vatek_hms_read_hal(handle, HALREG_SERVICE_MODE, value);
        break;
        
        case bc_infotype_peripheral_en:
            result = vatek_hms_read_hal(handle, HALREG_PERIPHERAL_EN, value);
        break;
        
        case bc_infotype_insupport:
            result = vatek_hms_read_hal(handle, HALREG_INPUT_SUPPORT, value);
        break;
        
        case bc_infotype_outsupport:
            result = vatek_hms_read_hal(handle, HALREG_OUTPUT_SUPPORT, value);
        break;

        default :
        break;
    }
    
    if(result!=vatek_result_success)
    {
        *value = 0xFFFFFFFF;
        return result;
    }
    
    return result;
}

vatek_result vatek_broadcast_create(Pboard_handle hboard, Phbroadcast *handle)
{
    vatek_result result = vatek_result_unknown;
    Phms_handle hms_handle = NULL;

    if (hboard == NULL || handle == NULL)
        return vatek_result_invalidparm;

    *handle = NULL;
    if ((result = vatek_hms_create(hboard, &hms_handle)) != vatek_result_success)
        return result;
    hms_handle->type = hms_type_broadcast;
    *handle = hms_handle;

    if ((result = broadcast_reset(*handle)) != vatek_result_success)
        return result;
  
    return result;
}

vatek_result vatek_broadcast_destroy(Phbroadcast handle)
{
    vatek_result result = vatek_result_unknown;

    if (handle == NULL)
        return vatek_result_invalidparm;

    result = vatek_hms_destroy((Phms_handle)handle);

    return result;
}

vatek_result vatek_broadcast_start(Phbroadcast handle)
{
    vatek_result result = vatek_result_unknown;
    uint32_t errcode;
    broadcast_status status = bc_status_unknown;
    uint32_t timeout = 0;
    
    if (handle == NULL)
        return vatek_result_invalidparm;

    if ((result = broadcast_ctrl(handle, BC_START, &errcode)) != vatek_result_success)
    {
        BROADCAST_ERR("BC_START fail, errcode = 0x%lX", errcode);
        return result;
    }
    
    while (timeout < 20) // 2 sec
    {
        vatek_system_delay(100);
        if ((result = broadcast_getstatus(handle, &status)) != vatek_result_success)
            return result;

        if (status == bc_status_broadcast)
            break;
        
        timeout++;
    }
    
    return result;
}

vatek_result vatek_broadcast_stop(Phbroadcast handle)
{
    vatek_result result = vatek_result_unknown;
    
    if (handle == NULL)
        return vatek_result_invalidparm;
    
    uint32_t errcode;
    if ((result = broadcast_ctrl(handle, BC_STOP, &errcode)) != vatek_result_success)
        BROADCAST_ERR("BC_STOP fail, errcode = 0x%1X", errcode);
    
    if ((result = broadcast_reset(handle)) != vatek_result_success)
        return result;

    return result;
}

vatek_result vatek_broadcast_reset(Phbroadcast handle)
{
    vatek_result result = vatek_result_unknown;
    
    if( handle == NULL)
        return vatek_result_invalidparm;
    
    if( ( result = broadcast_reset( handle)) != vatek_result_success)
        return result;
    
    return result;
}


vatek_result vatek_broadcast_bcstatus(Phbroadcast handle, broadcast_status *status)
{
    vatek_result result = vatek_result_unknown;

    if (handle == NULL || status == NULL)
        return vatek_result_invalidparm;

    if ((result = broadcast_getstatus(handle, status)) != vatek_result_success)
        return result;
    
    return result;
}

uint32_t read_bitrate = 0;
uint32_t read_datarate = 0;
uint32_t max_read_bitrate = 0;
uint32_t max_read_datarate = 0;
uint32_t read_chip_id = 0;
uint32_t read_chip_fw_ver = 0;
vatek_result vatek_broadcast_chipstatus(Pboard_handle hboard, chip_status *status)
{
    vatek_result result = vatek_result_unknown;
    uint32_t value = 0;

    if (hboard == NULL || status == NULL)
        return vatek_result_invalidparm;

    *status = chip_status_unknown;
    if ((result = vatek_hms_read_hal(hboard, HALREG_SYS_STATUS_0, &value)) != vatek_result_success)
        return result;
        


#if 1//add may
	if ((result = vatek_hms_read_hal(hboard, HALREG_BCINFO_CURRATE, &read_bitrate)) != vatek_result_success)		
        return result;
	if ((result = vatek_hms_read_hal(hboard, HALREG_BCINFO_DATARATE, &read_datarate)) != vatek_result_success)
        return result;
	
	if(read_bitrate > max_read_bitrate){
	  max_read_bitrate = read_bitrate; 	  
	}
	if(read_datarate > max_read_datarate){
	  max_read_datarate = read_datarate; 	  
	}
	
	result = vatek_hms_read_hal(hboard, HALREG_CHIP_ID, &read_chip_id);	       
	result = vatek_hms_read_hal(hboard, HALREG_FW_VER, &read_chip_fw_ver);
    
	
#endif	

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

/**
* @brief null null
* @details 
*
* @param null null
* @param null null
* 
* @return 
*   @retval vatek_result_success 
*   @retval vatek_result_unknown
*   @retval vatek_result_invalidparm
*   @retval vatek_result_timeout
*   @retval vatek_result_memfail
*   @retval vatek_result_fromat
*   @retval vatek_result_badstatus 
*   @retval vatek_result_busy
*   @retval vatek_result_idle
*   @retval vatek_result_overrange
*   @retval vatek_result_hwfail
*   @retval vatek_result_unsupport
*   @retval vatek_result_negative
*   @retval vatek_result_bufoverflow
*   @retval vatek_result_i2cfail
*   @retval vatek_result_gpiofail
*   @retval vatek_result_uartfail
*/
vatek_result vatek_broadcast_getinfo(Phbroadcast handle, broadcast_infotype type, uint32_t *value)
{
    vatek_result result = vatek_result_unknown;
    
    if (handle == NULL || value == NULL)
        return vatek_result_invalidparm;
    
    result = broadcast_getinfo(handle, type, value);

    return result;
}

vatek_result vatek_broadcast_encoder_setinputparm_phy(Phbroadcast handle, video_input_parm vparm, audio_input_parm aparm)
{
    vatek_result result = vatek_result_unknown;

    if (handle == NULL)
        return vatek_result_invalidparm;

    result = vatek_encoder_setinputparm_phy((Phms_handle)handle, vparm, aparm);
    
    return result;
}

vatek_result vatek_broadcast_encoder_setinputparm_logo(Phbroadcast handle, logo_input_parm parm)
{
    vatek_result result = vatek_result_unknown;

    if (handle == NULL)
        return vatek_result_invalidparm;

    result = vatek_encoder_setinputparm_logo((Phms_handle)handle, parm);
    
    return result;
}

vatek_result vatek_broadcast_encoder_setencodeparm(Phbroadcast handle, video_encode_parm vparm, audio_encode_parm aparm)
{
    vatek_result result = vatek_result_unknown;

    if (handle == NULL)
        return vatek_result_invalidparm;

    if ((result = vatek_userdata_setvideocodec(vparm.type)) != vatek_result_success)
        BROADCAST_ERR("set userdata's video enc type fail");
    
    result = vatek_encoder_setencodeparm((Phms_handle)handle, vparm, aparm);
    
    return result;
}

vatek_result vatek_broadcast_encoder_setmuxparm(Phbroadcast handle, encoder_mux_parm parm)
{
    vatek_result result = vatek_result_unknown;

    if (handle == NULL)
        return vatek_result_invalidparm;

    result = vatek_encoder_setmuxparm((Phms_handle)handle, parm);
    
    return result;
}

vatek_result vatek_broadcast_encoder_setqualityparm(Phbroadcast handle, encoder_quality_parm parm)
{
    vatek_result result = vatek_result_unknown;

    if (handle == NULL)
        return vatek_result_invalidparm;

    result = vatek_encoder_setqualityparm((Phms_handle)handle, parm);
    
    return result;
}

vatek_result vatek_broadcast_tsmux_setparm(Phbroadcast handle, tsmux_type type, Ptsmux_parm parm)
{
    vatek_result result = vatek_result_unknown;

    if (handle == NULL || parm == NULL)
        return vatek_result_invalidparm;

    result = vatek_tsmux_setparm((Phms_handle)handle, type, parm);
    
    return result;
}

vatek_result vatek_broadcast_modulator_setparm(Phbroadcast handle, modulator_base_parm parm_base, Pmodulator_advanced_parm parm_adv)
{
    vatek_result result = vatek_result_unknown;

    if (handle == NULL || parm_adv == NULL)
        return vatek_result_invalidparm;

    result =  vatek_modulator_setparm((Phms_handle)handle, parm_base, parm_adv);
    
    return result;
}

vatek_result vatek_broadcast_psitable_register(Phbroadcast handle, Ppsitablelist_parm parm)
{
    vatek_result result = vatek_result_unknown;

    if (handle == NULL || parm == NULL)
        return vatek_result_invalidparm;

    result =  vatek_psitable_register((Phms_handle)handle, parm);
    
    return result;
}

vatek_result vatek_broadcast_psitable_insert( Phbroadcast handle, uint16_t tspacket_num, uint8_t *tspackets)
{
    vatek_result result = vatek_result_unknown;

    if (handle == NULL || tspackets == NULL)
        return vatek_result_invalidparm;

    result = vatek_psitable_insert((Phms_handle)handle, tspacket_num, tspackets);
    
    return result;
}

vatek_result vatek_broadcast_userdata_insert_raw(Phbroadcast handle, uint16_t data_num, uint8_t *data)
{
    vatek_result result = vatek_result_unknown;

    if (handle == NULL || data == NULL || data_num == 0)
        return vatek_result_invalidparm;

    result = vatek_userdata_raw_insert((Phms_handle)handle, data_num, data);
    
    return result;
}

vatek_result vatek_broadcast_userdata_insert_cc(Phbroadcast handle, uint16_t cc_num, Puserdata_cc_parm cc_parm)
{
    vatek_result result = vatek_result_unknown;

    if (handle == NULL || cc_parm == NULL || cc_num == 0)
        return vatek_result_invalidparm;

    result = vatek_userdata_cc_insert((Phms_handle)handle, cc_num, cc_parm);
    
    return result;
}

vatek_result vatek_broadcast_psispec_rule_init(Phbroadcast handle, Ppsispec_rule rule)
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

vatek_result vatek_broadcast_psispec_rule_settablerule(Phbroadcast handle, const char *name, uint32_t version, uint32_t interval)
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

vatek_result vatek_broadcast_psispec_rule_setchannelrule(Phbroadcast handle, const char *name, void *buf, uint32_t len)
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

vatek_result vatek_broadcast_psispec_rule_start(Phbroadcast handle)
{
    vatek_result result = vatek_result_unknown;

    if (handle == NULL)
        return vatek_result_invalidparm;

    if ((result = vatek_psispec_rule_start(handle)) != vatek_result_success)
    {
        return result;
    }

    if ((result = vatek_psispec_rule_end(handle)) != vatek_result_success)
    {
        return result;
    }
    
    return result; 
}

vatek_result vatek_broadcast_psispec_default_init(Phbroadcast handle, psispec_default_type type, psispec_country_code country)
{
    vatek_result result = vatek_result_unknown;
    
    result = vatek_psispec_default_init_br((Phms_handle)handle, type, country);
    if(result != vatek_result_success)
        return result;
    
    return result;
}

vatek_result vatek_broadcast_psispec_default_config( Ppsispec_default_channel chan, Ppsispec_default_program prog)
{
    vatek_result result = vatek_result_unknown;
    
    if( chan==NULL || prog==NULL)
        return vatek_result_invalidparm;
    
    result = vatek_psispec_default_config( chan, prog);
    if( result != vatek_result_success)
        return result;
    
    return result;
}

vatek_result vatek_broadcast_psispec_default_start(void)
{
    vatek_result result = vatek_result_unknown;
    
    result = vatek_psispec_default_start();
    if( result != vatek_result_success)
        return result;
    
    return result;
}

#if defined (DEBUG)
vatek_result vatek_broadcast_hal_write(Phbroadcast handle, uint32_t addr, uint32_t val)
{
    vatek_result result = vatek_result_unknown;
    
    if((result = vatek_hms_write_hal(handle, addr, val)) != vatek_result_success)
        return result;
    
    return result;
}

vatek_result vatek_broadcast_hal_read(Phbroadcast handle, uint32_t addr, uint32_t* val)
{
    vatek_result result = vatek_result_unknown;
    
    if((result = vatek_hms_read_hal(handle, addr, val)) != vatek_result_success)
        return result;
    
    return result;
}

vatek_result vatek_broadcast_reg_write(Phbroadcast handle, uint32_t addr, uint32_t val)
{
    vatek_result result = vatek_result_unknown;
    
    if((result = vatek_hms_write_reg(handle, addr, val)) != vatek_result_success)
        return result;
    
    return result;
}

vatek_result vatek_broadcast_reg_read(Phbroadcast handle, uint32_t addr, uint32_t* val)
{
    vatek_result result = vatek_result_unknown;
    
    if((result = vatek_hms_read_reg(handle, addr, val)) != vatek_result_success)
        return result;
    
    return result;
}
#endif


#if defined (LOG_TABLE)
vatek_result vatek_broadcast_dump( Phbroadcast handle)
{
    if( handle == NULL)
        return vatek_result_invalidparm;
    
    return broadcast_dump(handle);
}
#endif

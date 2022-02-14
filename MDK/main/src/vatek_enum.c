
#include "vatek_system.h"
#include "vatek_hms.h"
#include "vatek_enum.h"

static Penum_list g_enum_list = NULL;

static vatek_result enum_ctrl(Phms_handle handle, uint32_t cmd, uint32_t *errcode)
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

static vatek_result enum_getstatus(Phms_handle handle, broadcast_status *status)
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

static void *enum_malloc(void *ptr, uint32_t size)
{
    void *ret = NULL;
    if (ptr == NULL)
        ret = malloc(size);
    else 
        ret = realloc(ptr, size);

    return ret;
}

vatek_result vatek_enum_create(Phms_handle handle)
{
    vatek_result result = vatek_result_unknown;
    uint32_t errcode;
    broadcast_status status = bc_status_unknown;
    uint32_t timeout = 0;
    uint32_t timeout_max = 100; //10 sec
    
    if (handle == NULL)
        return vatek_result_invalidparm;

    if ((result = vatek_hms_issystemidle(handle)) != vatek_result_success)
        return result;

    if ((result = vatek_hms_write_hal(handle, HALREG_TRANSFORM_MODE, TRANSFORM_ENUM)) != vatek_result_success)
        return result;
    
    if ((result = vatek_hms_write_hal(handle, HALREG_TRENUM_TIMEOUT, 5000)) != vatek_result_success)
        return result;
    
    if ((result = enum_ctrl(handle, BC_START, &errcode)) != vatek_result_success)
    {
        ENUM_ERR("BC_START fail, errcode = 0x%lX", errcode);
        return result;
    }
    
    while (timeout < timeout_max) 
    {
        vatek_system_delay(100);
        if ((result = enum_getstatus(handle, &status)) != vatek_result_success)
        {
            ENUM_ERR("enum_getstatus fail");
            return result;
        }

        if (status == bc_status_finish)
        {
            break;
        }
        else if (status >= bc_status_fail_unknown)
        {
            ENUM_ERR("enum start fail : %d", status);
            return vatek_result_hwfail;
        }
        timeout++;
    }
    
    if (timeout >= timeout_max)
        return vatek_result_timeout;



    return result;
}

vatek_result vatek_enum_destroy(Phms_handle handle)
{
    vatek_result result = vatek_result_unknown;
    uint32_t errcode;
    broadcast_status status = bc_status_unknown;
    uint32_t timeout = 0;
    uint32_t timeout_max = 100; //10 sec
    
    if ((result = enum_ctrl(handle, BC_STOP, &errcode)) != vatek_result_success)
    {
        ENUM_ERR("BC_STOP fail, errcode = 0x%lX", errcode);
        return result;
    }

    timeout = 0;
    while (timeout < timeout_max) 
    {
        vatek_system_delay(100);
        if ((result = enum_getstatus(handle, &status)) != vatek_result_success)
        {
            ENUM_ERR("enum_getstatus fail");
            return result;
        }

        if (status == bc_status_idle)
        {
            break;
        }
        else if (status >= bc_status_fail_unknown)
        {
            ENUM_ERR("enum stop fail : %d", status);
            return vatek_result_hwfail;
        }
        timeout++;
    }
    
    if (timeout >= timeout_max)
        return vatek_result_timeout;
    
    return result;
}

vatek_result vatek_enum_reset(Phms_handle handle)
{
    vatek_result result = vatek_result_success;
    uint16_t idx = 0, jdx = 0;

    if (handle == NULL)
        return vatek_result_invalidparm;

    if (g_enum_list != NULL && g_enum_list->program != NULL)
    {
        for (idx = 0; idx < g_enum_list->program_num; idx++)
        {
            if (g_enum_list->program[idx].stream != NULL)
            {
                for (jdx = 0; jdx < g_enum_list->program[idx].stream_num; jdx++)
                {
                    if (g_enum_list->program[idx].stream[jdx].esinfo != NULL)
                        free(g_enum_list->program[idx].stream[jdx].esinfo);
                }
                free(g_enum_list->program[idx].stream);
                g_enum_list->program[idx].stream = NULL;
            }
            
        }
        free(g_enum_list->program);
        g_enum_list->program = NULL;
        g_enum_list->program_num  = 0;
        
        free(g_enum_list);
        g_enum_list = NULL;
    }

    return result;
}

vatek_result vatek_enum_getlist(Phms_handle handle, Penum_list *list)
{
    vatek_result result = vatek_result_unknown;
    uint32_t val = 0;
    uint32_t addr = HALRANGE_PLAYLOAD_START;
    uint32_t layer = 0; // 0 : ch, 1 : program, 2 : stream
    uint32_t offset = 0;
    Penum_list newlist = NULL;
    Penum_program program = NULL;
    Penum_stream stream = NULL;
    uint32_t num = 0;

    if ((result = vatek_hms_read_hal(handle, addr++, &val)) == vatek_result_success)
    {
        //if (val != TRCHANNEL_START_TAG)
        if (val != DEFSPEC_V2_EN_START)
            return vatek_result_format;
    }else return result;
    
    if ((result = vatek_hms_read_hal(handle, addr++, &val)) == vatek_result_success)
    {
        //if (val != TRCHANNEL_START_TAG)
        if (val == DEFCH_EN_START)
        {
            while(val!=DEFCH_EN_END)
            {
                if ((result = vatek_hms_read_hal(handle, addr++, &val)) != vatek_result_success)
                    return result;
            }
        }else return vatek_result_format;
    }else return result;

    
    newlist = enum_malloc(NULL, sizeof(enum_list));
    if (newlist == NULL)
        return vatek_result_memfail;  

    newlist->program_num = 0;
    newlist->program = NULL;

    while (result == vatek_result_success)
    {
        if ((result = vatek_hms_read_hal(handle, addr++, &val)) != vatek_result_success)
            return result;
        
        offset++;
        if (layer == 0)
        {   //ch
            switch (val)
            {
                //case TRPROGRAM_START_TAG:
                case DEFPROG_EN_START:
                    layer++;
                    offset = 0;
                    newlist->program_num++;
                    program = newlist->program;
                    newlist->program = enum_malloc(program, sizeof(enum_program)*newlist->program_num);
                    if (newlist->program == NULL)
                        return vatek_result_memfail;
                    newlist->program[newlist->program_num-1].stream_num = 0;
                    newlist->program[newlist->program_num-1].stream = NULL;
                    break;
                
                //case TRCHANNEL_END_TAG:
                case DEFCH_EN_END:
                    *list = newlist;
                    g_enum_list = newlist;
                    return result;
       
                default:
//                    break;
                    return vatek_result_format;
            }
        }
        else if (layer == 1)
        {   //program
            switch (val)
            {
                //case TRPROGRAM_END_TAG:
                case DEFPROG_EN_END:
                    layer--;
                    break;
                
                //case TRSTREAM_START_TAG:
                case DEFSTREAM_EN_START:
                    layer++;
                    offset = 0;
                    newlist->program[newlist->program_num-1].stream_num++;
                    num = newlist->program[newlist->program_num-1].stream_num;                    
                    stream = newlist->program[newlist->program_num-1].stream;
                    newlist->program[newlist->program_num-1].stream = enum_malloc(stream, sizeof(enum_stream)*num);
                    if (newlist->program[newlist->program_num-1].stream == NULL)
                        return vatek_result_memfail;
                    break;
                    
                default:
                    //if (offset == HALOF_TRPROGRAM_PCR_PID)
                    if (offset == HALOF_DEFPROG_PCR_PID)
                        newlist->program[newlist->program_num-1].pcr_pid = val;
                    //else if (offset == HALOF_TRPROGRAM_PMT_PID)
                    else if (offset == HALOF_DEFPROG_PMT_PID)
                        newlist->program[newlist->program_num-1].pmt_pid = val;
//                    else
//                        return vatek_result_format;
                    break;
            }
        }
        else if (layer == 2)
        {
            //stream
            switch (val)
            {
                //case TRSTREAM_END_TAG:
                case DEFSTREAM_EN_END:
                    layer--;
                    break;
                    
                default:
                    num = newlist->program[newlist->program_num-1].stream_num - 1;
                    stream = &newlist->program[newlist->program_num-1].stream[num];
                    //if (offset == HALOF_TRSTREAM_TYPE)
                    if (offset == HALOF_DEFSTREAM_TYPE)
                    {
                        switch (val)
                        {
                            case MUX_STREAM_TYPE_VIDEO:
                                stream->type = stream_type_video;
                                break;

                            case MUX_STREAM_TYPE_AUDIO:
                                stream->type = stream_type_audio;
                                break;
                            
                            case MUX_STREAM_TYPE_DATA:
                                stream->type = stream_type_data;
                                break;
                            
                            default:
                                stream->type = stream_type_unknown;
                                break;
                        }
                    }
//                    else if (offset == HALOF_TRSTREAM_CODEC)                   
//                    {
//                        if (stream->type == stream_type_video)
//                        {
//                            switch (val)
//                            {
//                                case VIDEO_MPEG2:
//                                    stream->encode_type = ve_type_mpeg2;
//                                    break;

//                                case VIDEO_H264:
//                                    stream->encode_type = ve_type_h264;
//                                    break;
//                                
//                                default:
//                                    stream->encode_type = ve_type_unknown;
//                                    break;
//                            }
//                        }
//                        else if (stream->type == stream_type_audio)
//                        {
//                            switch (val)
//                            {
//                                case AUDIO_MP1_L2:
//                                    stream->encode_type = ae_type_mp1_l2;
//                                    break;

//                                case AUDIO_AAC_LC_ADTS:
//                                    stream->encode_type = ae_type_aac_lc_adts;
//                                    break;
//                                
//                                case AUDIO_AC_3:
//                                    stream->encode_type = ae_type_ac_3;
//                                    break;

//                                case AUDIO_AAC_LC_LATM:
//                                    stream->encode_type = ae_type_aac_lc_latm;
//                                    break;
//                                
//                                default:
//                                    stream->encode_type = ae_type_unknown;
//                                    break;
//                            }

//                        }
//                        else
//                            stream->encode_type = val;
//                    }
                    else if (offset == HALOF_DEFSTREAM_VIDEO_CODEC)
                    {
                        if (stream->type == stream_type_video)
                        {
                            switch (val)
                            {
                                case VIDEO_MPEG2:
                                    stream->encode_type = ve_type_mpeg2;
                                    break;

                                case VIDEO_H264:
                                    stream->encode_type = ve_type_h264;
                                    break;
                                
                                default:
                                    stream->encode_type = ve_type_unknown;
                                    break;
                            }
                        }
                    }
                    else if (offset == HALOF_DEFSTREAM_AUDIO_CODEC)
                    {
                        if (stream->type == stream_type_audio)
                        {
                            switch (val)
                            {
                                case AUDIO_MP1_L2:
                                    stream->encode_type = ae_type_mp1_l2;
                                    break;

                                case AUDIO_AAC_LC_ADTS:
                                    stream->encode_type = ae_type_aac_lc_adts;
                                    break;
                                
                                case AUDIO_AC_3:
                                    stream->encode_type = ae_type_ac_3;
                                    break;

                                case AUDIO_AAC_LC_LATM:
                                    stream->encode_type = ae_type_aac_lc_latm;
                                    break;
                                
                                default:
                                    stream->encode_type = ae_type_unknown;
                                    break;
                            }
                        }
                    }
                    //else if (offset == HALOF_TRSTREAM_PID)
                    else if (offset == HALOF_DEFSTREAM_PID)
                        newlist->program[newlist->program_num-1].stream[num].stream_pid = val;
                    //else if (offset == HALOF_TRSTREAM_STREAM_TYPE)
                    else if (offset == HALOF_DEFSTREAM_ESTYPE)
                        stream->stream_type = val;
                    //else if (offset == HALOF_TRSTREAM_ES_LEN)
                    else if (offset == HALOF_DEFSTREAM_ESLEN)
                    {
                        stream->esinfo_len = val;
                        stream->esinfo = NULL; //TODO
                    }
                    //else if (offset > HALOF_TRSTREAM_ES_LEN && offset <= (HALOF_TRSTREAM_ES_LEN+stream->esinfo_len))
                    else if (offset > HALOF_DEFSTREAM_ESLEN && offset <= (HALOF_DEFSTREAM_ESLEN+stream->esinfo_len))
                        ;
//                    else
//                        return vatek_result_format;
                    
                    break;
            }
        }
    }

    return result;
}


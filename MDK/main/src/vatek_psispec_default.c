
#include "vatek_system.h"
#include "vatek_hms.h"
#include "vatek_psispec_default.h"

static uint32_t table_position = 0;
static Ppsispec_default_table ptable = NULL;
static Phms_handle ph_handle;

//Check buffer is aligned with word size.
static uint8_t buffer_align( uint8_t len)
{
    if( ( len%4) != 0)
        return ( len|( len-1))+1;
    
    return len;
}

static vatek_result tool_stream_video(Pvideo_param param)
{
    vatek_result result = vatek_result_unknown;
    uint32_t type = 0, resolution = 0, framerate = 0, aspectrate = 0;
    
    switch (param->type)
    {
        case ve_type_mpeg2:
            type = VIDEO_MPEG2;
            break;

        case ve_type_h264:
            type = VIDEO_H264;
            break;
        
        case ve_type_unknown:
        default:
            type = VIDEO_MPEG2;
            break;
    } 
    
    switch (resolution)
    {
        case vi_resolution_1080p60:
            resolution = RESOLUTION_1080P;
            framerate = FRAMERATE_60;
            break;

        case vi_resolution_1080p59_94:
            resolution = RESOLUTION_1080P;
            framerate = FRAMERATE_59_94;
            break;

        case vi_resolution_1080p50:
            resolution = RESOLUTION_1080P;
            framerate = FRAMERATE_50;
            break;

        case vi_resolution_1080p30:
            resolution = RESOLUTION_1080P;
            framerate = FRAMERATE_30;
            break;
            
        case vi_resolution_1080p25:
            resolution = RESOLUTION_1080P;
            framerate = FRAMERATE_25;
            break;
            
        case vi_resolution_1080p24:
            resolution = RESOLUTION_1080P;
            framerate = FRAMERATE_24;
            break;

        case vi_resolution_1080p23_97:
            resolution = RESOLUTION_1080P;
            framerate = FRAMERATE_23_97;
            break;

        case vi_resolution_1080i60:
            resolution = RESOLUTION_1080I;
            framerate = FRAMERATE_60;
            break;

        case vi_resolution_1080i59_94:
            resolution = RESOLUTION_1080I;
            framerate = FRAMERATE_59_94;
            break;

        case vi_resolution_1080i50:
            resolution = RESOLUTION_1080I;
            framerate = FRAMERATE_50;
            break;

        case vi_resolution_720p60:
            resolution = RESOLUTION_720P;
            framerate = FRAMERATE_60;
            break;

        case vi_resolution_720p59_94:
            resolution = RESOLUTION_720P;
            framerate = FRAMERATE_59_94;
            break;

        case vi_resolution_720p50:
            resolution = RESOLUTION_720P;
            framerate = FRAMERATE_50;
            break;

        case vi_resolution_576p50:
            resolution = RESOLUTION_576P;
            framerate = FRAMERATE_50;
            break;

        case vi_resolution_576i50:
            resolution = RESOLUTION_576I;
            framerate = FRAMERATE_50;
            break;

        case vi_resolution_480p60:
            resolution = RESOLUTION_480P;
            framerate = FRAMERATE_60;
            break;

        case vi_resolution_480p59_94:
            resolution = RESOLUTION_480P;
            framerate = FRAMERATE_59_94;
            break;

        case vi_resolution_480i60:
            resolution = RESOLUTION_480I;
            framerate = FRAMERATE_60;
            break;

        case vi_resolution_480i59_94:
            resolution = RESOLUTION_480I;
            framerate = FRAMERATE_59_94;
            break;
            
        case vi_resolution_576p25:
            resolution = RESOLUTION_576P;
            framerate = FRAMERATE_25;
            break;

        case vi_resolution_480p30:
            resolution = RESOLUTION_480P;
            framerate = FRAMERATE_30;
            break;

        case vi_resolution_480p29_97:
            resolution = RESOLUTION_480P;
            framerate = FRAMERATE_29_97;
            break;

        case vi_resolution_unknown:
            resolution = RESOLUTION_1080P;
            framerate = FRAMERATE_60;
        default:
            break;
    }
    
    switch (param->aspectrate)
    {
        case vi_aspectrate_16_9:
            aspectrate = ASPECTRATE_16_9;
            break;

        case vi_aspectrate_4_3:
            aspectrate = ASPECTRATE_4_3;
            break;

        case vi_aspectrate_unknown:
        default:
            aspectrate = ASPECTRATE_16_9;
            break;
    }
    
    result = vatek_hms_write_hal( ph_handle, table_position, type);
    if(is_success(result)) result = vatek_hms_write_hal( ph_handle, table_position+1, resolution);
    if(is_success(result)) result = vatek_hms_write_hal( ph_handle, table_position+2, framerate);
    if(is_success(result)) result = vatek_hms_write_hal( ph_handle, table_position+3, aspectrate);
    
    return result;
}

static vatek_result tool_stream_audio(Paudio_param param)
{
    vatek_result result = vatek_result_unknown;
    
    uint32_t type = 0, samplerate = 0, channel = 0;
    
    switch (param->type)
    {
        case ae_type_mp1_l2:
            type = AUDIO_MP1_L2;
            break;

        case ae_type_aac_lc_adts:
            type = AUDIO_AAC_LC_ADTS;
            break;
        
        case ae_type_ac_3:
            type = AUDIO_AC_3;
            break;
        
        case ae_type_aac_lc_latm:
            type = AUDIO_AAC_LC_LATM;
            break;
        
        case ae_type_unknown:
        default:
            type = AUDIO_MP1_L2;
            break;
    } 
    
    switch (param->samplerate)
    {
        case ai_samplerate_48K:
            samplerate = SAMPLERATE_48KHZ;
            break;

        case ai_samplerate_44_1K:
            samplerate = SAMPLERATE_44_1KHZ;
            break;

        case ai_samplerate_32K:
            samplerate = SAMPLERATE_32KHZ;
            break;

        case ai_samplerate_unknown:
        default:
            samplerate = SAMPLERATE_48KHZ;
            break;
    }   
    
    switch (param->channel)
    {
        case ae_channel_mute:
            channel = CHANNEL_MUTE;
            break;

        case ae_channel_stereo:
            channel = CHANNEL_STEREO;
            break;
        
        case ae_channel_mono_l:
            channel = CHANNEL_MONO_L;
            break;
        
        case ae_channel_mono_r:
            channel = CHANNEL_MONO_R;
            break;
        
        case ae_channel_stereo_mono_l:
            channel = CHANNEL_STEREO_MONO_L;
            break;
        
        case ae_channel_stereo_mono_r:
            channel = CHANNEL_STEREO_MONO_R;
            break;
        
        case ae_channel_unknown:
        default:
            channel = CHANNEL_STEREO;
            break;
    } 
    
    result = vatek_hms_write_hal( ph_handle, table_position+1, type);
    if(is_success(result)) result = vatek_hms_write_hal( ph_handle, table_position+2, samplerate);
    if(is_success(result)) result = vatek_hms_write_hal( ph_handle, table_position+3, channel);
    
    return result;
}

static vatek_result iso_format(data_type type, void* pdata, Ppsispec_default_buf pbuf)
{
    uint8_t *ptr = {0};
    uint32_t pos = 0;        //ptr current position.
    
    if(pdata==NULL || pbuf==NULL)
        return vatek_result_invalidparm;
    
    if(data_type_chan==type)
    {
        psispec_default_iso13818_channel chan = *((Ppsispec_default_iso13818_channel)pdata);
        
        ptr = pbuf->buf;
        WBUF( chan.transport_stream_id, ptr, pos)
        pos += LEN(chan.transport_stream_id);
        
        pbuf->buf_len = pos;
        
    }
    else if(type==data_type_prog)
    {
        psispec_default_iso13818_program prog = *((Ppsispec_default_iso13818_program)pdata);
        
        ptr = pbuf->buf;
        WBUF( prog.program_number, ptr, pos)
        pos += LEN(prog.program_number);
        
        pbuf->buf_len = pos;
    }
    else if(type==data_type_stream)
    {
        psispec_default_stream_tr stream = *((Ppsispec_default_stream_tr)pdata);
        ptr = pbuf->buf;
        memset( &ptr[pos], 0, BUF_ALIGN(stream.es_len));
        memcpy( &ptr[pos], &stream.es, stream.es_len);
        
        pbuf->buf_len = BUF_ALIGN(stream.es_len);
    }
    
    return vatek_result_success;
}

static vatek_result dvb_format(data_type type, void* pdata, Ppsispec_default_buf pbuf)
{
    uint8_t *ptr = {0};
    uint32_t pos = 0;        //ptr current position.
    
    if(pdata==NULL || pbuf==NULL)
        return vatek_result_invalidparm;    

    if(data_type_chan==type)
    {
        psispec_default_dvb_channel chan = *((Ppsispec_default_dvb_channel)pdata);
        if( chan.network_name->len > DVBSTR_NWTWORK_NAME_MAXLEN)
            return vatek_result_overrange;
        
        ptr = pbuf->buf;
        WBUF( chan.transport_stream_id, ptr, pos)
        pos += LEN( chan.transport_stream_id);
        WBUF( chan.network_id, ptr, pos)
        pos += LEN( chan.network_id);
        WBUF( DVBSTR_NWTWORK_NAME_MAXLEN, ptr, pos)
        pos += LEN( DVBSTR_NWTWORK_NAME_MAXLEN);    
        memset( &ptr[pos], 0, BUF_ALIGN( DVBSTR_NWTWORK_NAME_MAXLEN));
        memcpy( &ptr[pos], chan.network_name->text, chan.network_name->len);
        pos += BUF_ALIGN( DVBSTR_NWTWORK_NAME_MAXLEN);
        
        pbuf->buf_len = pos;
        
    }
    else if(type==data_type_prog)
    {
        psispec_default_dvb_program prog = *((Ppsispec_default_dvb_program)pdata);
        if( prog.service_name->len > DVBSTR_NWTWORK_NAME_MAXLEN)
            return vatek_result_overrange;
        
        ptr = pbuf->buf;
        WBUF( prog.original_network_id, ptr, pos)
        pos += LEN( prog.original_network_id);
        WBUF( prog.program_number, ptr, pos)
        pos += LEN( prog.program_number);
        WBUF( prog.channel_no, ptr, pos)
        pos += LEN( prog.channel_no);
        WBUF( DVBSTR_SERVICE_NAME_MAXLEN, ptr, pos)
        pos += LEN(DVBSTR_SERVICE_NAME_MAXLEN);
        memset( &ptr[pos], 0, BUF_ALIGN( DVBSTR_SERVICE_NAME_MAXLEN));
        memcpy( &ptr[pos], prog.service_name->text, prog.service_name->len);
        pos += BUF_ALIGN( DVBSTR_SERVICE_NAME_MAXLEN);

        pbuf->buf_len = pos;
    }
    else if(type==data_type_stream)
    {
        psispec_default_stream_tr stream = *((Ppsispec_default_stream_tr)pdata);
        ptr = pbuf->buf;
        memset( &ptr[pos], 0, BUF_ALIGN(stream.es_len));
        memcpy( &ptr[pos], &stream.es, stream.es_len);
        
        pbuf->buf_len = BUF_ALIGN(stream.es_len);
    }
    
    return vatek_result_success;
}

static vatek_result arib_format(data_type type, void* pdata, Ppsispec_default_buf pbuf)
{
    uint8_t *ptr = {0};
    uint32_t pos = 0;        //ptr current position.
    
    if(pdata==NULL || pbuf==NULL)
        return vatek_result_invalidparm;

    if(data_type_chan==type)
    {
        psispec_default_arib_channel chan = *((Ppsispec_default_arib_channel)pdata);
        if( chan.network_name->len > ARIBSTR_NETWORK_NAME_MAXLEN)
            return vatek_result_overrange;
        
        ptr = pbuf->buf;
        WBUF( chan.region_id, ptr, pos)
        pos += LEN( chan.region_id);
        WBUF( chan.broadcaster_id, ptr, pos)
        pos += LEN( chan.broadcaster_id);
        WBUF( chan.remote_control_key_id, ptr, pos)
        pos += LEN( chan.remote_control_key_id);
        WBUF( ARIBSTR_NETWORK_NAME_MAXLEN, ptr, pos)
        pos += LEN( ARIBSTR_NETWORK_NAME_MAXLEN);
        memset( &ptr[pos], 0, BUF_ALIGN( ARIBSTR_NETWORK_NAME_MAXLEN));
        memcpy( &ptr[pos], chan.network_name->text, chan.network_name->len);
        pos += BUF_ALIGN( ARIBSTR_NETWORK_NAME_MAXLEN);
        
        pbuf->buf_len = pos;
        
    }
    else if(type==data_type_prog)
    {
        psispec_default_arib_program prog = *((Ppsispec_default_arib_program)pdata);
        if( prog.service_name->len > ARIBSTR_SERVICE_NAME_MAXLEN || prog.ts_name->len > ARIBSTR_TS_NAME_MAXLEN)
            return vatek_result_overrange;
        
        ptr = pbuf->buf;
        WBUF( prog.service_no, ptr, pos)
        pos += LEN( prog.service_no);
        memcpy( &ptr[pos], prog.main_lang.raw, sizeof(prog.main_lang));
        pos += sizeof(prog.main_lang);
        memcpy( &ptr[pos], prog.sub_lang.raw, sizeof(prog.sub_lang));
        pos += sizeof(prog.sub_lang);
        WBUF( prog.copy_flag, ptr, pos)
        pos += LEN( prog.copy_flag);
        WBUF( ARIBSTR_SERVICE_NAME_MAXLEN, ptr, pos)
        pos += LEN( ARIBSTR_SERVICE_NAME_MAXLEN);
        memset( &ptr[pos], 0, BUF_ALIGN( ARIBSTR_SERVICE_NAME_MAXLEN));
        memcpy( &ptr[pos], prog.service_name->text, prog.service_name->len);
        pos += BUF_ALIGN( ARIBSTR_SERVICE_NAME_MAXLEN);
        WBUF( ARIBSTR_TS_NAME_MAXLEN, ptr, pos)
        pos += LEN( ARIBSTR_TS_NAME_MAXLEN);
        memset( &ptr[pos], 0, BUF_ALIGN( ARIBSTR_TS_NAME_MAXLEN));
        memcpy( &ptr[pos], prog.ts_name->text, prog.ts_name->len);
        pos += BUF_ALIGN( ARIBSTR_TS_NAME_MAXLEN);

        pbuf->buf_len = pos;
    }
    else if(type==data_type_stream)
    {
        psispec_default_stream_tr stream = *((Ppsispec_default_stream_tr)pdata);
        
        ptr = pbuf->buf;
        memset( &ptr[pos], 0, BUF_ALIGN(stream.es_len));
        memcpy( &ptr[pos], &stream.es, stream.es_len);
        
        pbuf->buf_len = BUF_ALIGN(stream.es_len);
    }
    
    return vatek_result_success;
}

static vatek_result abnt_format(data_type type, void* pdata, Ppsispec_default_buf pbuf)
{
    uint8_t *ptr = {0};
    uint32_t pos = 0;        //ptr current position.
    
    if(pdata==NULL || pbuf==NULL)
        return vatek_result_invalidparm;

    if(data_type_chan==type)
    {
        psispec_default_abnt_channel chan = *((Ppsispec_default_abnt_channel)pdata);
        if( chan.network_name->len > ABNTSTR_NETWORK_NAME_MAXLEN)
            return vatek_result_overrange;
        
        ptr = pbuf->buf;
        WBUF( chan.remote_control_key_id, ptr, pos)
        pos += LEN( chan.remote_control_key_id);
        WBUF( chan.original_network_id, ptr, pos)
        pos += LEN( chan.original_network_id);
        WBUF( chan.area_code, ptr, pos)
        pos += LEN( chan.area_code);
        WBUF( ABNTSTR_NETWORK_NAME_MAXLEN, ptr, pos)
        pos += LEN( ABNTSTR_NETWORK_NAME_MAXLEN);
        memset( &ptr[pos], 0, BUF_ALIGN (ABNTSTR_NETWORK_NAME_MAXLEN));
        memcpy( &ptr[pos], chan.network_name->text, chan.network_name->len);
        pos += BUF_ALIGN( ABNTSTR_NETWORK_NAME_MAXLEN);
        
        pbuf->buf_len = pos;
    }
    else if(type==data_type_prog)
    {
        psispec_default_abnt_program prog = *((Ppsispec_default_abnt_program)pdata);
        if( prog.service_name->len > ABNTSTR_SERVICE_NAME_MAXLEN || prog.ts_name->len > ABNTSTR_TS_NAME_MAXLEN)
            return vatek_result_overrange;
        
        ptr = pbuf->buf;
        WBUF( prog.service_no, ptr, pos)
        pos += LEN( prog.service_no);
        memcpy( &ptr[pos], prog.main_lang.raw, sizeof(prog.main_lang));
        pos += sizeof(prog.main_lang);
        memcpy( &ptr[pos], prog.sub_lang.raw, sizeof(prog.sub_lang));
        pos += sizeof(prog.sub_lang);
        WBUF( prog.copy_flag, ptr, pos)
        pos += LEN( prog.copy_flag);
        WBUF( ABNTSTR_SERVICE_NAME_MAXLEN, ptr, pos)
        pos += LEN( ABNTSTR_SERVICE_NAME_MAXLEN);
        memset( &ptr[pos], 0, BUF_ALIGN( ABNTSTR_SERVICE_NAME_MAXLEN));
        memcpy( &ptr[pos], prog.service_name->text, prog.service_name->len);
        pos += BUF_ALIGN( ABNTSTR_SERVICE_NAME_MAXLEN);
        WBUF( ABNTSTR_TS_NAME_MAXLEN, ptr, pos)
        pos += LEN( ABNTSTR_TS_NAME_MAXLEN);
        memset( &ptr[pos], 0, BUF_ALIGN( ABNTSTR_TS_NAME_MAXLEN));
        memcpy( &ptr[pos], prog.ts_name->text, prog.ts_name->len);
        pos += BUF_ALIGN( ABNTSTR_TS_NAME_MAXLEN);

        pbuf->buf_len = pos;
    }
    else if(type==data_type_stream)
    {
        psispec_default_stream_tr stream = *((Ppsispec_default_stream_tr)pdata);
        
        ptr = pbuf->buf;
        memset( &ptr[pos], 0, BUF_ALIGN(stream.es_len));
        memcpy( &ptr[pos], &stream.es, stream.es_len);
        
        pbuf->buf_len = BUF_ALIGN(stream.es_len);
    }
    
    return vatek_result_success;
}

static vatek_result psip_format(data_type type, void* pdata, Ppsispec_default_buf pbuf)
{
    uint8_t *ptr = {0};
    uint32_t pos = 0;        //ptr current position.
    
    if(pdata==NULL || pbuf==NULL)
        return vatek_result_invalidparm;
   
    if(data_type_chan==type)
    {
        psispec_default_psip_channel chan = *((Ppsispec_default_psip_channel)pdata);
        if( chan.short_name->len > PSIPSTR_SHORT_NAME_MAXLEN)
            return vatek_result_overrange;
        
        ptr = pbuf->buf;
        WBUF( chan.psip_flags, ptr, pos)
        pos += LEN( chan.psip_flags);
        WBUF( chan.transport_stream_id, ptr, pos)
        pos += LEN( chan.transport_stream_id);
        WBUF( chan.gps_utc_offset, ptr, pos)
        pos += LEN( chan.gps_utc_offset);
        WBUF( chan.daylight_saving, ptr, pos)
        pos += LEN( chan.daylight_saving);
        WBUF( chan.cc_mode, ptr, pos)
        pos += LEN( chan.cc_mode);
        WBUF( PSIPSTR_SHORT_NAME_MAXLEN, ptr, pos)
        pos += LEN( PSIPSTR_SHORT_NAME_MAXLEN);
        memset( &ptr[pos], 0, BUF_ALIGN( PSIPSTR_SHORT_NAME_MAXLEN));
        memcpy( &ptr[pos], chan.short_name->text, chan.short_name->len);
        pos += BUF_ALIGN( PSIPSTR_SHORT_NAME_MAXLEN);
        
        pbuf->buf_len = pos;
        
    }
    else if(type==data_type_prog)
    {
        psispec_default_psip_program prog = *((Ppsispec_default_psip_program)pdata);
        if( prog.long_name->len  > PSIPSTR_LONG_NAME_MAXLEN)
            return vatek_result_overrange;
        
        ptr = pbuf->buf;
        WBUF( prog.program_number, ptr, pos)
        pos += LEN( prog.program_number);
        WBUF( prog.source_id, ptr, pos)
        pos += LEN( prog.source_id);
        WBUF( prog.channel_major, ptr, pos)
        pos += LEN( prog.channel_major);
        WBUF( prog.channel_minor, ptr, pos)
        pos += LEN( prog.channel_minor);
        WBUF( PSIPSTR_LONG_NAME_MAXLEN, ptr, pos)
        pos += LEN( PSIPSTR_LONG_NAME_MAXLEN);
        memset( &ptr[pos], 0, BUF_ALIGN( PSIPSTR_LONG_NAME_MAXLEN));
        memcpy( &ptr[pos], prog.long_name->text, prog.long_name->len);
        pos += BUF_ALIGN( PSIPSTR_LONG_NAME_MAXLEN);

        pbuf->buf_len = pos;
    }
    else if(type==data_type_stream)
    {
        psispec_default_stream_tr stream = *((Ppsispec_default_stream_tr)pdata);
        ptr = pbuf->buf;
        memset( &ptr[pos], 0, BUF_ALIGN(stream.es_len));
        memcpy( &ptr[pos], &stream.es, stream.es_len);
        pbuf->buf_len = BUF_ALIGN(stream.es_len);
    }
    
    return vatek_result_success;
}

static vatek_result psi_format(data_type type, void* pdata, Ppsispec_default_buf pbuf)
{
    vatek_result result = vatek_result_unknown;
    
    if(pdata==NULL || pbuf==NULL)
        return vatek_result_invalidparm;
    
    switch(ptable->type)
    {
        case psispec_default_iso:
            result = iso_format(type, pdata, pbuf);
            break;
        
        case psispec_default_dvb:
            result = dvb_format(type, pdata, pbuf);
            break;
        
        case psispec_default_arib:
            result = arib_format(type, pdata, pbuf);
            break;
        
        case psispec_default_abnt:
            result = abnt_format(type, pdata, pbuf);
            break;
        
        case psispec_default_psip:
            result = psip_format(type, pdata, pbuf);
            break;
    }
    
    return result;
}

/** for broadcast service */
vatek_result vatek_psispec_default_init_br(Phms_handle handle, psispec_default_type type, psispec_country_code country)
{
    vatek_result result = vatek_result_unknown;
    
    if(handle)
        ph_handle = handle;
    else return vatek_result_invalidparm;
    
    if(ptable==NULL)
    {
        ptable = (Ppsispec_default_table)malloc(sizeof(psispec_default_table));
        ptable->psi_buf = (Ppsispec_default_buf)malloc(sizeof(psispec_default_buf));
        ptable->psi_buf->buf = (uint8_t*)malloc(100);
        if(ptable==NULL || ptable->psi_buf==NULL || ptable->psi_buf->buf==NULL)
        {
            free(ptable);
            free(ptable->psi_buf);
            free(ptable->psi_buf->buf);
            return vatek_result_memfail;
        }
    }
     
    table_position = HALRANGE_PLAYLOAD_START;
    result = vatek_hms_write_hal( ph_handle, table_position++, LICENSED_DEFSPEC_V1_EN_START);
    if(is_success(result)) result = vatek_hms_write_hal( ph_handle, table_position++, type);
    if(is_success(result)) result = vatek_hms_write_hal( ph_handle, table_position++, country);
    if(is_success(result))
    {
        ptable->type    = type;
        ptable->country = country;
    }
    
    return vatek_result_success;
}

vatek_result vatek_psispec_default_config( Ppsispec_default_channel chan, Ppsispec_default_program prog)
{
    vatek_result result = vatek_result_unknown;
    
    if(chan==NULL || prog==NULL) return vatek_result_invalidparm;

    result = psi_format(data_type_chan, chan, ptable->psi_buf);
    if(is_success(result)) result = vatek_hms_write_hal( ph_handle, table_position++, ptable->psi_buf->buf_len);
    if(is_success(result))
    {
        result = vatek_hms_write_halbuf(ph_handle, table_position, ptable->psi_buf->buf, ptable->psi_buf->buf_len);
        table_position += WLEN(ptable->psi_buf->buf_len);
    }
    
    if(is_success(result)) result = psi_format(data_type_prog, prog, ptable->psi_buf);
    if(is_success(result)) result = vatek_hms_write_hal( ph_handle, table_position++, ptable->psi_buf->buf_len);
    if(is_success(result))
    {
        result = vatek_hms_write_halbuf(ph_handle, table_position, ptable->psi_buf->buf, ptable->psi_buf->buf_len);
        table_position += WLEN(ptable->psi_buf->buf_len);
    }

    return result;
}

vatek_result vatek_psispec_default_start(void)
{
    return vatek_hms_write_hal(ph_handle, table_position, LICENSED_DEFSPEC_EN_END);
}

///** for transform service */
vatek_result vatek_psispec_default_init_tr(Phms_handle handle, psispec_default_type type, psispec_country_code country)
{
    vatek_result result = vatek_result_unknown;
    
    if(handle)
    {
        ph_handle = handle;
    }else return vatek_result_invalidparm;  
    
    if(ptable==NULL)
    {
        ptable = (Ppsispec_default_table)malloc(sizeof(psispec_default_table));
        ptable->psi_buf = (Ppsispec_default_buf)malloc(sizeof(psispec_default_buf));
        ptable->psi_buf->buf = (uint8_t*)malloc(100);
        if(ptable==NULL || ptable->psi_buf==NULL || ptable->psi_buf->buf==NULL)
        {
            free(ptable);
            free(ptable->psi_buf);
            free(ptable->psi_buf->buf);
            return vatek_result_memfail;
        }
    }
    
    
    table_position = HALRANGE_PLAYLOAD_START;
    result = vatek_hms_write_hal( ph_handle, table_position++, DEFSPEC_V2_EN_START);
    if(is_success(result)) result = vatek_hms_write_hal( ph_handle, table_position++, DEFCH_EN_START);
    if(is_success(result)) result = vatek_hms_write_hal( ph_handle, table_position++, type);
    if(is_success(result)) result = vatek_hms_write_hal( ph_handle, table_position++, country);
    if(is_success(result))
    {
        ptable->type    = type;
        ptable->country = country;
    }
    
    return result;
}

vatek_result vatek_psispec_default_ch_config(Ppsispec_default_channel chan)
{
    vatek_result result = vatek_result_unknown;
    
    if(chan==NULL) return vatek_result_invalidparm;
    
    result = psi_format(data_type_chan, chan, ptable->psi_buf);
    if(is_success(result)) result = vatek_hms_write_hal( ph_handle, table_position++, ptable->psi_buf->buf_len);
    if(is_success(result))
    {
        result = vatek_hms_write_halbuf(ph_handle, table_position, ptable->psi_buf->buf, ptable->psi_buf->buf_len);
        table_position += WLEN(ptable->psi_buf->buf_len);
    }
    if(is_success(result)) result = vatek_hms_write_hal(ph_handle, table_position++, DEFCH_EN_END);
    
    return result;
}

vatek_result vatek_psispec_default_program_add(Ppsispec_default_program_tr program)
{
    vatek_result result = vatek_result_unknown;
    
    result = psi_format(data_type_prog, program, ptable->psi_buf);
    if(is_success(result)) result = vatek_hms_write_hal( ph_handle, table_position++, DEFPROG_EN_START);
    if(is_success(result)) result = vatek_hms_write_hal( ph_handle, table_position++, program->pcr_pid);
    if(is_success(result)) result = vatek_hms_write_hal( ph_handle, table_position++, program->pmt_pid);
    if(is_success(result)) result = vatek_hms_write_hal( ph_handle, table_position++, ptable->psi_buf->buf_len);
    if(is_success(result))
    {
        result = vatek_hms_write_halbuf(ph_handle, table_position, ptable->psi_buf->buf, ptable->psi_buf->buf_len);
        table_position += WLEN(ptable->psi_buf->buf_len);
    }
    
    return result;
}

vatek_result vatek_psispec_default_program_end(void)
{
    return vatek_hms_write_hal( ph_handle, table_position++, DEFPROG_EN_END);
}

vatek_result vatek_psispec_default_stream_add(Ppsispec_default_stream_tr stream)
{
    vatek_result result = vatek_result_unknown;
    uint32_t temp_pos = table_position;
    
    psi_format(data_type_stream, stream, ptable->psi_buf);
    
    result = vatek_hms_write_hal( ph_handle, table_position++, DEFSTREAM_EN_START);
    if(is_success(result)) result = vatek_hms_write_hal( ph_handle, table_position++, stream->type);
    if(is_success(result)) result = vatek_hms_write_hal( ph_handle, table_position++, stream->stream_pid);
    if(is_success(result)) result = vatek_hms_write_hal( ph_handle, table_position++, stream->es_type);
    if(is_success(result)) result = vatek_hms_write_hal( ph_handle, table_position++, stream->es_len);
    if(is_success(result))
    {
        switch(stream->type)
        {
            case psi_def_stream_type_video:
                result = tool_stream_video(stream->_stream.video_param);
            break;
            
            case psi_def_stream_type_audio:
                result = tool_stream_audio(stream->_stream.audio_param);
            break;
            
            case psi_def_stream_type_data:
                break;
            
            default:
                PSI_DEF_LOG("unsupport stream type");
                break;
        }
        table_position = temp_pos + 0x11;
    }
    if(is_success(result))
    {
        result = vatek_hms_write_halbuf(ph_handle, table_position, ptable->psi_buf->buf, ptable->psi_buf->buf_len);
        table_position += WLEN(ptable->psi_buf->buf_len);
    }
    if(is_success(result)) vatek_hms_write_hal(ph_handle, table_position++, DEFSTREAM_EN_END);
    
    return result;
}

vatek_result vatek_psispec_default_start_tr(void)
{
    return vatek_hms_write_hal( ph_handle, table_position, DEFSPEC_V2_EN_END);
}


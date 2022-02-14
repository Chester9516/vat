#include "vatek_system.h"
#include "vatek_hms.h"
#include "vatek_psispec_rule.h"

static uint32_t licensed_pos = 0;

Ppsispec_rule orig_rule = NULL;
uint8_t *g_rule_block = NULL;
uint8_t *g_program_block = NULL;    

static const licensed_table *_licensed_get_table(const char* name, const licensed_table *tables)
{
    uint32_t idx = 0;
    while(tables[idx].name != NULL)
    {
        if(strcmp(tables[idx].name, name) == 0)
            return &tables[idx];
        idx++;
    }
    return NULL;
}

static const licensed_prop *_licensed_get_prop(const char* name, const licensed_prop *props)
{
    int32_t idx = 0;
    while(props[idx].name != NULL)
    {
        if(strcmp(props[idx].name, name) == 0)
            return &props[idx];
        idx++;
    }
    return NULL;
}

static void _licensed_set_uint(int32_t pos, uint32_t *val, uint8_t* pblock)
{
    uint8_t* ptr = &pblock[pos];
    ptr[0] = *val >> 24;
    ptr[1] = *val >> 16;
    ptr[2] = *val >> 8;
    ptr[3] = *val;
}

static void _licensed_set_string(int32_t pos, uint8_t* pbuf, int32_t len, uint8_t* pblock)
{
    uint8_t* psur = &pblock[pos];
    while(len)
    {
        *psur++ = *pbuf++;
        len--;
    }
}

vatek_result vatek_psispec_rule_deinit(void)
{
    if (g_rule_block != NULL)
        free(g_rule_block);

    if (g_program_block != NULL)
        free(g_program_block);

    orig_rule = NULL;
    g_rule_block = NULL;
    g_program_block = NULL;
    
    return vatek_result_success;
}

vatek_result vatek_psispec_rule_init(Ppsispec_rule rule)
{
    uint32_t len = 0;

    if (rule == NULL || rule->len_rule_block == 0 || rule->len_program_block == 0 || 
        rule->program_block == NULL || rule->rule_block == NULL)
        return vatek_result_invalidparm;

    vatek_psispec_rule_deinit(); //reset

    len = rule->len_rule_block;
    if (len > 0)
    {
        g_rule_block = malloc(sizeof(uint8_t)*len);
        if (g_rule_block == NULL)
            return vatek_result_memfail;
        memcpy(g_rule_block, rule->rule_block, len);
    }

    len = rule->len_program_block;
    if (len > 0)
    {
        g_program_block = malloc(sizeof(uint8_t)*len);
        if (g_program_block == NULL)
            return vatek_result_memfail;
        memcpy(g_program_block, rule->program_block, len);
    }
    
    orig_rule = rule;
 
    return vatek_result_success;
}

vatek_result vatek_psispec_rule_setruletable(const char *name, uint32_t version, uint32_t interval)
{
    vatek_result result = vatek_result_overrange;

    if (name == NULL)
        return vatek_result_invalidparm;

    const licensed_table *ptr = _licensed_get_table(name, orig_rule->rule_tables);

    if (ptr != NULL)
    {
        _licensed_set_uint(ptr->offset+8, &version, g_rule_block);
        _licensed_set_uint(ptr->offset+12, &interval, g_rule_block);
        result = vatek_result_success;
    }

    return result;
}

vatek_result vatek_psispec_rule_setchannelprop(const char *name, void *buf, uint32_t len)
{
    vatek_result result = vatek_result_overrange;
    uint32_t prop_len = 0;

    if (name == NULL)
        return vatek_result_invalidparm;

    const licensed_prop *ptr = _licensed_get_prop(name, orig_rule->channel_props);

    if (ptr != NULL)
    {
        if(ptr->type & 0x8000)
        {
            prop_len = ptr->type & 0xFFF;
            if (len > prop_len)
            {
                PSISPEC_ERR("string exceeds length");
                len = prop_len;
            }
            _licensed_set_uint(ptr->offset, &len, g_rule_block);
            _licensed_set_string(ptr->offset+4, (uint8_t *)buf, len, g_rule_block);
        }
        else
        {
            _licensed_set_uint(ptr->offset, (uint32_t *)buf, g_rule_block);
        }
        result = vatek_result_success;
    }

    return result;
}

static vatek_result _vatek_psispec_rule_setprogramprop(const char *name, void* buf, uint32_t len)
{
    vatek_result result = vatek_result_overrange;
    uint32_t prop_len = 0;

    if (name == NULL)
        return vatek_result_invalidparm;

    const licensed_prop *ptr = _licensed_get_prop(name, orig_rule->program_props);

    if (ptr != NULL)
    {
        if(ptr->type & 0x8000)
        {
            prop_len = ptr->type & 0xFFF;
            if (len > prop_len)
            {
                PSISPEC_ERR("string exceeds length");
                len = prop_len;
            }
            _licensed_set_uint(ptr->offset, &len, g_program_block);
            _licensed_set_string(ptr->offset+4, (uint8_t*)buf, len, g_program_block);
        }
        else
        {
            _licensed_set_uint(ptr->offset, (uint32_t *)buf, g_program_block);
        }
        result = vatek_result_success;
    }

    return result;
}

vatek_result vatek_psispec_rule_setprogramprop_psip(Ppsip_props props)
{
    vatek_result result = vatek_result_overrange;
    uint32_t val = 0;

    if (props == NULL)
        return vatek_result_invalidparm;

    //reset program block
    memcpy(g_program_block, orig_rule->program_block, orig_rule->len_program_block);

    if (props->program_number > 0)
    {
        val = props->program_number;
        result = _vatek_psispec_rule_setprogramprop("program_number", &val, 2);
        if (result != vatek_result_success)
        {
            PSISPEC_ERR("program_number set error");
        }
    }

    if (props->source_id > 0)
    {
        val = props->source_id;
        result = _vatek_psispec_rule_setprogramprop("source_id", &val, 2);
        if (result != vatek_result_success)
        {
            PSISPEC_ERR("source_id set error");
        }
    }

    if (props->channel_major > 0)
    {
        val = props->channel_major;
        result = _vatek_psispec_rule_setprogramprop("channel_major", &val, 2);
        if (result != vatek_result_success)
        {
            PSISPEC_ERR("channel_major set error");
        }
    }

    if (props->channel_minor > 0)
    {
        val = props->channel_minor;
        result = _vatek_psispec_rule_setprogramprop("channel_minor", &val, 2);
        if (result != vatek_result_success)
        {
            PSISPEC_ERR("channel_minor set error");
        }
    }

    if (props->mode > 0)
    {
        val = props->mode;
        result = _vatek_psispec_rule_setprogramprop("mode", &val, 2);
        if (result != vatek_result_success)
        {
            PSISPEC_ERR("mode set error");
        }
    }

    if (strlen((const char *)props->channel_name) > 0)
    {
        result = _vatek_psispec_rule_setprogramprop("channel_name", props->channel_name, strlen((const char *)props->channel_name));
        if (result != vatek_result_success)
        {
            PSISPEC_ERR("channel_name set error");
        }
    }
    
    return vatek_result_success;
}

vatek_result vatek_psispec_rule_start(Phms_handle handle)
{
    vatek_result result = vatek_result_unknown;
    uint32_t len = orig_rule->len_rule_block;

    if (handle == NULL || g_rule_block == NULL)
        return vatek_result_invalidparm;

    licensed_pos = HALRANGE_PLAYLOAD_START;
    if ((result = vatek_hms_write_hal(handle, licensed_pos++, LICENSED_EN_START)) != vatek_result_success)
        return result;

    if ((result = vatek_hms_write_hal(handle, licensed_pos++, RULE_EN_START)) != vatek_result_success)
        return result;

    //HALOF_LIC_RULE_LEN
    if ((result = vatek_hms_write_hal(handle, licensed_pos++, len)) != vatek_result_success)
        return result;

    if ((result = vatek_hms_write_halbuf(handle, licensed_pos, g_rule_block, len)) != vatek_result_success)
        return result;

    licensed_pos += (len >> 2);
    
    if ((result = vatek_hms_write_hal(handle, licensed_pos++, RULE_EN_END)) != vatek_result_success)
        return result;

    return result;
}

vatek_result vatek_psispec_rule_end(Phms_handle handle)
{
    vatek_result result = vatek_result_unknown;

    if (handle == NULL)
        return vatek_result_invalidparm;

    if ((result = vatek_hms_write_hal(handle, licensed_pos, LICENSED_EN_END)) != vatek_result_success)
        return result;
    
    return result;
}

vatek_result vatek_psispec_rule_start_v2(Phms_handle handle)
{
    vatek_result result = vatek_result_unknown;
    uint32_t len = orig_rule->len_rule_block;

    if (handle == NULL || g_rule_block == NULL)
        return vatek_result_invalidparm;

    licensed_pos = HALRANGE_PLAYLOAD_START;
    if ((result = vatek_hms_write_hal(handle, licensed_pos++, LICENSED_V2_EN_START)) != vatek_result_success)
        return result;

    if ((result = vatek_hms_write_hal(handle, licensed_pos++, RULE_EN_START)) != vatek_result_success)
        return result;

    //HALOF_LIC_RULE_LEN
    if ((result = vatek_hms_write_hal(handle, licensed_pos++, len)) != vatek_result_success)
        return result;

    if ((result = vatek_hms_write_halbuf(handle, licensed_pos, g_rule_block, len)) != vatek_result_success)
        return result;

    licensed_pos += (len >> 2);
    
    if ((result = vatek_hms_write_hal(handle, licensed_pos++, RULE_EN_END)) != vatek_result_success)
        return result;

    return result;
}

vatek_result vatek_psispec_rule_end_v2(Phms_handle handle)
{
    vatek_result result = vatek_result_unknown;

    if (handle == NULL)
        return vatek_result_invalidparm;

    if ((result = vatek_hms_write_hal(handle, licensed_pos, LICENSED_V2_EN_END)) != vatek_result_success)
        return result;
    
    return result;
}

static vatek_result _vatek_psisepc_rule_addstream(Phms_handle handle, Penum_stream stream)
{
    vatek_result result = vatek_result_unknown;
    uint32_t pos = 0;
    uint32_t value = 0;

    if (handle == NULL || stream == NULL)
        return vatek_result_invalidparm;

    pos = licensed_pos;
    if ((result = vatek_hms_write_hal(handle, pos, STREAM_EN_START)) != vatek_result_success)
        return result;

    pos = licensed_pos + HALOF_LIC_STREAM_TYPE;
    switch (stream->type)
    {
        case stream_type_video:
            value = MUX_STREAM_TYPE_VIDEO;
            break;

        case stream_type_audio:
            value = MUX_STREAM_TYPE_AUDIO;
            break;

        default:
            value = MUX_STREAM_TYPE_DATA;
            break;
    }    
    if ((result = vatek_hms_write_hal(handle, pos, value)) != vatek_result_success)
        return result;

    pos = licensed_pos + HALOF_LIC_STREAM_PID;
    if ((result = vatek_hms_write_hal(handle, pos, stream->stream_pid)) != vatek_result_success)
        return result;

    pos = licensed_pos + HALOF_LIC_STREAM_ESTYPE;
    if ((result = vatek_hms_write_hal(handle, pos, stream->stream_type)) != vatek_result_success)
        return result;

    pos = licensed_pos + HALOF_LIC_STREAM_ESLEN;
    if ((result = vatek_hms_write_hal(handle, pos, stream->esinfo_len)) != vatek_result_success)
        return result;

    //set 0, (HALOF_LIC_SVIDEO_CODEC ~ HALOF_LIC_STREAM_ES)
    for (pos = (licensed_pos + HALOF_LIC_SVIDEO_CODEC); pos < (licensed_pos + HALOF_LIC_STREAM_ES); pos++)
    {
        if ((result = vatek_hms_write_hal(handle, pos, 0)) != vatek_result_success)
            return result;
    }

    licensed_pos += HALOF_LIC_STREAM_ES;
    if (stream->esinfo_len > 0)
    {
        if ((result = vatek_hms_write_halbuf(handle, licensed_pos, stream->esinfo, stream->esinfo_len)) != vatek_result_success)
            return result;
        licensed_pos += (stream->esinfo_len >> 2);
        if (stream->esinfo_len % 4 != 0)
            licensed_pos++;
    }

    if ((result = vatek_hms_write_hal(handle, licensed_pos++, STREAM_EN_END)) != vatek_result_success)
        return result;
    
    return result;
}

vatek_result vatek_psispec_rule_addprogram(Phms_handle handle, Penum_program program)
{
    vatek_result result = vatek_result_unknown;
    uint32_t idx = 0;
    uint32_t len = orig_rule->len_program_block;

    if (handle == NULL || g_program_block == NULL || program == NULL)
        return vatek_result_invalidparm;

    if ((result = vatek_hms_write_hal(handle, licensed_pos++, PROGRAM_EN_START)) != vatek_result_success)
        return result;

    //HALOF_LIC_PROGRAM_PCR_PID: offset is 0x1
    if ((result = vatek_hms_write_hal(handle, licensed_pos++, program->pcr_pid)) != vatek_result_success)
        return result;
    
    //HALOF_LIC_PROGRAM_PMT_PID: offset is 0x2
    if ((result = vatek_hms_write_hal(handle, licensed_pos++, program->pmt_pid)) != vatek_result_success)
        return result;

    //HALOF_LIC_PROGRAM_LEN: offset is 0x3
    if ((result = vatek_hms_write_hal(handle, licensed_pos++, len)) != vatek_result_success)
        return result;

    if ((result = vatek_hms_write_halbuf(handle, licensed_pos, g_program_block, len)) != vatek_result_success)
        return result;
    licensed_pos += (len >> 2);

    //add stream
    for (idx = 0; idx < program->stream_num; idx++)
    {
        if ((result = _vatek_psisepc_rule_addstream(handle, &program->stream[idx])) != vatek_result_success)
            return result;
    }
    
    if ((result = vatek_hms_write_hal(handle, licensed_pos++, PROGRAM_EN_END)) != vatek_result_success)
        return result;

    return result;
}

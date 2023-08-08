
#include "vatek_system.h"
#include "vatek_hms.h"
#include "vatek_psitable.h"

static uint32_t mux_tables_pos = 0;
static uint32_t mux_start_ptr = 0;
static uint32_t mux_end_ptr = 0;
static uint32_t mux_stream_size = 0;
static uint32_t mux_write_ptr = 0;
static uint32_t mux_read_ptr = 0;

#define is_mux_position_valid(pos) (pos >= HALRANGE_PLAYLOAD_START && pos < HALRANGE_PLAYLOAD_END)

static vatek_result psitable_register_start(void)
{
    mux_tables_pos = HALRANGE_PLAYLOAD_START;       

    return vatek_result_success;
}

static vatek_result psitable_register_put(Phms_handle handle, Ppsitable_parm ptable)
{
    vatek_result result = vatek_result_unknown;
    uint32_t poslen = 0;
    uint8_t *pbuf = NULL;

    if (handle == NULL || ptable == NULL)
        return vatek_result_invalidparm;

    if (ptable->tspacket_num == 0)
        return vatek_result_invalidparm;

    if (!is_mux_position_valid(mux_tables_pos))
        return vatek_result_badstatus;

    poslen = (PSI_TSPACKET_WLEN * ptable->tspacket_num) + 3;
    
    if ((mux_tables_pos + poslen) >= HALRANGE_PLAYLOAD_END)
        return vatek_result_overrange;

    if ((result = vatek_hms_write_hal(handle, mux_tables_pos++, RAWPSI_EN_TAG)) != vatek_result_success)
        return result;

    if ((result = vatek_hms_write_hal(handle, mux_tables_pos++, ptable->interval_ms)) != vatek_result_success)
        return result;

    if ((result = vatek_hms_write_hal(handle, mux_tables_pos++, ptable->tspacket_num)) != vatek_result_success)
        return result;

    pbuf = ptable->tspackets;
    poslen = ptable->tspacket_num;
    
    while (poslen)
    {
        if ((result = vatek_hms_write_halbuf(handle, mux_tables_pos, pbuf, PSI_TSPACKET_LEN)) != vatek_result_success)
            return result;
        mux_tables_pos += PSI_TSPACKET_WLEN;
        pbuf += PSI_TSPACKET_LEN;
        poslen--;
    }
    
    return result;
}

static vatek_result psitable_register_end(Phms_handle handle)
{
    vatek_result result = vatek_result_unknown;
    
    if (handle == NULL)
        return vatek_result_invalidparm;

    if (!is_mux_position_valid(mux_tables_pos))
        return vatek_result_badstatus;
    
    if ((result = vatek_hms_write_hal(handle, mux_tables_pos, RAWPSI_EN_ENDTAG)) != vatek_result_success)
        return result;

    mux_tables_pos = 0;
    return result;
}

static vatek_result psitable_insert_start(Phms_handle handle)
{
    vatek_result result = vatek_result_unknown;
    uint32_t value = 0;
    
    if (handle == NULL)
        return vatek_result_invalidparm;

    if ((result = vatek_hms_read_hal(handle, HALREG_PRIVATE_START, &mux_start_ptr)) != vatek_result_success)
        return result;

    if ((result = vatek_hms_read_hal(handle, HALREG_PRIVATE_END, &mux_end_ptr)) != vatek_result_success)
        return result;

    if(mux_start_ptr == 0 || mux_end_ptr == 0 || (mux_start_ptr >= mux_end_ptr))
    {
        PSITABLE_ERR("wrong psi insert status: 0x%08lx-0x%08ldx", mux_start_ptr, mux_end_ptr);    
        return vatek_result_badstatus;
    }

    mux_stream_size = mux_end_ptr - mux_start_ptr;
    if ((result = vatek_hms_read_hal(handle, HALREG_PRIVATE_CNTL, &value)) != vatek_result_success)
        return result;

    if ((result = vatek_hms_read_hal(handle, HALREG_PRIVATE_WPTR, &mux_write_ptr)) != vatek_result_success)
        return result;

    if (value == 0)
        result = vatek_hms_write_hal(handle, HALREG_PRIVATE_CNTL, PRIVATE_EN_TIMES);
    else if (value == PRIVATE_EN_CONTINUE)
        result = vatek_result_success;
    else
        result = vatek_result_badstatus;
    
    return result;
}

static vatek_result psitable_insert_gettspacketnum(Phms_handle handle, uint32_t *tspacket_num)
{
    vatek_result result = vatek_result_unknown;
    int32_t len = 0;
    
    if (handle == NULL || tspacket_num == NULL)
        return vatek_result_invalidparm;

    if ((result = vatek_hms_read_hal(handle, HALREG_PRIVATE_RPTR, &mux_read_ptr)) != vatek_result_success)
        return result;
    
    len = mux_stream_size - (mux_write_ptr - mux_read_ptr);
    if(len <= 0)
        len += mux_stream_size;
    *tspacket_num = len / PSI_TSPACKET_WLEN;

    return result;
}

static vatek_result psitable_insert_write(Phms_handle handle, uint32_t tspacket_num, uint8_t* tspackets)
{
    
    vatek_result result = vatek_result_unknown;
    uint32_t validnums = 0;
    uint8_t *p_tspackets = NULL;
    
    if (handle == NULL || tspackets == NULL || tspacket_num == 0)
        return vatek_result_invalidparm;

    if ((result = psitable_insert_gettspacketnum(handle, &validnums)) != vatek_result_success)
        return result;

    if (tspacket_num > validnums)
    {
        PSITABLE_ERR("psi insert buffer overflow");
        return vatek_result_bufoverflow;
    }

    validnums = tspacket_num;
    p_tspackets = tspackets;
    while(validnums)
    {
        if ((result = vatek_hms_write_halbuf(handle, mux_write_ptr, p_tspackets, PSI_TSPACKET_LEN)) != vatek_result_success)
            return result;
        mux_write_ptr += PSI_TSPACKET_WLEN;
        if(mux_write_ptr >= mux_end_ptr)
            mux_write_ptr = mux_start_ptr;
        p_tspackets += PSI_TSPACKET_LEN;
        validnums--;
    }

    if ((result = vatek_hms_write_hal(handle, HALREG_PRIVATE_WPTR, mux_write_ptr)) != vatek_result_success)
            return result;

    return result;
}

vatek_result vatek_psitable_register(Phms_handle handle, Ppsitablelist_parm parm)
{
    vatek_result result = vatek_result_unknown;
    uint32_t idx = 0;

    if (handle == NULL || parm == NULL || parm->table_num == 0)
        return vatek_result_invalidparm;

    if ((result = vatek_hms_issystemidle(handle)) != vatek_result_success)
        return result;

    if ((result = psitable_register_start()) != vatek_result_success)
        return result;

    for (idx = 0; idx < parm->table_num; idx++)
    {
        if ((result = psitable_register_put(handle, &parm->table[idx])) != vatek_result_success)
            return result;
    }

    if ((result = psitable_register_end(handle)) != vatek_result_success)
        return result;

    return result;
}

vatek_result vatek_psitable_insert(Phms_handle handle, uint16_t tspacket_num, uint8_t *tspackets)
{
    vatek_result result = vatek_result_unknown;

    if (handle == NULL || tspackets == NULL || tspacket_num == 0)
        return vatek_result_invalidparm;

    if (vatek_hms_issystemrun(handle) != vatek_result_success)
        return vatek_result_idle;       

    if ((result = psitable_insert_start(handle)) != vatek_result_success)
        return result; 

    if ((result = psitable_insert_write(handle, tspacket_num, tspackets)) != vatek_result_success)
        return result;
    
    return result;
}


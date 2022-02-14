
#include "vatek_system.h"
#include "vatek_hms.h"

#define I2CCMD_BUF_LEN		9
#define I2CCMD_WRITE		0x07
#define I2CCMD_READ			0x05
#define I2CCMD_REG_WRITE    0x06
#define I2CCMD_REG_READ		0x04
#define I2CCHIP_ADDRESS		0x10
#define I2CCHIP_WRADDR		(I2CCHIP_ADDRESS << 1)
#define I2CCHIP_RDADDR		(I2CCHIP_WRADDR | 1)

static uint8_t i2c_cmd[I2CCMD_BUF_LEN];

static vatek_result hms_i2c_write(Pvatek_i2c vi2c, uint8_t* pbuf, uint32_t len)
{
    vatek_result result = vatek_result_unknown;

    if (vi2c == NULL || pbuf == NULL || len == 0)
        return vatek_result_invalidparm;

    if ((result = vi2c->start(vi2c->hboard, I2CCHIP_WRADDR, 0)) != vatek_result_success)
        return vatek_result_i2cfail;

    if ((result = vi2c->write(vi2c->hboard, pbuf, len)) != vatek_result_success)
        return vatek_result_i2cfail;

    if ((result = vi2c->stop(vi2c->hboard)) != vatek_result_success)
        return vatek_result_i2cfail;
    
    return result;
}

static vatek_result hms_i2c_read(Pvatek_i2c vi2c, uint8_t* pbuf, uint32_t len)
{
    vatek_result result = vatek_result_unknown;
    
    if (vi2c == NULL || pbuf == NULL || len == 0)
        return vatek_result_invalidparm;

    if ((result = vi2c->start(vi2c->hboard, I2CCHIP_RDADDR, 0)) != vatek_result_success)
        return vatek_result_i2cfail;

    if ((result = vi2c->read(vi2c->hboard, pbuf, len)) != vatek_result_success)
        return vatek_result_i2cfail;

    if ((result = vi2c->stop(vi2c->hboard)) != vatek_result_success)
        return vatek_result_i2cfail;
    
    return result;
}

vatek_result vatek_hms_create(Pboard_handle hboard, Phms_handle *handle)
{
    vatek_result result = vatek_result_unknown;
    Phms_handle newhms = NULL;

    if (hboard == NULL || handle == NULL)
        return vatek_result_invalidparm;

    newhms = (Phms_handle)malloc(sizeof(hms_handle));
    if (newhms == NULL)
        return vatek_result_memfail;

    if ((result = vatek_system_i2c_create(hboard, &newhms->vi2c)) != vatek_result_success)
    {
        free(newhms);
        return result;
    }
    *handle = newhms;

    result = vatek_result_success;

    return result;
}

vatek_result vatek_hms_destroy(Phms_handle handle)
{    
    vatek_result result = vatek_result_unknown;

    if (handle == NULL)
        return vatek_result_invalidparm;

    if ((result = vatek_system_i2c_destroy(handle->vi2c)) != vatek_result_success)
        return result;

    free(handle);
    handle = NULL;
    result = vatek_result_success;

    return result;
}

vatek_result vatek_hms_reset(Phms_handle handle)
{
    vatek_result result = vatek_result_unknown;

    if (handle == NULL)
        return vatek_result_invalidparm;

    if ((result = vatek_system_reset(sys_dev_type_main, handle->vi2c->hboard)) != vatek_result_success)
        return result;

    return result;
}

#if defined(DEBUG)
vatek_result vatek_hms_read_reg(Phms_handle handle, uint32_t addr, uint32_t *val)
{  /* for internal debug only */
    vatek_result result = vatek_result_unknown;

    if (handle == NULL || val == NULL)
        return vatek_result_invalidparm;

    i2c_cmd[0] = 0x04;
		i2c_cmd[1] = (uint8_t)addr;
		i2c_cmd[2] = (uint8_t)(addr >> 8);
		i2c_cmd[3] = (uint8_t)(addr >> 16);
		i2c_cmd[4] = I2CCMD_REG_READ;
    
    if ((result = hms_i2c_write(handle->vi2c, &i2c_cmd[0], 5)) != vatek_result_success)
        return result;
    
    i2c_cmd[0] = 0;
    if ((result = hms_i2c_write(handle->vi2c, &i2c_cmd[0], 1)) != vatek_result_success)
        return result;

    if ((result = hms_i2c_read(handle->vi2c, &i2c_cmd[0], 4)) != vatek_result_success)
        return result;

    *val = i2c_cmd[0] | (i2c_cmd[1] << 8) | (i2c_cmd[2] << 16) | (i2c_cmd[3] << 24);

    return result;
}

vatek_result vatek_hms_write_reg(Phms_handle handle, uint32_t addr, uint32_t val)
{
    vatek_result result = vatek_result_unknown;

    if (handle == NULL)
        return vatek_result_invalidparm;

    i2c_cmd[0] = 0x00;
		i2c_cmd[1] = (uint8_t)val;
		i2c_cmd[2] = (uint8_t)(val >> 8);
		i2c_cmd[3] = (uint8_t)(val >> 16);
		i2c_cmd[4] = (uint8_t)(val >> 24);
		i2c_cmd[5] = (uint8_t)addr;
		i2c_cmd[6] = (uint8_t)(addr >> 8);
		i2c_cmd[7] = (uint8_t)(addr >> 16);
		i2c_cmd[8] = I2CCMD_REG_WRITE;

    if ((result = hms_i2c_write(handle->vi2c, &i2c_cmd[0], 9)) != vatek_result_success)
        return result;

    return result;
}
#endif

vatek_result vatek_hms_read_hal(Phms_handle handle, uint32_t addr, uint32_t *val)
{
    vatek_result result = vatek_result_unknown;

    if (handle == NULL || val == NULL)
        return vatek_result_invalidparm;

    i2c_cmd[0] = 0x04;
		i2c_cmd[1] = (uint8_t)addr;
		i2c_cmd[2] = (uint8_t)(addr >> 8);
		i2c_cmd[3] = (uint8_t)(addr >> 16);
		i2c_cmd[4] = I2CCMD_READ;
    
    if ((result = hms_i2c_write(handle->vi2c, &i2c_cmd[0], 5)) != vatek_result_success)
        return result;
    
    i2c_cmd[0] = 0;
    if ((result = hms_i2c_write(handle->vi2c, &i2c_cmd[0], 1)) != vatek_result_success)
        return result;

    if ((result = hms_i2c_read(handle->vi2c, &i2c_cmd[0], 4)) != vatek_result_success)
        return result;

    *val = i2c_cmd[0] | (i2c_cmd[1] << 8) | (i2c_cmd[2] << 16) | (i2c_cmd[3] << 24);

    return result;
}

vatek_result vatek_hms_write_hal(Phms_handle handle, uint32_t addr, uint32_t val)
{
    vatek_result result = vatek_result_unknown;

    if (handle == NULL)
        return vatek_result_invalidparm;

    i2c_cmd[0] = 0x00;
		i2c_cmd[1] = (uint8_t)val;
		i2c_cmd[2] = (uint8_t)(val >> 8);
		i2c_cmd[3] = (uint8_t)(val >> 16);
		i2c_cmd[4] = (uint8_t)(val >> 24);
		i2c_cmd[5] = (uint8_t)addr;
		i2c_cmd[6] = (uint8_t)(addr >> 8);
		i2c_cmd[7] = (uint8_t)(addr >> 16);
		i2c_cmd[8] = I2CCMD_WRITE;

    if ((result = hms_i2c_write(handle->vi2c, &i2c_cmd[0], 9)) != vatek_result_success)
        return result;

    return result;
}

vatek_result vatek_hms_read_halbuf(Phms_handle handle, uint32_t addr, uint8_t* pbuf, uint32_t len)
{
    vatek_result result = vatek_result_unknown;
    uint32_t val = 0, idx = 0;

    if (handle == NULL || pbuf == NULL)
        return vatek_result_invalidparm;

    if ((len % 4) != 0)
        return vatek_result_format;
    
    for (idx = 0; idx < len ; idx+=4)
    {
        val = 0;
        result = vatek_hms_read_hal(handle, addr, &val);
        pbuf[idx] = val & 0xFF;
        pbuf[idx+1] = (val >> 8) & 0xFF;
        pbuf[idx+2] = (val >> 16) & 0xFF;
        pbuf[idx+3] = (val >> 24) & 0xFF;
        addr++;
    }
    
    return result;
}

vatek_result vatek_hms_write_halbuf(Phms_handle handle, uint32_t addr, uint8_t* pbuf, uint32_t len)
{
    vatek_result result = vatek_result_unknown;
    uint32_t val = 0, idx = 0;

    if (handle == NULL || pbuf == NULL)
        return vatek_result_invalidparm;

    if ((len % 4) != 0)
        return vatek_result_format;

    for (idx = 0; idx < len ; idx+=4)
    {
        val = pbuf[idx+3] + (pbuf[idx+2] << 8) + (pbuf[idx+1] << 16) + (pbuf[idx] << 24);
        result = vatek_hms_write_hal(handle, addr, val);
        addr++;
    }
    
    return result;
}

vatek_result vatek_hms_issystemidle(Phms_handle handle)
{
    vatek_result result = vatek_result_unknown;
    uint32_t val = 0;
    
    if (handle == NULL)
        return vatek_result_invalidparm;

    if ((result = vatek_hms_read_hal(handle, HALREG_SYS_STATUS_0, &val)) == vatek_result_success)
    {
        if (val != SYS_STATUS_IDLE)
            return vatek_result_busy;

        if ((result = vatek_hms_read_hal(handle, HALREG_BROADCAST_CNTL, &val)) == vatek_result_success)
        {
            if (val != 0)
                return vatek_result_badstatus;
        }
    }
    
    return result;
}

vatek_result vatek_hms_issystemrun(Phms_handle handle)
{
    vatek_result result = vatek_result_unknown;
    uint32_t val = 0;
    
    if (handle == NULL)
        return vatek_result_invalidparm;

    if ((result = vatek_hms_read_hal(handle, HALREG_SYS_STATUS_0, &val)) == vatek_result_success)
    {
        if (val != SYS_STATUS_RUN)
            return vatek_result_badstatus;
    }
    
    return result;
}


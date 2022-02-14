#include <service/service_broadcast.h>
#include <service/ui/ui_service_broadcast.h>

vatek_result broadcast_param_set(hvatek_chip hchip,Pbroadcast_param pbc)
{
    vatek_result nres = modulator_param_set(hchip,&pbc->mod);
    if(is_vatek_success(nres))
    {
        nres = encoder_param_set(hchip,&pbc->enc);
        if(is_vatek_success(nres))
            nres = writehal(HALREG_BROADCAST_STREAM, STREAM_ENCODER);
        if(is_vatek_success(nres))
            nres = ui_props_write_hal(hchip,_ui_struct(mux_param),(uint8_t*)&pbc->mux);
    }
    return nres;
}

vatek_result broadcast_param_get(hvatek_chip hchip,Pbroadcast_param pbc)
{
    uint32_t val = 0;
    vatek_result nres = modulator_param_get(hchip, &pbc->mod);
    if(is_vatek_success(nres))
    {
        nres = readhal(HALREG_BROADCAST_STREAM,&val);
        if(is_vatek_success(nres))
        {
            if (val == STREAM_ENCODER)nres = encoder_param_get(hchip, &pbc->enc);
            else nres = vatek_badparam;
            if(is_vatek_success(nres))
                nres = ui_props_read_hal(hchip,_ui_struct(mux_param),(uint8_t*)&pbc->mux);
        }
    }
    return nres;
}

vatek_result broadcast_param_reset(vatek_ic_module icchip, Pbroadcast_param pbc)
{
    vatek_result nres = vatek_badparam;
    if (is_chip_b_serial(icchip))
    {
        nres = modulator_param_reset(modulator_dvb_t, &pbc->mod);
        if (icchip == ic_module_b3_lite)
            nres = encoder_param_reset(encvideo_h264, &pbc->enc);
        else nres = encoder_param_reset(encvideo_mpeg2, &pbc->enc);
        memcpy(&pbc->mux, &default_mux_param, sizeof(mux_param));
    }
    return nres;
}


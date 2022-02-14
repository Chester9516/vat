#include <core/base/stream_encoder.h>
#include <core/ui/ui_props/ui_props_encoder.h>
#include <core/ui/ui_props_api.h>

vatek_result encoder_param_set(hvatek_chip hchip,Pencoder_param penc)
{
    vatek_result nres = vatek_badparam;
    if(is_current_encmode(penc->mode))
    {
        nres = ui_props_write_hal(hchip,_ui_struct(encoder_param),(uint8_t*)penc);
        if(is_vatek_success(nres))
            nres = ui_props_write_hal(hchip,_ui_struct(vi_param),(uint8_t*)&penc->viparam);
        if(is_vatek_success(nres))
            nres = ui_props_write_hal(hchip,_ui_struct(quality_param),(uint8_t*)&penc->quality);
        if(is_vatek_success(nres))
            nres = ui_props_write_hal(hchip,_ui_struct(media_video),(uint8_t*)&penc->video);
        if(is_vatek_success(nres))
            nres = ui_props_write_hal(hchip,_ui_struct(media_audio),(uint8_t*)&penc->audio);
    }
    return nres;
}

vatek_result encoder_param_get(hvatek_chip hchip,Pencoder_param penc)
{
    vatek_result  nres = ui_props_read_hal(hchip,_ui_struct(encoder_param),(uint8_t*)penc);
    if(is_vatek_success(nres))
    {
        if(is_current_encmode(penc->mode))
        {
            nres = ui_props_read_hal(hchip,_ui_struct(vi_param),(uint8_t*)&penc->viparam);
            if(is_vatek_success(nres))
                nres = ui_props_read_hal(hchip,_ui_struct(quality_param),(uint8_t*)&penc->quality);
            if(is_vatek_success(nres))
                nres = ui_props_read_hal(hchip,_ui_struct(media_video),(uint8_t*)&penc->video);
            if(is_vatek_success(nres))
                nres = ui_props_read_hal(hchip,_ui_struct(media_audio),(uint8_t*)&penc->audio);
        }else nres = vatek_badparam;
    }
    return nres;
}

vatek_result encoder_param_reset(video_codec vcodec, Pencoder_param penc)
{
    memcpy(penc, (uint8_t*)&default_encoder_param, sizeof(encoder_param));
    penc->video.vcodec = vcodec;
    if (vcodec != encvideo_mpeg2)
    {
        penc->quality.minq = 10;
        penc->quality.maxq = 25;
    }
    return vatek_success;
}

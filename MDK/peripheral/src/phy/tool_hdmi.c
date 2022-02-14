
#include "vatek_system.h"
#include "phy/tool_hdmi.h"


static vatek_result get_pixel( Phdmi_video_timing ptiming)
{    
    if( ptiming==NULL)
        return vatek_result_invalidparm;
    
    if(is_1080(ptiming->resolution))
    {
        ptiming->pixel_per_line  = 1920;
        ptiming->pixel_per_frame = 1080;
        
    }else if(is_720(ptiming->resolution))
    {
        ptiming->pixel_per_line  = 1280;
        ptiming->pixel_per_frame = 720;
        
    }else if(is_576(ptiming->resolution))
    {
        ptiming->pixel_per_line  = 720;
        ptiming->pixel_per_frame = 576;
        
    }else if(is_480(ptiming->resolution))
    {
        ptiming->pixel_per_line  = 720;
        ptiming->pixel_per_frame = 480;
        
    }else return vatek_result_unsupport;
    
    if( is_interlaced(ptiming->resolution))
    {
        ptiming->pixel_per_frame >>= 1;
        ptiming->_int = 1;
    }

    return vatek_result_success;
}

vatek_result tool_hdmi_get_vid(Pphy_video_info pinfo, uint8_t *vid)
{
    if( pinfo==NULL)
        return vatek_result_invalidparm;
    
    for( uint8_t i=0; i<VIDEO_INFO_NUMS; i++)
    {
        if( pinfo->resolution == hdmi_video_table[i].resolution &&
            pinfo->aspectrate == hdmi_video_table[i].aspectrate)
        {
            *vid = hdmi_video_table[i].vid;
            return vatek_result_success;
        }
    }
    
    return vatek_result_unsupport;
}

vatek_result tool_hdmi_get_videoinfo(uint8_t vid, Phdmi_video_timing pvid)
{
    vatek_result result = vatek_result_unsupport;
    int32_t i = 0;

    if (pvid == NULL)
        return vatek_result_invalidparm;
    
    for(i = 0; i < VIDEO_INFO_NUMS; i++)
    {
        if(hdmi_video_table[i].vid == vid)
        {
            pvid->vid        = hdmi_video_table[i].vid;
            pvid->represent  = hdmi_video_table[i].represent;
            pvid->resolution = hdmi_video_table[i].resolution;
            pvid->aspectrate = hdmi_video_table[i].aspectrate;
            pvid->hfp        = hdmi_video_table[i].hfp;
            pvid->hbp        = hdmi_video_table[i].hbp;
            pvid->hpw        = hdmi_video_table[i].hpw;
            pvid->vfp        = hdmi_video_table[i].vfp;
            pvid->vbp        = hdmi_video_table[i].vbp;
            pvid->vpw        = hdmi_video_table[i].vpw;
            
            if((result = get_pixel(pvid))==vatek_result_success)
                return vatek_result_success;
        }
    }
    return result;
}


#if 0
vatek_result tool_ep9xxx_get_videoinfo(uint8_t vid, Pep9xxx_video_info info)
{
    vatek_result result = vatek_result_unsupport;
    int32_t i = 0;

    if (info == NULL)
        return vatek_result_invalidparm;
    
    for(i = 0; i < VIDEO_INFO_NUMS; i++)
    {
        if(ep9xxx_videoinfo_tablelist[i].vid == vid)
        {
            info->vid = ep9xxx_videoinfo_tablelist[i].vid;
            info->represent = ep9xxx_videoinfo_tablelist[i].represent;
            info->resolution = ep9xxx_videoinfo_tablelist[i].resolution;
            info->aspectrate = ep9xxx_videoinfo_tablelist[i].aspectrate;
            return vatek_result_success;
        }
    }
        
    return result;
}
#endif

void tool_hdmi_print_videotiming(Phdmi_video_timing ptiming)
{
    VATEK_LOG(hdmi,"printf video timing");
    VATEK_LOG(hdmi,"    APPL : %d", ptiming->pixel_per_line);
    VATEK_LOG(hdmi,"    ALPF : %d", ptiming->pixel_per_frame);
    VATEK_LOG(hdmi,"    HFP  : %d", ptiming->hfp);
    VATEK_LOG(hdmi,"    HBP  : %d", ptiming->hbp);
    VATEK_LOG(hdmi,"    HPW  : %d", ptiming->hpw);
    VATEK_LOG(hdmi,"    VFP  : %d", ptiming->vfp);
    VATEK_LOG(hdmi,"    VPW  : %d", ptiming->vpw);
    VATEK_LOG(hdmi,"    VBP  : %d", ptiming->vbp);
    VATEK_LOG(hdmi,"    INT  : %d", ptiming->_int);
}

#if 0
vatek_result tool_ep9xxx_get_videotiming(uint8_t* vtimebuf, Phdmi_video_timing ptiming)
{
    if (vtimebuf == NULL || ptiming == NULL)
        return vatek_result_invalidparm;

    #define TIME_APPL(a)            (((a[1]&0x0F) << 8) | a[0])		//Active Pixels Per Line
    #define TIME_ALPF(a)            (((a[9]&0x0F) << 8) | a[8])		//Active Lines Per Frame
    #define TIME_HFP(a)             (((a[3]&0x03) << 8) | a[2])		//H sync offset
    #define TIME_HBP(a)             (((a[5]&0x03) << 8) | a[4])		//H Back Porch
    #define TIME_HPW(a)             (((a[7]&0x03) << 8) | a[6])		//H sync pulse width
    #define TIME_VFP(a)             (a[10])							//V sync offset
    #define TIME_VBP(a)             (a[11])							//V Back Porch
    #define TIME_VPW(a)             (a[12] & 0x7F)					//V sync pulse width		
    #define TIME_INT(a)             (a[12] >> 7)
    
    ptiming->pixel_per_line = TIME_APPL(vtimebuf);
    ptiming->pixel_per_frame = TIME_ALPF(vtimebuf);
    ptiming->hfp = TIME_HFP(vtimebuf);
    ptiming->hbp = TIME_HFP(vtimebuf);
    ptiming->hpw = TIME_HPW(vtimebuf);
    ptiming->vfp = TIME_VFP(vtimebuf);
    ptiming->vbp = TIME_VBP(vtimebuf);
    ptiming->vpw = TIME_VPW(vtimebuf);
    ptiming->_int = TIME_INT(vtimebuf);

    return vatek_result_success;
}
#endif

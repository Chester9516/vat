
#include "vatek_system.h"
#if defined(PHY_EP9555E)
#include "phy/phy_ep9555e.h"
#endif
#if defined(PHY_ADV7182A)
#include "phy/phy_adv7182a.h"
#endif
#if defined(PHY_ADV7611)
#include "phy/phy_adv7611.h"
#endif
#if defined(PHY_ADV7180)
#include "phy/phy_adv7180.h"
#endif
#if defined(PHY_EP9351)
#include "phy/phy_ep9351.h"
#endif
#if defined(PHY_H1)
#include "phy/phy_h1.h"
#endif

typedef struct _hphy
{
    phy_type type;
    Pvatek_i2c vi2c;
    void *hdriver;
}hphy, *Phphy;

vatek_result vatek_phy_create(Pboard_handle hboard, phy_type type, Phphy *handle)
{
    vatek_result result = vatek_result_unknown;
    Phphy newphy = NULL;

    if (hboard == NULL || handle == NULL)
        return vatek_result_invalidparm;

    if (type > phy_type_max || type < phy_type_min)
        return vatek_result_overrange;

    *handle = NULL;    
    newphy = (Phphy)malloc(sizeof(hphy));
    if (newphy == NULL)
        return vatek_result_memfail;
    
    if ((result = vatek_system_i2c_create(hboard, &newphy->vi2c)) != vatek_result_success)
    {
        free(newphy);
        return result;
    }
    
    switch (type)
    {
        #if defined(PHY_ADV7182A)
        case phy_type_adv7182a_cvbs:
            newphy->type = phy_type_adv7182a_cvbs;
            result = adv7182a_create(newphy->vi2c, si_type_cvbs, (Padv7182a_handle *)&newphy->hdriver);
            break;

        case phy_type_adv7182a_ypbpr:
            newphy->type = phy_type_adv7182a_ypbpr;
            result = adv7182a_create(newphy->vi2c, si_type_ypbpr, (Padv7182a_handle *)&newphy->hdriver);
            break;
        #endif

        #if defined(PHY_EP9555E)
        case phy_type_ep9555e:
            newphy->type = phy_type_ep9555e;
            result = ep9555e_create(newphy->vi2c, (Pep9555e_handle *)&newphy->hdriver);
            break;
        #endif
        
        #if defined(PHY_EP9351)
        case phy_type_ep9351:
            newphy->type = phy_type_ep9351;
            result = ep9351_create(newphy->vi2c, (Pep9351_handle *)&newphy->hdriver);
            break;
        #endif

        #if defined(PHY_ADV7611)
        case phy_type_adv7611:
            newphy->type = phy_type_adv7611;
            result = adv7611_create(newphy->vi2c, (Padv7611_handle *)&newphy->hdriver);
            break;
        #endif

        #if defined(PHY_ADV7180)
        case phy_type_adv7180:
            newphy->type = phy_type_adv7180;
            result = adv7180_create(newphy->vi2c, (Padv7180_handle *)&newphy->hdriver);
            break;
        #endif
        
        #if defined(PHY_H1)
        case phy_type_h1:
            newphy->type = phy_type_h1;
            result = h1_create( newphy->vi2c, (Ph1_handle *)&newphy->hdriver);
            break;
        #endif

        default:
            result =  vatek_result_unsupport;
            break;
    }
    
    if (result != vatek_result_success)
    {
        vatek_system_i2c_destroy(newphy->vi2c);
        free(newphy);
    }

    *handle = newphy;

    return result;
}

vatek_result vatek_phy_destroy(Phphy handle)
{
    vatek_result result = vatek_result_unknown;
  
    if (handle == NULL)
        return vatek_result_invalidparm;
    
    switch (handle->type)
    {
        #if defined(PHY_ADV7182A)
        case phy_type_adv7182a_cvbs:
        case phy_type_adv7182a_ypbpr:
            result = adv7182a_destroy(handle->hdriver);
            break;
        #endif

        #if defined(PHY_EP9555E)
        case phy_type_ep9555e:
            result = ep9555e_destroy(handle->hdriver);
            break;  
        #endif

        #if defined(PHY_EP9351)
        case phy_type_ep9351:
            result = ep9351_destroy(handle->hdriver);
            break;  
        #endif

        #if defined(PHY_ADV7611)
        case phy_type_adv7611:
            result = adv7611_destroy(handle->hdriver);
            break;
        #endif

        #if defined(PHY_ADV7180)
        case phy_type_adv7180:
            result = adv7180_destroy(handle->hdriver);
            break;
        #endif

        #if defined(PHY_H1)
        case phy_type_h1:
            result = h1_destroy(handle->hdriver);
            break;
        #endif
        
        default:
            return vatek_result_unsupport;    
    }

    if (result == vatek_result_success)
    { 
        if ((result = vatek_system_i2c_destroy(handle->vi2c)) != vatek_result_success)
            return result;

        free(handle);
        handle = NULL;
    }

    return result;
}

vatek_result vatek_phy_enable(Phphy handle, uint8_t enable)
{
    vatek_result result = vatek_result_unknown;
    
    if (handle == NULL)
        return vatek_result_invalidparm;

    switch (handle->type)
    {
        #if defined(PHY_ADV7182A)
        case phy_type_adv7182a_cvbs:
        case phy_type_adv7182a_ypbpr:
            result = adv7182a_enable(handle->hdriver, enable);
            break;
        #endif

        #if defined(PHY_EP9555E)
        case phy_type_ep9555e:
            result = ep9555e_enable(handle->hdriver, enable);
            break;
        #endif

        #if defined(PHY_EP9351)
        case phy_type_ep9351:
            result = ep9351_enable(handle->hdriver, enable);
            break;  
        #endif

        #if defined(PHY_ADV7611)
        case phy_type_adv7611:
            result = adv7611_enable(handle->hdriver, enable);
            break;  
        #endif

        #if defined(PHY_ADV7180)
        case phy_type_adv7180:
            result = adv7180_enable(handle->hdriver, enable);
            break;
        #endif
        
        #if defined(PHY_H1)
        case phy_type_h1:
            result = h1_enable(handle->hdriver, enable);
            break;
        #endif
        
        default:
            return vatek_result_unsupport;
    }
    
    return result;
}

vatek_result vatek_phy_status(Phphy handle, phy_status *status)
{
    vatek_result result = vatek_result_unknown;

    if (handle == NULL || status == NULL)
        return vatek_result_invalidparm;

    switch (handle->type)
    {
        #if defined(PHY_ADV7182A)
        case phy_type_adv7182a_cvbs:
        case phy_type_adv7182a_ypbpr:
            result = adv7182a_get_status(handle->hdriver, status);
            break;
        #endif

        #if defined(PHY_EP9555E)
        case phy_type_ep9555e:
            result = ep9555e_get_status(handle->hdriver, status);
            break;
        #endif

        #if defined(PHY_EP9351)
        case phy_type_ep9351:
            result = ep9351_get_status(handle->hdriver, status);
            break;  
        #endif
        
        #if defined(PHY_ADV7611)
        case phy_type_adv7611:
            result = adv7611_get_status(handle->hdriver, status);
            break;  
        #endif
        
        #if defined(PHY_ADV7180)
        case phy_type_adv7180:
            result = adv7180_get_status(handle->hdriver, status);
            break;
        #endif
        
        #if defined(PHY_H1)
        case phy_type_h1:
            result = h1_get_status(handle->hdriver, status);
            break;
        #endif
        
        default:
            return vatek_result_unsupport; 
    }
    
    return result;
}



vatek_result vatek_phy_setvideoinfo(Phphy handle, Pphy_video_info info)
{
    vatek_result result = vatek_result_unknown;
    
    if (handle == NULL || info == NULL)
        return vatek_result_invalidparm;

    switch (handle->type)
    {
        #if defined(PHY_EP9555E)
        case phy_type_ep9555e:
            result = ep9555e_set_videoinfo(handle->hdriver, info);
            break;
        #endif

        default:
            return vatek_result_unsupport;
    }
    
    return result;
}

vatek_result vatek_phy_getvideoinfo(Phphy handle, Pphy_video_info info)
{
    vatek_result result = vatek_result_unknown;
    
    if (handle == NULL || info == NULL)
        return vatek_result_invalidparm;

    switch (handle->type)
    {
        #if defined(PHY_ADV7182A)
        case phy_type_adv7182a_cvbs:
        case phy_type_adv7182a_ypbpr:
            result = adv7182a_get_videoinfo(handle->hdriver, info);        
            break;
        #endif

        #if defined(PHY_EP9555E)
        case phy_type_ep9555e:
            result = ep9555e_get_videoinfo(handle->hdriver, info);
            break;
        #endif

        #if defined(PHY_EP9351)
        case phy_type_ep9351:
            result = ep9351_get_videoinfo(handle->hdriver, info);
            break;  
        #endif

        #if defined(PHY_ADV7611)
        case phy_type_adv7611:
            result = adv7611_get_videoinfo(handle->hdriver, info);
            break;
        #endif
        
        #if defined(PHY_ADV7180)
        case phy_type_adv7180:
            result = adv7180_get_videoinfo(handle->hdriver, info);
            break;
        #endif

        #if defined(PHY_H1)
        case phy_type_h1:
            result = h1_get_videoinfo(handle->hdriver, info);
            break;
        #endif
        
        default:
            return vatek_result_unsupport;    
    }

    return result;
}

vatek_result vatek_phy_getaudioinfo(Phphy handle, Pphy_audio_info info)
{
    vatek_result result = vatek_result_unknown;

    if (handle == NULL || info == NULL)
        return vatek_result_invalidparm;

    switch (handle->type)
    {
        #if defined(PHY_ADV7182A)
        case phy_type_adv7182a_cvbs:
        case phy_type_adv7182a_ypbpr:
            result = adv7182a_get_audioinfo(handle->hdriver, info);     
            break;
        #endif

        #if defined(PHY_EP9555E)
        case phy_type_ep9555e:
            result = ep9555e_get_audioinfo(handle->hdriver, info);
            break;
        #endif

        #if defined(PHY_EP9351)
        case phy_type_ep9351:
            result = ep9351_get_audioinfo(handle->hdriver, info);
            break;  
        #endif
        
        #if defined(PHY_ADV7611)
        case phy_type_adv7611:
            result = adv7611_get_audioinfo(handle->hdriver, info);
            break;
        #endif
        
        #if defined(PHY_ADV7180)
        case phy_type_adv7180:
            result = adv7180_get_audioinfo(handle->hdriver, info);
            break;
        #endif  
        
        #if defined(PHY_H1)
        case phy_type_h1:
            result = h1_get_audioinfo(handle->hdriver, info);
            break;
        #endif 

        default:
            return vatek_result_unsupport; 
    }
    
    return result;
}

vatek_result vatek_phy_getccdata(Phphy handle, Puserdata_cc_parm cc_parm)
{
    vatek_result result = vatek_result_unknown;

    if (handle == NULL)
        return vatek_result_invalidparm;

    switch (handle->type)
    {
        #if defined(PHY_ADV7182A)
        case phy_type_adv7182a_cvbs:
        case phy_type_adv7182a_ypbpr:
            result = adv7182a_get_ccdata(handle->hdriver, cc_parm);
            break;
        #endif

        #if defined(PHY_ADV7180)
        case phy_type_adv7180:
            result = adv7180_get_ccdata(handle->hdriver, cc_parm);
            break;
        #endif
        
        default:
            return vatek_result_unsupport; 
    }

    return result;
}

vatek_result vatek_phy_setbaseclk(Phphy handle, phy_output_baseclock clk)
{
    vatek_result result = vatek_result_unknown;
    
    if (handle == NULL || clk == NULL)
        return vatek_result_invalidparm;

    switch (handle->type)
    {
        #if defined(PHY_EP9555E)
        case phy_type_ep9555e:
            result = ep9555e_set_baseclock( handle->hdriver, clk);
            break;
        #endif
        
        #if defined(PHY_H1)
        case phy_type_h1:
            result = h1_set_baseclock( handle->hdriver, clk);
            break;
        #endif
        
                
        default:
            return vatek_result_unsupport;
    }
    
    return result;
}

vatek_result vatek_phy_setoutputmode(Phphy handle, phy_output_mode mode)
{
    vatek_result result = vatek_result_unknown;
    
    if( handle==NULL)
        return vatek_result_invalidparm;
    
    switch (handle->type)
    {
        
        #if defined(PHY_H1)
        case phy_type_h1:
            result = h1_set_outputmode( handle->hdriver, mode);
            break;
        #endif
       
                
        default:
            return vatek_result_unsupport;
    }
    
    return result;
}

#if defined(SELF_TEST)
vatek_result vatek_phy_write(Phphy handle, uint32_t addr, uint8_t val)
{
    vatek_result result = vatek_result_unknown;
    
    if (handle == NULL)
        return vatek_result_invalidparm;
    
    switch (handle->type)
    {
        #if defined(PHY_H1)
        case phy_type_h1:
            result = h1_write_reg( handle->hdriver, addr, val);
            break;
        #endif
       
        default:
            return vatek_result_unsupport;
    }
    result = vatek_result_unsupport;

    return result;
}

vatek_result vatek_phy_read(Phphy handle, uint32_t addr, uint8_t* val)
{
    vatek_result result = vatek_result_unknown;
    
    if (handle == NULL || val == NULL)
        return vatek_result_invalidparm;
    
    switch (handle->type)
    {
        #if defined(PHY_H1)
        case phy_type_h1:
            result = h1_read_reg( handle->hdriver, addr, val);
            break;
        #endif
                
        default:
            return vatek_result_unsupport;
    }
    result = vatek_result_unsupport;

    return result;
}
#endif

vatek_result vatek_phy_setparm(Phphy handle)
{
    vatek_result result = vatek_result_unknown;
    
    if (handle == NULL)
        return vatek_result_invalidparm;
    
    //TODO
    result = vatek_result_unsupport;

    return result;
}


#if defined (LOG_TABLE)

#include "vatek_log_table.h"

uint8_t *table_lookup( table_type type, uint32_t val)
{
    uint8_t *str = "no support";    
    
    if( /*type < table_type_min ||*/ type>table_type_max)
        return str;
    
    uint8_t i = 0;
    switch( type)
    {
        case vi_source_type:
            for( i=0; i<SOURCE_TABLE_NUMS; i++)
            {
                if( val == vi_source_table[i].val)
                {
                    str = vi_source_table[i].name;
                    break;
                }
            }
        break;
        
        case vi_resolution_type:
            for( i=0; i<RESOLUTION_TABLE_NUMS; i++)
            {
                if( val == vi_resolution_table[i].val)
                {
                    str = vi_resolution_table[i].name;
                    break;
                }
            }
        break;
        
        case vi_aspectrate_type:
            for( i=0; i<ASPECTRATE_TABLE_NUMS; i++)
            {
                if( val == vi_aspectrate_table[i].val)
                {
                    str = vi_aspectrate_table[i].name;
                    break;
                }
            }
        break;
        
        case vi_framerate_type:
            for( i=0; i<FRAMERATE_TABLE_NUMS; i++)
            {
                if( val == vi_framerate_table[i].val)
                {
                    str = vi_framerate_table[i].name;
                    break;
                }
            }
        break;
        
        case ve_encoder_type:
            for( i=0; i<VE_TABLE_NUMS; i++)
            {
                if( val == ve_encoder_table[i].val)
                {
                    str = ve_encoder_table[i].name;
                    break;
                }
            }
        break;
        
        case ai_samplerate_type:
        break;
        
        case ae_encoder_type:
            for( i=0; i<AE_TABLE_NUMS; i++)
            {
                if( val == ae_encoder_table[i].val)
                {
                    str = ae_encoder_table[i].name;
                    break;
                }
            }
        break;
        
        case psi_mode_type:
            for( i=0; i<PSI_TABLE_NUMS; i++)
            {
                if( val == psi_table[i].val)
                {
                    str = psi_table[i].name;
                    break;
                }
            }
        break;
        
        case mod_mode_type:
            for( i=0; i<MOD_TABLE_NUMS; i++)
            {
                if( val == mod_table[i].val)
                {
                    str = mod_table[i].name;
                    break;
                }
            }
        break;
    }
    
    return str;
}

#endif

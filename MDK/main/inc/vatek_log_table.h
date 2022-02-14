
#ifndef _LOG_TABLE_
#define _LOG_TABLE_

#include "stdint.h"
#include "hal/hal_register/halreg_stream.h"
#include "hal/hal_register/halreg_output.h"
#include "hal/hal_register/halreg_mux.h"
#include "hal/hal_common_define.h"

#define CONVERT_LOG(type, val) table_lookup(type, val)

typedef enum _table_type
{
    vi_source_type     = 0,
    vi_resolution_type = 1, 
    vi_aspectrate_type = 2, 
    vi_framerate_type  = 3,  
    ve_encoder_type    = 4,    
    
    ai_samplerate_type = 5,
    ae_encoder_type    = 6,
    
    psi_mode_type      = 7,
    
    mod_mode_type      = 8,
    
    rf_freq_type       = 9,
    
    table_type_min     = vi_source_type,
    table_type_max     = rf_freq_type,
}table_type;

typedef struct _log_struct
{
    uint32_t val;
    uint8_t *name;
}log_struct;

static const log_struct vi_source_table[] = 
{
    { ENCMOD_COLORBAR, "ENCMOD_COLORBAR"},
    { ENCMOD_BOOTLOGO, "ENCMOD_BOOTLOGO"},
    { ENCMOD_VI_0,     "ENCMOD_VI_0"},
};
#define SOURCE_TABLE_NUMS (sizeof(vi_source_table)/sizeof(log_struct))
    
static const log_struct vi_resolution_table[] = 
{
    { RESOLUTION_480I, "RESOLUTION_480I"},
    { RESOLUTION_480P, "RESOLUTION_480P"},
    { RESOLUTION_576I, "RESOLUTION_576I"},
    { RESOLUTION_576P, "RESOLUTION_576P"},
    { RESOLUTION_720P, "RESOLUTION_720P"},
    { RESOLUTION_1080I,"RESOLUTION_1080I"},
    { RESOLUTION_1080P,"RESOLUTION_1080P"},
};
#define RESOLUTION_TABLE_NUMS (sizeof(vi_resolution_table)/sizeof(log_struct))
    
static const log_struct vi_framerate_table[] = 
{
    { FRAMERATE_23_97, "FRAMERATE_23_97"},
    { FRAMERATE_24   , "FRAMERATE_24"},
    { FRAMERATE_25   , "FRAMERATE_25"},
    { FRAMERATE_29_97, "FRAMERATE_29_97"},
    { FRAMERATE_30   , "FRAMERATE_30"},
    { FRAMERATE_50   , "FRAMERATE_50"},
    { FRAMERATE_59_94, "FRAMERATE_59_94"},
    { FRAMERATE_60   , "FRAMERATE_60"},
};
#define FRAMERATE_TABLE_NUMS (sizeof(vi_framerate_table)/sizeof(log_struct))
    
static const log_struct vi_aspectrate_table[] = 
{
    { ASPECTRATE_4_3,  "ASPECTRATE_4_3"},
    { ASPECTRATE_16_9, "ASPECTRATE_16_9"},
};
#define ASPECTRATE_TABLE_NUMS (sizeof(vi_aspectrate_table)/sizeof(log_struct))
    
static const log_struct ve_encoder_table[] = 
{
    { VIDEO_MPEG2, "VIDEO_MPEG2"},
    { VIDEO_H264,  "VIDEO_H264"},
    { VIDEO_HEVC,  "VIDEO_HEVC"},    
};
#define VE_TABLE_NUMS (sizeof(ve_encoder_table)/sizeof(log_struct))

static const log_struct ae_encoder_table[] = 
{
    { AUDIO_MP1_L2      , "AUDIO_MP1_L2"},
    { AUDIO_AAC_LC_ADTS , "AUDIO_AAC_LC_ADTS"},
    { AUDIO_AC_3        , "AUDIO_AC_3"},
    { AUDIO_AAC_LC_LATM , "AUDIO_AAC_LC_LATM"},
};
#define AE_TABLE_NUMS (sizeof(ve_encoder_table)/sizeof(log_struct))
    
static const log_struct psi_table[] = 
{
    { MUX_PURE     , "MUX_PURE"},
    { MUX_ISO13818 , "MUX_ISO13818"},
    { MUX_SPEC     , "MUX_SPEC"},
    { MUX_DEFSPEC  , "MUX_DEFSPEC"},
};
#define PSI_TABLE_NUMS (sizeof(psi_table)/sizeof(log_struct))
    
static const log_struct mod_table[] = 
{
    { MOD_DVB_T  , "MOD_DVB_T"},
    { MOD_J83A   , "MOD_J83A"},
    { MOD_ATSC   , "MOD_ATSC"},
    { MOD_J83B   , "MOD_J83B"},
    { MOD_DTMB   , "MOD_DTMB"},
    { MOD_ISDB_T , "MOD_ISDB_T"},
    { MOD_J83C   , "MOD_J83C"},
    { MOD_DVB_T2 , "MOD_DVB_T2"},
    { MOD_DVB_S  , "MOD_DVB_S"},
    { MOD_DVB_S2 , "MOD_DVB_S2"},
};
#define MOD_TABLE_NUMS (sizeof(mod_table)/sizeof(log_struct))


uint8_t *table_lookup( table_type type, uint32_t val);

#endif

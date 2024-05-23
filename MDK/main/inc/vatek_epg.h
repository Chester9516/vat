#include <stdint.h>
#include "vatek_define.h"
#include "vatek_hms.h"

#ifndef _HAL_LITE_LICENSED_
#define _HAL_LITE_LICENSED_

#define LIC_PES_ESINFO_LEN  64
#define LIC_EPG_TITAL_LEN   64
#define LIC_EPG_CONTENT_LEN 256
#define LIC_EPG_DESC_LEN    64

#define XAPP_EPG_TITLE_MAXLEN           40
#define XAPP_EPG_TEXT_MAXLEN            200

#define XAPP_TOT_INTERVAL_MS            20000

#define is_success(result)       (result >= vatek_result_success)

#define PROP_TYPE_STRING    0x8000
#define PROP_TYPE_LANG      0x4000


#define LICENSED_EN_TOT     0x00000001
#define LICENSED_EN_EIT     0x00000002
#define LICENSED_EPG_TAG    0xF8F40007

#define DIGITAL_COPY_FREE                   0x00
#define DIGITAL_COPY_ONCE                   0x01
#define DIGITAL_COPY_FORBIDDEN              0x02

typedef struct _licensed_pes_stream
{
    struct _licensed_pes_stream* next;
    uint16_t stream_pid;
    uint8_t stream_type;
    uint8_t es_info_len;
    uint8_t es_info[LIC_PES_ESINFO_LEN];
}licensed_pes_stream,*Plicensed_pes_stream;


typedef enum _licensed_epg_mode
{
	licensed_epg_dvb = 0,
	licensed_epg_arib_0 = 1,
	licensed_epg_arib_1 = 2,
}licensed_epg_mode;

#define  is_current_epg_mode(mode)	(mode >= licensed_epg_dvb && mode <= licensed_epg_arib_1)

/*

typedef enum _mux_content_type
{
	mux_content_undefine = 0,
	mux_content_news = 1,
	mux_content_sport = 2,
	mux_content_movie = 3,
	mux_content_drama = 4,
	mux_content_music = 5,
}mux_content_type;

typedef enum _mux_parental_rating
{
	rating_disable = 0,
	rating_general = 1,
	rating_guidance = 2,
	rating_restricted = 3,
}mux_parental_rating;

*/

typedef struct _licensed_epg_section
{
	struct _licensed_epg_section* next;
	uint8_t duration;
    uint8_t type;               
	uint8_t parental_rating;
    uint8_t recv[3];
    uint8_t title_len;
    uint8_t content_len;
	uint8_t title[LIC_EPG_TITAL_LEN];
	uint8_t content[LIC_EPG_CONTENT_LEN];
}licensed_epg_section,*Plicensed_epg_section;

typedef struct _licensed_epg_desc
{
    struct _licensed_epg_desc* next;
    uint32_t tag;
    uint32_t len;
    uint8_t desc_buf[LIC_EPG_DESC_LEN];
}licensed_epg_desc,*Plicensed_epg_desc;

typedef struct _licensed_epg
{
	licensed_epg_mode mode;
	uint32_t epg_flags;
    uint32_t start_time_date;
    uint32_t start_time_clock;
    uint32_t schedule_days;
	uint16_t event_id;
	uint8_t days;
	uint8_t recv;
	uint32_t loop_ms;
	Plicensed_epg_section sections;
    Plicensed_epg_desc descs;
}licensed_epg,*Plicensed_epg;

/*xapp struct*/
typedef struct _xapp_handle
{
    int32_t is_firstboot;
    int32_t is_en_hdcp;
    uint32_t function_flags;
    uint32_t tick;
    uint32_t check_tick;
    uint32_t tot_tick;

    /* epg function implement */
    Plicensed_epg epg_param;
}xapp_handle,*Pxapp_handle;

/*rule parameter*/
typedef struct _VATLOCAL_JAPAN
{   /* Defined in ARIB TR-B14 Part 5. 7-67 */
    uint8_t nLocalCode;         /* 1 - 62 */
    uint8_t nStationCode;       /* 0 - 15 */
    uint8_t nRemoteCode;        /* 1 - 12 */
    uint8_t nServiceNo;         /* 0 - 7  */
    uint8_t nBroadcastType;     /* 0 - 3  */
    uint8_t nRecv;
    uint16_t nAreaCode;         /* ARIB STD-B10 Part 2. D */
}VATLOCAL_JAPAN,*PVATLOCAL_JAPAN;

static const VATLOCAL_JAPAN LOCAL_JPN_ISDBT_DEFPARAM = 
{
    .nLocalCode = 11,
    .nStationCode = 8,
    .nRemoteCode = 1,
    .nServiceNo = 1,
    .nBroadcastType = 0,
    .nAreaCode = 2732,
};

//0: 1/2, 1: 2/3, 2: 3/4, 3: 5/6, 4: 7/8
typedef enum _MOD_CODERATE
{
	CORERATE_1_2 = 0,
	CORERATE_2_3 = 1,
	CORERATE_3_4 = 2,
	CORERATE_5_6 = 3,
	CORERATE_7_8 = 4,
}MOD_CODERATE;

//Guard interval
//0: 1/32, 1: 1/16, 2: 1/8, 3: 1/4
typedef enum _GUARD_INTV
{
	GUARD_INTV_1_32 = 0,
	GUARD_INTV_1_16 = 1,
	GUARD_INTV_1_8 = 2,
	GUARD_INTV_1_4 = 3,
}GUARD_INTV;

//FFT mode
//0: 2K, 1: 8K, 2: 4K
typedef enum _MOD_FFTMODE
{
	MOD_FFT_2K = 0,
	MOD_FFT_8K = 1,
	MOD_FFT_4K = 2,
}MOD_FFTMODE;

//ISDB mode  : 0 : DQPSK, 1: QPSK,  2: 16-QAM, 3: 64-QAM
typedef enum _MOD_MODE
{
	ISDB_DQPSK		= 0,
	ISDB_QPSK		= 1,
	ISDB_QAM16		= 2,
	ISDB_QAM64		= 3,
}MOD_MODE;

/*------------------------------------------------------------------------------------------------------*/
/* ISDB Moduation Define																					*/
/*------------------------------------------------------------------------------------------------------*/

typedef enum _ISDB_INTLV
{
	ISDB_INTLV_0 = 0,
	ISDB_INTLV_1 = 1,
	ISDB_INTLV_2 = 2,
	ISDB_INTLV_3 = 3,
}ISDB_INTLV;

typedef struct _ISDB_MOD_PARAM
{
	MOD_MODE ModMode;
	MOD_FFTMODE FFTMode;
	GUARD_INTV GuardMode;
	MOD_CODERATE CoreRate;
	ISDB_INTLV TimeIntlvMode;
}ISDB_MOD_PARAM,*PISDB_MOD_PARAM;

/*! Param for initial MPEG TS Stream Device. */
typedef union _VATMPEGTS_STREAMPARAM
{
	ISDB_MOD_PARAM ISDB;
}VATMPEGTS_STREAMPARAM,*PVATMPEGTS_STREAMPARAM;



#ifdef __cplusplus
extern "C" {
#endif

    /* psi_table_api        */
	vatek_result hal_licensed_set_table_version(const char* name,uint8_t* pblock,Plicensed_table tables,uint16_t version);
	vatek_result hal_licensed_set_table_interval(const char* name,uint8_t* pblock,Plicensed_table tables,uint16_t interval);

    /* licensed_property_api */
	vatek_result hal_licensed_set_prop(const char* name,uint8_t* pblock,Plicensed_prop props,uint32_t val);
    vatek_result hal_licensed_set_string(const char* name,uint8_t* pblock,Plicensed_prop props,uint8_t* pbuf,int32_t len);

    vatek_result hal_licensed_check_epg(uint8_t* pblock,int32_t* start);
    
    /* licensed_operation_api */
    vatek_result hal_licensed_start(Phms_handle pi2c,uint8_t* pblock,int32_t len);
    vatek_result hal_licensed_add_epg(Phms_handle pi2c,Plicensed_epg pepg);
    vatek_result hal_licensed_end(Phms_handle pi2c);
		
		/*write table*/
		vatek_result hal_lite_write_table(Phms_handle pi2c,Phalreg_table ptable,int32_t tablesize);
		
		vatek_result vatek_pesstream_add(Plicensed_pes_stream pextpes);
    Plicensed_pes_stream vatek_pesstream_get(void);
		vatek_result vatek_pesstream_reset(void);
    uint8_t* vatek_pesstream_malloc(int32_t size);
		
    vatek_result vatek_board_set_rule_prop(const char* name,uint32_t val);
    vatek_result vatek_board_set_rule_string(const char* name,uint8_t* pbuf,int32_t len);
		vatek_result vatek_epg_sample(Phms_handle pi2c);

		#define XRULE_PROP(name,val)        vatek_board_set_rule_prop(name,val)
		#define XRULE_STR(name,str,len)     vatek_board_set_rule_string(name,str,len)
		
    #define licensed_table_set_version(name,version)    hal_licensed_set_table_version(name,&rule_block[0],(Plicensed_table)&rule_tables[0],version)
    #define licensed_table_set_interval(name,ms)        hal_licensed_set_table_interval(name,&rule_block[0],(Plicensed_table)&rule_tables[0],ms)
    #define licensed_prop_set_uint32(name,val)          hal_licensed_set_prop(name,&rule_block[0],(Plicensed_prop)&rule_props[0],val)
    #define licensed_prop_set_string(name,str,len)      hal_licensed_set_string(name,&rule_block[0],(Plicensed_prop)&rule_props[0],str,len)

#ifdef __cplusplus
}
#endif

#endif




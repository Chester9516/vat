
#ifndef _VATEK_PSI_DEFAULT_
#define _VATEK_PSI_DEFAULT_

		

#ifdef __cplusplus
    extern "C" {
#endif
    //    Text codec
    

    #define DVBSTR_NWTWORK_NAME_MAXLEN    16
    #define DVBSTR_SERVICE_NAME_MAXLEN    16

    #define ARIBSTR_NETWORK_NAME_MAXLEN   20
    #define ARIBSTR_SERVICE_NAME_MAXLEN   20
    #define ARIBSTR_TS_NAME_MAXLEN        20

    #define ABNTSTR_NETWORK_NAME_MAXLEN   20
    #define ABNTSTR_SERVICE_NAME_MAXLEN   20
    #define ABNTSTR_TS_NAME_MAXLEN        20

    #define PSIPSTR_SHORT_NAME_MAXLEN     14
    #define PSIPSTR_LONG_NAME_MAXLEN      32
    
    #define WBUF( val, buf, pos)    { buf[pos+3] =   val&0xFF;       \
						              buf[pos+2] = ( val>>8  )&0xFF; \
						              buf[pos+1] = ( val>>16 )&0xFF; \
						              buf[pos+0] = ( val>>24 )&0xFF;}

    #define LEN(x)         ((( sizeof( x)+3)>>2)<<2)
    #define WLEN(x)        ((x+3)>>2)
    #define BUF_ALIGN(len) buffer_align(len)
    
    #define is_success(result)    (result==vatek_result_success)

    typedef struct _psispec_default_buf
    {
        int32_t     buf_len;
        uint8_t*    buf;
    }psispec_default_buf, *Ppsispec_default_buf;

    typedef struct _psispec_default_table
    {
        psispec_default_type type;       
        psispec_country_code country;
        Ppsispec_default_buf psi_buf;     /** for transform service */
    }psispec_default_table, *Ppsispec_default_table;
    
    typedef enum _data_type
    {
        data_type_unknown = 0,
        data_type_chan    = 1,
        data_type_prog    = 2,
        data_type_stream  = 3,
        data_type_min     = data_type_unknown,
        data_type_max     = data_type_stream
    }data_type;
    
#if defined(SERVICE_BROADCAST)
    vatek_result vatek_psispec_default_init_br(Phms_handle handle, psispec_default_type type, psispec_country_code country);
    vatek_result vatek_psispec_default_config( Ppsispec_default_channel chan, Ppsispec_default_program prog);
    vatek_result vatek_psispec_default_start(void);
#elif defined(SERVICE_TRANSFORM)
    vatek_result vatek_psispec_default_init_tr(Phms_handle handle, psispec_default_type type, psispec_country_code country);
    vatek_result vatek_psispec_default_ch_config(Ppsispec_default_channel chan);
    vatek_result vatek_psispec_default_program_add(Ppsispec_default_program_tr program);
    vatek_result vatek_psispec_default_program_end(void);
    vatek_result vatek_psispec_default_stream_add(Ppsispec_default_stream_tr stream);    
    vatek_result vatek_psispec_default_start_tr(void);

#endif

	#define PSI_DEF_ERR(fmt, ...)	VATEK_ERR(psi, fmt, ##__VA_ARGS__)
	#define PSI_DEF_LOG(fmt, ...)	VATEK_LOG(psi, fmt, ##__VA_ARGS__)    
		


#ifdef __cplusplus
    }
#endif

#endif


#ifndef _SPEC_DVB_
#define _SPEC_DVB_

#include <mux/mux_define.h>

#define DVBSTR_NETWORK_NAME_LEN	16
#define DVBSTR_SERVICE_NAME_LEN	16

static const uint8_t network_name[6] = { 'V','A','T','E','K','\0', };
static const mux_string def_network_name = { 5,(uint8_t*)&network_name[0],DVBSTR_NETWORK_NAME_LEN };

static const uint8_t service_name[6] = { 'V','A','-','T','V','\0', };
static const mux_string def_service_name = { 5,(uint8_t*)&service_name[0],DVBSTR_SERVICE_NAME_LEN };

/**
 * @ingroup mprop_dvb
 * DVB PSI Channel Properties
 */
typedef struct _mux_spec_dvb_channel{
	uint16_t transport_stream_id;	/*!< transport_stream_id */
	uint16_t network_id;			/*!< network_id */
	Pmux_string network_name;		/*!< network_name */
}mux_spec_dvb_channel;

typedef mux_spec_dvb_channel* Pmux_spec_dvb_channel;

/**
 * @ingroup mprop_dvb
 * DVB PSI Program Properties
 */
typedef struct _mux_spec_dvb_program{
	uint16_t original_network_id;	/*!< original_network_id */
	uint16_t program_number;		/*!< program_number */
	uint16_t channel_no;			/*!< channel_no */
	uint16_t recv;
	Pmux_string service_name;		/*!< service_name */
}mux_spec_dvb_program;

typedef mux_spec_dvb_program* Pmux_spec_dvb_program;

static const mux_spec_dvb_channel default_mux_spec_dvb_channel = { 1,1,(Pmux_string)&def_network_name };
static const mux_spec_dvb_program default_mux_spec_dvb_program = { 1,1,1,0,(Pmux_string)&def_service_name };

#endif

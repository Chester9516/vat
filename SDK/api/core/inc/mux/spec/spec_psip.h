
#ifndef _SPEC_PSIP_
#define _SPEC_PSIP_

#include <mux/mux_define.h>

#define PSIPSTR_SHORT_NAME_LEN	14
#define PSIPSTR_LONG_NAME_LEN	32

/**
 * @ingroup mprop_psip
 * PSIP CC Mode 
 */
typedef enum _psip_cc_mode{
	cc_mode_disable = 0,
	cc_mode_608 = 1,
}psip_cc_mode;

/* short name _unicode		*/
static const uint8_t _psip_short_name[] = { 0x00,'V',0x00,'A',0x00,'T',0x00,'E',0x00,'K',0x00,0x00, };
/* A65 _mutil-string 6.10	*/
static const uint8_t _psip_long_name[] = { 0x01,'e','n','g',0x01,0x00,0x00,0x04,'V','A','T','V',0x00,0x00,0x00,0x00 };

static const mux_string _psip_string_short_name = 
{ 
	sizeof(_psip_short_name) - 2,
	(uint8_t*)&_psip_short_name[0],
	PSIPSTR_SHORT_NAME_LEN,
};

static const mux_string _psip_string_long_name = 
{ 
	sizeof(_psip_long_name) - 4,
	(uint8_t*)&_psip_long_name[0],
	PSIPSTR_LONG_NAME_LEN,
};

/**
 * @ingroup mprop_psip
 * PSIP Channel Properties
 */
typedef struct _mux_spec_psip_channel {
	uint32_t psip_flags;			/*!< psip function flags */
	uint16_t transport_stream_id;	/*!< psip ts id */
	uint8_t gps_utc_offset;			/*!< time gps_utc offect */
	uint8_t recv_0;
	uint16_t daylight_saving;		/*!< daylight saving */
	uint16_t recv_1;
	psip_cc_mode cc_mode;			/*!< cc mode */
	Pmux_string short_name;		/*!< short name 7*2 */
}mux_spec_psip_channel;

typedef mux_spec_psip_channel* Pmux_spec_psip_channel;

/**
 * @ingroup mprop_psip
 * PSIP Program Properties
 */
typedef struct _mux_spec_psip_program {
	uint16_t program_number;		/*!< program number */
	uint16_t source_id;				/*!< source id */
	uint16_t channel_major;			/*!< major id */
	uint16_t channel_minor;			/*!< minor id */
	Pmux_string long_name;		/*!< extend channel name */
}mux_spec_psip_program;

typedef mux_spec_psip_program* Pmux_spec_psip_program;

static const mux_spec_psip_channel default_mux_spec_psip_channel = { 0,1,0,0,0,0,cc_mode_608,(Pmux_string)&_psip_string_short_name, };
static const mux_spec_psip_program default_mux_spec_psip_program = { 1,1,31,1,(Pmux_string)&_psip_string_long_name, };

#endif


#ifndef _SPEC_ISO_13818_
#define _SPEC_ISO_13818_

#include <mux/mux_define.h>

/**
 * @ingroup mprop_iso
 * ISO Channel Properties
 */
typedef struct _mux_spec_iso_channel{
	uint16_t transport_stream_id;	/** PAT Table transport id */
	uint16_t recv;
}mux_spec_iso_channel;

typedef mux_spec_iso_channel* Pmux_spec_iso_channel;

/**
 * @ingroup mprop_iso
 * ISO Program Properties
 */
typedef struct _mux_spec_iso_program
{
	uint16_t program_number;		/* PMT Table program number*/
	uint16_t recv;
}mux_spec_iso_program;

typedef mux_spec_iso_program* Pmux_spec_iso_program;

static const mux_spec_iso_channel default_mux_spec_iso_channel = { 1,0, };
static const mux_spec_iso_program default_mux_spec_iso_program = { 1,0, };

#endif

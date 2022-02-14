
#ifndef _HALREG_TRANSFORM_
#define _HALREG_TRANSFORM_

#include <hal/hal_register/halreg_broadcast.h>

#define HALREG_TRANSFORM_CNTL				0x600				/* inheritance HALREG_BROADCAST_CNTL 						*/				
	
	
#define HALREG_TRANSFORM_MODE				0x601
	#define TRANSFORM_SINWAVE				0x00000001
	#define TRANSFORM_TESTSTREAM			0x00000002
	#define TRANSFORM_ENUM					0x00000003
	#define TRANSFORM_CAPTURE				0x00000004
	#define TRANSFORM_BROADCAST				0x00000005

#define HALREG_TRANSFORM_INPUT				0x602


#define HALREG_TRCAPTURE_PID				0x608				/* capture packet put start at HALRANGE_PLAYLOAD_START		*/
#define HALREG_TRCAPTURE_TIMEOUT			0x609


/* extended information	*/

#define HALREG_TRINFO_MODE					0x628
#define HALREG_TRINFO_PACKETNUMS			0x629

#if 0
/* enum_stream_info */

#define HALOF_TRCAPTURE_TAG					0x00
	#define TRCAPTURE_START					0xBBF10001
	#define TRCAPTURE_END					0xBFF10002

#define HALOF_TRCAPTURE_DATATYPE			0x01
	#define TRCAPTURE_DATA_UNKNWON			0x80000000
	#define TRCAPTURE_DATA_PSI				0x00000001
	#define TRCAPTURE_DATA_PES				0x00000002
	#define TRCAPTURE_DATA_ENCRYPT			0x00000003

#define HALOF_TRCAPTURE_LEN					0x02

#define HALOF_TRCHANNEL_TAG					0x00
	#define TRCHANNEL_START_TAG				0xBBF00001
	#define TRCHANNEL_END_TAG				0xBBF00002


/* used mux_stream_spec define in halreg_mux.h				*/
#define HALOF_TRPROGRAM_TAG					0x00
	#define TRPROGRAM_START_TAG				0xBBF10001
	#define TRPROGRAM_END_TAG				0xBBF10002

#define HALOF_TRPROGRAM_PMT_PID				0x02
#define HALOF_TRPROGRAM_PCR_PID				0x03
	
#define HALOF_TRSTREAM_TAG					0x00
	#define TRSTREAM_START_TAG				0xBBF20001
	#define TRSTREAM_END_TAG				0xBBF20002

#define HALOF_TRSTREAM_PID					0x01
#define HALOF_TRSTREAM_TYPE					0x02
	/* used mux_stream_type define in halreg_mux.h				*/	
#define HALOF_TRSTREAM_CODEC				0x03
	/* used video and audio codec defined in halreg_stream.h	*/
#define HALOF_TRSTREAM_STREAM_TYPE			0x04
#define HALOF_TRSTREAM_ES_LEN				0x05
#define HALOF_TRSTREAM_ES_START				0x10
#define HALOF_TRSTREAM_ES_END				0x20
#endif

#endif
